#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include "tinyxml2.h"
#include "args.h"

using namespace tinyxml2;

typedef void* (*func_t)(void*);

func_t getFunc(const char* lib,const char* funcName)
{
	void* handle = dlopen( lib , RTLD_LAZY );
	if( !handle )
	{
		fputs( dlerror() , stderr );
		exit(1);
	}

	func_t func = (func_t)dlsym( handle , funcName );

	char* error = NULL;
	if( (error = dlerror()) != NULL )
	{
		fputs( error , stderr );
		exit(1);
	}

	return func;
}

void monitorFuncHandler(const char* param,int fd[2])
{
	// using xml to load setting
	XMLDocument doc;
	doc.LoadFile(param);

	const char* freqs = doc.FirstChildElement("freq")->GetText();
	int sleepInterval = atoi(freqs);

	const char* libName = doc.FirstChildElement("libname")->GetText();
	const char* funcName = doc.FirstChildElement("funcname")->GetText();
	void* handle; // get handle and release at exit
	func_t func = getFunc(libName ,funcName);

	// get a pipe fd pair
	pipe(fd);
	printf("get fd = %d,%d\n",fd[0],fd[1]);

	int flags = fcntl(fd[0], F_GETFL, 0);
	fcntl(fd[0], F_SETFL, flags | O_NONBLOCK);

	struct args *P = (struct args*)malloc(sizeof(struct args));
	P->fd = fd[1];
	P->iv = sleepInterval;

	pthread_t id;
	if( pthread_create( &id , NULL , func , (void*)P  ) )
	{
		fputs("pthread create error",stderr);
		exit(1);
	}

}

int main()
{
	fd_set set;
	FD_ZERO(&set);

	int maxfd = 0;

	// fd[0] is input and set to non-block
	int fds[10][2]={};
	const char* xmllist[] = { "set.xml" , "set2.xml" };
	for( int i = 0 ; i < 2 ; ++i )
	{
		// for each xml , create a thread to handle and wait for data 
		monitorFuncHandler(xmllist[i],fds[i]);
		if( fds[i][0] > maxfd )
			maxfd = fds[i][0];
	}

	while(1)
	{
		// add all fd[0]
		for( int i = 0 ; i < 2 ; ++i )
			FD_SET(fds[i][0] , &set);

		// set select and waiting data ( no timed out )
		int rc = select( maxfd+1 , &set , NULL , NULL , NULL );
		for( int i = 0 ; i < 2 ; i++ )
		{
			int fdn = fds[i][0];
			if( FD_ISSET( fdn , &set ) )
			{
				char buff[256];
				memset(buff,0,256);
				while( read(fdn,buff,255) >= 0 )
				{
					fputs(buff,stdout);
					// get info and do something here
					memset(buff,0,256);
				}
			}
		}
	}

	return 0;
}
