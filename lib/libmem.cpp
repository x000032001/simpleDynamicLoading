#include "libmem.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

static int count;
static FILE *f = NULL;

extern "C" void init()
{
	count=0;
}

extern "C" void countInc()
{
	count++;
	//fprintf(stderr,"[TEST] now count = %d\n",count);
}

extern "C" void cleanup(void *useless)
{
	if(f)pclose(f);
}

extern "C" void* printMem(void* param)
{
	//init();
	fprintf( stderr , "[INFO] %s() has been called.\n" , __func__ );
	struct arguments *prm = (struct arguments *)param;
	fprintf( stderr , "[DEBUG] %s() using fd = %d.\n" , __func__ , prm->fd );
	pthread_cleanup_push (cleanup, NULL);
	while(1)
	{
		//countInc();
		char nl[256]={};
		f = popen("free -m","r");
		while( fgets(nl,255,f) != NULL )
		{
			//fprintf( stderr , "[DEBUG] write:%s\n" , nl );
			int rc = write(prm->fd,nl,strlen(nl));
			//fprintf( stderr , "[DEBUG] write ret:%d\n" , rc );
			
			if( rc < 0 )
			{
				fprintf( stderr , "[ERROR] %s() write fd:%d error.\n" , __func__ , prm->fd );
				return NULL;
			}
		}
		pclose(f);
		f=NULL;
		sleep(prm->iv);
	}
	pthread_cleanup_pop(1);

	return NULL;
}


