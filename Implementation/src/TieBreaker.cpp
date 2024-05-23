#include "TieBreaker.h"

#include <iostream>
#include <random>

void TieBreaker::breakTies(int numPeople, int numSeats) {
  tieBreakerResult.clear();

  // std::mt19937 is chosen for its superior quality of randomness.
  // It works with a wider range of applications requiring pseudorandom
  // number generation. This makes it a better choice for implementing
  // complex, statistically-driven functionality like simulating over
  // 200,000 iterations for fair tie-breaking in an election process.
  std::mt19937 rng(std::random_device{}());
  std::vector<int> wins(numPeople, 0);

  // Perform the random tie-breaking "numIterations" times
  const int numIterations = 200001;

  for (int i = 0; i < numIterations; ++i) {
    std::vector<int> tempSeats(numPeople, 0);

    // Fill with indices 0, 1, ..., numPeople - 1.
    std::iota(tempSeats.begin(), tempSeats.end(), 0);
    std::shuffle(tempSeats.begin(), tempSeats.end(), rng);

    for (int j = 0; j < numSeats; ++j) {
      wins[tempSeats[j]]++;
    }
  }

  // rate of win of each candidate
  std::cout << "\nOut of " << numIterations
            << " coin flips, the results of each candidate are below:\n";
  for (size_t i = 0; i < wins.size(); i++) {
    std::cout << wins.at(i) << "\n";
  }

  // Determine the winners based on who has the most wins
  for (int i = 0; i < numSeats; ++i) {
    auto maxIter = std::max_element(wins.begin(), wins.end());
    int winnerIndex = std::distance(wins.begin(), maxIter);

    // Add winner index to the result string.
    tieBreakerResult += std::to_string(winnerIndex) + " ";
    // Set the wins of this index to -1 to exclude from further consideration.
    *maxIter = -1;
  }

  // Remove the trailing space.
  if (!tieBreakerResult.empty()) {
    tieBreakerResult.pop_back();
  }

  std::cout << "Tie winners(s): " << tieBreakerResult << "\n\n";
}



// pre condition is that there are 2 or more people who are competing for 1 or more seats.

// No test for the first for loop, since the num of iterations is determined in the code
// Test the nested for loop

// Test the second for loop with the num of candidates

// Test the thrid for loop, with the num of sesats

// Test for the returned string is empty