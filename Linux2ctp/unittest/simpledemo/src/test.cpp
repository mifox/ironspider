#include "test.h"



void test_order(void)
{
  //��ʼ��UserApi
  CThostFtdcTraderApi* pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("trade");
  CtpTraderSpi* pUserSpi = new CtpTraderSpi(pUserApi);
  pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// ע���¼���
  pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);					// ע�ṫ����
  pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);			  // ע��˽����
  pUserApi->RegisterFront(tradeFront);							// ע�ύ��ǰ�õ�ַ
  
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
	  //cerr<<"��ʽ: ���� ����, ��������."<<endl;  
	  //cin>>x;
	  test_order();
  }
  
  else if(strcmp(argv[1],"--order")==0) test_order();
}