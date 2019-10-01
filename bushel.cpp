/*This is the main executable. It handles interpretation
of commands and the coordination of the various other objects.
Brantley Vose
2019/10/01
*/
// #include <cstdlib>
// #include <cstdio>
// #include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include "environment.h"

using namespace std;

/*The shell's environment, where variables are stored.*/
Environment env;
string prompt = "bushel$ ";

/*Main method. Bootstrap for the entire program.*/
int main(int argc, char *argv[]){
	//Setup
	//env.put_variable("shell","path/to/shell/bushel");
	//Main prompt loop
	while(true){
		cout << prompt;
		string userInput;
		getline(cin, userInput);
		if(!userInput.compare("quit")){
			exit(0);
		}
	}

	return 0;
}