#include "Candidate.h"
#include "Party.h"
#include "TieBreaker.h"
#include "CPLSystem.h"
#include "MPOSystem.h"
#include "ElectionSystem.h"
#include "OPLSystem.h"
#include "MPOSystem.h"
#include "Results.h"
#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>


TEST(CPLSystemTest, TestRusultsAndAudit) {
    std::ifstream file;
    std::string filename = "../testing/test_cpl.csv";
    std::string electionType;
    std::string line;
    int seats;
    int votes;
    int partiesCandidates;

    file.open(filename);
    
    std::getline(file, electionType);

    std::getline(file, line);
    seats = std::stoi(line);

    // Read num of votes
    std::getline(file, line);
    votes = std::stoi(line);

    // Read number of parties/candidates
    std::getline(file, line);
    partiesCandidates = std::stoi(line);
    file.close();

    CPLSystem election(filename);
    election.setSeats(seats);
    election.setVotesCurrentBallot(votes);
    election.setParties(partiesCandidates);

    
    // CPLSystem cplSystem("test_cpl.csv");
    election.processCandidates();
    election.countVotes();
    election.allocateSeats();
    std::string terminalResult = election.displayResults();
    std::string auditResult = election.auditResults();

    // std::cout << terminalResult << "\n";

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
    EXPECT_TRUE(terminalResult.find(expectedTerminalResults) != std::string::npos);
    EXPECT_TRUE(auditResult.find(expectedAuditResults) != std::string::npos);
}


TEST(OPLSystemTest, RusultsAndAudit) {
    std::ifstream file;
    std::string filename = "../testing/_opl.csv";
    std::string electionType;
    std::string line;
    int seats;
    int votes;
    int partiesCandidates;

    file.open(filename);
    
    std::getline(file, electionType);

    std::getline(file, line);
    seats = std::stoi(line);
 
    // Read num of votes
    std::getline(file, line);
    votes = std::stoi(line);

    // Read number of parties/candidates
    std::getline(file, line);
    partiesCandidates = std::stoi(line);
    file.close();
    
    OPLSystem election(filename);
   
    election.setSeats(seats);
    
    // election.setVotesCurrentBallot(votes);
    election.setCandidates(partiesCandidates);

    election.processCandidates();
    election.countVotes();

    election.giveVotesToParty();
    election.allocateSeats();
    std::string terminalResult = election.displayResults();
    std::string auditResult = election.auditResults();

    // std::cout << "\n\n\n"<< auditResult << "\n\n\n";

    // Expected results substring for each party
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
-----------------------------------------------------------------------------------------------------------|)";

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

    // Check if the results string contains the expected substrings
    EXPECT_TRUE(terminalResult.find(expectedTerminalResults) != std::string::npos);
    EXPECT_TRUE(auditResult.find(expectedAuditResults) != std::string::npos);
}


TEST(CPLSystemTest, CPL_Tie) {
    std::ifstream file;
    std::string filename = "../testing/_cpl.csv";
    std::string electionType;
    std::string line;
    int seats;
    int votes;
    int partiesCandidates;

    file.open(filename);
    
    std::getline(file, electionType);

    std::getline(file, line);
    seats = std::stoi(line);

    // Read num of votes
    std::getline(file, line);
    votes = std::stoi(line);

    // Read number of parties/candidates
    std::getline(file, line);
    partiesCandidates = std::stoi(line);
    file.close();

    CPLSystem election(filename);
    election.setSeats(seats);
    election.setVotesCurrentBallot(votes);
    election.setParties(partiesCandidates);

    
    // CPLSystem cplSystem("test_cpl.csv");
    election.processCandidates();
    election.countVotes();
    election.allocateSeats();
    std::string terminalResult = election.displayResults();
    std::string auditResult = election.auditResults();

    // std::cout << "@" << auditResult <<"@";

    // Expected results substring for each party
    std::string expectedTerminalResults = R"(A CPL election was run to fill 3 seats. There were 60 votes. The results are below:

  Party             |  Winners           |  Losers           |  Stats           
---------------------------------------------------------------------------------------------
Democratic          |Joe                 |Sally               |Votes: 23
                    |                    |Ahmed               |That is 38.33% of votes.
---------------------------------------------------------------------------------------------
Republican          |Allen Joe           |Nikki               |Votes: 23
                    |                    |Taihui              |That is 38.33% of votes.
---------------------------------------------------------------------------------------------
New Wave            |Sarah               |                    |Votes: 4
                    |                    |                    |That is 6.67% of votes.
---------------------------------------------------------------------------------------------
Reform              |                    |Xinyue              |Votes: 4
                    |                    |Nikita              |That is 6.67% of votes.
---------------------------------------------------------------------------------------------
Green               |                    |Bethany             |Votes: 3
                    |                    |                    |That is 5.00% of votes.
---------------------------------------------------------------------------------------------
Independent         |                    |Mike                |Votes: 3
                    |                    |                    |That is 5.00% of votes.
---------------------------------------------------------------------------------------------)";
    std::string expectedTerminalResults2 = R"(A CPL election was run to fill 3 seats. There were 60 votes. The results are below:

  Party             |  Winners           |  Losers           |  Stats           
---------------------------------------------------------------------------------------------
Democratic          |Joe                 |Sally               |Votes: 23
                    |                    |Ahmed               |That is 38.33% of votes.
---------------------------------------------------------------------------------------------
Republican          |Allen Joe           |Nikki               |Votes: 23
                    |                    |Taihui              |That is 38.33% of votes.
---------------------------------------------------------------------------------------------
New Wave            |                    |Sarah               |Votes: 4
                    |                    |                    |That is 6.67% of votes.
---------------------------------------------------------------------------------------------
Reform              |Xinyue              |Nikita              |Votes: 4
                    |                    |                    |That is 6.67% of votes.
---------------------------------------------------------------------------------------------
Green               |                    |Bethany             |Votes: 3
                    |                    |                    |That is 5.00% of votes.
---------------------------------------------------------------------------------------------
Independent         |                    |Mike                |Votes: 3
                    |                    |                    |That is 5.00% of votes.
---------------------------------------------------------------------------------------------)";
   
   
    std::string expectedAuditResults = R"(CPL Election Audit
There were 6 parties in this election.
Total Votes: 60
Total Seats: 3


Results of this CPL election: 

-------------------------------------------------------------------------------------------------------------
                                  First        Remaining        Second         Final         % of Vote
   Parties          Votes       Allocation       Votes        Allocation       Seat              to
                                 of Seats                      of Seats        Total         % of Seats
-------------------------------------------------------------------------------------------------------------
Democratic          23             1              3              0              1         38.33% / 33.33%
Republican          23             1              3              0              1         38.33% / 33.33%
New Wave            4              0              4              0              0         6.67% / 0.00%
Reform              4              0              4              1              1         6.67% / 33.33%
Green               3              0              3              0              0         5.00% / 0.00%
Independent         3              0              3              0              0         5.00% / 0.00%
-------------------------------------------------------------------------------------------------------------





These Candidates won a seat: 

  Party             |  Winners           |  Losers           |  Stats           
---------------------------------------------------------------------------------------------
Democratic          |Joe                 |Sally               |Votes: 23
                    |                    |Ahmed               |That is 38.33% of votes.
