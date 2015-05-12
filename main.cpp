#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "pluginManager.h"

bool run = true;

void my_exit(int sig)
{
	puts("catch SIGNAL Interrupt. set run flag to false.");
	run = false;
}

int main()
{
	signal(SIGINT, my_exit);

	for( int k = 0 ; k < 2 ; ++k )
	{
		fprintf(stderr,"creating pluginManager\n");
		pluginManager pm;
		pm.addPluginByXML("set.xml");
		pm.addPluginByXML("set2.xml");
		pm.start();
		
		int i = 4;
		while( i-- )
			pm.printInfo();
		fprintf(stderr,"deleting pluginManager\n");
	}

	return 0;
}
