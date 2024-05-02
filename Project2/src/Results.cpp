#include "Results.h"

#include <fstream>
#include <iostream>

void Results::produceAudit() {
  std::ofstream myfile("audit.txt", std::ofstream::trunc);
  if (!myfile.is_open()) {
    std::cout << "Failed to open audit file for writing header.\n";
    return;
  }

  myfile << "<<<<<<<< Audit Results for the Election >>>>>>>>\n";
  myfile << auditInfo;
  myfile.close();

  if (myfile.is_open()) {
    std::cout << "Failed to close audit file after writing header.\n";
    return;
  }
}

void Results::displayToTerminal() { std::cout << displayInfo; }

std::string Results::getAuditInfo() { return auditInfo; }

void Results::setAuditInfo(std::string info) { auditInfo = info; }

std::string Results::getDisplayInfo() { return displayInfo; }

void Results::setDisplayInfo(std::string info) { displayInfo = info; }