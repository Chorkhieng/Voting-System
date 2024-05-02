#include "ElectionSystem.h"

int ElectionSystem::getSeats() const { return seats; }

void ElectionSystem::setSeats(int num) { this->seats = num; }

int ElectionSystem::getVotes() const { return votes; }

void ElectionSystem::setVotes(int num) { this->votes = num; }

int ElectionSystem::getCandidates() const { return candidates; }

void ElectionSystem::setCandidates(int num) { this->candidates = num; }

int ElectionSystem::getParties() const { return parties; }

void ElectionSystem::setParties(int num) { this->parties = num; }
