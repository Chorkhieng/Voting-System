#include "CPLSystem.h"

#include <bits/stdc++.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Candidate.h"
#include "Party.h"
#include "TieBreaker.h"

CPLSystem::~CPLSystem() {}

CPLSystem::CPLSystem(std::string filename) : ElectionSystem(), filename(filename) {}


bool sortBySec(const std::pair<Party, int>& a, const std::pair<Party, int>& b) {
  return a.second > b.second;
}


std::pair<bool, int> isThereATie(
    const std::vector<std::pair<Party, int>>& remainders, int seatsRemaining) {
  if (remainders.empty() || seatsRemaining <= 0) {
    return std::make_pair(false, 0);
  }

  int count = 0;
  int highestRemainder = remainders.front().second;

  for (const auto& remainder : remainders) {
    if (remainder.second == highestRemainder) {
      count++;
    }
  }

  if (count > seatsRemaining) {
    std::cout << "\nTIE!!... " << count << "-way party tie between: \n";
    for (const auto& r : remainders) {
      if (r.second == highestRemainder) {
        std::cout << r.first.getName() << " has " << r.second << " votes\n";
      }
    }
  }

  // std::cout <<count << " parties have " <<highestRemainder << " votes ";
  return std::make_pair(count > seatsRemaining, count);
}


void CPLSystem::countVotes() {
  std::string line;
  std::ifstream file;
  file.open(filename);

  if (!file.is_open()) {
    std::cout << "Unable to open file. (CPL system)" << std::endl;
  }

  std::string candidateName;
  std::string partyName;

  // loop down the first few lines of the ballot to get party & candidate info
  for (int i = 0; i < getParties() + 4; i++) {
    std::getline(file, line);

    // on the lines that contain Parties and their Candidates
    if (i > 3) {
      // We first get the line as a stringstream, & add the first word, a Party, to partyName
      std::stringstream ss(line);
      std::getline(ss, partyName, ',');
      std::vector<Candidate> partyCandidates;

      // we now make a Party object using this Party's name, then add it to our vector later
      Party party(partyName);

      // we read the name of each candidate in the Party
      while (std::getline(ss, candidateName, ',')) {
        candidateName.erase(0, candidateName.find_first_not_of(" \t")); // Remove leading spaces
        candidateName.erase(candidateName.find_last_not_of(" \t") + 1); // Remove trailing spaces
                            
        // make a candidate object and add it to a temp vector
        Candidate cand(candidateName);
        partyCandidates.push_back(cand);
      }

      parties.push_back(std::make_pair(party, partyCandidates));
      partyVotes.push_back(std::make_pair(party, 0));
      partySeats.push_back(std::make_pair(party, 0));
    }
  }

  int votesCounted = 0;
  while (votesCounted < votes && std::getline(file, line)) {
    std::size_t index = line.find("1");
    partyVotes.at(index).second++;
    votesCounted++;
  }

  setVotes(votesCounted);

  file.close();

  // sort the partyVotes to show most popular parties first
  std::sort(partyVotes.begin(), partyVotes.end(), sortBySec);
}


void allocateFirstSeats(int quota,
                        std::vector<std::pair<Party, int>>& partyVotes,
                        std::vector<std::pair<Party, int>>& partySeats,
                        std::vector<std::pair<Party, int>>& voteRemainders,
                        int& totalSeats) {
  // First seat allocation

  // We go through each party, and we take this party's votes, divide that by the quota, and 
  // store it in the "seats" variable. We also get the remainder for a possible second allocation
  for (auto& party : partyVotes) {
    // Avoid division by 0 by ensuring quota is greater than 0
    int seats = quota > 0 ? party.second / quota : 0;
    // If quota is 0, remainder is just the party's second value.
    int remainder = quota > 0 ? party.second % quota : party.second;

    // Find this party in "partySeats", and add the number of seats to this party's seat count
    for (auto& partySeat : partySeats) {
      if (party.first == partySeat.first) {
        partySeat.second += seats;
        totalSeats -= seats;
        break;
      }
    }

    // check if this party's has votes left, but not enough for a full quota, and save it for later
    if (remainder > 0) {
      voteRemainders.push_back({party.first, remainder});
    }
  }
}


