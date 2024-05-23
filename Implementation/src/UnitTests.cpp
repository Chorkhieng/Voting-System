#include "Candidate.h"
#include "Party.h"
#include "TieBreaker.h"
#include "CPLSystem.h"
#include "ElectionSystem.h"
#include "OPLSystem.h"
#include "MPOSystem.h"
#include "Results.h"
#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <chrono>

//g++ -o UnitTests UnitTests.cpp Candidate.cpp Party.cpp TieBreaker.cpp CPLSystem.cpp -lgtest -lgtest_main -pthread


// ------------------------------------------------------------------------------------------------------------------------------------

//Test Cases for Candidate Class

// Test the default constructor of Candidate class
TEST(CandidateTest, DefaultConstructor) {
    Candidate candidate;
    EXPECT_EQ(candidate.getName(), "No Name");
    EXPECT_EQ(candidate.getID(), 0);
}

// Test the parameterized constructor of Candidate class
TEST(CandidateTest, ParameterizedConstructor) {
    Candidate candidate("John Doe", 123);
    EXPECT_EQ(candidate.getName(), "John Doe");
    EXPECT_EQ(candidate.getID(), 123);
}

// // Test getting the name of the candidate
TEST(CandidateTest, GetName) {
    Candidate candidate("Jane Smith", 456);
    EXPECT_EQ(candidate.getName(), "Jane Smith");
}

// // Test getting the ID of the candidate
TEST(CandidateTest, GetID) {
    Candidate candidate("Michael Johnson", 789);
    EXPECT_EQ(candidate.getID(), 789);
}



// ------------------------------------------------------------------------------------------------------------------------------------
// Test Cases for Party Class

// Test the default constructor of Party class
TEST(PartyTest, DefaultConstructor) {
    Party party;
    EXPECT_EQ(party.getName(), "No Name");
}

// // Test the parameterized constructor of Party class
TEST(PartyTest, ParameterizedConstructor) {
    Party party("Democratic Party");
    EXPECT_EQ(party.getName(), "Democratic Party");
}

// // Test getting the name of the party
TEST(PartyTest, GetName) {
    Party party("Republican Party");
    EXPECT_EQ(party.getName(), "Republican Party");
}

// // Test equality operator for Party class
TEST(PartyTest, EqualityOperator) {
    Party party1("Republican Party");
    Party party2("Republican Party");

    EXPECT_TRUE(party1 == party2);
}

TEST(PartyTest, EqualityOperatorTwo) {
    Party party1("Republican Party");
    Party party2("Democratic Party");

    EXPECT_FALSE(party1 == party2);
}



// ------------------------------------------------------------------------------------------------------------------------------------
//Test Cases for TieBreaker Class

// Test the default constructor of TieBreaker class
TEST(TieBreakerTest, DefaultConstructor) {
    TieBreaker tieBreaker;
    // Check if the tieBreakerResult is initially empty
    EXPECT_TRUE(tieBreaker.tieBreakerResult.empty());
}

// Test breaking ties when there's only one candidate
TEST(TieBreakerTest, BreakTies_OneCandidate) {
    TieBreaker tieBreaker;
    tieBreaker.breakTies(1, 1); // Only one candidate, one seat
    // In this case, there's only one candidate, so the result should be "0"
    EXPECT_EQ(tieBreaker.tieBreakerResult, "0");
}

// Test breaking ties when there's more than one candidate and seat
TEST(TieBreakerTest, BreakTies_MultipleCandidates) {
    TieBreaker tieBreaker;
    tieBreaker.breakTies(5, 2); // 5 candidates, 2 seats
    // This test doesn't check the specific result because it's randomized
    // It just checks if the result string is not empty
    EXPECT_FALSE(tieBreaker.tieBreakerResult.empty());
}


