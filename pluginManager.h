#ifndef PM_H
#define PM_H

#include "plugin.h"
#include "tinyxml2.h"

#define MAX_SIZE 10

using namespace tinyxml2;

class pluginManager {
	public:
		pluginManager();
		~pluginManager();
		void start();
		void addPlugin(const char*,const char*,const struct arguments&);
		void addPluginByXML(const char*);

		void printInfo();
	private:
		plugin plugins[MAX_SIZE];
		fd_set current_fds;
		int max_fd;
		size_t size;
		
};
#endif
