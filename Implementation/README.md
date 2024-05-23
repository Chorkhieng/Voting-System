# Implementation
This program is implemented in C++ and is followed Object-Oriented design principles.  
### How to Run this Program
1. go into the `Implementation/src` directory
    - You can do this by running `cd Implementation/src`, assuming that you are in the directory `Voting System`
2. To compile and build the election system, run one of the below commands in your terminal
    - `make all`
    - `make run`
    - `make`

3. Now we can run the program. There are two ways that files can be given to the program. The first is
as a command line argument, and the second is by having the program ask you for the file name. The file name
should be spelled correctly. Our test files are in the "testing/" directory, so include that in the file path.
Select one of the options below to run the program.
    - Run the program, it will ask you for a file name:
        - `./main`
    - Run the program, and give it a file as a command line argument (filename and path must exist):
        - `./main ../testing/_cpl2.csv`
    - Run the program with multiple files, and give it files as a command line argument (filename and path must exist):
        - `./main ../testing/m_cpl1.csv ../testing/m_cpl2.csv ../testing/m_cpl3.csv`
        - `./main ../testing/m_opl1.csv ../testing/m_opl2.csv ../testing/m_opl3.csv`
        - `./main ../testing/m_mpo1.csv ../testing/m_mpo2.csv ../testing/m_mpo3.csv`
    - If you make changes to the program, you can recompile it, run it, and give it a command line argument:
        - `make run FILE=../testing/_cpl2.csv`
    

### How to Run Unit Tests 
- Run command: `make unittest`

### How to Run System Tests 
- Run command: `make systemtest`

### Clean up Objects and Compiled Files
- Run command: `make clean`
