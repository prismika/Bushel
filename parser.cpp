#include "parser.h"
#include <sstream>
#include <cstdlib>
#include <iostream>

using namespace std;

int Parser::parse(string *str){
	Command *new_command = new Command();
	stringstream str_stream(*str);
	str_stream >> new_command->name;
	while(true){
		string token;
		str_stream >> token;
		if(token.empty()) break;
		new_command-> args.push_back(token);
	}
	new_command->background = false;
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
