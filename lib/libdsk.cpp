#include "libdsk.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern "C" void* printDisk(void* param)
{
	fprintf( stderr , "[INFO] %s() has been called.\n" , __func__ );
	struct arguments *prm = (struct arguments *)param;
	fprintf( stderr , "[DEBUG] %s() using fd = %d.\n" , __func__ , prm->fd );
	while(1)
	{
		char nl[256]={};
		FILE *f = popen("df -h","r");
		while( fgets(nl,256,f) != NULL )
		{
			if( 0 > write(prm->fd,nl,strlen(nl)) )
			{
				fprintf( stderr , "[ERROR] %s() write fd:%d error.\n" , __func__ , prm->fd );
				return NULL;
			}
		}
		pclose(f);
		sleep(prm->iv);
	}

	return NULL;
}

