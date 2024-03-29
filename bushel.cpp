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
#include <fstream>
#include <fcntl.h>
#include "parser.h"

using namespace std;

int execute(const Command*);
int execute_special(const Command*);
int apply_alias(Command *command);


string pwd = "";
//Parser object. Parses user input into command structs and holds them in a queue
Parser parser;
string txtblue = "\e[01;34m";
string txtgreen = "\e[01;32m";
string txtreg = "\e[0;37m";

/*Main method. Bootstrap for the entire shell.
Returns -1 if provided batch file fails to open*/
int main(int argc, char *argv[]){
	//Setup
	//TODO put in environment: "shell","path/to/shell/bushel"
	//Check for file specified in argument
	if(argc==2){
		ifstream batch_file(argv[1]);
		if(batch_file.fail()){
			cout << "Error: Failed to open batch file: " << argv[1] << endl;
			return -1;
		}
		parser.parse_file(&batch_file);
		while(parser.has_next_command()){
			Command *current_command;
			current_command = parser.next_command();
			execute(current_command);
		}
	}else{
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
				apply_alias(current_command);
				execute(current_command);
			}
		}
	}
	return 0;
}

/*Handles hardcoded aliases by changing the provided command
returns 0*/
int apply_alias(Command *command){
	if(!command->name.compare("clr")){
		command->name = "clear";
	}else if(!command->name.compare("environ")){
		command->name = "env";
	}
	return 0;
}

/*Execute given command object.
Returns 0
*/
int execute(const Command *command){

	//Execute any "special" commands
	if(!execute_special(command)){
		return 0;
	}

	//Do necessary plumbing
	int fd_pipe_from_child[2];
	pid_t more_pid;
	if(command->pipe){
		pipe(fd_pipe_from_child);
		more_pid=fork();
		if(more_pid==0){//"more" child
			dup2(fd_pipe_from_child[0],0);
			close(fd_pipe_from_child[1]);
			char *exec_more[] = {(char*)"more",NULL};
			execvp(exec_more[0],exec_more);
			cout << "Failed to open \"more\"." << endl;
			exit(0);
		}
	}

	//Create child that will execute command
	pid_t child_pid = fork();
	if(child_pid == 0){//child
		//Set up output file if specified
		int fd_out;
		if(!command->outfile.empty()){//If an outfile is specified, try opening it.
			string outfile_path = getenv("PWD");
			outfile_path += "/";
			outfile_path += command->outfile;
			//Set write flags.
			//					Write only 		Create file if necessary
			int write_flags = 	O_WRONLY	|	O_CREAT;
			if(command->append){
				write_flags = write_flags | O_APPEND;
			}else{
				write_flags = write_flags | O_TRUNC;
			}
			fd_out = open(outfile_path.c_str(),write_flags,S_IRUSR|S_IWUSR);
			if(fd_out < 0){
				cout << "Error: failed to open output file: " << command->outfile;
			}else{//If successful, replace stdout and stderr with the new file descriptor
				dup2(fd_out, 1);
				dup2(fd_out, 2);
			}
		}

		int fd_in;
		if(!command->infile.empty()){
			string infile_path = getenv("PWD");
			infile_path += "/";
			infile_path += command->infile;
			fd_in = open(infile_path.c_str(),O_RDONLY);
			if(fd_in < 0){
				cout << "Error: failed to open input file: " << command->infile;
			}else{
				dup2(fd_in,0);
			}
		}

		if(command->pipe){
			//Replace stdout of child with write end of pipe
			dup2(fd_pipe_from_child[1],1);
			close(fd_pipe_from_child[0]);
		}

		//TODO put in environment parent
		//Process command name and arguments into array
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
		// cout << execvpc_args[0] << endl;
		execvp(exec_args[0],exec_args);
		cout << "Command not found: " << exec_args[0] << endl;
		exit(0);
	}else{//parent
		if(!command->background){//command->background is a flag that the parser sets
			// cout << "Waiting for child to die." << endl;
			waitpid(child_pid,NULL,0);
			// cout << "Child dead" << endl;
		}
		if(command->pipe){
			close(fd_pipe_from_child[1]);
			close(fd_pipe_from_child[0]);
			// cout << "Waiting for more to die" << endl;
			waitpid(more_pid,NULL,0);
			// cout << "More dead" << endl;
		}
	}
	return 0;
}

string help_message = "Welcome to Bushel - the Barely Usable SHELl.\n"
"\n"
"Bushel supports output redirection:\n"
"\tcommand [arguments] < input_file > output_file\n"
"To append to the output file instead of replacing\n"
"it, use '>>' instead of '>'.\n"
"\n"
"Bushel supports piping to \"more\" like so:\n"
"\tcommand [arguments] | more\n"
"\n"
"Instead of opening an interactive prompt, Bushel\n"
"can read instructions from a file:\n"
"./bushel batch_file\n"
"\n"
"Multiple commands can be fed into Bushel at once\n"
"by separating them with semicolons:\n"
"\tcommand1 ; command2 ; command3\n"
"These commands will run sequentially.\n"
"\n"
"Thank you for using Bushel!\n";

/*Checks if command needs special execution procedure and executes it.
Returns 0 if command was executed. Returns -1 otherwise.*/
int execute_special(const Command *command){
	if(!command->name.compare("quit") || !command->name.compare("exit")){
		exit(0);
	}else if(!command->name.compare("cd")){
		string new_path;
		if(command->args.size()==0){
			cout << getenv("PWD") << endl;
			return 0;
		}
		if(!command->args[0].compare(".")){
			return 0;
		}else if(!command->args[0].compare("..")){
			//'cd ..' Move to parent directory
			//Delete everything after last '/' in the present working directory
			new_path = getenv("PWD");
			size_t slash_pos = new_path.find_last_of("/");
			if(slash_pos==0|| slash_pos==string::npos){
				cout << "Error: No parent directory" << endl;
			}
			new_path = new_path.substr(0,slash_pos);
		}else if(command->args[0][0] == '/'){
			//Interpret argument an absolute file path
			new_path = command->args[0];
		}else{
			//relative file path
			new_path = getenv("PWD");
			new_path += "/";
			new_path += command->args[0];
		}
		//Change directory
		if(chdir(new_path.c_str())<0){
			cout << "Error: Failed to change directory" << endl;
			return 0;
		}
		//Update environment
		char buffer[1024];
		getcwd(buffer,1024);
		string to_put_in_env(buffer);
		to_put_in_env = "PWD=" + to_put_in_env;
		// new_path = "PWD="+new_path;
		char *to_put_in_env_char = (char*)malloc(to_put_in_env.length()+1);
		strcpy(to_put_in_env_char, to_put_in_env.c_str());
		putenv(to_put_in_env_char);
		return 0;
	}else if(!command->name.compare("help")){
		cout << help_message << endl;
	}
	return -1;
}
