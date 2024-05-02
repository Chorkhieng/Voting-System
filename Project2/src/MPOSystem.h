#ifndef MPOSYSTEM_H_
#define MPOSYSTEM_H_

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

#include "Candidate.h"
#include "ElectionSystem.h"
#include "Party.h"

class Party;
class Candidate;

/**
 * @brief Represents an MPO (Multiple Popularity Only) election, where candidates
 * are sorted by popularity within the party
 */
class MPOSystem : public ElectionSystem {
 public:
  /**
   * @brief Constructs an MPOSystem instance.
   *
   * Initializes the system with a ballot file taht will be read to build
   * party and candidate vectors and count votes.
   * @param[in] filename Name of the ballot file, defaults to "ballot.txt".
   */
  explicit MPOSystem(std::string filename = "ballot.txt");

  /**
   * @brief Destructor
   *
   * Cleans up dynamically allocated memory
   */
  virtual ~MPOSystem() override;


  /**
   * @brief Change the election's file that it will use to count votes.
   */
  void setFilename(const std::string& newFilename);

  /**
   * @brief Builds candidate & party list.
   *
   * Inherits from the abstract Class ElectionSystem, and overrides the method.
   * This method will read the Canidates and their Party, and make different
   * vaectors and pairs that will be used to hold votes for the Candidate
   * and the Party.
   */
  virtual void processCandidates() override;

  /**
   * @brief Counts the votes for the candidate.
   *
   * Inherits from the abstract Class ElectionSystem, and overrides the method.
   * This method will count the votes and assign them to the candidate.
   */
  virtual void countVotes() override;


/**
   * @brief Sums up this Party's total votes.
   *
   * Inherits from the abstract Class ElectionSystem, and overrides the method.
   * Loops through all Candidates, and sums up this Candidate's votes with
   * their Party's total votes. 
   */
  void giveVotesToParty();

  /**
   * @brief Allocates seats based on party total votes, and assigns winning
   * candidates based on their popularity within the party.
   *
   * Inherits from the abstract Class ElectionSystem, and overrides the method.
   * This function will find the quota to do the first seat allocation. It will
   * then move on to the second seat allocation, where it will see if there
   * are possible ties between the parties. Handling ties will be handled
   * by another class, this function can get the result. After all seats are
   * allocated to the parties, we then have to find the winning candidates
   * within the parties, and handle any ties between candidates of any
   * specific party. To make it easier to provide results, we will move
   * Candidate objects from the "candidates" vector into a vector of
   * winners, "winners".
   */
  virtual void allocateSeats() override;

  /**
   * @brief This function builds an election result and a table.
   *
   * This function will use logic that loops through our winners and
   * candidates vector to build a table of the winners and losers. It will
   * provide important info, like votes won, % of votes, and so on.
   *
   * @return When called, it will return a string with information like
   * the winners, the losers, votes for the previosuly mentioned and each
   * party, as well as % of votes won. This strign will be printed
   * to the terminal.
   */
  std::string displayResults();

  /**
   * @brief This function builds an election result and a table.
   *
   * This function will use logic that loops through our winners and
   * candidates vector to build a table of the winners and losers. It will
   * provide important info, like votes won, % of votes, and so on.
   *
   * @return When called, it will return a string with information like
   * the winners, the losers, votes for the previosuly mentioned and each
   * party, as well as % of votes won. This string will ultimately be
   * written to an Audit file.
   */
  std::string auditResults();

 private:
  std::vector<std::pair<Candidate, std::pair<Party, int>>> candidates;
  std::vector<std::pair<Candidate, std::pair<Party, int>>> winners;
  // std::unordered_map<Party, int> partyVotes;
  std::vector<std::pair<Party, int>> parties;
  std::vector<std::pair<Party, int>> partySeats;
  std::string filename;
};

#endif