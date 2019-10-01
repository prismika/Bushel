#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <string>
#include <vector>

using namespace std;

class Environment{
private:
	struct var_pair{
		string key;
		string value;
	};
	vector<struct var_pair> vars;
public:
	Environment();
	string *find_variable(string str);
	int put_variable(string key, string value);
};


#endif
