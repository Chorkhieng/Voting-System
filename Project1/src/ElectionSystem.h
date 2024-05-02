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
   * @brief Get the number of votes.
   * @return Number of votes.
   */
  int getVotes() const;

  /**
   * @brief Set the number of votes.
   * @param num Number of votes.
   */
  void setVotes(int num);

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
  int candidates;
  int parties;
};

#endif