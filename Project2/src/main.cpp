#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <sstream>

#include "CPLSystem.h"
#include "OPLSystem.h"
#include "MPOSystem.h"
#include "Party.h"
#include "Candidate.h"
#include "ElectionSystem.h"
#include "Results.h"
#include "MPOSystem.h"

// #include <format> 
#include <cstdio>
#include <stdio.h>

// git pull --rebase

// clear; g++ -o election *.cpp ; ./election _cpl.txt

// clear; make clean; make run FILES=../testing/test_cpl.csv
// clear; make clean; make run FILES="../testing/_cpl.csv ../testing/_cpl.csv"
// clear; make run FILES="../testing/m_cpl1.csv ../testing/m_cpl2.csv ../testing/m_cpl3.csv"
// clear; make run FILES="../testing/m_opl1.csv ../testing/m_opl2.csv ../testing/m_opl3.csv"


// clear; make run FILES="../testing/m_mpo1.csv ../testing/m_mpo2.csv ../testing/m_mpo3.csv"

// clear; make run FILES="../testing/m_mpo2.csv ../testing/m_mpo3.csv"
// clear; make run FILES="../testing/m_mpo1.csv  ../testing/m_mpo3.csv"
// clear; make run FILES="../testing/m_mpo1.csv ../testing/m_mpo2.csv"

// clear; make unittest


// clear; make clean; make run FILES=../testing/_mpo.csv


// Split string based on a delimiter
std::vector<std::string> splitFile(const std::string &s, char delimiter) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delimiter)) {
        elems.push_back(item);
    }
    return elems;
}

// Check if files can be opened, return a list of filenames that couldn't be opened
void tryToOpenFiles(std::vector<std::string>& filenames, std::vector<std::string>& successfulFiles, std::vector<std::string>& failedFiles) {
    for (const auto& filename : filenames) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            failedFiles.push_back(filename);
        } else {
            successfulFiles.push_back(filename);
        }
    }
}



int main(int argc, char* argv[]) {
  std::vector<std::string> allFilenames, successfulFiles, failedToOpen;

  // try to open given files, rememeber failedToOpen
  if (argc > 1) {
        // first argument is program name, so start at the second argument,
        // then go until the end of the past the last element of the array
        // using "argv + argc"
        allFilenames.assign(argv + 1, argv + argc);
    } else {
        std::string input;
        std::cout << "\nPlease enter filenames separated by spaces: ";
        std::getline(std::cin, input);
        allFilenames = splitFile(input, ' ');
    }

  tryToOpenFiles(allFilenames, successfulFiles, failedToOpen);

  // while there are still files that cannot be opened, ask user to retype file name
  while (!failedToOpen.empty()) {

    std::cout << "Unable to open the following files:\n";
    for (const auto& file : failedToOpen) {
        std::cout << file << std::endl;
    }

    std::vector<std::string> retryFilenames;

    std::cout << "Please re-enter the filenames for the files above, separated by spaces: ";
    std::string input;
    std::getline(std::cin, input);
    retryFilenames = splitFile(input, ' ');

   // Attempt to open the files from the retry list
    std::vector<std::string> newSuccessfulFiles, newFailedFiles;
    tryToOpenFiles(retryFilenames, newSuccessfulFiles, newFailedFiles);

    // Merge new successful files into the main successful files list
    successfulFiles.insert(successfulFiles.end(), newSuccessfulFiles.begin(), newSuccessfulFiles.end());

    // Prepare for the next iteration if there are still files that failed to open
    failedToOpen.clear();
    for (const auto& file : newFailedFiles) {
    if (std::find(successfulFiles.begin(), successfulFiles.end(), file) == successfulFiles.end()) {
        // Only re-add to failedToOpen if not found in successfulFiles
        failedToOpen.push_back(file);
        }
    }
  }

  std::cout << "\n\n";

  


  std::string electionType, line;
  int seats,  partiesCandidates;

  std::ifstream file;
  std::string filename;

  filename = successfulFiles.at(0);
  // filename ="../testing/_cpl0.csv";
  file.open(filename);

  // Read election type
  std::getline(file, electionType);

  // Results result;
  std::string displayResult;
  std::string auditResult;


  if (electionType == "CPL" || electionType == "OPL") { 
    // Read num of seats
    std::getline(file, line);
    seats = std::stoi(line);

    // Read num of votes
    std::getline(file, line);

    // Read number of parties/candidates
    std::getline(file, line);
    partiesCandidates = std::stoi(line);
    file.close(); 


    // Now we start to process elections with similar headers
    if (electionType == "CPL") {
      CPLSystem election(filename);
      election.setSeats(seats);
      election.setParties(partiesCandidates);
      election.processCandidates();

      for (const auto& file : successfulFiles) {
        std::cout << file << "\n";
        election.setFilename(file);
        election.countVotes();
      }

      election.allocateSeats();
      displayResult = election.displayResults();
      auditResult = election.auditResults();
      // std::cout<<"\n\n"<<auditResult << "\n\n";
    } else if (electionType == "OPL") {
      OPLSystem election(filename);
      election.setSeats(seats);
      election.setCandidates(partiesCandidates);

      election.processCandidates();
      for (const auto& file : successfulFiles) {
        std::cout << file << "\n";
        election.setFilename(file);
        election.countVotes();
      }
      election.giveVotesToParty();
      election.allocateSeats();
      displayResult = election.displayResults();
      auditResult = election.auditResults();
    }
  } 


  else if (electionType == "MPO" || electionType == "MV") { 
    // Read num of seats
    std::getline(file, line);
    seats = std::stoi(line);

    // Read number of candidates
    std::getline(file, line);
    partiesCandidates = std::stoi(line);

    // No need to read anything else
    file.close();


    // Now we start to process elections with similar headers
    if (electionType == "MPO") {
      // std::cout << "\n coming soon \n";
      MPOSystem election(filename);
      election.setSeats(seats);
      election.setParties(partiesCandidates);

      election.processCandidates();

      for (const auto& file : successfulFiles) {
        std::cout << file << "\n";
        election.setFilename(file);
        election.countVotes();
      }

      election.giveVotesToParty();
      election.allocateSeats();
      displayResult = election.displayResults();
      auditResult = election.auditResults();

      // std::cout<<"\n\n"<<auditResult << "\n\n";

    } else if (electionType == "MV") {

      std::cout << "\n coming soon \n";
      // MVSystem election(filename);
      // election.setSeats(seats);
      // election.setCandidates(partiesCandidates);

      // election.processCandidates();
      // for (const auto& file : successfulFiles) {
      //   std::cout << file << "\n";
      //   election.setFilename(file);
      //   election.countVotes();
      // }
      // election.giveVotesToParty();
      // election.allocateSeats();
      // displayResult = election.displayResults();
      // auditResult = election.auditResults();
    }
  } else {
    std::cout << "This type of election is not yet supported.\n";
  }

  Results result;
  result.setAuditInfo(auditResult);
  result.getAuditInfo();
  result.produceAudit();
  result.setDisplayInfo(displayResult);
  result.displayToTerminal();

  return 0;
}