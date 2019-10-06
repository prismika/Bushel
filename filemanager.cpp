#include "filemanager.h"
#include <iostream>
#include <string>

using namespace std;

int FileManager::write(const string *path, const char *text){
	cout << "Writing to a file (BUT NOT REALLY!!!1!!)" << endl;
	return 0;
}

int FileManager::read(const string *path, char * buffer, int buffsize){
	cout << "Reading from a file (FAKEOUT!!!!11!!1!!!)" << endl;	
	return 0;
}
