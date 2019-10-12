/*
This header defines the Command struct that the interpreter (bushel.cpp)
and the parser use to communicate.
*/
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;
/*
After the parser parses a string of user input, it places command
objects in an internal queue for the interpreter to remove one
at a time, interpret, and execute. The Command struct contains
all the necessary information to execute a command correctly.
*/
typedef struct{
	//Command name
	string name;
	//False if the command should block the shell
	bool background;
	//True if piping the output
	bool pipe;
	//List of arguments
	vector<string> args;
	//File name if redirecting input
	string infile;
	//File name if redirecting output
	string outfile;
	//True if output redirection should append.
	bool append;
}Command;

/*
The interpreter has one Parser object, which is tasked with
parsing the raw user input strings and files and processing
them into command structs, which it places in its internal queue.
*/
class Parser{
private:
	//Queue of command structs that are ready to be interpreted
	queue<Command*> q;
public:
	//Parse user input in given string
	//Returns 0
	int parse(string*);
	//Parse given file
	//Returns 0
	int parse_file(ifstream*);
	//Check if another command waiting in queue
	//Returns true if queue is not empty
	bool has_next_command(void);
	//Get next command struct from queue
	//Returns next command
	Command *next_command(void);
};

#endif