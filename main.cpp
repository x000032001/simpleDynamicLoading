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

	pluginManager pm;
	pm.addPluginByXML("set.xml");
	pm.addPluginByXML("set2.xml");
	pm.start();
	
	while( run )
		pm.printInfo();

	return 0;
}
