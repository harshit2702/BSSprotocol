# BSS Protocol Simulation

This project simulates the BSS (Byzantine State Synchronization) protocol using C++. The program reads input commands to simulate processes sending and receiving messages, and it maintains a time matrix to track the state of each process.

## Project Structure

- `input.txt` : Input files for the simulation.
- `myBSSProg_24535010`: Directory for the main program.
- `myBSSProg_24535010.cpp`: Main source code file for the BSS protocol simulation.
- `README.txt`: This file.

## Setup

1. **Install Dependencies**: Ensure you have `g++` installed on your system.

2. **Build the Project**: Use the `g++` compiler to build the project.
    - Open a terminal and run the following command:
    ```sh
    g++ -o myBSSProg_24535010 myBSSProg_24535010.cpp
    ```

## Usage

1. **Run the Program**: After building, you can run the executable generated.
    ```sh
    ./myBSSProg_24535010
    ```

2. **Provide Input**: The program reads input from the standard input. You can redirect input from a file:
    ```sh
    ./myBSSProg_24535010 < input.txt
    ```

## Example Input

3
begin process p0
send m0
recv_B p1 m1
end process
begin process p1
recv_B p0 m0
send m1
end process
begin process p2
recv_B p1 m1
recv_B p0 m0
end process
exit

## Example Output

Initial Time Matrix
Time for process 0 -> 0 0 0 
Time for process 1 -> 0 0 0 
Time for process 2 -> 0 0 0 
begin process p0
send m0 (100)
recv_B p1 m1 (100)
recv_A p1 m1 (110)
end process
begin process p1
recv_B p0 m0 (000)
recv_A p0 m0 (100)
send m1 (110)
end process
begin process p2
recv_B p1 m1 (000)
recv_B p0 m0 (000)
recv_A p0 m0 (100)
recv_A p1 m1 (110)
end process
Final Time Matrix
Time for process 0 -> 1 1 0 
Time for process 1 -> 1 1 0 
Time for process 2 -> 1 1 0 
BSS Protocol Completed

## Code Overview

- `main()`: The entry point of the program. Initializes the time matrix and processes input commands.
- `inputstring()`: Parses input strings and updates the state of the simulation.
- `recievetransaction()`: Handles the reception of messages and updates the buffer and transaction logs.
- `checkBSSRule()`: Checks the BSS protocol rules to validate message reception.

•	Structure message2 — Is used for send message 
	⁃	it contains the message string
	⁃	sender id 
	⁃	boolean array of size n (equal to number of process) to indicate which process has received the message.
	⁃	time array to indicate the time when the message was sent.
•	checkBSSRule — used to check wether the message is to be delivered to the application or not.
•	recievetransaction — it handles receiving transaction. 
	⁃	it make use of checkBSSRule to make a decision that message is to be delivered to the application or to store in buffer.
	⁃	when an application is delivered to the application it checks its buffer to see can any other message be also delivered to the application 
•	inputstring — handles all the input given to the program
	⁃	it initialise the process and transaction 
	⁃	also checks for invalid input string
•	main
	⁃	initialise number of process (< 9).
	⁃	initialise the time matrix of n * n to 0.
	⁃	initialise transaction array that store the transaction in order made by the process
	⁃	initialise the buffer to store message that are received out of order .
	⁃	initialise the output to store the result for each transaction 