// ------------------------------------------------------------------------------------------------------------------------------------
// Test cases for Results class
TEST(ResultsTest, TestDidsplayAndAudit) {
    Results result;
    std::string st = " sdfsdfsfwe eofahonefo ejasejfao;eahwe;fe";
    std::string audit = "selfkalsdhahoidhoiqj;weeflhidhwelrlfdawdfldfldnskjai0o3528979754l43l45t woe3IHU##%)";
    result.setDisplayInfo(st);
    result.setAuditInfo(audit);

    EXPECT_EQ(result.getDisplayInfo(), st);
    EXPECT_EQ(result.getAuditInfo(), audit);
}

TEST(ResultsTest, TestEmpty) {
    Results result;
    std::string st = "";
    std::string audit = "";
    result.setDisplayInfo(st);
    result.setAuditInfo(audit);

    EXPECT_EQ(result.getDisplayInfo(), st);
    EXPECT_EQ(result.getAuditInfo(), audit);
}


// ------------------------------------------------------------------------------------------------------------------------------------
//Test Cases for CPLSystem Class to test result of the strings when no file is given
TEST(CPLSystemTest, TestConstructor) {
    CPLSystem election;
    std::string result = R"(A CPL election was run to fill 0 seats. There were 0 votes. The results are below:

  Party             |  Winners           |  Losers           |  Stats           
---------------------------------------------------------------------------------------------
)";

    EXPECT_EQ(election.displayResults(), result);
    EXPECT_EQ(election.getVotesTotal(), 0);
    EXPECT_EQ(election.getVotesCurrentBallot(), 0);
    EXPECT_EQ(election.getSeats(), 0);
    EXPECT_EQ(election.getParties(), 0);
    EXPECT_EQ(election.getCandidates(), 0);
}
TEST(CPLSystemTest, TestConstructorTwo) {
    CPLSystem election;
    std::string result = R"(
CPL Election Audit
There were 0 parties in this election.
Total Votes: 0
Total Seats: 0


Results of this CPL election: 

-------------------------------------------------------------------------------------------------------------
                                  First        Remaining        Second         Final         % of Vote
   Parties          Votes       Allocation       Votes        Allocation       Seat              to
                                 of Seats                      of Seats        Total         % of Seats
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------





These Candidates won a seat: 

  Party             |  Winners           |  Losers           |  Stats           
---------------------------------------------------------------------------------------------
)";

    EXPECT_EQ(election.auditResults(), result);
    EXPECT_EQ(election.getVotesTotal(), 0);
    EXPECT_EQ(election.getVotesCurrentBallot(), 0);
    EXPECT_EQ(election.getSeats(), 0);
    EXPECT_EQ(election.getCandidates(), 0);
    EXPECT_EQ(election.getParties(), 0);
}

//Test Cases for CPLSystem Class where file has 0 seats, votes, parties
TEST(CPLSystemTest, EmptyFile) {
    std::string line; 
    std::ifstream file;
    std::string filename = "../testing/_cpl0.csv";
    CPLSystem election(filename);

    file.open(filename);
    
    std::getline(file, line);
    std::getline(file, line);   // Read num of seats
    election.setSeats(std::stoi(line));

    // Read num of votes
    std::getline(file, line);   // Read num of votes
    election.setVotesTotal(std::stoi(line));
    election.setVotesCurrentBallot(std::stoi(line));

    std::getline(file, line);   // Read number of parties/candidates
    election.setParties(std::stoi(line));
    
    file.close();
    election.countVotes();
    election.allocateSeats();
    std::string terminalResult = election.displayResults();


    std::string expectedTerminalResults = R"(A CPL election was run to fill 0 seats. There were 0 votes. The results are below:

  Party             |  Winners           |  Losers           |  Stats           
---------------------------------------------------------------------------------------------
)";

    std::string expectedAuditResults = R"(
CPL Election Audit
There were 0 parties in this election.
Total Votes: 0
Total Seats: 0


Results of this CPL election: 

-------------------------------------------------------------------------------------------------------------
                                  First        Remaining        Second         Final         % of Vote
   Parties          Votes       Allocation       Votes        Allocation       Seat              to
                                 of Seats                      of Seats        Total         % of Seats
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------





