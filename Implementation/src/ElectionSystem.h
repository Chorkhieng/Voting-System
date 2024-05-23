#ifndef ELECTIONSYSTEM_H_
#define ELECTIONSYSTEM_H_

/**
 * @brief Abstract base class for election systems.
 *
 * Provides interfaces for managing election data and defines election
 * behaviors for vote counting and seat allocation.
 */
class ElectionSystem {
 public:
  /**
   * @brief Default constructor.
   */
  ElectionSystem() = default;

  /**
   * @brief Virtual destructor.
   */
  virtual ~ElectionSystem() {}

  /**
   * @brief Get the number of seats.
   * @return Number of seats.
   */
  int getSeats() const;

  /**
   * @brief Set the number of seats.
   * @param num Number of seats.
   */
  void setSeats(int num);

  /**
   * @brief Get the number of votes in this current ballot.
   * @return Number of votes.
   */
  int getVotesCurrentBallot() const;

  /**
   * @brief Set the number of votes in this current ballot.
   * @param num Number of votes.
   */
  void setVotesCurrentBallot(int num);


  /**
   * @brief Get the number of votes from this whole election.
   * @return Number of votes.
   */
  int getVotesTotal() const;

  /**
   * @brief Set the number of votes from this whole election..
   * @param num Number of votes.
   */
  void setVotesTotal(int num);


  /**
   * @brief Get the number of candidates.
   * @return Number of candidates.
   */
  int getCandidates() const;

  /**
   * @brief Set the number of candidates.
   * @param num Number of candidates.
   */
  void setCandidates(int num);

  /**
   * @brief Get the number of parties.
   * @return Number of parties.
   */
  int getParties() const;

  /**
   * @brief Set the number of parties.
   * @param num Number of parties.
   */
  void setParties(int num);

  /**
   * @brief Reads the header of a file and processes the candidates and parties. 
   * Pure virtual function.
   */
  virtual void processCandidates() = 0;

  /**
   * @brief Counts the votes. Pure virtual function.
   */
  virtual void countVotes() = 0;


  /**
   * @brief Allocates the seats based on votes. Pure virtual function.
   */
  virtual void allocateSeats() = 0;

 protected:
  int seats;
  int votes;
  int totalVotes;
  int candidates;
  int parties;
};

#endif