#ifndef CPLSYSTEM_H_
#define CPLSYSTEM_H_

#include <algorithm>
#include <string>
#include <vector>

#include "Candidate.h"
#include "ElectionSystem.h"
#include "Party.h"

class Party;
class Candidate;

/**
 * @brief Represents an CPL (Closed Party List) election, where party
 * seats are awarded to candidates in the order they appear on the ballot.
 */
class CPLSystem : public ElectionSystem {
 public:
  /**
   * @brief Constructs an CPLSystem instance.
   *
   * Initializes the system with a ballot file taht will be read to build
   * party and candidate vectors and count votes.
   * @param[in] filename Name of the ballot file, defaults to "ballot.txt".
   */
  explicit CPLSystem(std::string filename = "ballot.txt");

  /**
   * @brief Destructor
   *
   * Cleans up dynamically allocated memory
   */
  virtual ~CPLSystem() override;


  /**
   * @brief Change the election's file that it will use to count votes.
   */
  void setFilename(const std::string& newFilename);

  /**
   * @brief Builds party list with a list of their candidates.
   *
   * Inherits from the abstract Class ElectionSystem, and overrides the method.
   * This method will read each line, take the Party, add it to "parties",
   * then read each Candidate on that line. Later this will help us allocate
   * the seats within the parties. At the same time while we read each Party,
   * we also add these parties to the "partyVotes", which will store their votes
   * in a piar. 
   */
  virtual void processCandidates() override;


  /**
   * @brief Counts the votes for each party.
   *
   * Inherits from the abstract Class ElectionSystem, and overrides the method.
   * To count votes, we read the votes in the file, and assign votes
   * based on Party index in the "partyVotes". This function can be called
   * for each ballot file, since the party structures are known.
   */
  virtual void countVotes() override;

  /**
   * @brief Allocates seats based on party total votes, and assigns winning
   * candidates based on their popularity within the party.
   *
   * Inherits from the abstract Class ElectionSystem, and overrides the method.
   * This function will find the quota to do the first seat allocation. It will
   * then move on to the second seat allocation, where it will see if there
   * are possible ties between the parties. Handling ties will be handled
   * by another class, this function can get the result. Since the order
   * of candidates in each party is predetermined, there is nothing more to do.
   */
  virtual void allocateSeats() override;

  /**
   * @brief This function builds an election result and a table.
   *
   * This function will use logic that loops through the "parties" vector
   * to build a table of the winners and losers. It will provide important
   *  info, like votes won, % of votes, and so on.
   *
   * @return When called, it will return a string with information like
   * the winners, the losers, votes for the party, as well as % of votes won.
   * This strign will be printed to the terminal.
   */
  std::string displayResults();

  /**
   * @brief This function builds an election result and a table.
   *
   * This function will use logic that loops through the "parties" vector
   * to build a table of the winners and losers. It will provide important
   *  info, like votes won, % of votes, and so on.
   *
   * @return When called, it will return a string with information like
   * the winners, the losers, votes for the party, as well as % of votes won.
   * This string will ultimately be written to an Audit file.
   */
  std::string auditResults();

 private:
  std::vector<std::pair<Party, std::vector<Candidate>>> parties;
  std::vector<std::pair<Party, int>> partyVotes;
  std::vector<std::pair<Party, int>> partySeats;
  std::string filename;
};

#endif