---------------------------------------------------------------------------------------------
Republican          |Allen Joe           |Nikki               |Votes: 23
                    |                    |Taihui              |That is 38.33% of votes.
---------------------------------------------------------------------------------------------
New Wave            |                    |Sarah               |Votes: 4
                    |                    |                    |That is 6.67% of votes.
---------------------------------------------------------------------------------------------
Reform              |Xinyue              |Nikita              |Votes: 4
                    |                    |                    |That is 6.67% of votes.
---------------------------------------------------------------------------------------------
Green               |                    |Bethany             |Votes: 3
                    |                    |                    |That is 5.00% of votes.
---------------------------------------------------------------------------------------------
Independent         |                    |Mike                |Votes: 3
                    |                    |                    |That is 5.00% of votes.
---------------------------------------------------------------------------------------------)";
    std::string expectedAuditResults2 = R"(CPL Election Audit
There were 6 parties in this election.
Total Votes: 60
Total Seats: 3


Results of this CPL election: 

-------------------------------------------------------------------------------------------------------------
                                  First        Remaining        Second         Final         % of Vote
   Parties          Votes       Allocation       Votes        Allocation       Seat              to
                                 of Seats                      of Seats        Total         % of Seats
-------------------------------------------------------------------------------------------------------------
Democratic          23             1              3              0              1         38.33% / 33.33%
Republican          23             1              3              0              1         38.33% / 33.33%
New Wave            4              0              4              1              1         6.67% / 33.33%
Reform              4              0              4              0              0         6.67% / 0.00%
Green               3              0              3              0              0         5.00% / 0.00%
Independent         3              0              3              0              0         5.00% / 0.00%
-------------------------------------------------------------------------------------------------------------





These Candidates won a seat: 

  Party             |  Winners           |  Losers           |  Stats           
---------------------------------------------------------------------------------------------
Democratic          |Joe                 |Sally               |Votes: 23
                    |                    |Ahmed               |That is 38.33% of votes.
---------------------------------------------------------------------------------------------
Republican          |Allen Joe           |Nikki               |Votes: 23
                    |                    |Taihui              |That is 38.33% of votes.
---------------------------------------------------------------------------------------------
New Wave            |Sarah               |                    |Votes: 4
                    |                    |                    |That is 6.67% of votes.
---------------------------------------------------------------------------------------------
Reform              |                    |Xinyue              |Votes: 4
                    |                    |Nikita              |That is 6.67% of votes.
---------------------------------------------------------------------------------------------
Green               |                    |Bethany             |Votes: 3
                    |                    |                    |That is 5.00% of votes.
---------------------------------------------------------------------------------------------
Independent         |                    |Mike                |Votes: 3
                    |                    |                    |That is 5.00% of votes.
---------------------------------------------------------------------------------------------)";


    EXPECT_TRUE(terminalResult.find(expectedTerminalResults) != std::string::npos ||
                terminalResult.find(expectedTerminalResults2) != std::string::npos);
    EXPECT_TRUE(auditResult.find(expectedAuditResults) != std::string::npos ||
                auditResult.find(expectedAuditResults2) != std::string::npos);
}


// CPL System Test with multiple files
TEST(CPLSystemTest, CPL_multiple_files) {
    std::ifstream file;
    std::vector<std::string> filenames = {"../testing/_cpl.csv", "../testing/_cpl3.csv", "../testing/_cpl2.csv"};
    std::string electionType;
    std::string line;
    int seats;
    int votes;
    int partiesCandidates;

    file.open(filenames[0]);
    
    std::getline(file, electionType);

    std::getline(file, line);
    seats = std::stoi(line);

    
    std::getline(file, line);  // Read num of votes

    // Read number of parties/candidates
    std::getline(file, line);
    partiesCandidates = std::stoi(line);
    file.close();
    
    CPLSystem election(filenames[0]);
   
    election.setSeats(seats);
    election.setParties(partiesCandidates);
    election.processCandidates();


    for (const auto& file : filenames) {
        // std::cout << file << "\n";
        election.setFilename(file);
        election.countVotes();
    }

        
    // election.giveVotesToParty();
    election.allocateSeats();
    
    std::string terminalResult = election.displayResults();
    std::string auditResult = election.auditResults();

    // std::cout  << "\n\n"<< election.auditResults() << "\n\n";

    
    std::string expectedTerminalResults = R"(A CPL election was run to fill 3 seats. There were 100089 votes. The results are below:

  Party             |  Winners           |  Losers           |  Stats           
---------------------------------------------------------------------------------------------
Democratic          |Joe                 |Sally               |Votes: 38027
                    |                    |Ahmed               |That is 37.99% of votes.
---------------------------------------------------------------------------------------------
Republican          |Allen Joe           |Nikki               |Votes: 23028
                    |                    |Taihui              |That is 23.01% of votes.
---------------------------------------------------------------------------------------------
New Wave            |Sarah               |                    |Votes: 21009
                    |                    |                    |That is 20.99% of votes.
---------------------------------------------------------------------------------------------
Reform              |                    |Xinyue              |Votes: 12009
                    |                    |Nikita              |That is 12.00% of votes.
---------------------------------------------------------------------------------------------
Green               |                    |Bethany             |Votes: 6008
                    |                    |                    |That is 6.00% of votes.
---------------------------------------------------------------------------------------------
Independent         |                    |Mike                |Votes: 8
                    |                    |                    |That is 0.01% of votes.
---------------------------------------------------------------------------------------------)";
    
    std::string expectedAuditResults = R"(CPL Election Audit
There were 6 parties in this election.
Total Votes: 100089
Total Seats: 3


Results of this CPL election: 

-------------------------------------------------------------------------------------------------------------
                                  First        Remaining        Second         Final         % of Vote
   Parties          Votes       Allocation       Votes        Allocation       Seat              to
                                 of Seats                      of Seats        Total         % of Seats
-------------------------------------------------------------------------------------------------------------
Democratic          38027          1              4664           0              1         37.99% / 33.33%
Republican          23028          0              23028          1              1         23.01% / 33.33%
New Wave            21009          0              21009          1              1         20.99% / 33.33%
Reform              12009          0              12009          0              0         12.00% / 0.00%
Green               6008           0              6008           0              0         6.00% / 0.00%
Independent         8              0              8              0              0         0.01% / 0.00%
-------------------------------------------------------------------------------------------------------------





These Candidates won a seat: 

  Party             |  Winners           |  Losers           |  Stats           
---------------------------------------------------------------------------------------------
Democratic          |Joe                 |Sally               |Votes: 38027
                    |                    |Ahmed               |That is 37.99% of votes.
---------------------------------------------------------------------------------------------
Republican          |Allen Joe           |Nikki               |Votes: 23028
                    |                    |Taihui              |That is 23.01% of votes.
---------------------------------------------------------------------------------------------
New Wave            |Sarah               |                    |Votes: 21009
                    |                    |                    |That is 20.99% of votes.
---------------------------------------------------------------------------------------------
Reform              |                    |Xinyue              |Votes: 12009
                    |                    |Nikita              |That is 12.00% of votes.
---------------------------------------------------------------------------------------------
Green               |                    |Bethany             |Votes: 6008
                    |                    |                    |That is 6.00% of votes.
---------------------------------------------------------------------------------------------
Independent         |                    |Mike                |Votes: 8
                    |                    |                    |That is 0.01% of votes.
