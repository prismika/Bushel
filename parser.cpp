#include "parser.h"
#include <sstream>
#include <cstdlib>
#include <iostream>

using namespace std;

// static string get_next_token(string *str, size_t *progress);

void pad_symbols(string *str){
	size_t pipe_position = str->find_first_of("|");
	if(pipe_position != string::npos){
		str->insert(pipe_position, " ");
		str->insert(pipe_position+1, " ");
	}
}

int Parser::parse(string *str){
	Command *new_command = new Command();
	new_command->background = false;
	new_command->pipe = false;
	new_command->infile = "";
	new_command->outfile = "";
	pad_symbols(str);
	stringstream str_stream(*str);
	str_stream >> new_command->name;
	while(true){
		string token;
		str_stream >> token;
		if(token.empty()) break;
		if(token.compare("|")==0){
			string token_pipe_to;
			str_stream >> token_pipe_to;
			if(!token_pipe_to.compare("more")){
				new_command->pipe = true;
			}else{
				cout << "Error: Unrecognized pipe listener" << endl;
			}
			continue;
		}
		new_command-> args.push_back(token);
	}
	q.push(new_command);
	return 0;
}

// static string get_next_token(string *str, size_t *progress){
// 	//If we've progressed to the end of the string, we're done.
// 	if(str->length() <= *progress) return "";
// 	string return_string;
// 	//Find the next space
// 	size_t pos = str->find_first_of(" ", *progress);
// 	//If there are no more spaces, just return to the end of the string.
// 	if(pos == string::npos){
// 		return_string = str->substr(*progress);
// 		//Increment the progress
// 		*progress = str->length();
// 		return return_string;
// 	}
// 	//
// 	pos = str->find_first_not_of(" ", pos);
// 	if(pos == string::npos){
// 		return_string = str->substr(*progress);
// 	}else{
// 		return_string = str->substr(*progress, pos - *progress);
// 	}
// 	*progress += return_string.length()+1;
// 	return return_string;
// }

bool Parser::has_next_command(void){
	return !q.empty();
}

Command *Parser::next_command(void){
	Command * to_return = q.front();
	q.pop();
	return to_return;
}
