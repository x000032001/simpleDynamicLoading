#ifndef PLUGIN_H
#define PLUGIN_H

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dlfcn.h>

#include "args.h"

using namespace std;

typedef void* (*func_t)(void*);

class plugin {
	public:
		plugin();
		~plugin();		

		void createThread();
		void setArgs(const struct arguments&);
		void getFunc(const char*,const char*);
		void initPipe();
		int getReadFd(){return fd[0];}
	private:
		int fd[2];
		pthread_t id;
		// To prevent variable be modified ,store it in class.
		struct arguments args;

		void* dlHandle;
		func_t runFunc;
		char *libN ,*funcN;
		
};

#endif
