#include "Party.h"

Party::Party() : name("No Name") {  // member initializer
}

Party::Party(std::string name) : name(name) {}

std::string Party::getName() const { return name; }

bool Party::operator==(const Party& other) const { return name == other.name; }
