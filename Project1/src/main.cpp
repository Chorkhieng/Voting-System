#include <fstream>
#include <iostream>
#include <limits>
#include <string>

#include "CPLSystem.h"
#include "Candidate.h"
#include "ElectionSystem.h"
#include "OPLSystem.h"
#include "Party.h"
#include "Results.h"

// #include <format>
#include <cstdio>


// git pull --rebase

// clear; g++ -o election *.cpp ; ./election _cpl.txt

// clear; make clean; make run FILE=../testing/test_cpl.csv

// clear; make unittest


int main(int argc, char* argv[]) {
  std::ifstream file;
  std::string filename;

  if (argc > 1) {
    filename = argv[1];
    file.open(filename);
    if (!file.is_open()) {
      // Exit with if we can't open file
      std::cerr << "Error opening file: " << filename << std::endl;
      return 1;
    }
  } else {
    // No filename provided, ask the user in a loop
    while (true) {
      std::cout << "\nPlease enter a filename: ";
      std::cin >> filename;

      file.open(filename);
      // Check if file is successfully opened, exit if yes
      if (file.is_open()) {
        break;
      } else {
        std::cout << "Unable to open file. Please try again." << std::endl;
        // Clear flags and ignore the rest of the line to prevent infinite loops
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  }
  std::cout << "\n\n";
  std::string electionType;
  std::string line;
  int seats;
  int votes;
  int partiesCandidates;

  // Read election type
  std::getline(file, electionType);

  // Read num of seats
  std::getline(file, line);
  seats = std::stoi(line);

  // Read num of votes
  std::getline(file, line);
  votes = std::stoi(line);

  // Read number of parties/candidates
  std::getline(file, line);
  partiesCandidates = std::stoi(line);
  file.close();

  // Results result;
  std::string displayResult;
  std::string auditResult;

  if (electionType == "CPL") {
    CPLSystem election(filename);
    election.setSeats(seats);
    election.setVotes(votes);
    election.setParties(partiesCandidates);

    election.countVotes();
    election.allocateSeats();
    displayResult = election.displayResults();
    auditResult = election.auditResults();

  } else if (electionType == "OPL") {
    OPLSystem election(filename);
    election.setSeats(seats);
    election.setVotes(votes);
    election.setCandidates(partiesCandidates);

    election.countVotes();
    election.allocateSeats();
    displayResult = election.displayResults();
    auditResult = election.auditResults();
  } else {
    std::cout << "This type of election is not yet supported.\n";
  }

  // std::cout << "\n" << displayResult;

  // TODO: if txt file is saved in CRLF, then the first char doesn't get printed
  // to terminal and we need to remove possible "/r" fromthe string. CRLF is
  // made in a Windows environment, and Linux makes LF. I found this problem by
  // copying my widnows text file into SSH session....

  // TODO:      clear()   vectors

  // std::cout << std::format("{} {} {} {}", electionType, seats, votes,
  // partiesCandidates) << std::endl; printf("%s %d %d %d\n",
  // electionType.c_str(), seats, votes, partiesCandidates);

  // std::cout << electionType << " \n" << seats << " \n" << votes << " \n" <<
  // partiesCandidates << "\n";

  // CHORK's example for Results class

  Results result;
  result.setAuditInfo(auditResult);
  result.getAuditInfo();
  result.produceAudit();
  result.setDisplayInfo(displayResult);
  result.displayToTerminal();

  return 0;
}