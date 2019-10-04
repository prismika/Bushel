/*
Bushel: The Barely Usable SHELl
This is the main executable. It handles interpretation
of commands and the coordination of the various other objects.
Brantley Vose
2019/10/01
*/
// #include <cstdlib>
// #include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>
#include "parser.h"

using namespace std;

int execute(const Command*);
int execute_special(const Command*);

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
			cout << "Executing" << endl;
			execute(current_command);
		}
	}

	return 0;
}

int execute(const Command *command){
	if(execute_special(command)){
		return 0;
	}
	if(!fork()){//child
		//put in environment parent
		char *exec_name = (char*)malloc(command->name.length()+1);
		strcpy(exec_name,command->name.c_str());
		char *exec_args[command->args.size()+2];
		cout << command->args.size() <<endl;
		exec_args[0]=exec_name;
		size_t i;
		for(i = 1; i < command->args.size()+1; i++){
			cout << "Argument " << i << " is of length " << command->args[i-1].length() << endl;
			cout << "Argument is " << command->args[i-1] << endl;
			exec_args[i]=(char*)malloc(command->args[i-1].length()+1);
			cout << command->args[i-1].length() << endl;
			// strcpy(exec_args[i],command->args[i].c_str());
			cout << exec_args[i] << endl;
		}
		exec_args[i]=NULL;
		// execvp(exec_args[0],exec_args);
		char *tmp[] = {(char*)"ls",(char*)"-lah",NULL};
		execvp(tmp[0],tmp);
		cout << "Error executing command" << endl;
		exit(0);
	}else{//parent
		if(!command->background){
			wait(NULL);
		}
	}
	return 0;
}


int execute_special(const Command *command){
	/*string command_name = command->name();
	if(command_name.compare("cd")){
		...
	}...*/
	return 0;
}