---------------------------------------------------------------------------------------------)";

    EXPECT_TRUE(terminalResult.find(expectedTerminalResults) != std::string::npos);
    EXPECT_TRUE((auditResult.find(expectedAuditResults) != std::string::npos));
    EXPECT_EQ(election.getVotesTotal(), 100089);
    EXPECT_EQ(election.getVotesCurrentBallot(), 100000);
    EXPECT_EQ(election.getSeats(), 3);
    EXPECT_EQ(election.getCandidates(), 0);
    EXPECT_EQ(election.getParties(), 6);
}


// CPL System Test with multiple files
TEST(CPLSystemTest, CPL_multiple_files_ties) {
    std::ifstream file;
    std::vector<std::string> filenames = {"../testing/_cpl_tie.csv", "../testing/_cpl_tie2.csv",};
    std::string electionType;
    std::string line;
    int seats;
    int votes;
    int partiesCandidates;

    file.open(filenames[0]);
    
    std::getline(file, electionType);

    std::getline(file, line);
    seats = std::stoi(line);

    
    std::getline(file, line);  // Read num of votes

    // Read number of parties/candidates
    std::getline(file, line);
    partiesCandidates = std::stoi(line);
    file.close();
    
    CPLSystem election(filenames[0]);
   
    election.setSeats(seats);
    election.setParties(partiesCandidates);
    election.processCandidates();


    for (const auto& file : filenames) {
        // std::cout << file << "\n";
        election.setFilename(file);
        election.countVotes();
    }

        
    // election.giveVotesToParty();
    election.allocateSeats();
    
    // std::string terminalResult = election.displayResults();
    // std::string auditResult = election.auditResults();

    // std::cout  << "\n\n"<< election.auditResults() << "\n\n";

    
    std::string expectedResult = R"(A CPL election was run to fill 3 seats. There were 66 votes. The results are below:

  Party             |  Winners           |  Losers           |  Stats           
---------------------------------------------------------------------------------------------
Democratic          |Joe                 |Sally               |Votes: 24
                    |                    |Ahmed               |That is 36.36% of votes.
---------------------------------------------------------------------------------------------
Republican          |Allen               |Nikki               |Votes: 24
                    |                    |Taihui              |That is 36.36% of votes.
---------------------------------------------------------------------------------------------
New Wave            |                    |Sarah               |Votes: 5
                    |                    |                    |That is 7.58% of votes.
---------------------------------------------------------------------------------------------
Reform              |Xinyue              |Nikita              |Votes: 5
                    |                    |                    |That is 7.58% of votes.
---------------------------------------------------------------------------------------------
Green               |                    |Bethany             |Votes: 4
                    |                    |                    |That is 6.06% of votes.
---------------------------------------------------------------------------------------------
Independent         |                    |Mike                |Votes: 4
                    |                    |                    |That is 6.06% of votes.
---------------------------------------------------------------------------------------------
)";
 std::string expectedResult2 = R"(A CPL election was run to fill 3 seats. There were 66 votes. The results are below:

  Party             |  Winners           |  Losers           |  Stats           
---------------------------------------------------------------------------------------------
Democratic          |Joe                 |Sally               |Votes: 24
                    |                    |Ahmed               |That is 36.36% of votes.
---------------------------------------------------------------------------------------------
Republican          |Allen               |Nikki               |Votes: 24
                    |                    |Taihui              |That is 36.36% of votes.
---------------------------------------------------------------------------------------------
New Wave            |Sarah               |                    |Votes: 5
                    |                    |                    |That is 7.58% of votes.
---------------------------------------------------------------------------------------------
Reform              |                    |Xinyue              |Votes: 5
                    |                    |Nikita              |That is 7.58% of votes.
---------------------------------------------------------------------------------------------
Green               |                    |Bethany             |Votes: 4
                    |                    |                    |That is 6.06% of votes.
---------------------------------------------------------------------------------------------
Independent         |                    |Mike                |Votes: 4
                    |                    |                    |That is 6.06% of votes.
---------------------------------------------------------------------------------------------
)";
    
    std::string expectedAudit = R"(
CPL Election Audit
There were 6 parties in this election.
Total Votes: 66
Total Seats: 3


Results of this CPL election: 

-------------------------------------------------------------------------------------------------------------
                                  First        Remaining        Second         Final         % of Vote
   Parties          Votes       Allocation       Votes        Allocation       Seat              to
                                 of Seats                      of Seats        Total         % of Seats
-------------------------------------------------------------------------------------------------------------
Democratic          24             1              2              0              1         36.36% / 33.33%
Republican          24             1              2              0              1         36.36% / 33.33%
New Wave            5              0              5              0              0         7.58% / 0.00%
Reform              5              0              5              1              1         7.58% / 33.33%
Green               4              0              4              0              0         6.06% / 0.00%
Independent         4              0              4              0              0         6.06% / 0.00%
-------------------------------------------------------------------------------------------------------------





These Candidates won a seat: 

  Party             |  Winners           |  Losers           |  Stats           
---------------------------------------------------------------------------------------------
Democratic          |Joe                 |Sally               |Votes: 24
                    |                    |Ahmed               |That is 36.36% of votes.
---------------------------------------------------------------------------------------------
Republican          |Allen               |Nikki               |Votes: 24
                    |                    |Taihui              |That is 36.36% of votes.
---------------------------------------------------------------------------------------------
New Wave            |                    |Sarah               |Votes: 5
                    |                    |                    |That is 7.58% of votes.
---------------------------------------------------------------------------------------------
Reform              |Xinyue              |Nikita              |Votes: 5
                    |                    |                    |That is 7.58% of votes.
---------------------------------------------------------------------------------------------
Green               |                    |Bethany             |Votes: 4
                    |                    |                    |That is 6.06% of votes.
---------------------------------------------------------------------------------------------
Independent         |                    |Mike                |Votes: 4
                    |                    |                    |That is 6.06% of votes.
---------------------------------------------------------------------------------------------
)";

 std::string expectedAudit2 = R"(
CPL Election Audit
There were 6 parties in this election.
Total Votes: 66
Total Seats: 3


Results of this CPL election: 

-------------------------------------------------------------------------------------------------------------
                                  First        Remaining        Second         Final         % of Vote
   Parties          Votes       Allocation       Votes        Allocation       Seat              to
                                 of Seats                      of Seats        Total         % of Seats
-------------------------------------------------------------------------------------------------------------
Democratic          24             1              2              0              1         36.36% / 33.33%
Republican          24             1              2              0              1         36.36% / 33.33%
New Wave            5              0              5              1              1         7.58% / 33.33%
Reform              5              0              5              0              0         7.58% / 0.00%
Green               4              0              4              0              0         6.06% / 0.00%
Independent         4              0              4              0              0         6.06% / 0.00%
-------------------------------------------------------------------------------------------------------------





These Candidates won a seat: 

  Party             |  Winners           |  Losers           |  Stats           
---------------------------------------------------------------------------------------------
Democratic          |Joe                 |Sally               |Votes: 24
                    |                    |Ahmed               |That is 36.36% of votes.
---------------------------------------------------------------------------------------------
Republican          |Allen               |Nikki               |Votes: 24
                    |                    |Taihui              |That is 36.36% of votes.
---------------------------------------------------------------------------------------------
New Wave            |Sarah               |                    |Votes: 5
                    |                    |                    |That is 7.58% of votes.
