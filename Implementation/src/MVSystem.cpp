#include "MVSystem.h"

#include <bits/stdc++.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

#include "Candidate.h"
#include "Party.h"
#include "TieBreaker.h"

MVSystem::~MVSystem() {}


MVSystem::MVSystem(std::string filename) : ElectionSystem(), filename(filename) {}


void MVSystem::setFilename(const std::string& newFilename) {
    filename = newFilename;
}

bool sortByPartyMV(const std::pair<Party, int>& a, const std::pair<Party, int>& b) {
  return a.second > b.second;              
}


bool sortByCandidateMV(const std::pair<Candidate, std::pair<Party, int>>& a,
                     const std::pair<Candidate, std::pair<Party, int>>& b) {
  return a.second.second > b.second.second;
}


std::pair<bool, int> isThereATieMV(const std::vector<std::pair<Party, int>>& remainders, int seatsRemaining) {
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


void MVSystem::processCandidates() {
  std::string line;
  std::ifstream file;
  file.open(filename);

  if (!file.is_open()) {
    std::cout << "\nUnable to open file. (MV system)" << std::endl;
  }

  std::string partyName;
  std::string candidateName;


  // skip 3 lines of the header to get to the candidates
  std::getline(file, line);
  std::getline(file, line);
  std::getline(file, line);

  //  now read the candidates
  std::getline(file, line);
  file.close();


  // replace brackets, with spaces, then remove the spaces
  std::replace(line.begin(), line.end(), '[', ' ');
  std::replace(line.begin(), line.end(), ']', ' ');

  line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

  std::istringstream iss(line);


  while (std::getline(iss, candidateName, ',')) {
    std::getline(iss, partyName, ',');

    Candidate cand(candidateName);
    Party party(partyName);

    candidates.push_back(std::make_pair(cand, std::make_pair(party, 0)));
  }
}


void MVSystem::countVotes() {
  std::string line;
  std::ifstream file;
  file.open(filename);

  if (!file.is_open()) {
    std::cout << "\nUnable to open file. (MV system)" << std::endl;
  }

  // read in file until we get to the votes
  for (int i = 0; i < 5 ; i++) {
    
    getline(file, line);
    if (i == 4) {
      setVotesCurrentBallot(std::stoi(line));
    }
  }

  // we cound the votes for each candidate
  int votesCounted = 0;
  int linesCounted = 0;
  while (linesCounted < votes && std::getline(file, line)) { // doing the && check slows down the program
    size_t index = 0;
    int idx_csv = 0;
    while (index < line.length()) {
      if (line[index] == '1') {
        candidates.at(idx_csv).second.second++;
        votesCounted++;
        index++;
      }
      if (line[index] == ',') {
        index++;
        idx_csv++;
      }
    }
    linesCounted++;
  }

  setVotesTotal(votesCounted);

  file.close();
}


void MVSystem::giveVotesToParty() {
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
  std::sort(candidates.begin(), candidates.end(), sortByCandidateMV);
  std::sort(parties.begin(), parties.end(), sortByPartyMV);
}




// In a MV election, any voter can give one or more votes, but one per candidate.
// MV is purely based on popularity, so if there a 4 seats, and the top 4 candidates
// come from the same party, then that Party's Candidates will get the 4 seats.
void MVSystem::allocateSeats() {
  int totalSeats = getSeats();


  std::cout << "\n\n";
  int idx = 0;

  
  // while there are seats to give, give seats and handle ties
  while (totalSeats > 0) {
    // check if this candidate has more votes than the next candidate
    if (idx + 1 < static_cast<int>(candidates.size()) && 
          candidates[idx].second.second > candidates[idx + 1].second.second ) {

      // since this candidate has more votes than the next candidate, we don't need to worry about ties.
      // We add this candidate to the winners vector, and remove them from candidates vector. Subtract seat
      winners.push_back(candidates[idx]);
      candidates.erase(candidates.begin() + idx);
      totalSeats--;          
    } else {
      TieBreaker tie;
      size_t seatsLeft = totalSeats;
      std::vector<int> tiedCandidates;

      // This candidate and at least the next one have the same number of votes. This can lead to a tie.
      // We will keep track of the position/index of the candiates with the same number of votes, and
      // run them through the tie breaker.
      for (int i = 0; i < static_cast<int>(candidates.size()); i++) {
        if (candidates[i].second.second == candidates[0].second.second) {
          tiedCandidates.push_back(i);
        }
      }

      // announce that there is a tie to teh terminal, and who is tied
      std::cout << "TIE!!... " << tiedCandidates.size() << "-way party tie between: ";
      for (size_t k = 0; k < tiedCandidates.size(); k++) {
        std::cout << candidates[tiedCandidates[k]].first.getName() << " (" << k << "), ";
      }

      // we see there are N candidates, and X seats, so find out who gets a seat. This function will
      // run may coin tosses (results seen on terminal), and it will return the index of the 
      // tied Candidate that has won. This index is stored in tiedCandidates
      tie.breakTies(tiedCandidates.size(), totalSeats);


      std::istringstream iss(tie.tieBreakerResult);
      int winnerIndex;
      Candidate cand("a");
      // "tieBreakerResult" says a candidate(s) at index(s) X of vector "tiedCandidates" has won. 
      // "tiedCandidates" tells us this candidates position in "candidates" vector. 
      // We take the the winner, move them to winner vector, and remove them from "candidates". 
      while (iss >> winnerIndex) {
        // winnerIndex is the postion of winning candidate in "candidates" vector
        winners.push_back(candidates[winnerIndex]);
        candidates[winnerIndex].second.second = -1;
        seatsLeft--;
        totalSeats--;
      }
    }
  }

  // Remove the winning candidates from "candidates" vector. They have -1 votes
  size_t i = 0;
  while (i < candidates.size()) {
    if (candidates[i].second.second == -1) {
        candidates.erase(candidates.begin() + i);
    } else {
        ++i;
    }
  }

  


  // we now want to update the total amound of seats that each party won. We do this by looping
  // through each party, and then looping through the winners vector. If their party names match,
  // then we increment the total seats of that party in partySeats
  for (auto& p : partySeats) {
    for (const auto& w : winners) {
      if (w.second.first.getName() == p.first.getName()) {
        p.second++;
      }
    }
  }


//   std::cout << "\n\n\n------------Cands--------------------\n";
//   for (const auto& c : candidates) {
//     std::cout << "|" << c.first.getName() << "--"<<c.second.first.getName() <<"--" << c.second.second<<"|\n";
//     // break;
//   }
// std::cout << "\nwinners\n";
//   for (const auto& c : winners) {
//     std::cout << "|" << c.first.getName() << "--"<<c.second.first.getName() <<"--" << c.second.second<<"|\n";
//     // break;
//   }

//   std::cout << "\nparties\n";
//   for (const auto& c : parties) {
//     std::cout << "|" << c.first.getName() << "--"<<c.second<<"|\n";
//     // break;
//   }
}

// This is a helper function that will make a table of the winning and losing
// candidates of each party. It will also provide the votes, and the % of votes
// won out of the whole election for both the party and the candidate. This
// function is provided here as a helper as it will be used by both
// displayToTerminal() and produceAudit() for the winners/losers table.
std::string MVCandidates(
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
      << "|             Winners" << std::setw(35) << " |               Losers              |" << "\n";
  oss << "---------------------------------------------------------------------"
         "--------------------------------------|" << std::endl;
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
    float per = static_cast<float>(parties.at(i).second) / static_cast<float>(totalVotes) * 100.0;
    std::ostringstream tempStream;
    tempStream << std::fixed << std::setprecision(2) << per;
    temp = "Votes: " + std::to_string(parties.at(i).second) + "   (" + tempStream.str() + "%" + " of total)";

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
        table +="                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |\n";
      }

      // if there are winners left, we get their name, votes, and % of votes
      if (idx_W < numWinners.size()) {
        per = static_cast<float>(winners.at(numWinners.at(idx_W)).second.second) /
            static_cast<float>(totalVotes) * 100.0;
        tempStream.str("");
        tempStream.clear();
        tempStream << std::fixed << std::setprecision(2) << per;
        temp = "Votes: " + std::to_string(winners.at(numWinners.at(idx_W)).second.second) +
               "   (" + tempStream.str() + "%" + " of total)";

        oss << std::setw(35) << winners.at(numWinners.at(idx_W)).first.getName() << "|";
        oss2 << std::setw(35) << temp << "|";
      } else {
        oss << std::setw(35) << "" << "|";
        oss2 << std::setw(35) << "" << "|";
      }

      // if there are losers left, we get their name, votes, and % of votes
      if (idx_L < numLosers.size()) {
        per = static_cast<float>(candidates.at(numLosers.at(idx_L)).second.second) /
              static_cast<float>(totalVotes) * 100.0;
        tempStream.str("");
        tempStream.clear();
        tempStream << std::fixed << std::setprecision(2) << per;
        temp = "Votes: " + std::to_string(candidates.at(numLosers.at(idx_L)).second.second) +
            "   (" + tempStream.str() + "%" + " of total)";

        oss << std::setw(35) << candidates.at(numLosers.at(idx_L)).first.getName() << "|";
        oss2 << std::setw(35) << temp << "|";
      } else {
        oss << std::setw(35) << "" << "|";
        oss2 << std::setw(35) << "" << "|";
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

      oss << std::setw(35) << "" << "|";
      oss2 << std::setw(35) << "" << "|";
    }
    table += "-----------------------------------------------------------------------------------------------------------|\n";
  }

  result += table;

  return result;
}

