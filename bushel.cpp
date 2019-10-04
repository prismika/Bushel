/*
Bushel: The Barely Usable SHELl
This is the main executable. It handles interpretation
of commands and the coordination of the various other objects.
Brantley Vose
2019/10/01
*/
// #include <cstdlib>
// #include <cstdio>
// #include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include "parser.h"

using namespace std;

int execute(Command*);
int execute_special(Command*);

string pwd = "";
Parser parser;

/*Main method. Bootstrap for the entire program.*/
int main(int argc, char *argv[]){
	//Setup
	//put in environment: "shell","path/to/shell/bushel"
	//Main prompt loop
	while(true){
		pwd = getenv("PWD");
		string prompt = "bushel:" + pwd + "$ ";
		cout << prompt;
		string userInput;
		getline(cin, userInput);
		if(!userInput.compare("quit")){
			exit(0);
		}
		parser.parse(&userInput);
		while(parser.has_next_command()){
			Command *current_command;
			current_command = parser.next_command();
			execute(current_command);
		}
	}

	return 0;
}

int execute(Command *command){
	/*int ret;
	if(ret = execute_special(command)){
		return ret;
	}
	string command_name = command->get_name();
	string path = get_env_or_whatever("PATH");
	//Split at semicolons into vector<string> paths
	for(int i = 0; i < paths.size(); i++){
		//Look in paths[i] for a command matching command_name
		if(!fork()){
			//put in environment parent
			execlp("paths[i]" + command_name, command_name, NULL);
			//This but with arguments that work
			exit(0);
		}else{//parent
			if(!command.background()){
				wait(NULL);
			}
		}
	}*/
	return 0;
}


int execute_special(Command *command){
	/*string command_name = command->name();
	if(command_name.compare("cd")){
		...
	}...*/
	return 0;
}