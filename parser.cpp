/*
Parser.cpp
This defindes the functions of the Parser object, which
takes user input strings and processes them into Command
structs. The structs are placed in a queue for the main
executable to interpret.
*/
#include "parser.h"
#include <sstream>
#include <cstdlib>
#include <iostream>

using namespace std;

// static string get_next_token(string *str, size_t *progress);

static void pad_symbols(string *str){
	size_t symbol_position = 0;
	while(symbol_position <= str->length()){
		symbol_position = str->find_first_of("|;", symbol_position);
		if(symbol_position != string::npos){
			str->insert(symbol_position + 1, " ");
			str->insert(symbol_position++, " ");
			symbol_position++;
		}
	}
}

int Parser::parse(string *str){
	pad_symbols(str);
	stringstream str_stream(*str);
	bool another_command_in_string = true;
	while(another_command_in_string){
		another_command_in_string = false;
		Command *new_command = new Command();
		new_command->background = false;
		new_command->pipe = false;
		new_command->infile = "";
		new_command->outfile = "";
		str_stream >> new_command->name;
		while(true){
			string token;
			str_stream >> token;
			if(token.empty()) break;
			//Handle pipe symbol
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
			if(token.compare(";")==0){
				another_command_in_string = true;
				break;
			}else{
				new_command-> args.push_back(token);
			}
		}
		q.push(new_command);
	}
	return 0;
}

int Parser::parse_file(ifstream *file_stream){
	cout << "Parsing file..." << endl;
	while(!file_stream->eof()){
		string cur_line;
		getline(*file_stream, cur_line);
		parse(&cur_line);
	}
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