---------------------------------------------------------------------------------------------
Reform              |                    |Xinyue              |Votes: 5
                    |                    |Nikita              |That is 7.58% of votes.
---------------------------------------------------------------------------------------------
Green               |                    |Bethany             |Votes: 4
                    |                    |                    |That is 6.06% of votes.
---------------------------------------------------------------------------------------------
Independent         |                    |Mike                |Votes: 4
                    |                    |                    |That is 6.06% of votes.
---------------------------------------------------------------------------------------------
)";


    std::cout << "\n\n\n"<< election.auditResults() << "\n\n\n";


    // EXPECT_TRUE(terminalResult.find(expectedTerminalResults) != std::string::npos);
    // EXPECT_TRUE((auditResult.find(expectedAuditResults) != std::string::npos));

    // ASSERT_TRUE(election.displayResults() == expectedResult);
    ASSERT_TRUE(election.displayResults() == expectedResult || election.displayResults() == expectedResult2);
    ASSERT_TRUE(election.auditResults() == expectedAudit || election.auditResults() == expectedAudit2);

    // ASSERT_EQ(election.displayResults(), expectedResult);


    bool condition1 = (election.auditResults() == expectedAudit);
    bool condition2 = (election.auditResults() == expectedAudit2);

    EXPECT_TRUE(condition1 || condition2);


    
    EXPECT_EQ(election.getVotesTotal(), 66);
    EXPECT_EQ(election.getVotesCurrentBallot(), 6);
    EXPECT_EQ(election.getSeats(), 3);
    EXPECT_EQ(election.getCandidates(), 0);
    EXPECT_EQ(election.getParties(), 6);
}




// -----------------------------------------------------------------------------------------------------------------------------------------------------------

TEST(OPLSystemTest, OPL_Tie) {
    std::ifstream file;
    std::string filename = "../testing/_opl5.csv";
    std::string electionType;
    std::string line;
    int seats;
    int votes;
    int partiesCandidates;

    file.open(filename);
    
    std::getline(file, electionType);

    std::getline(file, line);
    seats = std::stoi(line);

    // Read num of votes
    std::getline(file, line);
    votes = std::stoi(line);

    // Read number of parties/candidates
    std::getline(file, line);
    partiesCandidates = std::stoi(line);
    file.close();
    
    OPLSystem election(filename);
   
    election.setSeats(seats);
    
    
    // election.setVotesCurrentBallot(votes);
    election.setCandidates(partiesCandidates);

    election.processCandidates();
    election.countVotes();
    election.giveVotesToParty();
    election.allocateSeats();
    
    std::string terminalResult = election.displayResults();
    std::string auditResult = election.auditResults();

    // std::cout << "\n\n\n"<< auditResult << "\n\n\n";


    std::string expectedTerminalResults = R"(A OPL election was run to fill 2 seats. There were 15 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
Democrat                           |Pike                               |Lucy                               |
Votes: 4   (26.67% of total)       |Votes: 2   (13.33% of total)       |Votes: 2   (13.33% of total)       |
-----------------------------------------------------------------------------------------------------------|
Star                               |Brook                              |Tom                                |
Votes: 9   (60.00% of total)       |Votes: 9   (60.00% of total)       |Votes: 0   (0.00% of total)        |
-----------------------------------------------------------------------------------------------------------|
Republican                         |                                   |Etta                               |
Votes: 2   (13.33% of total)       |                                   |Votes: 1   (6.67% of total)        |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Alawa                              |
                                   |                                   |Votes: 1   (6.67% of total)        |
-----------------------------------------------------------------------------------------------------------|)";
    std::string expectedTerminalResults2 = R"(A OPL election was run to fill 2 seats. There were 15 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
Democrat                           |Lucy                               |Pike                               |
Votes: 4   (26.67% of total)       |Votes: 2   (13.33% of total)       |Votes: 2   (13.33% of total)       |
-----------------------------------------------------------------------------------------------------------|
Star                               |Brook                              |Tom                                |
Votes: 9   (60.00% of total)       |Votes: 9   (60.00% of total)       |Votes: 0   (0.00% of total)        |
-----------------------------------------------------------------------------------------------------------|
Republican                         |                                   |Etta                               |
Votes: 2   (13.33% of total)       |                                   |Votes: 1   (6.67% of total)        |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Alawa                              |
                                   |                                   |Votes: 1   (6.67% of total)        |
-----------------------------------------------------------------------------------------------------------|)";
    
    
    std::string expectedAuditResults = R"(OPL Election Audit
There were 3 parties in this election.
Total Votes: 15
Total Seats: 2

-------------------------------------------------------------------------------------------------------------
                                 First         Remaining      Second          Final         % of Vote
   Parties        Votes        Allocation        Votes       Allocation       Seat              to
                                of Seats                      of Seats        Total         % of Seats
-------------------------------------------------------------------------------------------------------------
Democrat            4              0              4              1              1         26.67% / 50.00%
Star                9              1              2              0              1         60.00% / 50.00%
Republican          2              0              2              0              0         13.33% / 0.00%
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
Democrat                           |Pike                               |Lucy                               |
Votes: 4   (26.67% of total)       |Votes: 2   (13.33% of total)       |Votes: 2   (13.33% of total)       |
-----------------------------------------------------------------------------------------------------------|
Star                               |Brook                              |Tom                                |
Votes: 9   (60.00% of total)       |Votes: 9   (60.00% of total)       |Votes: 0   (0.00% of total)        |
-----------------------------------------------------------------------------------------------------------|
Republican                         |                                   |Etta                               |
Votes: 2   (13.33% of total)       |                                   |Votes: 1   (6.67% of total)        |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Alawa                              |
                                   |                                   |Votes: 1   (6.67% of total)        |
-----------------------------------------------------------------------------------------------------------|
)";
    std::string expectedAuditResults2 = R"(OPL Election Audit
There were 3 parties in this election.
Total Votes: 15
Total Seats: 2

-------------------------------------------------------------------------------------------------------------
                                 First         Remaining      Second          Final         % of Vote
   Parties        Votes        Allocation        Votes       Allocation       Seat              to
                                of Seats                      of Seats        Total         % of Seats
-------------------------------------------------------------------------------------------------------------
Democrat            4              0              4              1              1         26.67% / 50.00%
Star                9              1              2              0              1         60.00% / 50.00%
Republican          2              0              2              0              0         13.33% / 0.00%
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
Democrat                           |Lucy                               |Pike                               |
Votes: 4   (26.67% of total)       |Votes: 2   (13.33% of total)       |Votes: 2   (13.33% of total)       |
-----------------------------------------------------------------------------------------------------------|
Star                               |Brook                              |Tom                                |
Votes: 9   (60.00% of total)       |Votes: 9   (60.00% of total)       |Votes: 0   (0.00% of total)        |
-----------------------------------------------------------------------------------------------------------|
Republican                         |                                   |Etta                               |
Votes: 2   (13.33% of total)       |                                   |Votes: 1   (6.67% of total)        |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Alawa                              |
                                   |                                   |Votes: 1   (6.67% of total)        |
