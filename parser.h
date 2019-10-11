#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

typedef struct{
	string name;
	bool background;
	bool pipe;
	vector<string> args;
	string infile;
	string outfile;
	bool append;
}Command;

class Parser{
private:
	queue<Command*> q;
public:
	int parse(string*);
	int parse_file(ifstream*);
	bool has_next_command(void);
	Command *next_command(void);
};

#endif