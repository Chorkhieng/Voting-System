#include "Candidate.h"

Candidate::Candidate() : name("No Name"), id(0) {}

Candidate::Candidate(std::string name, int id) : name(name), id(id) {}

std::string Candidate::getName() const { return name; }

int Candidate::getID() const { return id; }