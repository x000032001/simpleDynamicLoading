#include "libdsk.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

static FILE *f = NULL;

extern "C" void cleanup(void *useless)
{
	if(f)pclose(f);
}

extern "C" void* printDisk(void* param)
{
	fprintf( stderr , "[INFO] %s() has been called.\n" , __func__ );
	struct arguments *prm = (struct arguments *)param;
	fprintf( stderr , "[DEBUG] %s() using fd = %d.\n" , __func__ , prm->fd );
	pthread_cleanup_push (cleanup, NULL);
	while(1)
	{
		char nl[256]={};
		f = popen("df -h 2>/dev/null","r");
		while( fgets(nl,256,f) != NULL )
		{
			if( 0 > write(prm->fd,nl,strlen(nl)) )
			{
				fprintf( stderr , "[ERROR] %s() write fd:%d error.\n" , __func__ , prm->fd );
				return NULL;
			}
		}
		pclose(f);
		f = NULL;
		sleep(prm->iv);
	}
	pthread_cleanup_pop(1);

	return NULL;
}

