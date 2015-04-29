#include "lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


extern "C" void printMem(int fd)
{
	char nl[256]={};
	FILE *f = popen("free -m","r");
	while( fgets(nl,256,f) != NULL )
	{
		write(fd,nl,strlen(nl));
	}
	pclose(f);
}