// This function will build the string that will be displayed to the terminal, including 
// a table that shows which candidates won or loss, and their Party's total votes won.
std::string MVSystem::displayResults() {
  std::string result = "A MV election was run to fill " + std::to_string(getSeats()) +" seats. "
      "There were " + std::to_string(getVotesTotal()) + " votes. The results are below:\n\n";

  // We call our helper function to build the table of winning and losing candidates
  result += MVCandidates(candidates, winners, parties, partySeats, getVotesTotal());

  return result;
}

std::string MVSystem::auditResults() {
  std::string result = "MV Election Audit\n";
  // Add total seats and total votes to the audit report
  result += "There were " + std::to_string(parties.size()) + " parties in this election.\n";
  result += "Total Votes: " + std::to_string(getVotesTotal()) + "\n";
  result += "Total Seats: " + std::to_string(getSeats()) + "\n\n";

  std::cout << "\n\n\n";

  result += "-------------------------------------------------------------------------------------------------------------\n";
  result += "                                                         Seats                %" " of Vote\n";                 
  result += "               Parties               Votes               Won by                    to\n";                      
  result += "                                                         Party                %" " of Seats\n";                
  result += "-------------------------------------------------------------------------------------------------------------\n";

  std::ostringstream oss;
  float perOfVotes = 0.0;
  float perOfSeats = 0.0;
  for (size_t i = 0; i < parties.size(); i++) {
    oss.str("");
    oss.clear();
    
    perOfVotes = static_cast<float>(parties.at(i).second) / static_cast<float>(getVotesTotal()) * 100.0;
    perOfSeats = static_cast<float>(partySeats.at(i).second) / static_cast<float>(getSeats()) * 100.0;

    oss << "                  " <<std::left << std::setw(20) << parties.at(i).first.getName()
        << " " << std::setw(20) << parties.at(i).second << std::fixed
        << std::setw(17) << partySeats.at(i).second << std::fixed
        << "" << std::setprecision(2) << perOfVotes << "% / " << perOfSeats << "%\n";

    result += oss.str();
  }
  result += "-------------------------------------------------------------------------------------------------------------\n";
  result += "\n\nList of each party and their candidates, and the winners and losers.\n\n";
  result += MVCandidates(candidates, winners, parties, partySeats, getVotesTotal());

  return result;
}