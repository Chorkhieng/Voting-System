#ifndef PARTY_H_
#define PARTY_H_
#include <functional>
#include <string>

/**
 * @brief Represents a Party in this election.
 *
 * A Party has a name. It can also be compared to another Party.
 */
class Party {
 public:
  /**
   * @brief Default constructor.
   * Initializes a Party with default values.
   */
  Party();

  /**
   * @brief Constructs an Party instance.
   *
   * Initializes the Party with a name.
   * @param[in] name Name of the Party.
   */
  explicit Party(std::string name);

  /**
   * @brief Method to get Party name.
   *
   * @return This Party's name.
   */
  std::string getName() const;

  /**
   * @brief Compares two Party instances for equality.
   *
   * Determines if the current Party instance is equal to another by comparing
   * their names.
   * @param[in] other The Party instance to compare with.
   * @return True if both Party instances have the same name, false otherwise.
   */
  bool operator==(const Party& other) const;

 private:
  std::string name;
};

#endif