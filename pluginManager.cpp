#include "pluginManager.h"

pluginManager::pluginManager()
{
	max_fd=0;
	size=0;
	FD_ZERO(&current_fds);
}

pluginManager::~pluginManager()
{
}

void pluginManager::start()
{
	for( size_t i = 0 ; i < size ; ++i )
		plugins[i].createThread();
}

void pluginManager::addPlugin(const char* lib,const char* func,const struct arguments &A)
{
	//fprintf(stderr,"[DEBUG] addPlugin(%s,%s)\n",lib,func);
	
	plugins[size].initPipe();
	plugins[size].getFunc(lib,func);
	plugins[size].setArgs(A);
	int rfd = plugins[size].getReadFd();
	FD_SET(rfd ,&current_fds);
	if( rfd > max_fd )
		max_fd = rfd;
	size++;
	if( size >= MAX_SIZE )
	{
		fprintf(stderr,"[CRIT] plugins reach max size.\n");
	}
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
	
	for( size_t i = 0 ; i < size ; ++i )
	{
		fprintf( stderr , "[DEBUG] testing fd = %d\n" , plugins[i].getReadFd() );
		if( FD_ISSET( plugins[i].getReadFd() , &fds ) )
		{
			char buff[256] = {};
			while( read(plugins[i].getReadFd(),buff,255) >= 0 )
			{
				fprintf( stderr , "[DEBUG] read info from pipe:\n %s\n",buff );
				memset(buff,0,256);
			}
		}
	}
}