void allocateSecondSeats(int& seatsRemaining,
                         std::vector<std::pair<Party, int>>& voteRemainders,
                         std::vector<std::pair<Party, int>>& partySeats,
                         TieBreaker& tie) {
  // At this point, there are some seats left, and there could be ties between the parties.
  // IF there are ties, we break the tie, look into which party's position/index won the tie
  // results, then give them a seat & remove them from the remainders list. ELSE there is no 
  // tie, we just give votes to party with largest remainder of votes, keeping order.
  while (seatsRemaining > 0 && !voteRemainders.empty()) {
    auto result = isThereATie(voteRemainders, seatsRemaining);
    bool isTie = result.first;
    int tieCount = result.second;

    // if there is a tie, else
    if (isTie) {
      // Resolve the tie
      tie.breakTies(tieCount, seatsRemaining);

      // Parse the tieBreakerResult to get the winners index in voteRemainders
      std::istringstream iss(tie.tieBreakerResult);
      std::vector<int> winners;
      int winnerIndex;

      // add the winners index to a temp vector, winners
      while (iss >> winnerIndex) {
        winners.push_back(winnerIndex);
      }

      sort(winners.begin(), winners.end());

      // Allocate seats to the winners of the tie-break
      for (int i = winners.size() - 1; i >= 0; i--) {
        // Get the Party object from voteRemainders
        auto winnerParty = voteRemainders[winners[i]].first;

        std::cout << winnerParty.getName() << " won a seat, they are idx: " << winners[i] << " \n";
                  
        for (auto& partySeat : partySeats) {
          // Find the same party in partySeats
          if (partySeat.first == winnerParty) {
            // Allocate a seat, remove the party that won the seat and it's remainder 
            // votes, and break since we found the party and allocated the seat
            partySeat.second++;
            seatsRemaining--;
            voteRemainders.erase(voteRemainders.begin() + winners[i]);
            break;
          }
        }
      }

      // We've allocated all the remaining seats or resolved the tie
      break;
    } else {
      // No tie, allocate next seat to the party with the largest remainder.
      // Get the Party object with the largest remainder (they were sorted previously)
      auto largestRemainderParty = voteRemainders[0].first;

      // Find the same party in partySeats
      for (auto& partySeat : partySeats) {
        if (partySeat.first == largestRemainderParty) {
          // Allocate a seat to this partyand break since we found the party and gave the seat
          partySeat.second++;
          seatsRemaining--;
          break;
        }
      }
      // Remove the party that just received a seat
      voteRemainders.erase(voteRemainders.begin());
    }
  }
}


void CPLSystem::allocateSeats() {
  int totalSeats = getSeats();
  int quota = totalSeats > 0 ? getVotes() / (totalSeats > 0 ? totalSeats : 1) : 0;

  // After the first seat allocation, there may be a second seat allocation if not all the
  // seats were filled. We need to keep track of the remaing votes each party has.
  std::vector<std::pair<Party, int>> voteRemainders;

  // First seat allocation
  allocateFirstSeats(quota, partyVotes, partySeats, voteRemainders, totalSeats);

  // sort the parties with remainder votes from most votes to least, for convenience.
  std::sort(voteRemainders.begin(), voteRemainders.end(), sortBySec);

  TieBreaker tie;

  // Second seat allocation
  allocateSecondSeats(totalSeats, voteRemainders, partySeats, tie);

  // Reorder the "parties" vector so that the Party order matches the "partySeats", where
  // the party with the most seats is first in the vector. This will help us easily produce
  // the results string in the following functions. To do this we make a temp vector,
  // fill it in the order of "partySeats", and then reassign it to "parties".
  std::vector<std::pair<Party, std::vector<Candidate>>> orderedParties;

  for (const auto& seatPair : partySeats) {
    for (const auto& partyPair : parties) {
      if (seatPair.first.getName() == partyPair.first.getName()) {
        orderedParties.push_back(partyPair);
        break;
      }
    }
  }

  parties = std::move(orderedParties);

  std::cout << "\n\n";
}


