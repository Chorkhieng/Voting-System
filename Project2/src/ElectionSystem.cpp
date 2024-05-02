#include "ElectionSystem.h"

int ElectionSystem::getSeats() const { return seats; }

void ElectionSystem::setSeats(int num) { this->seats = num; }



int ElectionSystem::getVotesCurrentBallot() const { return votes; }

void ElectionSystem::setVotesCurrentBallot(int num) { this->votes = num; }

int ElectionSystem::getVotesTotal() const { return totalVotes; }

void ElectionSystem::setVotesTotal(int num) { this->totalVotes += num; }



int ElectionSystem::getCandidates() const { return candidates; }

void ElectionSystem::setCandidates(int num) { this->candidates = num; }

int ElectionSystem::getParties() const { return parties; }

void ElectionSystem::setParties(int num) { this->parties = num; }