These Candidates won a seat: 

  Party             |  Winners           |  Losers           |  Stats           
---------------------------------------------------------------------------------------------
)";

    EXPECT_EQ(election.displayResults(), expectedTerminalResults);
    EXPECT_EQ(election.auditResults(), expectedAuditResults);
    EXPECT_EQ(election.getVotesTotal(), 0);
    EXPECT_EQ(election.getSeats(), 0);
    EXPECT_EQ(election.getParties(), 0);
}


TEST(CPLSystemTest, DisplayResults) {
    std::string line; 
    std::ifstream file;
    std::string filename = "../testing/test_cpl.csv";
    CPLSystem election(filename);

    file.open(filename);
    
    std::getline(file, line);
    std::getline(file, line);   // Read num of seats
    election.setSeats(std::stoi(line));

    // Read num of votes
    std::getline(file, line);   // Read num of votes
    // election.setVotesTotal(std::stoi(line));
    election.setVotesCurrentBallot(std::stoi(line));

    std::getline(file, line);   // Read number of parties/candidates
    election.setParties(std::stoi(line));
    
    file.close();
    election.processCandidates();
    election.countVotes();
    election.allocateSeats();
    std::string terminalResult = election.displayResults();
    // std::cout << terminalResult;
    // std::cout << "   seats" << election.getSeats() << "\n";
    // std::cout << "   seats" << election.getSeats() << "\n";
    // std::cout << "   seats" << election.getSeats() << "\n";
    

    // Expected results substring for each party
    std::string expectedTerminalResults = R"(A CPL election was run to fill 3 seats. There were 60 votes. The results are below:

  Party             |  Winners           |  Losers           |  Stats           
---------------------------------------------------------------------------------------------
Democratic          |Joe                 |Sally               |Votes: 20
                    |                    |Ahmed               |That is 33.33% of votes.
---------------------------------------------------------------------------------------------
Republican          |Allen Joe           |Nikki               |Votes: 20
                    |                    |Taihui              |That is 33.33% of votes.
---------------------------------------------------------------------------------------------
Green               |Bethany             |                    |Votes: 20
                    |                    |                    |That is 33.33% of votes.
---------------------------------------------------------------------------------------------
)";

    // Check if the results string contains the expected substrings
    EXPECT_TRUE(terminalResult.find(expectedTerminalResults) != std::string::npos);
}

TEST(CPLSystemTest, AuditResults) {
    std::string line; 
    std::ifstream file;
    std::string filename = "../testing/test_cpl.csv";
    CPLSystem election(filename);

    file.open(filename);
    
    std::getline(file, line);
    std::getline(file, line);   // Read num of seats
    election.setSeats(std::stoi(line));

    // Read num of votes
    std::getline(file, line);   // Read num of votes
    // election.setVotesTotal(std::stoi(line));
    election.setVotesCurrentBallot(std::stoi(line));

    std::getline(file, line);   // Read number of parties/candidates
    election.setParties(std::stoi(line));
    
    file.close();
    election.processCandidates();
    election.countVotes();
    election.allocateSeats();
    std::string auditResult = election.auditResults();

    // Expected results substring for each party
    std::string expectedAuditResults = R"(
CPL Election Audit
There were 3 parties in this election.
Total Votes: 60
Total Seats: 3


Results of this CPL election: 

-------------------------------------------------------------------------------------------------------------
                                  First        Remaining        Second         Final         % of Vote
   Parties          Votes       Allocation       Votes        Allocation       Seat              to
                                 of Seats                      of Seats        Total         % of Seats
-------------------------------------------------------------------------------------------------------------
Democratic          20             1              0              0              1         33.33% / 33.33%
Republican          20             1              0              0              1         33.33% / 33.33%
Green               20             1              0              0              1         33.33% / 33.33%
-------------------------------------------------------------------------------------------------------------





These Candidates won a seat: 

  Party             |  Winners           |  Losers           |  Stats           
---------------------------------------------------------------------------------------------
Democratic          |Joe                 |Sally               |Votes: 20
                    |                    |Ahmed               |That is 33.33% of votes.
---------------------------------------------------------------------------------------------
Republican          |Allen Joe           |Nikki               |Votes: 20
                    |                    |Taihui              |That is 33.33% of votes.
---------------------------------------------------------------------------------------------
Green               |Bethany             |                    |Votes: 20
                    |                    |                    |That is 33.33% of votes.
---------------------------------------------------------------------------------------------)";

    // Check if the results string contains the expected substrings
    EXPECT_TRUE(auditResult.find(expectedAuditResults) != std::string::npos);
}

