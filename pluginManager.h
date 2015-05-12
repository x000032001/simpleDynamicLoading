#ifndef PM_H
#define PM_H

#include <vector>
#include "plugin.h"
#include "tinyxml2.h"

#define MAX_SIZE 10

using std::vector;
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
		//plugin plugins[MAX_SIZE];
		vector<plugin*> plugins;
		fd_set current_fds;
		int max_fd;
		//size_t size;
		
};
#endif
