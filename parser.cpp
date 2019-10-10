#include "parser.h"
#include <sstream>
#include <cstdlib>
#include <iostream>

using namespace std;

static string get_next_token(string *str, size_t *progress);

int Parser::parse(string *str){
	Command *new_command = new Command();
	new_command->background = false;
	new_command->pipe = false;
	new_command->infile = "";
	new_command->outfile = "";
	size_t progress = 0;
	new_command->name = get_next_token(str,&progress);
	while(true){
		string token = get_next_token(str,&progress);
		if(token.empty()) break;
		//This is a mess we'll have to clean up.
		// if(token.find("|")!=string::npos){
		// 	string next_token;
		// 	str_stream >> next_token;
		// 	if(token.find("more")!=string::npos
		// 		|string next_token;
		// 	str_stream >> next_token;
		// 	if(token.find("more")!=string::npos
		// 		|| !next_token.compare(| !next_token.compare("more")){
		// 		new_command->pipe=true;
		// 	}
		// 	continue;
		// }
		new_command-> args.push_back(token);
	}
	q.push(new_command);
	return 0;
}

static string get_next_token(string *str, size_t *progress){
	if(str->length() <= *progress) return "";
	size_t pos = str->find_first_of(" ", *progress);
	string return_string;
	if(pos == string::npos){
		return_string = str->substr(*progress);
	}else{
		return_string = str->substr(*progress, pos - *progress);
	}
	*progress += return_string.length()+1;
	return return_string;
}

bool Parser::has_next_command(void){
	return !q.empty();
}

Command *Parser::next_command(void){
	Command * to_return = q.front();
	q.pop();
	return to_return;
}
