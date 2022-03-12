#include "ArgParser.h"

#include <string.h>
#include <stdio.h>

ArgParser::ArgParser(int argc, const char** argv) 
	: argc(argc)
	, argv(argv) {
	
	pid = 0;
	if (NULL != getByTag("-pid")) {
		pid = atoi(getByTag("-pid"));
	}

	processName = getByTag("-name");
	functionName = getByTag("-func");
	fileToHide = getByTag("-hide");

	if ((0 != pid) && (NULL != processName)) {
		printf("Please write one thing: the name of the process or its pid\n");
		exit(0);
	}

	if ((0 == pid) && (NULL == processName)) {
		printf("Please write process: -pid or -name\n");
		exit(0);
	}

	if (NULL == functionName) {
		printf("Please write function: -func\n");
		exit(0);
	}
}


const char* ArgParser::getByTag(const char* tag) {
	for (int i = 0; i < argc; ++i) {
		if (!strcmp(tag, argv[i])) {
			if (i == argc - 1) {
				return NULL;
			}

			return argv[i + 1];
		}
	}

	return NULL;
}