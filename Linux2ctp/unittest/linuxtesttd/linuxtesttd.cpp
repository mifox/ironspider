#include <iostream>
#include "api/trade/linux64/public/ThostFtdcTraderApi.h"
#include "order/traderspi.h"
#include "config.h"
using namespace std;
/*
int main1(int argc, char *argv[])
{
	
	 //��ʼ��UserApi
	CThostFtdcTraderApi* pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("trade");
	CtpTraderSpi* pUserSpi = new CtpTraderSpi(pUserApi);
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// ע���¼���
	pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);					// ע�ṫ����
	pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);			  // ע��˽����
	pUserApi->RegisterFront(tradeFront);							// ע�ύ��ǰ�õ�ַ
  
	pUserApi->Init();
	//ShowTraderCommand(pUserSpi, true); 
	pUserApi->Join();  
	
	char sz[] = "Hello, World!";	//Hover mouse over "sz" while debugging to see its contents
	cout << sz << endl;	//<================= Put a breakpoint here
	return 0;
}*/