#include "lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern "C" void* printMem(void* param)
{
	struct args *prm = (struct args *)param;
	printf("FD=%d\n",prm->fd);
	// just get info from process and write to fd pipe
	while(1)
	{
		char nl[256]={};
		FILE *f = popen("free -m","r");
		while( fgets(nl,256,f) != NULL )
		{
			write(prm->fd,nl,strlen(nl));
		}
		pclose(f);
		sleep(prm->iv);
	}
}

