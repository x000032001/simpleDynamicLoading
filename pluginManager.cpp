#include "pluginManager.h"

pluginManager::pluginManager()
{
	max_fd=0;
	FD_ZERO(&current_fds);
}

pluginManager::~pluginManager()
{
	while( !plugins.empty() )
	{
		plugin *ptrPlugin = plugins.back();
		delete ptrPlugin;
		plugins.pop_back();
	}
}

void pluginManager::start()
{
	for( size_t i = 0 ; i < plugins.size() ; ++i )
		plugins[i]->createThread();
}

void pluginManager::addPlugin(const char* lib,const char* func,const struct arguments &A)
{
	fprintf(stderr,"[DEBUG] addPlugin(%s,%s)\n",lib,func);
	
	plugin* newPlugin = new plugin();
	plugins.push_back(newPlugin);
	size_t size = plugins.size()-1;
	plugins[size]->initPipe();
	plugins[size]->getFunc(lib,func);
	plugins[size]->setArgs(A);
	int rfd = plugins[size]->getReadFd();
	FD_SET(rfd ,&current_fds);
	if( rfd > max_fd )
		max_fd = rfd;
}

void pluginManager::addPluginByXML(const char* name)
{
	//fprintf(stderr,"[DEBUG] addPluginByXML(%s)\n",name);
	XMLDocument doc;
	doc.LoadFile(name);

	// TODO add exception handle here
	const char* libName = doc.FirstChildElement("libname")->GetText();
	const char* funcName = doc.FirstChildElement("funcname")->GetText();
	const char* freqs = doc.FirstChildElement("freq")->GetText();

	int sleepInterval = atoi(freqs);

	struct arguments A;
	A.iv = sleepInterval;

	addPlugin(libName ,funcName ,A);
}

void pluginManager::printInfo()
{
	fd_set fds = current_fds;

	// if need nonblock, edit select parameter
	int rc = select( max_fd+1 , &fds , NULL , NULL , NULL );	
	
	for( size_t i = 0 ; i < plugins.size() ; ++i )
	{
		if( FD_ISSET( plugins[i]->getReadFd() , &fds ) )
		{
			fprintf(stderr,"[DEBUG] FD:%d ======== recv info ========\n",plugins[i]->getReadFd() );
			char buff[256] = {};
			while( read(plugins[i]->getReadFd(),buff,255) >= 0 )
			{
				fprintf( stderr , "%s",buff );
				memset(buff,0,256);
			}
			fprintf(stderr,"[DEBUG] FD:%d ========   done    ========\n",plugins[i]->getReadFd() );
		}
	}
}
