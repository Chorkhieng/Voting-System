#include "OPLSystem.h"

#include <bits/stdc++.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "Candidate.h"
#include "Party.h"
#include "TieBreaker.h"

OPLSystem::~OPLSystem() {}


OPLSystem::OPLSystem(std::string filename) : ElectionSystem(), filename(filename) {}


void OPLSystem::setFilename(const std::string& newFilename) {
    filename = newFilename;
}

bool sortByParty(const std::pair<Party, int>& a, const std::pair<Party, int>& b) {
  return a.second > b.second;              
}


bool sortByCandidate(const std::pair<Candidate, std::pair<Party, int>>& a,
                     const std::pair<Candidate, std::pair<Party, int>>& b) {
  return a.second.second > b.second.second;
}


std::pair<bool, int> isThereATiie(const std::vector<std::pair<Party, int>>& remainders, int seatsRemaining) {
  if (remainders.empty() || seatsRemaining <= 0) {
    return std::make_pair(false, 0);
  }

  int count = 0;
  int highestRemainder = remainders.front().second;
  std::cout << "\n";

  for (const auto& remainder : remainders) {
    if (remainder.second == highestRemainder) {
      count++;
    }
  }
  // std::cout << count << " parties have " << highestRemainder << " votes ";
  return std::make_pair(count > seatsRemaining, count);
}


void OPLSystem::processCandidates() {
  std::string line;
  std::ifstream file;
  file.open(filename);

  if (!file.is_open()) {
    std::cout << "\nUnable to open file. (OPL system)" << std::endl;
  }

  std::string partyName;
  std::string candidateName;

  // loop down the first few lines of the ballot to get candidate & their party info
  for (int i = 0; i < getCandidates() + 4; i++) {
    std::getline(file, line);

    // on the lines that contain Parties and their Candidates
    if (i > 3) {
      // We first get the line as a stringstream, then split the party and candidate name with the ","
      std::stringstream ss(line);
      std::getline(ss, partyName, ',');
      std::getline(ss, candidateName, ',');
      candidateName.erase(0, candidateName.find_first_not_of(" \t"));

      Candidate cand(candidateName);
      Party party(partyName);

      // make a pair of the candidate, and a pair of their party and candidates
      // votes store this in a vector of all candidates
      candidates.push_back(std::make_pair(cand, std::make_pair(party, 0)));
    }
  }
  file.close();
}


void OPLSystem::countVotes() {
  std::string line;
  std::ifstream file;
  file.open(filename);

  if (!file.is_open()) {
    std::cout << "\nUnable to open file. (OPL system)" << std::endl;
  }

  // std::cout<< "\nSkipping:\n";
  // read in file until we get to the votes
  for (int i = 0; i < 4 + getCandidates(); i++) {
    getline(file, line);
    // std::cout << line <<"\n";
    if (i == 2) {
      setVotesCurrentBallot(std::stoi(line));
    }
  }

  // we cound the votes for each candidate
  int votesCounted = 0;
  while (
      votesCounted < votes &&
      std::getline(file, line)) {  // doing the && check slows down the program
    std::size_t index = line.find("1");
    candidates.at(index).second.second++;
    votesCounted++;
  }

  setVotesTotal(votesCounted);

  file.close();
}


void OPLSystem::giveVotesToParty() {
  // we now have two vectors to fill, one for Party & votes, another for Party & won seats 
  // we go through the candidates list, and if their Party is not in our vector, we add it
  for (const auto& candidatePair : candidates) {
    const Party& party = candidatePair.second.first;  // Extract the Party from the pair
    bool exists = false;

    // check if this party has already been added to parties vector
    for (auto& partyPair : parties) {
      if (partyPair.first == party) {
        // party already exists in parties, no need to add it but we have to add this
        // candidates votes to the party total
        exists = true;
        partyPair.second += candidatePair.second.second;
        break;
      }
    }

    if (!exists) {  // If the party wasn't found in parties, add it
      parties.push_back(std::make_pair(party, candidatePair.second.second));
      partySeats.push_back(std::make_pair(party, 0));
    }
  }

  // sorting, and printing vote count results
  std::sort(candidates.begin(), candidates.end(), sortByCandidate);
  std::sort(parties.begin(), parties.end(), sortByParty);
}



