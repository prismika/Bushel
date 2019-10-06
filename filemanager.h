#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>

using namespace std;

class FileManager{
public:
	/*Write data specified by text variable to file specified by path variable*/
	int write(const string *path, const char *text);
	/*Read data from file specified by path variable into buffer provided by buffer variable*/
	int read(const string *path, char * buffer, int buffsize);
};

#endif