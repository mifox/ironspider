#include <iostream>
#include "api/trade/linux64/public/ThostFtdcTraderApi.h"
#include "order/traderspi.h"
#include "config.h"
using namespace std;
/*
int main1(int argc, char *argv[])
{
	
	 //初始化UserApi
	CThostFtdcTraderApi* pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("trade");
	CtpTraderSpi* pUserSpi = new CtpTraderSpi(pUserApi);
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// 注册事件类
	pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);					// 注册公有流
	pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);			  // 注册私有流
	pUserApi->RegisterFront(tradeFront);							// 注册交易前置地址
  
	pUserApi->Init();
	//ShowTraderCommand(pUserSpi, true); 
	pUserApi->Join();  
	
	char sz[] = "Hello, World!";	//Hover mouse over "sz" while debugging to see its contents
	cout << sz << endl;	//<================= Put a breakpoint here
	return 0;
}*/