TEST(CPLSystemTest, TestWithBallot) {
    CPLSystem election("../testing/test_cpl.csv");
    election.setSeats(3);
    election.setVotesTotal(60);
    election.setVotesCurrentBallot(60);
    election.setParties(3);

    EXPECT_EQ(election.getSeats(), 3);
    EXPECT_EQ(election.getVotesTotal(), 60);
    EXPECT_EQ(election.getVotesCurrentBallot(), 60);
    EXPECT_EQ(election.getParties(), 3);
}







// ------------------------------------------------------------------------------------------------------------------------------------
// Test cases for OPLSystem Class

TEST(OPLSystemTest, TestConstructor) {
    OPLSystem election;
    std::string result = R"(A OPL election was run to fill 0 seats. There were 0 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
)";

    EXPECT_EQ(election.displayResults(), result);
    EXPECT_EQ(election.getVotesTotal(), 0);
    EXPECT_EQ(election.getVotesCurrentBallot(), 0);
    EXPECT_EQ(election.getCandidates(), 0);
    EXPECT_EQ(election.getParties(), 0);
}


TEST(OPLSystemTest, TestConstructorTwo) {
    OPLSystem election;
    std::string result = R"(OPL Election Audit
There were 0 parties in this election.
Total Votes: 0
Total Seats: 0

-------------------------------------------------------------------------------------------------------------
                                 First         Remaining      Second          Final         % of Vote
   Parties        Votes        Allocation        Votes       Allocation       Seat              to
                                of Seats                      of Seats        Total         % of Seats
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
)";

    EXPECT_EQ(election.auditResults(), result);
    EXPECT_EQ(election.getSeats(), 0);
    EXPECT_EQ(election.getVotesTotal(), 0);
    EXPECT_EQ(election.getVotesCurrentBallot(), 0);
    EXPECT_EQ(election.getCandidates(), 0);
    EXPECT_EQ(election.getParties(), 0);
}


//Test Cases for CPLSystem Class where file has 0 seats, votes, parties
TEST(OPLSystemTest, EmptyFile) {
    std::string line; 
    std::ifstream file;
    std::string filename = "../testing/_opl0.csv";
    OPLSystem election(filename);

    file.open(filename);
    
    std::getline(file, line);
    std::getline(file, line);   // Read num of seats
    election.setSeats(std::stoi(line));

    // Read num of votes
    std::getline(file, line);   // Read num of votes
    election.setVotesTotal(std::stoi(line));
    election.setVotesCurrentBallot(std::stoi(line));

    std::getline(file, line);   // Read number of parties/candidates
    election.setCandidates(std::stoi(line));
    
    file.close();
    election.countVotes();
    election.allocateSeats();
    std::string terminalResult = election.displayResults();


    std::string expectedTerminalResults = R"(A OPL election was run to fill 0 seats. There were 0 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
)";

    std::string expectedAuditResults = R"(OPL Election Audit
There were 0 parties in this election.
Total Votes: 0
Total Seats: 0

-------------------------------------------------------------------------------------------------------------
                                 First         Remaining      Second          Final         % of Vote
   Parties        Votes        Allocation        Votes       Allocation       Seat              to
                                of Seats                      of Seats        Total         % of Seats
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
)";

    EXPECT_EQ(election.displayResults(), expectedTerminalResults);
    EXPECT_EQ(election.auditResults(), expectedAuditResults);
    EXPECT_EQ(election.getVotesTotal(), 0);
    EXPECT_EQ(election.getVotesCurrentBallot(), 0);
    EXPECT_EQ(election.getSeats(), 0);
    EXPECT_EQ(election.getCandidates(), 0);
}












