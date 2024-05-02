#ifndef CANDIDATE_H_
#define CANDIDATE_H_

#include <string>

/**
 * @brief Represents an Candidate in this election.
 *
 * A Candidate has a name, and possibly and ID, in case there are
 * other Candidates with the same name.
 */
class Candidate {
 public:
  /**
   * @brief Default constructor.
   * Initializes a candidate with default values.
   */
  Candidate();

  /**
   * @brief Constructs an Candidate instance.
   *
   * Initializes the Candidate with a name and an ID, which defaults to 0.
   * @param[in] name Name of the Candidate.
   * @param[in] id ID of the Candidate, specified if there are Candidates with.
   * the same name.
   */
  Candidate(std::string name, int id = 0);

  /**
   * @brief Method to get Candidate name.
   *
   * @return This Candidate's name.
   */
  std::string getName() const;

  /**
   * @brief Method to get the Candidate ID.
   *
   * @return This Candidate's ID.
   */
  int getID() const; 

 private:  
  std::string name;
  int id;
};

#endif