// This is a helper function that will make a table of the winning and losing candidates of
// each party. It will also provide the votes, and the % of votes won by the party.
// This function is provided here as a helper as it will be used by both
// displayToTerminal() and produceAudit() for the winners/losers table.
std::string CPLCandidates(
      const std::vector<std::pair<Party, std::vector<Candidate>>> parties,
      const std::vector<std::pair<Party, int>>& partySeats,
      const std::vector<std::pair<Party, int>>& partyVotes, int totalVotes) {
      std::string result = "";

  // make an output string stream, which can be formated, and we then turn it into a string
  // that will be displayed to the terminal. First we need to make the header of the table
  std::ostringstream oss;
  oss << std::left << std::setw(20) << "  Party" << std::setw(20) << "|  Winners" << std::setw(20)
      << " |  Losers" << std::setw(20) << " |  Stats" << "\n";
  oss << "---------------------------------------------------------------------------------------------" << std::endl;
      
  std::string table = oss.str();

  // Second, we need to make each table row for each party.
  for (size_t i = 0; i < partySeats.size(); i++) {
    // clear output string stream contents and flags
    oss.str("");
    oss.clear();
    oss << std::left << std::setw(20) << partyVotes.at(i).first.getName() << "|";
        
    int currentPartySeats = partySeats.at(i).second;
    // int currentPartyVotes = partyVotes.at(i).second;

    int line = 1;
    int numWinners = 0;  // partySeats.at(i).second;
    size_t numLosers = currentPartySeats;

    // After the prep work above, we can print both winning and losing candidates on the same row for each party
    // 
    while (numWinners < currentPartySeats || numLosers < parties.at(i).second.size() || line < 3) {
      if (numWinners < currentPartySeats) {
        oss << std::setw(20) << parties.at(i).second.at(numWinners).getName() << "|";
        numWinners++;
      } else {
        oss << std::setw(20) << "" << "|";
      }

      if (numLosers < parties.at(i).second.size()) {
        oss << std::setw(20) << parties.at(i).second.at(numLosers).getName() << "|";
        numLosers++;
      } else {
        oss << std::setw(20) << "" << "|";
      }

      if (line == 1) {
        oss << "Votes: " << partyVotes.at(i).second;
      }

      if (line == 2) {
        float per = static_cast<float>(partyVotes.at(i).second) / static_cast<float>(totalVotes) * 100.0;
        oss << "That is " << std::fixed << std::setprecision(2) << per << "%" << " of votes.";
      }

      if (numWinners < currentPartySeats || numLosers < parties.at(i).second.size() || line < 2) {
        oss << "\n" << std::setw(20) << "" << "|";
      }
      line++;
    }
    oss << "\n---------------------------------------------------------------------------------------------"
        << std::endl;

    table += oss.str();
  }

  result += table;

  return result;
}

// This function will build the string that will be displayed to the terminal, including a
// table that shows which candidates won or loss, and their Party's total votes won.
std::string CPLSystem::displayResults() {
  std::string result = "A CPL election was run to fill " + std::to_string(getSeats()) + " seats. There were " + 
                    std::to_string(getVotes()) + " votes. The results are below:\n\n";
                         
  // We call our helper function to build the table of winning Candidates
  result += CPLCandidates(parties, partySeats, partyVotes, getVotes());

  return result;
}

// produces a table that is easy to read with all of the stats, including the first and second seat allocations.
// At the end, another table will be shown with candidates who won or loss, and their Party's total votes won.
std::string CPLSystem::auditResults() {
  std::string result = "\nCPL Election Audit\n";
  // Total seats and total votes are added to the audit report
  result += "There were " + std::to_string(parties.size()) + " parties in this election.\n";
  result += "Total Votes: " + std::to_string(getVotes()) + "\n";
  result += "Total Seats: " + std::to_string(getSeats()) + "\n";

  result += "\n\nResults of this CPL election: \n\n";

  result += "------------------------------------------------------------------"
            "-------------------------------------------\n";

  result += "                                  First        Remaining        "
            "Second         Final         %" " of Vote\n";
      
  result += "   Parties          Votes       Allocation       Votes        "
            "Allocation       Seat              to\n";

  result += "                                 of Seats                      "
            "of Seats        Total         %" " of Seats\n";
      

  result += "------------------------------------------------------------------------"
            "-------------------------------------\n";

  std::ostringstream oss;
  int quota = 0;
  int firstSeats = 0;
  int secondSeats = 0;
  int remainingVotes = 0;
  float perOfVotes = 0.0;
  float perOfSeats = 0.0;

  for (size_t i = 0; i < parties.size(); i++) {
    oss.str("");
    oss.clear();

    // Avoid division by 0 with the below checks
    quota = getSeats() > 0 ? getVotes() / (getSeats() > 0 ? getSeats() : 1) : 0;

    firstSeats = quota > 0 ? partyVotes.at(i).second / quota : 0;
    secondSeats = partySeats.at(i).second - firstSeats;
    remainingVotes =quota > 0 ? partyVotes.at(i).second % quota : partyVotes.at(i).second;

    perOfVotes = static_cast<float>(partyVotes.at(i).second) / static_cast<float>(getVotes()) * 100.0;
    perOfSeats = static_cast<float>(partySeats.at(i).second) / static_cast<float>(getSeats()) * 100.0;
                 

    oss << std::left << std::setw(20) << parties.at(i).first.getName()
        << std::setw(15) << partyVotes.at(i).second << std::setw(15)
        << firstSeats << std::setw(15) << remainingVotes << std::setw(15)
        << secondSeats << std::setw(10) << partySeats.at(i).second << std::fixed
        << std::setprecision(2) << perOfVotes << "% / " << perOfSeats << "%\n";

    result += oss.str();
  }
  result += "------------------------------------------------------------------------"
            "-------------------------------------\n\n\n\n";

  result += "\n\nThese Candidates won a seat: \n\n";
  // We call our helper function to build the table of winning Candidates
  result += CPLCandidates(parties, partySeats, partyVotes, getVotes());

  return result;
}