// test with file name
TEST(OPLSystemTest, TestWithBallot) {
    std::string line; 
    std::ifstream file;
    std::string filename = "../testing/_opl.csv";
    OPLSystem election(filename);

    file.open(filename);
    
    std::getline(file, line);
    std::getline(file, line);   // Read num of seats
    election.setSeats(std::stoi(line));

    // Read num of votes
    std::getline(file, line);   // Read num of votes
    election.setVotesCurrentBallot(std::stoi(line));

    std::getline(file, line);   // Read number of parties/candidates
    election.setCandidates(std::stoi(line));
    
    file.close();
    election.processCandidates();
    election.countVotes();
    election.giveVotesToParty();
    election.allocateSeats();

    std::string terminalResult = election.displayResults();
    std::string auditResult = election.auditResults();


    std::string expectedTerminalResults = R"(A OPL election was run to fill 2 seats. There were 285 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
Republican                         |Alawa                              |Etta                               |
Votes: 132   (46.32% of total)     |Votes: 67   (23.51% of total)      |Votes: 65   (22.81% of total)      |
-----------------------------------------------------------------------------------------------------------|
Independent1                       |Sasha                              |                                   |
Votes: 87   (30.53% of total)      |Votes: 87   (30.53% of total)      |                                   |
-----------------------------------------------------------------------------------------------------------|
Democrat                           |                                   |Pike Jpe                           |
Votes: 66   (23.16% of total)      |                                   |Votes: 44   (15.44% of total)      |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Lucy                               |
                                   |                                   |Votes: 22   (7.72% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Beiye                              |
                                   |                                   |Votes: 0   (0.00% of total)        |
-----------------------------------------------------------------------------------------------------------|
)";


    std::string expectedAuditResults = R"(OPL Election Audit
There were 3 parties in this election.
Total Votes: 285
Total Seats: 2

-------------------------------------------------------------------------------------------------------------
                                 First         Remaining      Second          Final         % of Vote
   Parties        Votes        Allocation        Votes       Allocation       Seat              to
                                of Seats                      of Seats        Total         % of Seats
-------------------------------------------------------------------------------------------------------------
Republican          132            0              132            1              1         46.32% / 50.00%
Independent1        87             0              87             1              1         30.53% / 50.00%
Democrat            66             0              66             0              0         23.16% / 0.00%
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
Republican                         |Alawa                              |Etta                               |
Votes: 132   (46.32% of total)     |Votes: 67   (23.51% of total)      |Votes: 65   (22.81% of total)      |
-----------------------------------------------------------------------------------------------------------|
Independent1                       |Sasha                              |                                   |
Votes: 87   (30.53% of total)      |Votes: 87   (30.53% of total)      |                                   |
-----------------------------------------------------------------------------------------------------------|
Democrat                           |                                   |Pike Jpe                           |
Votes: 66   (23.16% of total)      |                                   |Votes: 44   (15.44% of total)      |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Lucy                               |
                                   |                                   |Votes: 22   (7.72% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Beiye                              |
                                   |                                   |Votes: 0   (0.00% of total)        |
-----------------------------------------------------------------------------------------------------------|
)";
    
    EXPECT_EQ(election.displayResults(), expectedTerminalResults);
    EXPECT_EQ(election.auditResults(), expectedAuditResults);
    EXPECT_EQ(election.getSeats(), 2);
    EXPECT_EQ(election.getCandidates(), 6);
    EXPECT_EQ(election.getVotesTotal(), 285);
    EXPECT_EQ(election.getVotesCurrentBallot(), 285);
}


//MPO Unit Testing cases:

