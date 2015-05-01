
#include "pluginManager.h"

int main()
{
	pluginManager pm;
	pm.addPluginByXML("set.xml");
	pm.addPluginByXML("set2.xml");
	pm.start();
	
	pm.printInfo();

	return 0;
}
