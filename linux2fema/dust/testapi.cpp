// TestApi.cpp : Defines the entry point for the console application.
//
#include "PublicFuncs.h"
#include "TraderSpi.h"
//#include "include/USTPFtdcTraderApi.h"
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
int main(int argc, char* argv[])
{
	ConfigParser cfg;
	cfg.Load("config.ini");
	string test1;
	int u = cfg.GetFileSize();
	cfg.GetConfigString(test1,string("BROKER_ID"));
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
	//setlocale(LC_ALL, "zh_CN.GB18030");
	//init
	memset(g_frontaddr,0,BUFLEN);
	//strcpy(g_frontaddr,"tcp://172.31.112.133:15555");
	strcpy(g_frontaddr,argv[1]);
	char test [] = "中国人";
	printf("req login中国人\n");
	printf("broker id：\n");
	scanf("%s",g_BrokerID);
	printf("broker id[%s]\n", g_BrokerID);
	printf("userid：\n" );
	scanf("%s",g_UserID);
	printf("userid[%s]\n", g_UserID);
	printf("password：\n" );
	scanf("%s",g_Password);
	printf("password is[%s]\n", g_Password);
	printf("Input g_BrokerID=[%s]g_UserID=[%s]g_Password=[%s]\n",g_BrokerID,g_UserID,g_Password);
	getchar();
	CMyTraderApi *pTrader = CMyTraderApi::CreateFtdcTraderApi("");	
	//CUstpFtdcTraderApi * realapi = pTrader->realapi;

	g_puserapi=pTrader;

 	CTraderSpi spi((CUstpFtdcTraderApi *)pTrader);
 	pTrader->RegisterFront(g_frontaddr);	
	pTrader->SubscribePrivateTopic(USTP_TERT_RESTART);
	pTrader->SubscribePublicTopic(USTP_TERT_RESTART);
	pTrader->RegisterSpi(&spi);
	pTrader->Init();

	pTrader->Join();
	pTrader->Release();


	return 0;
}