TEST(MPOSystemTest, TestConstructor) {
    MPOSystem election;
    std::string result = R"(A MPO election was run to fill 0 seats. There were 0 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
)";

    EXPECT_EQ(election.displayResults(), result);
    EXPECT_EQ(election.getVotesTotal(), 0);
    EXPECT_EQ(election.getVotesCurrentBallot(), 0);
    EXPECT_EQ(election.getCandidates(), 0);
    EXPECT_EQ(election.getParties(), 0);
}



TEST(MPOSystemTest, TestWithBallot) {
   
    std::string filename = "../testing/_mpo.csv";
    MPOSystem election(filename);

    // election.processCandidates();
    // election.countVotes();
    // election.allocateSeats();

    // Expecting a specific formatted string output from displayResults which details the election results
    std::string expectedResults = R"(A MPO election was run to fill 0 seats. There were 0 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
)";
    ASSERT_EQ(election.displayResults(), expectedResults);
    EXPECT_EQ(election.getVotesCurrentBallot(), 0);
    EXPECT_EQ(election.getVotesTotal(), 0);
    EXPECT_EQ(election.getSeats(), 0);
    EXPECT_EQ(election.getParties(), 0);
    EXPECT_EQ(election.getCandidates(), 0); 
}




// ------------------------------------------------------------------------------------------------------------------------------------
// Test cases for MPOSystem Class


TEST(MPOSystemTest, NoVotesCounted) {
    // Testing the system with a setup where no votes are cast
    std::string filename = "../testing/_mpo_empty.csv";  // This file should ideally contain zero votes
    MPOSystem election(filename);

    election.processCandidates(); // Should not find any candidates to process
    election.countVotes();       // Should count zero votes
    election.allocateSeats();    // Should not allocate any seats

    std::string expectedResults = R"(A MPO election was run to fill 0 seats. There were 0 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
)";
    ASSERT_EQ(election.displayResults(), expectedResults);
}




TEST(MPOSystemTest, TestConstructorTwo) {
    MPOSystem election;
    std::string result = R"(MPO Election Audit
There were 0 parties in this election.
Total Votes: 0
Total Seats: 0

-------------------------------------------------------------------------------------------------------------
                                                         Seats                % of Vote
               Parties               Votes               Won by                    to
                                                         Party                % of Seats
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
)";

    EXPECT_EQ(election.auditResults(), result);
    EXPECT_EQ(election.getSeats(), 0);
    EXPECT_EQ(election.getVotesTotal(), 0);
    EXPECT_EQ(election.getVotesCurrentBallot(), 0);
    EXPECT_EQ(election.getCandidates(), 0);
    EXPECT_EQ(election.getParties(), 0);
}