-----------------------------------------------------------------------------------------------------------|
)";

    EXPECT_TRUE(terminalResult.find(expectedTerminalResults) != std::string::npos ||
                terminalResult.find(expectedTerminalResults2) != std::string::npos);
    EXPECT_TRUE((auditResult.find(expectedAuditResults) != std::string::npos) ||
                (auditResult.find(expectedAuditResults2) != std::string::npos));
}


// OPL system test for multiple files
TEST(OPLSystemTest, OPL_multiple_files) {
    std::ifstream file;
    std::vector<std::string> filenames = {"../testing/_opl.csv", "../testing/_opl3.csv", "../testing/_opl4.csv"};
    std::string electionType;
    std::string line;
    int seats;
    int votes;
    int partiesCandidates;

    file.open(filenames[0]);
    
    std::getline(file, electionType);

    std::getline(file, line);
    seats = std::stoi(line);

    // Read num of votes
    std::getline(file, line);
    votes = std::stoi(line);

    // Read number of parties/candidates
    std::getline(file, line);
    partiesCandidates = std::stoi(line);
    file.close();
    
    OPLSystem election(filenames[0]);
   
    election.setSeats(seats);
    
    
    // election.setVotesCurrentBallot(votes);
    election.setCandidates(partiesCandidates);

    election.processCandidates();

    for (const auto& file : filenames) {
      // std::cout << file << "\n";
      election.setFilename(file);
      election.countVotes();
    }

    election.giveVotesToParty();
    election.allocateSeats();
    
    std::string terminalResult = election.displayResults();
    std::string auditResult = election.auditResults();

    // std::cout << "\n\n\n"<< auditResult << "\n\n\n";


    std::string expectedTerminalResults = R"(A OPL election was run to fill 2 seats. There were 299 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
Republican                         |Etta                               |Alawa                              |
Votes: 136   (45.48% of total)     |Votes: 69   (23.08% of total)      |Votes: 67   (22.41% of total)      |
-----------------------------------------------------------------------------------------------------------|
Independent1                       |Sasha                              |                                   |
Votes: 87   (29.10% of total)      |Votes: 87   (29.10% of total)      |                                   |
-----------------------------------------------------------------------------------------------------------|
Democrat                           |                                   |Pike Jpe                           |
Votes: 76   (25.42% of total)      |                                   |Votes: 44   (14.72% of total)      |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Lucy                               |
                                   |                                   |Votes: 26   (8.70% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Beiye                              |
                                   |                                   |Votes: 6   (2.01% of total)        |
-----------------------------------------------------------------------------------------------------------|
)";
    
    

    
    std::string expectedAuditResults = R"(OPL Election Audit
There were 3 parties in this election.
Total Votes: 299
Total Seats: 2

-------------------------------------------------------------------------------------------------------------
                                 First         Remaining      Second          Final         % of Vote
   Parties        Votes        Allocation        Votes       Allocation       Seat              to
                                of Seats                      of Seats        Total         % of Seats
-------------------------------------------------------------------------------------------------------------
Republican          136            0              136            1              1         45.48% / 50.00%
Independent1        87             0              87             1              1         29.10% / 50.00%
Democrat            76             0              76             0              0         25.42% / 0.00%
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
Republican                         |Etta                               |Alawa                              |
Votes: 136   (45.48% of total)     |Votes: 69   (23.08% of total)      |Votes: 67   (22.41% of total)      |
-----------------------------------------------------------------------------------------------------------|
Independent1                       |Sasha                              |                                   |
Votes: 87   (29.10% of total)      |Votes: 87   (29.10% of total)      |                                   |
-----------------------------------------------------------------------------------------------------------|
Democrat                           |                                   |Pike Jpe                           |
Votes: 76   (25.42% of total)      |                                   |Votes: 44   (14.72% of total)      |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Lucy                               |
                                   |                                   |Votes: 26   (8.70% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Beiye                              |
                                   |                                   |Votes: 6   (2.01% of total)        |
-----------------------------------------------------------------------------------------------------------|
)";

    ASSERT_EQ(election.auditResults(), expectedAuditResults);

    ASSERT_TRUE(election.displayResults() == expectedTerminalResults);
    ASSERT_TRUE(election.auditResults() == expectedAuditResults);

    EXPECT_EQ(election.getVotesTotal(), 299);
    EXPECT_EQ(election.getVotesCurrentBallot(), 8);
    EXPECT_EQ(election.getSeats(), 2);
    EXPECT_EQ(election.getCandidates(), 6);
    EXPECT_EQ(election.getParties(), 0);
}



