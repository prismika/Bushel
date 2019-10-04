#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <queue>

using namespace std;

typedef struct{
	string name;
	bool background;
	bool pipe;
	vector<string> args;
	string infile;
	string outfile;
}Command;

class Parser{
private:
	queue<Command*> q;
public:
	int parse(string*);
	bool has_next_command(void);
	Command *next_command(void);
};

#endif