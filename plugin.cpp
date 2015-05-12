#include "plugin.h"
#include <signal.h>

plugin::plugin()
{
	runFunc = NULL;
	dlHandle = NULL;
	libN = NULL;
	funcN = NULL;
	fd[0]=0;
	fd[1]=0;
}

plugin::~plugin()
{
	if(fd[0]){
		fprintf(stderr,"[DEBUG] destroy plugin, close fd %d,%d\n",fd[0],fd[1]);
		close(fd[0]);
		close(fd[1]);

		fprintf(stderr,"[DEBUG] destroy plugin, wait for thread join.\n");
		pthread_cancel(id);
		pthread_join(id,NULL);

		fprintf(stderr,"[DEBUG] destroy plugin, release resource.\n");
		if(dlHandle)dlclose(dlHandle);
		if(libN)free(libN);
		if(funcN)free(funcN);

		fprintf(stderr,"[DEBUG] destroy plugin, done.\n");
	}
}

void plugin::initPipe()
{
	if( pipe(fd) )
	{
		fprintf(stderr,"[ERROR] %s:%d create pipe error.\n",__FILE__,__LINE__);
		exit(1);
	}

	// must set to nonblock
	// or Locking in while loop pluginManager.cpp:printInfo()
	int flags = fcntl(fd[0], F_GETFL, 0);
	fcntl(fd[0], F_SETFL, flags | O_NONBLOCK);

	fprintf(stderr,"[INFO] pipe inited ,plugin get fd = %d,%d\n",fd[0],fd[1]);
}

void plugin::setArgs(const struct arguments &passArgs)
{
	//fprintf(stderr,"[DEBUG] creating thread\n");
	if( runFunc == NULL )
	{
		fprintf( stderr , "[ERROR] null function.\n" );
		return;
	}
	args = passArgs;
	args.fd = fd[1];
	//fprintf(stderr,"[DEBUG] passing fd = %d\n" , args.fd );
	//fprintf(stderr,"[DEBUG] after passing fd = %d\n" , args.fd );
}

void plugin::createThread()
{
	if( pthread_create( &id , NULL , runFunc , (void*)&args  ) )
	{
		fputs("[ERROR] pthread create error",stderr);
		exit(1);
	}
}

void plugin::getFunc(const char* lib,const char* funcName)
{
	libN = strdup(lib);
	funcN = strdup(funcName);
	dlHandle = dlopen( libN , RTLD_NOW );
	if( !dlHandle )
	{
		fputs( dlerror() , stderr );
		exit(1);
	}

	runFunc = (func_t)dlsym( dlHandle , funcN );

	char* error = NULL;
	if( (error = dlerror()) != NULL )
	{
		fputs( error , stderr );
		exit(1);
	}
	
	//fprintf(stderr,"[DEBUG] get %s:%s\n",libN,funcN);
}
