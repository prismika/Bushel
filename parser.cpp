#include "parser.h"
#include <sstream>
#include <cstdlib>
#include <iostream>

using namespace std;

int Parser::parse(string *str){
	Command *new_command = new Command();
	new_command->background = false;
	new_command->pipe = false;
	new_command->infile = "";
	new_command->outfile = "";
	stringstream str_stream(*str);
	str_stream >> new_command->name;
	while(true){
		string token;
		str_stream >> token;
		if(token.empty()) break;
		if(token.find("|")!=string::npos){
			string next_token;
			str_stream >> next_token;
			if(token.find("more")!=string::npos
				|| !next_token.compare("more")){
				new_command->pipe=true;
			}
			continue;
		}
		new_command-> args.push_back(token);
	}
	q.push(new_command);
	return 0;
}

bool Parser::has_next_command(void){
	return !q.empty();
}

Command *Parser::next_command(void){
	Command * to_return = q.front();
	q.pop();
	return to_return;
}