// MPO System test
TEST(MPOSystemTest, MPO_Tie) {
    std::ifstream file;
    std::string filename = "../testing/_mpo_tie.csv";
    std::string electionType;
    std::string line;
    int seats;
    int votes;
    int partiesCandidates;

    file.open(filename);
    
    std::getline(file, electionType);

    std::getline(file, line);
    seats = std::stoi(line);

    // Read number of parties/candidates
    std::getline(file, line);
    partiesCandidates = std::stoi(line);
    file.close();
    
    MPOSystem election(filename);
   
    election.setSeats(seats);
    
    
    // election.setVotesCurrentBallot(votes);
    election.setCandidates(partiesCandidates);

    election.processCandidates();
    election.countVotes();
    election.giveVotesToParty();
    election.allocateSeats();
    
    std::string terminalResult = election.displayResults();
    std::string auditResult = election.auditResults();

    // std::cout << "audit MPO after this line!\n";
    // std::cout << auditResult;
    // std::cout << "display MPO after this line!\n";
    // std::cout << terminalResult;


    std::string expectedTerminalResults = R"(A MPO election was run to fill 2 seats. There were 56 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |                                   |Pike                               |
Votes: 27   (48.21% of total)      |                                   |Votes: 10   (17.86% of total)      |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Foster                             |
                                   |                                   |Votes: 9   (16.07% of total)       |
-----------------------------------------------------------------------------------------------------------|
R                                  |Borg                               |Jones                              |
Votes: 19   (33.93% of total)      |Votes: 14   (25.00% of total)      |Votes: 9   (16.07% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Deutsch                            |
                                   |                                   |Votes: 4   (7.14% of total)        |
-----------------------------------------------------------------------------------------------------------|
I                                  |Smith                              |                                   |
Votes: 10   (17.86% of total)      |Votes: 10   (17.86% of total)      |                                   |
-----------------------------------------------------------------------------------------------------------|
)";
    std::string expectedTerminalResults2 = R"(A MPO election was run to fill 2 seats. There were 56 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |Pike                               |Foster                             |
Votes: 27   (48.21% of total)      |Votes: 10   (17.86% of total)      |Votes: 9   (16.07% of total)       |
-----------------------------------------------------------------------------------------------------------|
R                                  |Borg                               |Jones                              |
Votes: 19   (33.93% of total)      |Votes: 14   (25.00% of total)      |Votes: 9   (16.07% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Deutsch                            |
                                   |                                   |Votes: 4   (7.14% of total)        |
-----------------------------------------------------------------------------------------------------------|
I                                  |                                   |Smith                              |
Votes: 10   (17.86% of total)      |                                   |Votes: 10   (17.86% of total)      |
-----------------------------------------------------------------------------------------------------------|
)";
    
    
    std::string expectedAuditResults = R"(OPL Election Audit
There were 3 parties in this election.
Total Votes: 6
Total Seats: 2

-------------------------------------------------------------------------------------------------------------
                                 First         Remaining      Second          Final         % of Vote
   Parties        Votes        Allocation        Votes       Allocation       Seat              to
                                of Seats                      of Seats        Total         % of Seats
-------------------------------------------------------------------------------------------------------------
                    4              1              1              1              2         66.67% / 100.00%
56                  1              0              1              0              0         16.67% / 0.00%
1                   1              0              1              0              0         16.67% / 0.00%
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
                                   |                                   |1                                  |
Votes: 4   (66.67% of total)       |Votes: 1   (16.67% of total)       |Votes: 2   (33.33% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |                                   |
                                   |                                   |Votes: 1   (16.67% of total)       |
-----------------------------------------------------------------------------------------------------------|
56                                 |                                   |                                   |
Votes: 1   (16.67% of total)       |                                   |Votes: 1   (16.67% of total)       |
-----------------------------------------------------------------------------------------------------------|
1                                  |                                   |                                   |
Votes: 1   (16.67% of total)       |Votes: 0   (0.00% of total)        |Votes: 1   (16.67% of total)       |
-----------------------------------------------------------------------------------------------------------|
)";
    std::string expectedAuditResults2 = R"(MPO Election Audit
There were 3 parties in this election.
Total Votes: 56
Total Seats: 2

-------------------------------------------------------------------------------------------------------------
                                                         Seats                % of Vote
               Parties               Votes               Won by                    to
                                                         Party                % of Seats
-------------------------------------------------------------------------------------------------------------
                  R                    27                  0                48.21% / 0.00%
                  D                    19                  1                33.93% / 50.00%
                  I                    10                  1                17.86% / 50.00%
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |                                   |Pike                               |
Votes: 27   (48.21% of total)      |                                   |Votes: 10   (17.86% of total)      |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Foster                             |
                                   |                                   |Votes: 9   (16.07% of total)       |
-----------------------------------------------------------------------------------------------------------|
R                                  |Borg                               |Jones                              |
Votes: 19   (33.93% of total)      |Votes: 14   (25.00% of total)      |Votes: 9   (16.07% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Deutsch                            |
                                   |                                   |Votes: 4   (7.14% of total)        |
-----------------------------------------------------------------------------------------------------------|
I                                  |Smith                              |                                   |
Votes: 10   (17.86% of total)      |Votes: 10   (17.86% of total)      |                                   |
-----------------------------------------------------------------------------------------------------------|
)";

    std::string expectedAuditResults3 = R"(MPO Election Audit
There were 3 parties in this election.
Total Votes: 56
Total Seats: 2

-------------------------------------------------------------------------------------------------------------
                                                         Seats                % of Vote
               Parties               Votes               Won by                    to
                                                         Party                % of Seats
-------------------------------------------------------------------------------------------------------------
                  R                    27                  1                48.21% / 50.00%
                  D                    19                  1                33.93% / 50.00%
                  I                    10                  0                17.86% / 0.00%
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |Pike                               |Foster                             |
Votes: 27   (48.21% of total)      |Votes: 10   (17.86% of total)      |Votes: 9   (16.07% of total)       |
-----------------------------------------------------------------------------------------------------------|
R                                  |Borg                               |Jones                              |
Votes: 19   (33.93% of total)      |Votes: 14   (25.00% of total)      |Votes: 9   (16.07% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Deutsch                            |
                                   |                                   |Votes: 4   (7.14% of total)        |
-----------------------------------------------------------------------------------------------------------|
I                                  |                                   |Smith                              |
Votes: 10   (17.86% of total)      |                                   |Votes: 10   (17.86% of total)      |
-----------------------------------------------------------------------------------------------------------|)";

    EXPECT_TRUE(terminalResult.find(expectedTerminalResults) != std::string::npos ||
                terminalResult.find(expectedTerminalResults2) != std::string::npos);
    EXPECT_TRUE((auditResult.find(expectedAuditResults) != std::string::npos) ||
                (auditResult.find(expectedAuditResults2) != std::string::npos) ||
                (auditResult.find(expectedAuditResults3) != std::string::npos));
}


// MPO System Test with 100000 votes
TEST(MPOSystemTest, MPO_100000_Votes) {
    std::ifstream file;
    std::string filename = "../testing/_mpo3.csv";
    std::string electionType;
    std::string line;
    int seats;
    int votes;
    int partiesCandidates;

    file.open(filename);
    
    std::getline(file, electionType);

    std::getline(file, line);
    seats = std::stoi(line);

    // Read number of parties/candidates
    std::getline(file, line);
    partiesCandidates = std::stoi(line);
    file.close();
    
    MPOSystem election(filename);
   
    election.setSeats(seats);
    
    
    // election.setVotesCurrentBallot(votes);
    election.setCandidates(partiesCandidates);

    election.processCandidates();
    election.countVotes();
    election.giveVotesToParty();
    election.allocateSeats();
    
    std::string terminalResult = election.displayResults();
    std::string auditResult = election.auditResults();
    
    // std::cout << "audit MPO after this line!\n";
    // std::cout << auditResult;
    // std::cout << "display MPO after this line!\n";
    // std::cout << terminalResult;


    std::string expectedTerminalResults = R"(A MPO election was run to fill 2 seats. There were 100000 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |Pike                               |                                   |
Votes: 61000   (61.00% of total)   |Votes: 38000   (38.00% of total)   |                                   |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |Foster                             |                                   |
                                   |Votes: 23000   (23.00% of total)   |                                   |
-----------------------------------------------------------------------------------------------------------|
R                                  |                                   |Deutsch                            |
Votes: 39000   (39.00% of total)   |                                   |Votes: 21000   (21.00% of total)   |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Borg                               |
                                   |                                   |Votes: 12000   (12.00% of total)   |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Jones                              |
                                   |                                   |Votes: 6000   (6.00% of total)     |
-----------------------------------------------------------------------------------------------------------|)";
    
    std::string expectedAuditResults = R"(MPO Election Audit
There were 2 parties in this election.
Total Votes: 100000
Total Seats: 2

-------------------------------------------------------------------------------------------------------------
                                                         Seats                % of Vote
               Parties               Votes               Won by                    to
                                                         Party                % of Seats
-------------------------------------------------------------------------------------------------------------
                  D                    61000               2                61.00% / 100.00%
                  R                    39000               0                39.00% / 0.00%
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |Pike                               |                                   |
Votes: 61000   (61.00% of total)   |Votes: 38000   (38.00% of total)   |                                   |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |Foster                             |                                   |
                                   |Votes: 23000   (23.00% of total)   |                                   |
-----------------------------------------------------------------------------------------------------------|
R                                  |                                   |Deutsch                            |
Votes: 39000   (39.00% of total)   |                                   |Votes: 21000   (21.00% of total)   |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Borg                               |
                                   |                                   |Votes: 12000   (12.00% of total)   |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Jones                              |
                                   |                                   |Votes: 6000   (6.00% of total)     |
-----------------------------------------------------------------------------------------------------------|)";

    EXPECT_TRUE(terminalResult.find(expectedTerminalResults) != std::string::npos);
    EXPECT_TRUE((auditResult.find(expectedAuditResults) != std::string::npos));
}





// ------------------------------------------------------------------------------------------------------------------------------------