TEST(MPOSystemTest, TestWithBallot2) {
    std::string filename = "../testing/_mpo1.csv";
    std::string electionType, line;
    int seats,  partiesCandidates;
    std::ifstream file;

    file.open(filename);
    std::getline(file, line);    // election type
    std::getline(file, line);    // Read num of seats
    seats = std::stoi(line);
    std::getline(file, line);    // Read number of candidates
    partiesCandidates = std::stoi(line);

    file.close(); // No need to read anything else


    MPOSystem election(filename);
    std::cout << "seats; " << seats << "  " << partiesCandidates << "\n";

    election.setSeats(seats);
    election.setCandidates(partiesCandidates);

    election.processCandidates();
    election.countVotes();
    election.giveVotesToParty();
    election.allocateSeats();


    // Expecting a specific formatted string output from displayResults which details the election results
    std::string expectedResults = R"(A MPO election was run to fill 2 seats. There were 8 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |Pike                               |                                   |
Votes: 5   (62.50% of total)       |Votes: 3   (37.50% of total)       |                                   |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |Foster                             |                                   |
                                   |Votes: 2   (25.00% of total)       |                                   |
-----------------------------------------------------------------------------------------------------------|
R                                  |                                   |Borg                               |
Votes: 2   (25.00% of total)       |                                   |Votes: 1   (12.50% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Jones                              |
                                   |                                   |Votes: 1   (12.50% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Deutsch                            |
                                   |                                   |Votes: 0   (0.00% of total)        |
-----------------------------------------------------------------------------------------------------------|
I                                  |                                   |Smith                              |
Votes: 1   (12.50% of total)       |                                   |Votes: 1   (12.50% of total)       |
-----------------------------------------------------------------------------------------------------------|
)";

std::string expectedAudit = R"(MPO Election Audit
There were 3 parties in this election.
Total Votes: 8
Total Seats: 2

-------------------------------------------------------------------------------------------------------------
                                                         Seats                % of Vote
               Parties               Votes               Won by                    to
                                                         Party                % of Seats
-------------------------------------------------------------------------------------------------------------
                  D                    5                   2                62.50% / 100.00%
                  R                    2                   0                25.00% / 0.00%
                  I                    1                   0                12.50% / 0.00%
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |Pike                               |                                   |
Votes: 5   (62.50% of total)       |Votes: 3   (37.50% of total)       |                                   |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |Foster                             |                                   |
                                   |Votes: 2   (25.00% of total)       |                                   |
-----------------------------------------------------------------------------------------------------------|
R                                  |                                   |Borg                               |
Votes: 2   (25.00% of total)       |                                   |Votes: 1   (12.50% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Jones                              |
                                   |                                   |Votes: 1   (12.50% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Deutsch                            |
                                   |                                   |Votes: 0   (0.00% of total)        |
-----------------------------------------------------------------------------------------------------------|
I                                  |                                   |Smith                              |
Votes: 1   (12.50% of total)       |                                   |Votes: 1   (12.50% of total)       |
-----------------------------------------------------------------------------------------------------------|
)";
    ASSERT_EQ(election.displayResults(), expectedResults);
    ASSERT_EQ(election.auditResults(), expectedAudit);
    EXPECT_EQ(election.getVotesTotal(), 8);
    EXPECT_EQ(election.getVotesCurrentBallot(), 8);
    EXPECT_EQ(election.getSeats(), 2);
    EXPECT_EQ(election.getCandidates(), 6);
    EXPECT_EQ(election.getParties(), 0);
}




bool isHeaderCorrect(const std::string& header) {
    // Simulate checking the header
    // In practice, this would check for the expected format of the MV file header
    return header == "MV";
}


int countVotesInFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    int voteCount = 0;

    // Assuming first line is header
    std::getline(file, line);
    if (!isHeaderCorrect(line)) {
        return -1;  // Indicate header check failed
    }

    // Count non-header lines
    while (std::getline(file, line)) {
        std::size_t index = line.find("1");
        if (index > 0) {  // Assuming every non-empty line represents a vote
            voteCount++;
        }
    }

    return voteCount;
}
// Test to check if a single MV file's header is correct and votes are counted
TEST(MVSystemTest, SingleFileProcessing) {
    std::string filename = "../testing/_mv.csv";
    std::ifstream file;
    file.open(filename);
    std::string electionType,  line;
    int seats, votes, partiesCandidates;

    std::getline(file, electionType);

    std::getline(file, line);
    seats = std::stoi(line);

    // Read number of parties/candidates
    std::getline(file, line);
    partiesCandidates = std::stoi(line);
    std::getline(file, line);

    std::getline(file, line);
    votes = std::stoi(line);

    file.close();


    EXPECT_EQ(electionType, "MV");
    EXPECT_EQ(seats, 3);
    EXPECT_EQ(partiesCandidates, 6);
    EXPECT_EQ(votes, 9);
}

// Test to check if multiple MV files' headers are correct and votes are tallied
TEST(MVSystemTest, MultipleFileProcessing) {
    std::vector<std::string> filenames = {
        "../testing/m_mv1.csv",
        "../testing/m_mv2.csv",
        "../testing/m_mv3.csv"
    };
    for (const auto& filename : filenames) {
        int votes = countVotesInFile(filename);
        ASSERT_NE(votes, -1) << "Header check failed for file: " << filename;
        ASSERT_GT(votes, 0) << "No votes counted in file: " << filename;
    }
}





int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}