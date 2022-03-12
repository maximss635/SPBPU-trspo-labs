#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <stdlib.h>

class ArgParser {
public:
	ArgParser(int argc, const char** argv);
	const char* getByTag(const char* tag);

	inline int getPid() const {
		return pid;
	}

	inline const char* getProcessName() const {
		return processName;
	}
	
	inline const char* getFunctionName() const {
		return functionName;
	}

	inline const char* getFileToHide() const {
		return fileToHide;
	}

private:
	// Input
	int argc;
	const char** argv;

	// Output
	unsigned int pid;
	const char* processName;
	const char* functionName;
	const char* fileToHide;
};


#endif // ARGPARSER_H
