#ifndef TIEBREAKER_H_
#define TIEBREAKER_H_

#include <string>
#include <vector>

#include "CPLSystem.h"
#include "OPLSystem.h"

/**
 * @brief Resolves ties in election results.
 * 
 * @pre The TieBreaker class requires the CPLSystem and OPLSystem to be initialized
 * and loaded with election data before being utilized.
 *
 * Implements a mechanism to break ties that occur during the election, ensuring
 * a fair and deterministic outcome.
 */
class TieBreaker {
 public:
  std::string tieBreakerResult;
  // std::vector tiedCandidates;

  /**
   * @brief Default constructor.
   */
  TieBreaker() = default;

  /**
   * @brief Destructor.
   */
  ~TieBreaker() {}

  /**
   * @brief Breaks ties based on predefined criteria.
   * 
   * @pre The `numPeople` parameter must be greater than 0, and `numSeats` must be
   * less than or equal to the number of tied candidates or parties, but greater than 0.
   *
   * This function simulates a series of randomized selections to determine
   * tiebreakers among candidates. It uses the Mersenne Twister engine
   * (std::mt19937) seeded with a non-deterministic random device to ensure fair
   * and unpredictable outcomes. The method shuffles candidate indices and
   * assigns seats in each of over 200,000 iterations to simulate many
   * tie-breaking scenarios. Candidates with the most wins across these
   * simulations are selected to resolve the tie.
   *
   * Function was designed so that it can be used in diverse scenarios,
   * where candidates, parties, or both can be tied. This results in less code,
   * and more concise code.
   *
   * @param numPeople The number of parties/candidates involved in the tie.
   * @param numSeats The number of available seats.
   */
  void breakTies(int numPeople, int numSeats);
};

#endif