TEST(MPOSystemTest, TestWithTie) {
    std::string filename = "../testing/_mpo.csv";
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

    election.setSeats(seats);
    election.setCandidates(partiesCandidates);

    election.processCandidates();
    
    election.countVotes();
    election.giveVotesToParty();
    election.allocateSeats();

    // Expecting a specific formatted string output from displayResults which details the election results
    std::string expectedResults = R"(A MPO election was run to fill 2 seats. There were 9 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |Pike                               |Foster                             |
Votes: 5   (55.56% of total)       |Votes: 3   (33.33% of total)       |Votes: 2   (22.22% of total)       |
-----------------------------------------------------------------------------------------------------------|
R                                  |Borg                               |Jones                              |
Votes: 3   (33.33% of total)       |Votes: 2   (22.22% of total)       |Votes: 1   (11.11% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Deutsch                            |
                                   |                                   |Votes: 0   (0.00% of total)        |
-----------------------------------------------------------------------------------------------------------|
I                                  |                                   |Smith                              |
Votes: 1   (11.11% of total)       |                                   |Votes: 1   (11.11% of total)       |
-----------------------------------------------------------------------------------------------------------|
)";

std::string expectedResults2 = R"(A MPO election was run to fill 2 seats. There were 9 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |Pike                               |                                   |
Votes: 5   (55.56% of total)       |Votes: 3   (33.33% of total)       |                                   |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |Foster                             |                                   |
                                   |Votes: 2   (22.22% of total)       |                                   |
-----------------------------------------------------------------------------------------------------------|
R                                  |                                   |Borg                               |
Votes: 3   (33.33% of total)       |                                   |Votes: 2   (22.22% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Jones                              |
                                   |                                   |Votes: 1   (11.11% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Deutsch                            |
                                   |                                   |Votes: 0   (0.00% of total)        |
-----------------------------------------------------------------------------------------------------------|
I                                  |                                   |Smith                              |
Votes: 1   (11.11% of total)       |                                   |Votes: 1   (11.11% of total)       |
-----------------------------------------------------------------------------------------------------------|
)";

std::string expectedAudit = R"(MPO Election Audit
There were 3 parties in this election.
Total Votes: 9
Total Seats: 2

-------------------------------------------------------------------------------------------------------------
                                                         Seats                % of Vote
               Parties               Votes               Won by                    to
                                                         Party                % of Seats
-------------------------------------------------------------------------------------------------------------
                  D                    5                   1                55.56% / 50.00%
                  R                    3                   1                33.33% / 50.00%
                  I                    1                   0                11.11% / 0.00%
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |Pike                               |Foster                             |
Votes: 5   (55.56% of total)       |Votes: 3   (33.33% of total)       |Votes: 2   (22.22% of total)       |
-----------------------------------------------------------------------------------------------------------|
R                                  |Borg                               |Jones                              |
Votes: 3   (33.33% of total)       |Votes: 2   (22.22% of total)       |Votes: 1   (11.11% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Deutsch                            |
                                   |                                   |Votes: 0   (0.00% of total)        |
-----------------------------------------------------------------------------------------------------------|
I                                  |                                   |Smith                              |
Votes: 1   (11.11% of total)       |                                   |Votes: 1   (11.11% of total)       |
-----------------------------------------------------------------------------------------------------------|
)";

std::string expectedAudit2 = R"(MPO Election Audit
There were 3 parties in this election.
Total Votes: 9
Total Seats: 2

-------------------------------------------------------------------------------------------------------------
                                                         Seats                % of Vote
               Parties               Votes               Won by                    to
                                                         Party                % of Seats
-------------------------------------------------------------------------------------------------------------
                  D                    5                   2                55.56% / 100.00%
                  R                    3                   0                33.33% / 0.00%
                  I                    1                   0                11.11% / 0.00%
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |Pike                               |                                   |
Votes: 5   (55.56% of total)       |Votes: 3   (33.33% of total)       |                                   |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |Foster                             |                                   |
                                   |Votes: 2   (22.22% of total)       |                                   |
-----------------------------------------------------------------------------------------------------------|
R                                  |                                   |Borg                               |
Votes: 3   (33.33% of total)       |                                   |Votes: 2   (22.22% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Jones                              |
                                   |                                   |Votes: 1   (11.11% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Deutsch                            |
                                   |                                   |Votes: 0   (0.00% of total)        |
-----------------------------------------------------------------------------------------------------------|
I                                  |                                   |Smith                              |
Votes: 1   (11.11% of total)       |                                   |Votes: 1   (11.11% of total)       |
-----------------------------------------------------------------------------------------------------------|
)";
    ASSERT_TRUE(election.displayResults() == expectedResults || election.displayResults() == expectedResults2);
    ASSERT_TRUE(election.auditResults() == expectedAudit || election.auditResults() == expectedAudit2);


    EXPECT_EQ(election.getVotesTotal(), 9);
    EXPECT_EQ(election.getVotesCurrentBallot(), 9);
    EXPECT_EQ(election.getSeats(), 2);
    EXPECT_EQ(election.getCandidates(), 6);
    EXPECT_EQ(election.getParties(), 0);
}







// MPO System Test with multiple files
TEST(MPOSystemTest, MPO_multiple_files) {
    std::ifstream file;
    std::vector<std::string> filenames = {"../testing/m_mpo1.csv", "../testing/m_mpo2.csv", "../testing/m_mpo3.csv"};
    std::string electionType;
    std::string line;
    int seats;
    int votes;
    int partiesCandidates;

    file.open(filenames[0]);
    
    std::getline(file, electionType);

    std::getline(file, line);
    seats = std::stoi(line);

    // Read number of parties/candidates
    std::getline(file, line);
    partiesCandidates = std::stoi(line);
    file.close();
    
    MPOSystem election(filenames[0]);
   
    election.setSeats(seats);
    election.setCandidates(partiesCandidates);
    election.processCandidates();


    for (const auto& file : filenames) {
        // std::cout << file << "\n";
        election.setFilename(file);
        election.countVotes();
    }

        
    election.giveVotesToParty();
    election.allocateSeats();
    
    std::string terminalResult = election.displayResults();
    std::string auditResult = election.auditResults();
    
    std::string expectedTerminalResults = R"(A MPO election was run to fill 2 seats. There were 251 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |                                   |Pike                               |
Votes: 126   (50.20% of total)     |                                   |Votes: 38   (15.14% of total)      |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Foster                             |
                                   |                                   |Votes: 38   (15.14% of total)      |
-----------------------------------------------------------------------------------------------------------|
R                                  |Borg                               |Jones                              |
Votes: 76   (30.28% of total)      |Votes: 65   (25.90% of total)      |Votes: 42   (16.73% of total)      |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Deutsch                            |
                                   |                                   |Votes: 19   (7.57% of total)       |
-----------------------------------------------------------------------------------------------------------|
I                                  |Smith                              |                                   |
Votes: 49   (19.52% of total)      |Votes: 49   (19.52% of total)      |                                   |
-----------------------------------------------------------------------------------------------------------|)";
    
    std::string expectedAuditResults = R"(MPO Election Audit
There were 3 parties in this election.
Total Votes: 251
Total Seats: 2

-------------------------------------------------------------------------------------------------------------
                                                         Seats                % of Vote
               Parties               Votes               Won by                    to
                                                         Party                % of Seats
-------------------------------------------------------------------------------------------------------------
                  R                    126                 0                50.20% / 0.00%
                  D                    76                  1                30.28% / 50.00%
                  I                    49                  1                19.52% / 50.00%
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |                                   |Pike                               |
Votes: 126   (50.20% of total)     |                                   |Votes: 38   (15.14% of total)      |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Foster                             |
                                   |                                   |Votes: 38   (15.14% of total)      |
-----------------------------------------------------------------------------------------------------------|
R                                  |Borg                               |Jones                              |
Votes: 76   (30.28% of total)      |Votes: 65   (25.90% of total)      |Votes: 42   (16.73% of total)      |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Deutsch                            |
                                   |                                   |Votes: 19   (7.57% of total)       |
-----------------------------------------------------------------------------------------------------------|
I                                  |Smith                              |                                   |
Votes: 49   (19.52% of total)      |Votes: 49   (19.52% of total)      |                                   |
-----------------------------------------------------------------------------------------------------------|)";

    EXPECT_TRUE(terminalResult.find(expectedTerminalResults) != std::string::npos);
    EXPECT_TRUE((auditResult.find(expectedAuditResults) != std::string::npos));
    EXPECT_EQ(election.getVotesTotal(), 251);
    EXPECT_EQ(election.getVotesCurrentBallot(), 80);
    EXPECT_EQ(election.getSeats(), 2);
    EXPECT_EQ(election.getCandidates(), 6);
    EXPECT_EQ(election.getParties(), 0);
}




