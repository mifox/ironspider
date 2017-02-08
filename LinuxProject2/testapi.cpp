// TestApi.cpp : Defines the entry point for the console application.
//
#include "PublicFuncs.h"
#include "TraderSpi.h"
#include "MdSpi.h"
//#include "include/DINGFtdcTraderApi.h"
#include <iostream>
#include <locale.h>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/sleep.h>
#include <log4cplus/loggingmacros.h>
#include "webserver/webserverInstance.h"
#include "ConfigParser.h"
#include "myapi.h"
#include "myMdapi.h"
using namespace log4cplus;
using namespace log4cplus::helpers;
int g_nSwitch=0;

extern int g_nOrdSpeed;

void usage(char* pName)
{
	printf("usage:%s FrontAddress (tcp://117.185.125.9:53126)\n",pName);
	return;
}

Logger log_1 =  Logger::getInstance(LOG4CPLUS_TEXT("test.log_1"));
Logger log_2 =  Logger::getInstance(LOG4CPLUS_TEXT("test.log_2"));
Logger log_3 =  Logger::getInstance(LOG4CPLUS_TEXT("test.log_3"));

#include"zthread/Thread.h"
#include"zthread/Runnable.h"
#include<iostream>
#include "config.h"
#include "atk.h"
class RunThread:public ZThread::Runnable{
public:
	RunThread(int nID=0):id(nID){}
	~RunThread(){std::cout<<"Thread "<<id<<" exit"<<std::endl;}
	void run()
	{
// 		for(int i=0;i!=10;++i){
// 			std::cout<<"Thread "<<id<<" is running "<<std::endl;
// 		}
		try{
			webserver(atoi(webport), Get);
			//ZThread::Thread t2(new RunThread(2));
		}catch(char* t){
			std::cerr<<"listen error"<<std::endl;
		}

		
	}

private:
	int id;
};
int webthreadTest()
{
	try{
		ZThread::Thread t1(new RunThread(1));
		//ZThread::Thread t2(new RunThread(2));
	}catch(ZThread::Synchronization_Exception &err){
		std::cerr<<err.what()<<std::endl;
	}
	
	//system("pause");
	return 0;
}


void ConfigureFileTest()
{
	
}

int main(int argc, char* argv[])
{
	
// 	ConfigParser cfg;
// 	cfg.Load("config.ini");
// 	string test1;
// 	int u = cfg.GetFileSize();
// 	cfg.GetConfigString(test1, string("BROKER_ID"));
	configFromIniFile();
	
	
	//webserver(8888, Get);
	log4cplus::initialize();
	LogLog::getLogLog()->setInternalDebugging(true);
	Logger root = Logger::getRoot();
	try 
	{
		ConfigureAndWatchThread configureThread(
			LOG4CPLUS_TEXT("log4cplus.properties"),
			5 * 1000);

		LOG4CPLUS_WARN(root, "Testing....");
	}
	catch (...) {
		LOG4CPLUS_FATAL(root, "Exception occured...");
	}
	
	
	
	if(argc!=2)
	{
		usage(argv[0]);
		return -1;
	}
	else
	{
		if (!strcmp(argv[1],"--closetoday"))
		{
			functionNO = FUNCTION_CLOSETODAY;
			LOG4CPLUS_DEBUG_FMT(log_1,"functionNO %d",functionNO);
		}
	}
	//web server start
// 	ConfigureFileTest();
 	webthreadTest();
// 	//setlocale(LC_ALL, "zh_CN.GB18030");
// 	//init
// 	memset(g_frontaddr,0,BUFLEN);
// 	//strcpy(g_frontaddr,"tcp://172.31.112.133:15555");
// 	strcpy(g_frontaddr,argv[1]);
// 	char test [] = "";
// 	printf("req login\n");
// 	printf("broker id：\n");
// 	scanf("%s",g_BrokerID);
// 	printf("broker id[%s]\n", g_BrokerID);
// 	printf("userid：\n" );
// 	scanf("%s",g_UserID);
// 	printf("userid[%s]\n", g_UserID);
// 	printf("password：\n" );
// 	scanf("%s",g_Password);
// 	printf("password is[%s]\n", g_Password);
// 	printf("Input g_BrokerID=[%s]g_UserID=[%s]g_Password=[%s]\n",g_BrokerID,g_UserID,g_Password);
// 	getchar();

	CtpAtkMd* recMd=CtpAtkMd::CreateAtkApi();
	
// 	CMyMdApi *pMd = CMyMdApi::CreateFtdcTraderApi("");	
// 
// 	g_puserMdapi=pMd;
// 	CMdSpi spiMd((CDINGFtdcMduserApi *)pMd);
// 	pMd->RegisterFront(g_frontaddr);	
// 	pMd->RegisterSpi((CDINGFtdcMduserSpi*)&spiMd);
// 	pMd->Init();
// 
// 	pMd->Join();
// 	pMd->Release(); 
// 
// 	CMyTraderApi *pTrader = CMyTraderApi::CreateFtdcTraderApi("");	
// 
//  	CTraderSpi spi((CMyTraderApi *)pTrader);
//  	pTrader->RegisterFront(g_frontaddr);	
// 	pTrader->SubscribePrivateTopic(DING_TERT_RESTART);
// 	pTrader->SubscribePublicTopic(DING_TERT_RESTART);
// 	pTrader->RegisterSpi((CDINGFtdcTraderSpi*)&spi);
// 	pTrader->Init();
// 
// 	pTrader->Join();
// 	pTrader->Release();


	return 0;
}
