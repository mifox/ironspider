#include "test.h"



void test_order(void)
{
  //初始化UserApi
  CThostFtdcTraderApi* pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("trade");
  CtpTraderSpi* pUserSpi = new CtpTraderSpi(pUserApi);
  pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// 注册事件类
  pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);					// 注册公有流
  pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);			  // 注册私有流
  pUserApi->RegisterFront(tradeFront);							// 注册交易前置地址
  
  pUserApi->Init();
  ShowTraderCommand(pUserSpi,true); 
  pUserApi->Join();  
  //pUserApi->Release();
}
void main(int argc, const char* argv[]){
 
	
	char str_a[100]="abc,gc,gd,g";
	char * b=strtok(str_a,",");
	char * c=strtok(NULL,",");

	
	g_hEvent=CreateEvent(NULL, true, false, NULL); 
  int x;
  if(argc < 2) {
	  //cerr<<"格式: 命令 参数, 输入有误."<<endl;  
	  //cin>>x;
	  test_order();
  }
  
  else if(strcmp(argv[1],"--order")==0) test_order();
}