#ifndef RESULTS_H_
#define RESULTS_H_

#include <string>

#include "CPLSystem.h"
#include "Candidate.h"
#include "ElectionSystem.h"
#include "OPLSystem.h"
#include "Party.h"

/**
 * @brief Produces aufit file and displays election results.
 *
 * This class is stores election results to produce an audit file, and displays
 * results to the terminal. It interacts with other classes to gather necessary
 * information.
 */
class Results {
 public:
  std::string fileName;

  /**
   * @brief Default constructor.
   */
  Results() = default;

  /**
   * @brief Virtual destructor.
   */
  virtual ~Results() {}

  /**
   * @brief Produces an audit file with election results.
   *
   * Produces a detailed audit file with the information it was given from
   * main or other classes.
   */
  void produceAudit();

  /**
   * @brief Displays summarized election results to the terminal.
   *
   * Prints to terminal the detailed information it was given from
   * main or other classes.
   */
  void displayToTerminal();

  /**
   * @brief Gets the audit information.
   * @return A string containing audit information.
   */
  std::string getAuditInfo();

  /**
   * @brief Sets the audit information.
   * @param[in] info A string containing audit info to be stored.
   */
  void setAuditInfo(std::string info);

  /**
   * @brief Gets the current display information for the terminal.
   * @return A string containing information for terminal display.
   */
  std::string getDisplayInfo();

  /**
   * @brief Sets the display information for the terminal.
   * @param[in] info A string containing information for terminal display.
   */
  void setDisplayInfo(std::string info);

 private:
  std::string auditInfo;
  std::string displayInfo;
};

#endif