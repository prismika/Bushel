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
/*
Places spaces around special symbols in the given string so they will be treated
like tokens during parsing. This is a bad solution but it works fine.
*/
static void pad_symbols(string *str){
	size_t symbol_position = 0;
	while(symbol_position <= str->length()){
		symbol_position = str->find_first_of("|;><&", symbol_position);
		if(symbol_position != string::npos){
			str->insert(symbol_position + 1, " ");
			str->insert(symbol_position++, " ");
			symbol_position++;
		}
	}
}

/*
Parse user input into command struct and place struct in queue
Returns 0
*/
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
		new_command->append = false;
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
			}else if(token.compare(";")==0){
				another_command_in_string = true;
				break;
			}else if(token.compare(">")==0){
				string token_next;
				str_stream >> token_next;
				if(!token_next.compare(">")){//Appending
					new_command->append = true;
					str_stream >> new_command->outfile;
				}else{
					new_command->outfile = token_next;
				}
			}else if(token.compare("<")==0){
				str_stream >> new_command->infile;
			}else if(token.compare("&")==0){
				new_command->background = true;
			}else{
				new_command-> args.push_back(token);
			}
		}
		q.push(new_command);
	}
	return 0;
}


/*
Parse commands from file into command structs and place
structs into queue.
Takes a pointer to a file stream for the file.
Returns 0
*/
int Parser::parse_file(ifstream *file_stream){
	while(!file_stream->eof()){
		string cur_line;
		getline(*file_stream, cur_line);
		parse(&cur_line);
	}
	return 0;
}

/*
Check if parser has another command struct in the queue.
Returns true if queue is not empty.
*/
bool Parser::has_next_command(void){
	return !q.empty();
}

/*
Get next command struct from the queue.
Returns pointer to next command
*/
Command *Parser::next_command(void){
	Command * to_return = q.front();
	q.pop();
	return to_return;
}



