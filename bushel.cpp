/*
Bushel: The Barely Usable SHELl
This is the main executable. It handles interpretation
of commands and the coordination of the various other objects.
Brantley Vose
2019/10/01
*/
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
string txtblue = "\e[01;34m";
string txtgreen = "\e[01;32m";
string txtreg = "\e[0;37m";

/*Main method. Bootstrap for the entire shell.*/
int main(int argc, char *argv[]){
	//Setup
	//put in environment: "shell","path/to/shell/bushel"
	//Main prompt loop
	while(true){
		pwd = getenv("PWD");
		string prompt = txtgreen + "[bushel]" + txtblue + "<" + pwd + ">" + txtreg + "$ ";
		cout << prompt;
		string userInput;
		getline(cin, userInput);
		parser.parse(&userInput);
		while(parser.has_next_command()){
			Command *current_command;
			current_command = parser.next_command();
			execute(current_command);
		}
	}

	return 0;
}

int fd_pipe_from_child[2];
int execute(const Command *command){
	//Execute any "special" commands
	if(!execute_special(command)){
		return 0;
	}

	//Do necessary plumbing
	pid_t more_pid;
	if(command->pipe){
		pipe(fd_pipe_from_child);
		more_pid=fork();
		if(more_pid==0){//"more" child
			dup2(fd_pipe_from_child[0],0);
			char *exec_more[] = {(char*)"more",NULL};
			execvp(exec_more[0],exec_more);
			cout << "Failed to open \"more\"." << endl;
			exit(0);
		}
	}

	//Create child that will execute command
	pid_t child_pid = fork();
	if(child_pid == 0){//child
		//put in environment parent
		char *exec_name = (char*)malloc(command->name.length()+1);
		strcpy(exec_name,command->name.c_str());
		char *exec_args[command->args.size()+2];
		exec_args[0]=exec_name;
		size_t i;
		for(i = 1; i < command->args.size()+1; i++){
			exec_args[i]=(char*)malloc(command->args[i-1].length()+1);
			strcpy(exec_args[i],command->args[i-1].c_str());
		}
		exec_args[i]=NULL;
		if(command->pipe){//Fancy pipe things. Ignore.
			//Replace stdout of child with write end of pipe
			dup2(fd_pipe_from_child[1],1);
		}
		execvp(exec_args[0],exec_args);
		cout << "Command not found: " << exec_args[0] << endl;
		exit(0);
	}else{//parent
		if(!command->background){//command->background is a flag that the parser sets
			cout << "Waiting for child to die." << endl;
			waitpid(child_pid,NULL,0);
			cout << "Child dead" << endl;
		}
		if(command->pipe){
			cout << "Waiting for more to die" << endl;
			waitpid(more_pid,NULL,0);
			cout << "More dead" << endl;
		}

		
	}
	return 0;
}

/*Checks if command needs special execution procedure and executes it.
Returns 0 if command was executed. Returns -1 otherwise.*/
int execute_special(const Command *command){
	/*string command_name = command->name();
	if(command_name.compare("cd")){
		...
	}...*/
	if(!command->name.compare("quit") || !command->name.compare("exit")){
		exit(0);
	}else if(!command->name.compare("cd")){
		cout << "I can't change directories yet :(" << endl;
		return 0;
	}
	return -1;
}
