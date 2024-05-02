#include "Candidate.h"
#include "Party.h"
#include "TieBreaker.h"
#include "CPLSystem.h"
#include "ElectionSystem.h"
#include "OPLSystem.h"
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
    election.setVotes(votes);
    election.setParties(partiesCandidates);

    
    // CPLSystem cplSystem("test_cpl.csv");
    election.countVotes();
    election.allocateSeats();
    std::string terminalResult = election.displayResults();
    std::string auditResult = election.auditResults();

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
    
    election.setVotes(votes);
    election.setCandidates(partiesCandidates);
    election.countVotes();

    election.allocateSeats();
    std::string terminalResult = election.displayResults();
    std::string auditResult = election.auditResults();

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
    election.setVotes(votes);
    election.setParties(partiesCandidates);

    
    // CPLSystem cplSystem("test_cpl.csv");
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
    
    election.setVotes(votes);
    election.setCandidates(partiesCandidates);
    election.countVotes();

    election.allocateSeats();
    std::string terminalResult = election.displayResults();
    std::string auditResult = election.auditResults();


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



 add #5 in OPL: group candidates by party?

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

    EXPECT_TRUE(terminalResult.find(expectedTerminalResults) != std::string::npos ||
                terminalResult.find(expectedTerminalResults2) != std::string::npos);
    EXPECT_TRUE(auditResult.find(expectedAuditResults) != std::string::npos ||
                auditResult.find(expectedAuditResults2) != std::string::npos);
}




int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
