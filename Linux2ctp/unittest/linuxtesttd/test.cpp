#include "test.h"
#include "PublicFuncs.h"  
;

int requestId=0;
void test_order(void)
{
	//char tradeFront[]="tcp://203.156.223.164:41205";
	char tradeFront[]="tcp://172.37.98.51:41205"; 
	//char tradeFront[]="tcp://58.246.173.2:31803"; 
  //初始化UserApi
  CThostFtdcTraderApi* pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("trade");
  CtpTraderSpi* pUserSpi = new CtpTraderSpi(pUserApi);
  pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// 注册事件类
  pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);					// 注册公有流
  pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);			  // 注册私有流
  pUserApi->RegisterFront(tradeFront);							// 注册交易前置地址
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
	  //cerr<<"格式: 命令 参数, 输入有误."<<endl;  
	  //cin>>x;
	  test_order();
  }
  
  else if(strcmp(argv[1],"--order")==0) test_order();
}

void ShowTraderCommand( CtpTraderSpi* p, bool print/*=false*/ )
{
	
	if(print){
		
		cerr<<"-----------------------------------------------"<<endl;
		char * t = UTF8("中国人");
		//xxxxx
			
// 		xxxxx
// 		//free(t);
		//free(buffer);
		cerr<<" [1] ReqUserLogin              -- "<<endl;
		cerr<<" [2] ReqSettlementInfoConfirm  -- 结算单确认"<<endl;
		cerr<<" [3] ReqQryInstrument          -- 查询合约"<<endl;
		cerr<<" [4] ReqQryTradingAccount      -- 查询资金"<<endl;
		cerr<<" [5] ReqQryInvestorPosition    -- 查询持仓"<<endl;
		cerr<<" [6] ReqOrderInsert            -- 报单"<<endl;
		cerr<<" [7] ReqOrderAction            -- 撤单"<<endl;
		cerr<<" [8] PrintOrders               -- 显示报单"<<endl;
		cerr<<" [9] PrintOrders               -- 显示成交"<<endl;
		cerr<<" [0] Exit                      -- 退出"<<endl;
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
		cerr<<" 应用单元 > ";//cin>>appId;
		cerr<<" 投资者代码 > ";//cin>>userId;
		cerr<<" 交易密码 > ";//cin>>passwd;
		strcpy(appId,"6000");
		strcpy(userId,"00800142");
		//strcpy(userId,"00802339");
		strcpy(passwd,"play4444");

		p->ReqUserLogin(appId,userId,passwd); break;
			}
	case 11: {
		cerr<<" 应用单元 > ";//cin>>appId;
		cerr<<" 投资者代码 > ";//cin>>userId;
		cerr<<" 交易密码 > ";//cin>>passwd;
		strcpy(appId,"6000");
		strcpy(userId,"00800142");
		//strcpy(userId,"00802339");
		strcpy(passwd,"play4444");

		p->ReqUserLoginout(appId,userId,passwd); break;
			}
	case 2: p->ReqSettlementInfoConfirm(); break;
	case 3: {
		cerr<<" 合约 > "; cin>>instId; 
		p->ReqQryInstrument(instId); break;
			}
	case 4: p->ReqQryTradingAccount(); break;
	case 5: {
		cerr<<" 合约 > "; cin>>instId; 
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
		cerr<<" 序号 > "; cin>>orderSeq;
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