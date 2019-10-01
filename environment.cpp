/*Environment
Stores and retrieves environment variables.
Brantley Vose
2019/10/01
*/
#include "environment.h"
#include <string>
#include <cstring>
#include <stdlib.h>
#include <iostream>

using namespace std;

Environment::Environment(){
	cout << "Environment created" << endl;
	return;
}

string *Environment::find_variable(string str){
	int i;
	for(i=0;i<(int)this->vars.size(); i++){
		if(str.compare(vars[i].key)==0){
			return &vars[i].value;
		}
	}
	return NULL;
}

int Environment::put_variable(string key, string value){
	// struct var_pair *new_pair = (struct var_pair*)malloc(sizeof(struct var_pair));
	struct var_pair new_pair;
	new_pair.key = key;
	new_pair.value = value;
	vars.push_back(new_pair);
	return 0;
}