TEST(MPOSystemTest, TestMultipleWithTie) {
    std::vector<std::string> filenames = {"../testing/m_mpo1.csv",  "../testing/m_mpo4.csv"};
    std::string electionType, line;
    int seats,  partiesCandidates;
    std::ifstream file;

    file.open(filenames[0]);
    std::getline(file, line);    // election type
    std::getline(file, line);    // Read num of seats
    seats = std::stoi(line);
    std::getline(file, line);    // Read number of candidates
    partiesCandidates = std::stoi(line);

    file.close(); // No need to read anything else


    MPOSystem election(filenames[0]);
   
    election.setSeats(seats);
    election.setCandidates(partiesCandidates);
    election.processCandidates();


    for (const auto& file : filenames) {
        // std::cout << file << "\n";
        election.setFilename(file);
        election.countVotes();
    }

    election.giveVotesToParty();
    election.allocateSeats();

    // Expecting a specific formatted string output from displayResults which details the election results
    std::string expectedResults = R"(A MPO election was run to fill 2 seats. There were 24 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |                                   |Foster                             |
Votes: 14   (58.33% of total)      |                                   |Votes: 5   (20.83% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Pike                               |
                                   |                                   |Votes: 4   (16.67% of total)       |
-----------------------------------------------------------------------------------------------------------|
R                                  |Deutsch                            |Jones                              |
Votes: 9   (37.50% of total)       |Votes: 9   (37.50% of total)       |Votes: 0   (0.00% of total)        |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |Borg                               |                                   |
                                   |Votes: 5   (20.83% of total)       |                                   |
-----------------------------------------------------------------------------------------------------------|
I                                  |                                   |Smith                              |
Votes: 1   (4.17% of total)        |                                   |Votes: 1   (4.17% of total)        |
-----------------------------------------------------------------------------------------------------------|
)";

std::string expectedResults2 = R"(A MPO election was run to fill 2 seats. There were 24 votes. The results are below:

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |Foster                             |Pike                               |
Votes: 14   (58.33% of total)      |Votes: 5   (20.83% of total)       |Votes: 4   (16.67% of total)       |
-----------------------------------------------------------------------------------------------------------|
R                                  |Deutsch                            |Borg                               |
Votes: 9   (37.50% of total)       |Votes: 9   (37.50% of total)       |Votes: 5   (20.83% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Jones                              |
                                   |                                   |Votes: 0   (0.00% of total)        |
-----------------------------------------------------------------------------------------------------------|
I                                  |                                   |Smith                              |
Votes: 1   (4.17% of total)        |                                   |Votes: 1   (4.17% of total)        |
-----------------------------------------------------------------------------------------------------------|
)";

std::string expectedAudit = R"(MPO Election Audit
There were 3 parties in this election.
Total Votes: 24
Total Seats: 2

-------------------------------------------------------------------------------------------------------------
                                                         Seats                % of Vote
               Parties               Votes               Won by                    to
                                                         Party                % of Seats
-------------------------------------------------------------------------------------------------------------
                  R                    14                  0                58.33% / 0.00%
                  D                    9                   2                37.50% / 100.00%
                  I                    1                   0                4.17% / 0.00%
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |                                   |Foster                             |
Votes: 14   (58.33% of total)      |                                   |Votes: 5   (20.83% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Pike                               |
                                   |                                   |Votes: 4   (16.67% of total)       |
-----------------------------------------------------------------------------------------------------------|
R                                  |Deutsch                            |Jones                              |
Votes: 9   (37.50% of total)       |Votes: 9   (37.50% of total)       |Votes: 0   (0.00% of total)        |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |Borg                               |                                   |
                                   |Votes: 5   (20.83% of total)       |                                   |
-----------------------------------------------------------------------------------------------------------|
I                                  |                                   |Smith                              |
Votes: 1   (4.17% of total)        |                                   |Votes: 1   (4.17% of total)        |
-----------------------------------------------------------------------------------------------------------|
)";

std::string expectedAudit2 = R"(MPO Election Audit
There were 3 parties in this election.
Total Votes: 24
Total Seats: 2

-------------------------------------------------------------------------------------------------------------
                                                         Seats                % of Vote
               Parties               Votes               Won by                    to
                                                         Party                % of Seats
-------------------------------------------------------------------------------------------------------------
                  R                    14                  1                58.33% / 50.00%
                  D                    9                   1                37.50% / 50.00%
                  I                    1                   0                4.17% / 0.00%
-------------------------------------------------------------------------------------------------------------


List of each party and their candidates, and the winners and losers.

             Party                 |             Winners               |               Losers              |
-----------------------------------------------------------------------------------------------------------|
D                                  |Foster                             |Pike                               |
Votes: 14   (58.33% of total)      |Votes: 5   (20.83% of total)       |Votes: 4   (16.67% of total)       |
-----------------------------------------------------------------------------------------------------------|
R                                  |Deutsch                            |Borg                               |
Votes: 9   (37.50% of total)       |Votes: 9   (37.50% of total)       |Votes: 5   (20.83% of total)       |
                                   |   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   |
                                   |                                   |Jones                              |
                                   |                                   |Votes: 0   (0.00% of total)        |
-----------------------------------------------------------------------------------------------------------|
I                                  |                                   |Smith                              |
Votes: 1   (4.17% of total)        |                                   |Votes: 1   (4.17% of total)        |
-----------------------------------------------------------------------------------------------------------|
)";

    // std::cout  << "\n\n"<< election.auditResults() << "\n\n";
    ASSERT_TRUE(election.displayResults() == expectedResults || election.displayResults() == expectedResults2);
    ASSERT_TRUE(election.auditResults() == expectedAudit || election.auditResults() == expectedAudit2);


    EXPECT_EQ(election.getVotesTotal(), 24);
    EXPECT_EQ(election.getVotesCurrentBallot(), 13);
    EXPECT_EQ(election.getSeats(), 2);
    EXPECT_EQ(election.getCandidates(), 6);
    EXPECT_EQ(election.getParties(), 0);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}