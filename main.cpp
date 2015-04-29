#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include "tinyxml2.h"

using namespace tinyxml2;

typedef void (*func_t)(int);

func_t getFunc(const char* lib,const char* funcName,void *handle)
{
	handle = dlopen( lib , RTLD_LAZY );
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

void* monitorFuncHandler(void* param)
{

	XMLDocument doc;
	int fd[2];
	char buff[256];

	// using xml to load setting
	doc.LoadFile((char*)param);

	const char* freqs = doc.FirstChildElement("freq")->GetText();
	int sleepInterval = atoi(freqs);

	const char* libName = doc.FirstChildElement("libname")->GetText();
	const char* funcName = doc.FirstChildElement("funcname")->GetText();
	void* handle; // get handle and release at exit
	func_t func = getFunc(libName ,funcName ,&handle);

	// get a pipe fd pair
	pipe(fd);

	// set to nonblocking ,or using select()
	int flags = fcntl(fd[0], F_GETFL, 0);
	fcntl(fd[0], F_SETFL, flags | O_NONBLOCK);

	while(1)
	{
		// call function
		func(fd[1]);

		memset(buff,0,256);
		while( read(fd[0],buff,256) > 0 )
		{
			system("date");
			fputs(buff,stdout);
			// get info and do something here
			memset(buff,0,256);
		}

		sleep(sleepInterval);
	}

	dlclose(handle);
	pthread_exit(NULL);
}

int main()
{
	pthread_t id;
	const char* xml = "set.xml";
	if( pthread_create( &id , NULL , &monitorFuncHandler , (void*)xml ) )
	{
		fputs("pthread create error",stderr);
		exit(1);
	}

	pthread_join(id,NULL);

	return 0;
}
