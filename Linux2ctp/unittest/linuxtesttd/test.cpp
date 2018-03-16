#include "test.h"
#include "PublicFuncs.h"  
;

int requestId=0;
void test_order(void)
{
	//char tradeFront[]="tcp://203.156.223.164:41205";
	char tradeFront[]="tcp://172.37.98.51:41205"; 
	//char tradeFront[]="tcp://58.246.173.2:31803"; 
  //��ʼ��UserApi
  CThostFtdcTraderApi* pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("trade");
  CtpTraderSpi* pUserSpi = new CtpTraderSpi(pUserApi);
  pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// ע���¼���
  pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);					// ע�ṫ����
  pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);			  // ע��˽����
  pUserApi->RegisterFront(tradeFront);							// ע�ύ��ǰ�õ�ַ
  CountedPtr<Car> car(new Car);
  pUserSpi->setCar(car);
  pUserApi->Init();
  ShowTraderCommand(pUserSpi,true); 
  pUserApi->Join();  
  
  //pUserApi->Release();
}
int main(int argc, const char* argv[]){
 
	

	
	
	//g_hEvent=CreateEvent(NULL, true, false, NULL); 
  int x;
  if(argc < 2) {
	  //cerr<<"��ʽ: ���� ����, ��������."<<endl;  
	  //cin>>x;
	  test_order();
  }
  
  else if(strcmp(argv[1],"--order")==0) test_order();
}

void ShowTraderCommand( CtpTraderSpi* p, bool print/*=false*/ )
{
	
	if(print){
		
		cerr<<"-----------------------------------------------"<<endl;
		char * t = UTF8("�й���");
		//xxxxx
			
// 		xxxxx
// 		//free(t);
		//free(buffer);
		cerr<<" [1] ReqUserLogin              -- "<<endl;
		cerr<<" [2] ReqSettlementInfoConfirm  -- ���㵥ȷ��"<<endl;
		cerr<<" [3] ReqQryInstrument          -- ��ѯ��Լ"<<endl;
		cerr<<" [4] ReqQryTradingAccount      -- ��ѯ�ʽ�"<<endl;
		cerr<<" [5] ReqQryInvestorPosition    -- ��ѯ�ֲ�"<<endl;
		cerr<<" [6] ReqOrderInsert            -- ����"<<endl;
		cerr<<" [7] ReqOrderAction            -- ����"<<endl;
		cerr<<" [8] PrintOrders               -- ��ʾ����"<<endl;
		cerr<<" [9] PrintOrders               -- ��ʾ�ɽ�"<<endl;
		cerr<<" [0] Exit                      -- �˳�"<<endl;
		cerr<<"----------------------------------------------"<<endl;
	}   
	TThostFtdcBrokerIDType	    appId;
	TThostFtdcUserIDType	        userId;
	TThostFtdcPasswordType	    passwd;
	TThostFtdcInstrumentIDType    instId;
	TThostFtdcDirectionType       dir;
	TThostFtdcCombOffsetFlagType  kpp;
	TThostFtdcPriceType           price;
	TThostFtdcVolumeType          vol;
	TThostFtdcSequenceNoType      orderSeq;

	int cmd;  cin>>cmd;
	switch(cmd){
	case 1: {
		cerr<<" Ӧ�õ�Ԫ > ";//cin>>appId;
		cerr<<" Ͷ���ߴ��� > ";//cin>>userId;
		cerr<<" �������� > ";//cin>>passwd;
		strcpy(appId,"6000");
		strcpy(userId,"00800142");
		//strcpy(userId,"00802339");
		strcpy(passwd,"play4444");

		p->ReqUserLogin(appId,userId,passwd); break;
			}
	case 11: {
		cerr<<" Ӧ�õ�Ԫ > ";//cin>>appId;
		cerr<<" Ͷ���ߴ��� > ";//cin>>userId;
		cerr<<" �������� > ";//cin>>passwd;
		strcpy(appId,"6000");
		strcpy(userId,"00800142");
		//strcpy(userId,"00802339");
		strcpy(passwd,"play4444");

		p->ReqUserLoginout(appId,userId,passwd); break;
			}
	case 2: p->ReqSettlementInfoConfirm(); break;
	case 3: {
		cerr<<" ��Լ > "; cin>>instId; 
		p->ReqQryInstrument(instId); break;
			}
	case 4: p->ReqQryTradingAccount(); break;
	case 5: {
		cerr<<" ��Լ > "; cin>>instId; 
		p->ReqQryInvestorPosition(instId); break;
			}
	case 6: {
		cerr<<" instId > "; cin>>instId; 
		cerr<<" dir > "; cin>>dir; 
		cerr<<" kpp > "; cin>>kpp;
		cerr<<" price > "; cin>>price;
		cerr<<" vol > "; cin>>vol;              
		p->ReqOrderInsert(instId,dir,kpp,price,vol); break;
			}
	case 7: { 
		cerr<<" ��� > "; cin>>orderSeq;
		p->ReqOrderAction(orderSeq);break;
			}
	case 8: p->PrintOrders();break;
	case 9: p->PrintTrades();break;
	case 0: exit(0);
	}  
	//WaitForSingleObject(g_hEvent,INFINITE);
	//ResetEvent(g_hEvent);
	p->pCar->waitForState(INFINITE,EVENT_A);
	ShowTraderCommand(p);
}