void allocateFirstSeats(std::vector<std::pair<Party, int>>& parties,
                        std::vector<std::pair<Party, int>>& partySeats,
                        std::vector<std::pair<Party, int>>& voteRemainders,
                        int quota, int& totalSeats) {
  // We go through each party, and we take this party's votes, divide that by the quota, and store
  // it in the "seats" variable. We also get the remainder for a possible second allocation
  for (auto& party : parties) {
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

void allocateSecondSeats(std::vector<std::pair<Party, int>>& partySeats,
                         std::vector<std::pair<Party, int>>& voteRemainders,
                         int& seatsRemaining, TieBreaker& tie) {
  // At this point, there are some seats left, and there could be ties between the parties.
  // IF there are ties, we break the tie, look into which party's position/index won the
  // tie results, then give them a seat & remove them from the remainders list.
  // ELSE there is no tie, we just give votes to party with largest remainder of votes, keeping order.
  while (seatsRemaining > 0 && !voteRemainders.empty()) {
    auto result = isThereATiie(voteRemainders, seatsRemaining);
    bool isTie = result.first;
    int tieCount = result.second;

    // if there is a tie, else
    if (isTie) {
      // Resolve the tie
      tie.breakTies(tieCount, seatsRemaining);
      std::cout << "\n    Tie:";

      // Parse the tieBreakerResult to get the winners index in voteRemainders
      std::istringstream iss(tie.tieBreakerResult);
      std::vector<int> winnersVec;
      int winnerIndex;

      // add the winners index to a temp vector, winnersVec
      while (iss >> winnerIndex) {
        winnersVec.push_back(winnerIndex);
      }

      sort(winnersVec.begin(), winnersVec.end());

      // Allocate seats to the winners of the tie-break
      for (int i = winnersVec.size() - 1; i >= 0; i--) {
        // Get the Party object from voteRemainders
        auto winnerParty = voteRemainders[winnersVec[i]].first;

        for (auto& partySeat : partySeats) {
          // Find the same party in partySeats
          if (partySeat.first == winnerParty) {
            // Allocate a seat, remove the party that won the seat and it's remainder votes, 
            // and break since we found the party and allocated the seat
            partySeat.second++;
            seatsRemaining--;
            voteRemainders.erase(voteRemainders.begin() + winnersVec[i]);
            break;
          }
        }
      }
      // We've allocated all the remaining seats or resolved the tie
      break;
    } else {
      // No tie, allocate next seat to the party with the largest remainder
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

void allocateCandidateSeats(
    std::vector<std::pair<Candidate, std::pair<Party, int>>>& candidates,
    std::vector<std::pair<Candidate, std::pair<Party, int>>>& winners,
    const std::vector<std::pair<Party, int>>& partySeats) {
  // To do this, we will go though each party and get the number of seats they have. 
  // If they have no seats, then we can skip this party. We will make a temp vector to hold
  // all the candidates of this party. This vector, "thisPartyCands", will store a pair: 
  // the Candidate's index in the candidates vector, and their # of votes. After that we
  // check if there are any seats to assign. If there are seats left and this party has
  // canadidates, we start by checking if there are any candidates with the same amount
  // of votes, and we take the index of where this Candidate is in the candidates vector.
  // We store tied Candidate's position index in "tiedCandidates".
  //
  // We call the tie breaker function to give us N winners for N seats out of X Candidates.
  // We then parse those results and match indexes to get the correct canidadte.
  //
  // If there is no tie, we just add the Candidates in order they are read from
  // "thisPartyCands", while there are seats.

  for (const auto& party : partySeats) {
    size_t seatsLeft = party.second;
    std::string currentPartyName = party.first.getName();

    // if party has no seats, skip to next party
    if (seatsLeft == 0) {
      continue;
    }

    // We look throuh all Candidates, and if they are from this party, keep them in temp 
    // "thisPartyCands" vector Pair: (Candidate position index in "candidates", votes)
    std::vector<std::pair<int, int>> thisPartyCands;
    for (size_t i = 0; i < candidates.size(); i++) {
      if (candidates[i].second.first.getName() == party.first.getName()) {
        thisPartyCands.push_back({i, candidates[i].second.second});
      }
    }

    // if there are candidates in this party and seats left to give
    if (!thisPartyCands.empty() && seatsLeft > 0) {
      // Lets say this party has 3 seats, and the candidates have these votes:
      // 100, 80, 60, 60, 50 The 100 and 80 get seats, but idx 2 & 3, where votes
      // are 60 & 60 are tied. Because we get seatsLeft - 1, we get idx 2 in this
      // case, or the first 60. We can see there are two ties, and this helps us
      // give out seats accordigly, then we focus on the non-tied, winning candidates
      int lastSeatVotes = thisPartyCands[seatsLeft - 1].second;

      // "tiedCandidates" keeps track of which Candidates are tied in this
      // party, and stores their idx in "candidates". we loop through this
      // party's candidates and see which ones are tied with the "lastSeatVotes"
      std::vector<int> tiedCandidates;
      for (const auto& cand : thisPartyCands) {
        if (cand.second == lastSeatVotes) {
          tiedCandidates.push_back(cand.first);
        }
      }

      // A tie exists
      if (tiedCandidates.size() > 1) {
        std::cout << "TIE!!... " << currentPartyName << "-way party tie between: ";
        for (size_t k = 0; k < tiedCandidates.size(); k++) {
          std::cout << candidates[tiedCandidates[k]].first.getName() << " (" << k << "), ";
        }

        // we see there are N candidates, and X seats, so find out who gets a seat
        TieBreaker tie;
        tie.breakTies(tiedCandidates.size(), party.second);

        std::istringstream iss(tie.tieBreakerResult);
        int winnerIndex;
        // "tieBreakerResult" says a candidate(s) at index(s) X of vector "tiedCandidates" has won. 
        // "tiedCandidates" tells us this candidates position in "candidates vector". 
        // We take the the winner, move them to winner vecotr, and remove them from "candidates". 
        // Since we are done with this candidate, they are erased from this party's candidate list
        // (temp vector "thisPartyCands")
        while (iss >> winnerIndex) {
          // candidateIndex is the postion of winning candidate in "candidates" vector
          int candidateIndex = tiedCandidates[winnerIndex];

          winners.push_back(candidates[candidateIndex]);
          thisPartyCands.erase(thisPartyCands.begin() + winnerIndex);
          candidates.erase(candidates.begin() + candidateIndex);
          seatsLeft--;
        }
      } else {
        // No tie
        // This party has N seats, so add first N candidates of this party to winners list
        for (size_t i = 0; i < seatsLeft && i < thisPartyCands.size(); i++) {
          winners.push_back(candidates[thisPartyCands[i].first]);
        }
        // erase candidates from this party's temp candidate vector

        // since size_t doesn't go to zero, it will loop to positive max int. We cannot do 
        // "i >= 0; i--", since when i = 0, and we decrement, we will get i = max_int,
        // since size_t doesn't cover negative numebrs. To void this, we declare i,
        // a then use "i-- > 0", which decrements "i", after it checks "i > 0",
        // therefore when we get to i = 0, the condition is false, and we finish the iteration.
        for (size_t i = thisPartyCands.size(); i-- > 0;) {
          if (i < seatsLeft) {
            candidates.erase(candidates.begin() + thisPartyCands[i].first);
          }
        }
      }
    }
  }
}

// In a OPL election, any voter can give ONE candidate a vote, and only ONE candidate. 
// This allows for popularity ranking within the party. So all the candidates votes
// go the party, which determines how many seats the party gets. The candidates are
// sorted by their popularity, and the top X candidates get seats in a party.
void OPLSystem::allocateSeats() {
  int totalSeats = getSeats();
  // Avoid division by zero. If there are 0 seats, we set quota to equal 0
  int quota = totalSeats > 0 ? getVotesTotal() / (totalSeats > 0 ? totalSeats : 1) : 0;

  // if quota is 1, then when giving out first seats, all parties will recieve the number of votes
  // as their number of seats won. There are other ways this could be safe guarded, but this seems
  // to be the fastest fix, and it seems fair from further testing.
  if (quota == 1) {
    quota = 2;
  }

  // After the first seat allocation, there may be a second seat allocation if not all
  // the seats were filled. We need to keep track of the remaing votes each party has.
  std::vector<std::pair<Party, int>> voteRemainders;

  // First seat allocation

  // We go through each party, and we take this party's votes, divide that by the quota,  and
  // store it in the "seats" variable. We also get the remainder for a possible second allocation
  allocateFirstSeats(parties, partySeats, voteRemainders, quota, totalSeats);

  // sort the parties with remainder votes from most votes to least, for convenience
  std::sort(voteRemainders.begin(), voteRemainders.end(), sortByParty);

  TieBreaker tie;

  // Second seat allocation

  // At this point, there are some seats left, and there could be ties between the parties. 
  // IF there are ties, we break the tie, look into which party's position/index won 
  // the tie results, then give them a seat & remove them from the remainders list.
  // ELSE there is no tie, we just give votes to party with largest remainder of votes, keeping order.
  allocateSecondSeats(partySeats, voteRemainders, totalSeats, tie);

  // Now we want to reorder the vector that has the parties and their votes: "parties". 
  // To do so, we first sort the vectors that has the parties and their seats: "partySeats".
  // Now that this seats vector is ordered and we have a temp vector "orderedSeats", 
  // we can sort the "parties" vector to match the order of "partySeats".
  // We then assign the temp vector to the "parties" vector.
  std::vector<std::pair<Party, int>> orderedSeats;

  std::sort(partySeats.begin(), partySeats.end(), sortByParty);

  for (const auto& party : partySeats) {
    for (const auto& partyPair : parties) {
      if (party.first.getName() == partyPair.first.getName()) {
        orderedSeats.push_back(partyPair);
        break;
      }
    }
  }

  parties = std::move(orderedSeats);

  // Lastly, we have to handle any ties for seats within each party. A party may
  // have 1 seat, but two or more candidates that have the same number of votes.
  allocateCandidateSeats(candidates, winners, partySeats);
}

// This is a helper function that will make a table of the winning and losing
// candidates of each party. It will also provide the votes, and the % of votes
// won out of the whole election for both the party and the candidate. This
// function is provided here as a helper as it will be used by both
// displayToTerminal() and produceAudit() for the winners/losers table.
std::string OPLCandidates(
    const std::vector<std::pair<Candidate, std::pair<Party, int>>>& candidates,
    const std::vector<std::pair<Candidate, std::pair<Party, int>>>& winners,
    const std::vector<std::pair<Party, int>>& parties,
    const std::vector<std::pair<Party, int>>& partySeats, int totalVotes) {
  std::string result = "";

  // each row has two rows inside, so we use this to store necessary data at
  // once
  std::ostringstream oss;
  std::ostringstream oss2;

  // build header
  oss << std::left << std::setw(35) << "             Party" << std::setw(35)
      << "|             Winners" << std::setw(35)
      << " |               Losers              |"
      << "\n";
  oss << "---------------------------------------------------------------------"
         "--------------------------------------|"
      << std::endl;
  std::string table = oss.str();
  std::string temp = "";

  // for all parties, we build a row, even if they will be empty
  for (size_t i = 0; i < partySeats.size(); i++) {
    oss.str("");
    oss.clear();
    oss2.str("");
    oss2.clear();
    std::string partyName = partySeats.at(i).first.getName();

    // we get this Party's name and info in the first column and row
    float per = static_cast<float>(parties.at(i).second) /
                static_cast<float>(totalVotes) * 100.0;
    std::ostringstream tempStream;
    tempStream << std::fixed << std::setprecision(2) << per;
    temp = "Votes: " + std::to_string(parties.at(i).second) + "   (" +
           tempStream.str() + "%" + " of total)";

    oss << std::left << std::setw(35) << partyName << "|";
    oss2 << std::left << std::setw(35) << temp << "|";

    // below temp vectors hold the index where a winner/loser of this party is
    // the winners/candidates vectors and the below ints make sure we don't go
    // out of range of these temp vectors
    int line = 0;
    size_t idx_W = 0;
    size_t idx_L = 0;
    std::vector<int> numWinners;
    std::vector<int> numLosers;
    for (size_t j = 0; j < winners.size(); j++) {
      if (winners.at(j).second.first.getName() == partyName) {
        numWinners.push_back(j);
      }
    }
    for (size_t j = 0; j < candidates.size(); j++) {
      if (candidates.at(j).second.first.getName() == partyName) {
        numLosers.push_back(j);
      }
    }

    // now we fill in the winner and loser columns for each party
    while (idx_W < numWinners.size() || idx_L < numLosers.size()) {
      if (line > 0) {
        table +=
            "                                   |   -   -   -   -   -   -   -  "
            " -   -   -   -   -   -   -   -   -   -   |\n";
      }

      // if there are winners left, we get their name, votes, and % of votes
      if (idx_W < numWinners.size()) {
        per =
            static_cast<float>(winners.at(numWinners.at(idx_W)).second.second) /
            static_cast<float>(totalVotes) * 100.0;
        tempStream.str("");
        tempStream.clear();
        tempStream << std::fixed << std::setprecision(2) << per;
        temp = "Votes: " +
               std::to_string(winners.at(numWinners.at(idx_W)).second.second) +
               "   (" + tempStream.str() + "%" + " of total)";

        oss << std::setw(35) << winners.at(numWinners.at(idx_W)).first.getName()
            << "|";
        oss2 << std::setw(35) << temp << "|";
      } else {
        oss << std::setw(35) << ""
            << "|";
        oss2 << std::setw(35) << ""
             << "|";
      }

      // if there are losers left, we get their name, votes, and % of votes
      if (idx_L < numLosers.size()) {
        per = static_cast<float>(
                  candidates.at(numLosers.at(idx_L)).second.second) /
              static_cast<float>(totalVotes) * 100.0;
        tempStream.str("");
        tempStream.clear();
        tempStream << std::fixed << std::setprecision(2) << per;
        temp =
            "Votes: " +
            std::to_string(candidates.at(numLosers.at(idx_L)).second.second) +
            "   (" + tempStream.str() + "%" + " of total)";

        oss << std::setw(35)
            << candidates.at(numLosers.at(idx_L)).first.getName() << "|";
        oss2 << std::setw(35) << temp << "|";
      } else {
        oss << std::setw(35) << ""
            << "|";
        oss2 << std::setw(35) << ""
             << "|";
      }

      // clear & append everything so it's ready for the next row
      oss << "\n";
      oss2 << "\n";
      idx_W++;

      table += oss.str();
      table += oss2.str();

      idx_L++;
      line++;
      oss.str("");
      oss.clear();
      oss2.str("");
      oss2.clear();

      oss << std::setw(35) << ""
          << "|";
      oss2 << std::setw(35) << ""
           << "|";
    }
    table +=
        "----------------------------------------------------------------------"
        "-------------------------------------|\n";
  }

  result += table;

  return result;
}

// This function will build the string that will be displayed to the terminal, including 
// a table that shows which candidates won or loss, and their Party's total votes won.
std::string OPLSystem::displayResults() {
  std::string result = "A OPL election was run to fill " + std::to_string(getSeats()) +" seats. "
      "There were " + std::to_string(getVotesTotal()) + " votes. The results are below:\n\n";

  // We call our helper function to build the table of winning and losing candidates
  result += OPLCandidates(candidates, winners, parties, partySeats, getVotesTotal());

  return result;
}

std::string OPLSystem::auditResults() {
  std::string result = "OPL Election Audit\n";
  // Add total seats and total votes to the audit report
  result += "There were " + std::to_string(parties.size()) + " parties in this election.\n";
  result += "Total Votes: " + std::to_string(getVotesTotal()) + "\n";
  result += "Total Seats: " + std::to_string(getSeats()) + "\n\n";

  // result += "\n\n add #5 in OPL: group candidates by party?\n\n";

  std::cout << "\n\n\n";

  result += "---------------------------------------------------------------"
            "----------------------------------------------\n";

  result += "                                 First         Remaining      "
            "Second          Final         %" " of Vote\n";

  result += "   Parties        Votes        Allocation        Votes       "
            "Allocation       Seat              to\n";

  result += "                                of Seats                      "
            "of Seats        Total         %" " of Seats\n";

  result += "---------------------------------------------------------------"
            "----------------------------------------------\n";

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
    quota = getSeats() > 0 ? getVotesTotal() / (getSeats() > 0 ? getSeats() : 1) : 0;
    firstSeats = quota > 0 ? parties.at(i).second / quota : 0;
    secondSeats = partySeats.at(i).second - firstSeats;
    
    remainingVotes = quota > 0 ? parties.at(i).second % quota : parties.at(i).second;
    perOfVotes = static_cast<float>(parties.at(i).second) / static_cast<float>(getVotesTotal()) * 100.0;
    perOfSeats = static_cast<float>(partySeats.at(i).second) / static_cast<float>(getSeats()) * 100.0;

    oss << std::left << std::setw(20) << parties.at(i).first.getName()
        << std::setw(15) << parties.at(i).second << std::setw(15) << firstSeats
        << std::setw(15) << remainingVotes << std::setw(15) << secondSeats
        << std::setw(10) << partySeats.at(i).second << std::fixed
        << std::setprecision(2) << perOfVotes << "% / " << perOfSeats << "%\n";

    result += oss.str();
  }
  result += "------------------------------------------------------------------------"
            "-------------------------------------\n";

  result += "\n\nList of each party and their candidates, and the winners and losers.\n\n";

  result += OPLCandidates(candidates, winners, parties, partySeats, getVotesTotal());

  return result;
}