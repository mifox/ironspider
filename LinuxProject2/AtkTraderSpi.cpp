// TraderSpi.cpp: implementation of the AtkCTraderSpi class.
//
//////////////////////////////////////////////////////////////////////
#include "PublicFuncs.h"
#include "AtkTraderSpi.h"
#include "config.h"
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/sleep.h>
#include <log4cplus/loggingmacros.h>
using namespace log4cplus;
using namespace log4cplus::helpers;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern int  nRequestID;

extern FILE * g_fpRecv;
class buffer 
{
public:
	buffer(int size)
	{
		body = (char *) malloc(sizeof(int) * size);
	}
	;
	~buffer()
	{
		free(body);
	}
	char * body;	
};
#ifdef WIN32
#define  UTF8(A) A
#else
#define  UTF8(A) g2uc(A, sizeof(A), buffer(sizeof(A)).body, sizeof(A))
#endif
AtkCTraderSpi::AtkCTraderSpi(CMyTraderApi *pTrader):m_pUserApi(pTrader)
{
	
}
AtkCTraderSpi::~AtkCTraderSpi()
{

}


int AtkCTraderSpi::setCar(CountedPtr<Car>& pcar)
{
	this->pCar = pcar;
	return 0;
}


void AtkCTraderSpi::OnFrontConnected()
{
	this->pCar->sendSignal(EVENT_TD_CONNECT);
//	CDINGFtdcReqUserLoginField reqUserLogin;
// 	memset(&reqUserLogin,0,sizeof(CDINGFtdcReqUserLoginField));		
// 	strcpy(reqUserLogin.BrokerID,g_BrokerID);
// 	strcpy(reqUserLogin.UserID, g_UserID);
// 	strcpy(reqUserLogin.Password, g_Password);	
// 	strcpy(reqUserLogin.UserProductInfo,g_pProductInfo);

	//m_pUserApi->ReqUserLogin(&reqUserLogin, g_nOrdLocalID);	
	
	LOG4CPLUS_DEBUG_FMT(log_1,"请求登录，BrokerID=[%s]UserID=[%s]\n",g_BrokerID,g_UserID);
#ifdef WIN32
	Sleep(1000);
#else
	usleep(1000);
#endif
}



void AtkCTraderSpi::OnRspUserLogin(CDINGFtdcRspUserLoginField *pRspUserLogin, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		LOG4CPLUS_DEBUG_FMT(log_1,"登录失败...错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		return;
	}
	g_nOrdLocalID=atoi(pRspUserLogin->MaxOrderLocalID)+1;
    sprintf(orderRef,"%d",g_nOrdLocalID);
 	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
 	LOG4CPLUS_DEBUG_FMT(log_1,"登录成功，最大本地报单号:%d\n",g_nOrdLocalID);
	
 	LOG4CPLUS_DEBUG_FMT(log_1,"pRspUserLogin->DCETime:%s\n",pRspUserLogin->DCETime);
	LOG4CPLUS_DEBUG_FMT(log_1,"pRspUserLogin->CZCETime:%s\n",pRspUserLogin->CZCETime);
	LOG4CPLUS_DEBUG_FMT(log_1,"pRspUserLogin->SHFETime:%s\n",pRspUserLogin->SHFETime);
	LOG4CPLUS_DEBUG_FMT(log_1,"pRspUserLogin->FFEXTime:%s\n",pRspUserLogin->FFEXTime);
	memcpy(&rspUserLogin,pRspUserLogin,sizeof(CDINGFtdcRspUserLoginField));
	this->pCar->sendSignal(EVENT_TD_LOGIN);
 	//StartAutoOrder();
}

void AtkCTraderSpi::OnRspUserLogout(CDINGFtdcRspUserLogoutField *pRspUserLogout, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	LOG4CPLUS_DEBUG_FMT(log_1,"登chu成功，");
	this->pCar->sendSignal(EVENT_TD_LOGOUT);
};

void AtkCTraderSpi::OnRspOrderInsert(CDINGFtdcInputOrderField *pInputOrder, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		LOG4CPLUS_DEBUG_FMT(log_1,"报单失败 错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		return;
	}
	if(pInputOrder==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有报单数据\n");
		return;
	}
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"报单成功\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	return ;
	
}


void AtkCTraderSpi::OnRtnTrade(CDINGFtdcTradeField *pTrade)
{
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"收到成交回报\n");
	Show(pTrade);
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	return;
}

void AtkCTraderSpi::Show(CDINGFtdcOrderField *pOrder)
{
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"交易所代码=[%s]\n",pOrder->ExchangeID);
	LOG4CPLUS_DEBUG_FMT(log_1,"交易日=[%s]\n",pOrder->TradingDay);
	LOG4CPLUS_DEBUG_FMT(log_1,"会员编号=[%s]\n",pOrder->ParticipantID);
	LOG4CPLUS_DEBUG_FMT(log_1,"下单席位号=[%s]\n",pOrder->SeatID);
	LOG4CPLUS_DEBUG_FMT(log_1,"投资者编号=[%s]\n",pOrder->InvestorID);
	LOG4CPLUS_DEBUG_FMT(log_1,"客户号=[%s]\n",pOrder->ClientID);
	LOG4CPLUS_DEBUG_FMT(log_1,"系统报单编号=[%s]\n",pOrder->OrderSysID);
	LOG4CPLUS_DEBUG_FMT(log_1,"本地报单编号=[%s]\n",pOrder->OrderLocalID);
	LOG4CPLUS_DEBUG_FMT(log_1,"用户本地报单号=[%s]\n",pOrder->UserOrderLocalID);
	LOG4CPLUS_DEBUG_FMT(log_1,"合约代码=[%s]\n",pOrder->InstrumentID);
	LOG4CPLUS_DEBUG_FMT(log_1,"报单价格条件=[%c]\n",pOrder->OrderPriceType);
	LOG4CPLUS_DEBUG_FMT(log_1,"买卖方向=[%c]\n",pOrder->Direction);
	LOG4CPLUS_DEBUG_FMT(log_1,"开平标志=[%c]\n",pOrder->OffsetFlag);
	LOG4CPLUS_DEBUG_FMT(log_1,"投机套保标志=[%c]\n",pOrder->HedgeFlag);
	LOG4CPLUS_DEBUG_FMT(log_1,"价格=[%lf]\n",pOrder->LimitPrice);
	LOG4CPLUS_DEBUG_FMT(log_1,"数量=[%d]\n",pOrder->Volume);
	LOG4CPLUS_DEBUG_FMT(log_1,"报单来源=[%c]\n",pOrder->OrderSource);
	LOG4CPLUS_DEBUG_FMT(log_1,"报单状态=[%c]\n",pOrder->OrderStatus);
	LOG4CPLUS_DEBUG_FMT(log_1,"报单时间=[%s]\n",pOrder->InsertTime);
	LOG4CPLUS_DEBUG_FMT(log_1,"撤销时间=[%s]\n",pOrder->CancelTime);
	LOG4CPLUS_DEBUG_FMT(log_1,"有效期类型=[%c]\n",pOrder->TimeCondition);
	LOG4CPLUS_DEBUG_FMT(log_1,"GTD日期=[%s]\n",pOrder->GTDDate);
	LOG4CPLUS_DEBUG_FMT(log_1,"最小成交量=[%d]\n",pOrder->MinVolume);
	LOG4CPLUS_DEBUG_FMT(log_1,"止损价=[%lf]\n",pOrder->StopPrice);
	LOG4CPLUS_DEBUG_FMT(log_1,"强平原因=[%c]\n",pOrder->ForceCloseReason);
	LOG4CPLUS_DEBUG_FMT(log_1,"自动挂起标志=[%d]\n",pOrder->IsAutoSuspend);
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	return ;
}

void AtkCTraderSpi::OnRtnOrder(CDINGFtdcOrderField *pOrder)
{
	if (onRtnOrderLogFlag==0 || atkmode == 1)
	{
		return;
	}

	CDINGFtdcOrderField* order = new CDINGFtdcOrderField;
	memcpy(order,  pOrder, sizeof(CDINGFtdcOrderField));
	bool founded=false;    unsigned int i=0;
	for(i=0; i<orderList.size(); i++){
		if(!strcmp(orderList[i]->UserOrderLocalID ,order->UserOrderLocalID)) {
			founded=true;    break;
		}
	}
	CDINGFtdcOrderField lastOrderStatus={0};
	if(founded) 
	{
		memcpy(&lastOrderStatus,orderList[i],sizeof(CDINGFtdcOrderField));
		orderList[i]= order;
	}   
	else  orderList.push_back(order);


	//atkorderList;

	bool founded2=false;     i=0;
	for(i=0; i<atkorderList.size(); i++){

		if(!strcmp(atkorderList[i]->main.UserOrderLocalID , order->UserOrderLocalID)) {
			founded2=true;    break;
		}
	}
	if(founded2) 
	{
		LOG4CPLUS_DEBUG(log_1,"founded2"<<founded2<<endl);
		memcpy(&(atkorderList[i]->main), pOrder,sizeof(CDINGFtdcOrderField));	
	}



	char tmpchar[512]={0};
	int	step=10;

	TDINGFtdcInstrumentIDType instId={0};
	TDINGFtdcDirectionType dir;
	TDINGFtdcOffsetFlagType kpp='1'; 
	TDINGFtdcPriceType price=0.0;
	TDINGFtdcVolumeType vol=0;

//	map<string,string>::iterator it;
	TDINGFtdcInstrumentIDType atkinstrumentID;
// 	it=common_params_maps.find("instrument1");
// 	if (it!=common_params_maps.end())
// 	{
// 		strcpy(atkinstrumentID,common_params_maps["instrument1"].data());
// 	}
// 	it=common_params_maps.find("step1");
// 	if (it!=common_params_maps.end())
// 	{
// 		int atkStep=atof((common_params_maps["step1"]).data());
// 		if (atkStep>0)
// 		{
// 			step = atkStep;
// 		}
// 	}
// 	LOG4CPLUS_DEBUG(log_1,"atkStep"<<step<<endl);
	double closeprice=0;

// 	it=common_params_maps.find("closeprice1");
// 	if (it!=common_params_maps.end())
// 	{
// 		closeprice=atof((common_params_maps["closeprice1"]).data());
// 
// 	}
// 	LOG4CPLUS_DEBUG(log_1,"closeprice1"<<closeprice<<endl);
	if (founded2)
	{
		closeprice = atkorderList[i]->closeprice;
		LOG4CPLUS_DEBUG(log_1,"closeprice1"<<closeprice<<endl);
		strcpy(atkinstrumentID,pOrder->InstrumentID);

	}
	

	int findmark = 0;
	LOG4CPLUS_DEBUG(log_1,"atkinstrumentID"<<atkinstrumentID<<endl);	
	LOG4CPLUS_DEBUG(log_1,"pOrder->InstrumentID"<<pOrder->InstrumentID<<endl);	
	if (strcmp(atkinstrumentID,pOrder->InstrumentID)== 0)
	{
		findmark=1;
	}
	LOG4CPLUS_DEBUG(log_1,"findmark"<<findmark<<endl);	
	//InitializeCriticalSection(&g_cs);
	LOG4CPLUS_DEBUG(log_1,"pOrder->OrderStatus"<<pOrder->OrderStatus<<endl);
	if(pOrder->OrderStatus == DING_FTDC_OS_AllTraded || pOrder->OrderStatus == DING_FTDC_OS_PartTradedNotQueueing || pOrder->OrderStatus ==DING_FTDC_OS_Canceled
		|| pOrder->OrderStatus ==DING_FTDC_OS_PartTradedQueueing)
	{
		LOG4CPLUS_DEBUG(log_1,"成交mark"<<endl);		
		//list<CThostFtdcOrderField*>::iterator pos;
		//for (pos=l2.begin(); pos!= l2.end(); ++pos)
		int inTheListFlag=0;
		//for(i=0; i<orderList.size(); i++)
		if(lastOrderStatus.OrderSysID)//非空
		{
			//LOG4CPLUS_DEBUG(log_1,"lastOrderStatus.BrokerOrderSeq"<<lastOrderStatus.BrokerOrderSeq<<endl);
			if(!strcmp(lastOrderStatus.OrderLocalID, pOrder->OrderLocalID)) 
			{			

				LOG4CPLUS_DEBUG(log_1,"lastOrderStatus.BrokerOrderSeq"<<lastOrderStatus.OrderLocalID<<endl);
				inTheListFlag=1;
				//CThostFtdcOrderField* temp=*pos;
				if (!strcmp(lastOrderStatus.UserOrderLocalID,lastOrderStatus.UserOrderLocalID) && lastOrderStatus.SessionID==pOrder->SessionID) //相同
				{				
					CDINGFtdcOrderField *m1=&lastOrderStatus;
					CDINGFtdcOrderField *m2=pOrder;
					LOG4CPLUS_DEBUG(log_1,"今日成交数量"<<m1->VolumeTraded<<endl);
					LOG4CPLUS_DEBUG(log_1,"今日成交数量"<<m2->VolumeTraded<<endl);
					if (m1->VolumeTraded < m2->VolumeTraded)
					{
						vol=m2->VolumeTraded-m1->VolumeTraded;
						LOG4CPLUS_DEBUG(log_1,"break"<<endl);
						//break;
					}
					else return;
				}

			}
		}
		if (0 == inTheListFlag)
		{
			vol=pOrder->VolumeTraded;
			LOG4CPLUS_DEBUG(log_1,"inTheListFlag==0;|vol:"<<vol<<endl);
		}
		if (vol == 0)
		{
			return;
		}
		strcpy(instId, pOrder->InstrumentID);


		if(1) 
		{
			LOG4CPLUS_DEBUG(log_1,pOrder->InstrumentID<<"##############"<<endl);
			LOG4CPLUS_DEBUG(log_1,pOrder->OrderStatus<<"##############"<<endl);
			LOG4CPLUS_DEBUG(log_1,pOrder->VolumeTraded<<"##############"<<endl);

			if (functionNO != FUNCTION_CLOSETODAY)
			{
				//std::cerr<<"组合"<<endl;
				LOG4CPLUS_DEBUG(log_1,pOrder->OffsetFlag<<"##############"<<endl);
				if(pOrder->OffsetFlag ==DING_FTDC_OF_Open)
				{
					kpp=DING_FTDC_OF_Close;	
				}
				else
				{
					return;
				}


			}

			else if(functionNO == FUNCTION_CLOSETODAY)
				//上期所产品

			{
				LOG4CPLUS_DEBUG(log_1,pOrder->OffsetFlag<<"FUNCTION_CLOSETODAY ##############"<<endl);
				//cerr<<"上期所"<<endl;
				if(pOrder->OffsetFlag ==DING_FTDC_OF_Open)
				{
					kpp=DING_FTDC_OF_CloseToday;	
				}
				else
				{
					return;
				}

			}

			LOG4CPLUS_DEBUG(log_1,pOrder->Direction<<"pOrder->Direction ##############"<<endl);
			if(findmark)
			{
				//买成
				if(pOrder->Direction==DING_FTDC_D_Buy)
				{
					LOG4CPLUS_DEBUG(log_1,"卖委托 "<<endl);
					//卖委托 
					strcpy(instId, pOrder->InstrumentID);
					dir=DING_FTDC_D_Sell;
					//price=pOrder->LimitPrice+step; 
					price=closeprice; 
					//Mycerr<<LIMIT_PRICE;
					CDINGFtdcInputOrderField inputOrder;
					//this->m_pUserApi.ReqOrderInsert();
					ReqOrderInsertReady(instId,dir,kpp,price,vol,pOrder->ExchangeID,inputOrder);
					m_pUserApi->ReqOrderInsert(&inputOrder,nRequestID++);


				}
				//卖成
				if(pOrder->Direction==DING_FTDC_D_Sell)
				{
					LOG4CPLUS_DEBUG(log_1,"买委托 "<<endl);
					//买平
					strcpy(instId, pOrder->InstrumentID);
					dir=DING_FTDC_D_Buy;
					//price=pOrder->LimitPrice-step;
					price=closeprice; 
					//Mycerr<<LIMIT_PRICE;
					//ReqOrderInsert(instId,dir,kpp,price,vol);
					CDINGFtdcInputOrderField inputOrder;
					//this->m_pUserApi.ReqOrderInsert();
					ReqOrderInsertReady(instId,dir,kpp,price,vol,pOrder->ExchangeID,inputOrder);
					m_pUserApi->ReqOrderInsert(&inputOrder,nRequestID++);
					//Mycerr<<"OnRtnTrade"<<curentI++<<"--########################------------"<<endl;
				}
			}
		}
	}
	//-----roll------end
	LOG4CPLUS_DEBUG(log_1," 回报 | 报单已提交...序号:"<<order->OrderLocalID<<endl);
	LOG4CPLUS_DEBUG(log_1," ...................InstrumentID:"<<order->InstrumentID<<endl);
	LOG4CPLUS_DEBUG(log_1," ...................OrderRef:"<<order->UserOrderLocalID<<endl);
	LOG4CPLUS_DEBUG(log_1," ...................SessionID:"<<order->SessionID<<endl);
	LOG4CPLUS_DEBUG(log_1," ...................UserID:"<<order->UserID<<endl);
	LOG4CPLUS_DEBUG(log_1," ...................LimitPrice:"<<order->LimitPrice<<endl);
	LOG4CPLUS_DEBUG(log_1," ...................VolumeTotalOriginal:"<<order->Volume<<endl);
	LOG4CPLUS_DEBUG(log_1," ...................VolumeTraded:"<<order->VolumeTraded<<endl);
	//LOG4CPLUS_DEBUG(log_1," ...................VolumeTotal:"<<order->VolumeTotal<<endl);
	//LOG4CPLUS_DEBUG(log_1," ...................OrderSubmitStatus:"<<order-><<endl);
	LOG4CPLUS_DEBUG(log_1," ...................OrderStatus:"<<order->OrderStatus<<endl);
	LOG4CPLUS_DEBUG(log_1," ...................OrderSource:"<<order->OrderSource<<endl);
	LOG4CPLUS_DEBUG(log_1," ...................BrokerOrderSeq:"<<pOrder->OrderLocalID<<endl);
	//LeaveCriticalSection(&g_cs);
	//SetEvent(g_hEvent);
	return ;
}

void AtkCTraderSpi::OnRspOrderAction(CDINGFtdcOrderActionField *pOrderAction, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		LOG4CPLUS_DEBUG_FMT(log_1,"撤单失败 错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		return;
	}
	if(pOrderAction==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有撤单数据\n");
		return;
	}
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"撤单成功\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	return ;
}
void AtkCTraderSpi::OnRspUserPasswordUpdate(CDINGFtdcUserPasswordUpdateField *pUserPasswordUpdate, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		LOG4CPLUS_DEBUG_FMT(log_1,"修改密码失败 错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		return;
	}
	if(pUserPasswordUpdate==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有修改密码数据\n");
		return;
	}
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"修改密码成功\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	return ;
}

void AtkCTraderSpi::OnErrRtnOrderInsert(CDINGFtdcInputOrderField *pInputOrder, CDINGFtdcRspInfoField *pRspInfo)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		LOG4CPLUS_DEBUG_FMT(log_1,"报单错误回报失败 错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		return;
	}
	if(pInputOrder==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有数据\n");
		return;
	}
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"报单错误回报\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	return ;
}
void AtkCTraderSpi::OnErrRtnOrderAction(CDINGFtdcOrderActionField *pOrderAction, CDINGFtdcRspInfoField *pRspInfo)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		LOG4CPLUS_DEBUG_FMT(log_1,"撤单错误回报失败 错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		return;
	}
	if(pOrderAction==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有数据\n");
		return;
	}
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"撤单错误回报\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	return ;
}

void AtkCTraderSpi::OnRspQryOrder(CDINGFtdcOrderField *pOrder, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		LOG4CPLUS_DEBUG_FMT(log_1,"查询报单失败 错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		return;
	}
	if(pOrder==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有查询到报单数据\n");
		return;
	}




	//	LOG4CPLUS_DEBUG(log_1," OnRspQryOrder"<<pOrder->OrderStatus<<endl);
	CDINGFtdcOrderField* order = new CDINGFtdcOrderField;
	memcpy(order,  pOrder, sizeof(CDINGFtdcOrderField));
	bool founded=false;    unsigned int i=0;
	for(i=0; i<orderList.size(); i++){
		if(!strcmp(orderList[i]->OrderLocalID , order->OrderLocalID)) {
			founded=true;    break;
		}
	}
	if(founded) orderList[i]= order;   
	else  orderList.push_back(order);	
	bool founded2=false;     i=0;	
	for(i=0; i<atkorderList.size(); i++){		
		if(!strcmp(atkorderList[i]->main.UserOrderLocalID , order->UserOrderLocalID)) {	
			LOG4CPLUS_DEBUG(log_1," main.OrderRef"<<order->UserOrderLocalID<<endl);
			founded2=true;    break;
		}
	}
	if(founded2) 
	{
		LOG4CPLUS_DEBUG(log_1,"found"<<order->BrokerID<<endl);
		memcpy(&(atkorderList[i]->main), pOrder,sizeof(CDINGFtdcOrderField));
	}
	else
	{
		LOG4CPLUS_DEBUG(log_1,"not found"<<order->UserOrderLocalID<<endl);
	}
	//Show(pOrder);
	return ;
}
void AtkCTraderSpi::Show(CDINGFtdcTradeField *pTrade)
{
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"交易所代码=[%s]\n",pTrade->ExchangeID);
	LOG4CPLUS_DEBUG_FMT(log_1,"交易日=[%s]\n",pTrade->TradingDay);
	LOG4CPLUS_DEBUG_FMT(log_1,"会员编号=[%s]\n",pTrade->ParticipantID);
	LOG4CPLUS_DEBUG_FMT(log_1,"下单席位号=[%s]\n",pTrade->SeatID);
	LOG4CPLUS_DEBUG_FMT(log_1,"投资者编号=[%s]\n",pTrade->InvestorID);
	LOG4CPLUS_DEBUG_FMT(log_1,"客户号=[%s]\n",pTrade->ClientID);
	LOG4CPLUS_DEBUG_FMT(log_1,"成交编号=[%s]\n",pTrade->TradeID);

	LOG4CPLUS_DEBUG_FMT(log_1,"用户本地报单号=[%s]\n",pTrade->UserOrderLocalID);
	LOG4CPLUS_DEBUG_FMT(log_1,"合约代码=[%s]\n",pTrade->InstrumentID);
	LOG4CPLUS_DEBUG_FMT(log_1,"买卖方向=[%c]\n",pTrade->Direction);
	LOG4CPLUS_DEBUG_FMT(log_1,"开平标志=[%c]\n",pTrade->OffsetFlag);
	LOG4CPLUS_DEBUG_FMT(log_1,"投机套保标志=[%c]\n",pTrade->HedgeFlag);
	LOG4CPLUS_DEBUG_FMT(log_1,"成交价格=[%lf]\n",pTrade->TradePrice);
	LOG4CPLUS_DEBUG_FMT(log_1,"成交数量=[%d]\n",pTrade->TradeVolume);
	LOG4CPLUS_DEBUG_FMT(log_1,"清算会员编号=[%s]\n",pTrade->ClearingPartID);
	
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	return ;
}
void AtkCTraderSpi::OnRspQryTrade(CDINGFtdcTradeField *pTrade, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		LOG4CPLUS_DEBUG_FMT(log_1,"查询成交失败 错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		return;
	}
	if(pTrade==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有查询到成交数据");
		return;
	}
	Show(pTrade);
	return ;
}
void AtkCTraderSpi::OnRspQryExchange(CDINGFtdcRspExchangeField *pRspExchange, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		LOG4CPLUS_DEBUG_FMT(log_1,"查询交易所失败 错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		return;
	}
	if (pRspExchange==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有查询到交易所信息\n");
		return ;
	}
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"[%s]\n",pRspExchange->ExchangeID);
	LOG4CPLUS_DEBUG_FMT(log_1,"[%s]\n",pRspExchange->ExchangeName);
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	return;
}

void AtkCTraderSpi::OnRspQryInvestorAccount(CDINGFtdcRspInvestorAccountField *pRspInvestorAccount, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		LOG4CPLUS_DEBUG_FMT(log_1,"查询投资者账户失败 错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		return;
	}
	
	if (pRspInvestorAccount==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有查询到投资者账户\n");
		return ;
	}
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"投资者编号=[%s]\n",pRspInvestorAccount->InvestorID);
	LOG4CPLUS_DEBUG_FMT(log_1,"资金帐号=[%s]\n",pRspInvestorAccount->AccountID);
	LOG4CPLUS_DEBUG_FMT(log_1,"上次结算准备金=[%lf]\n",pRspInvestorAccount->PreBalance);
	LOG4CPLUS_DEBUG_FMT(log_1,"入金金额=[%lf]\n",pRspInvestorAccount->Deposit);
	LOG4CPLUS_DEBUG_FMT(log_1,"出金金额=[%lf]\n",pRspInvestorAccount->Withdraw);
	LOG4CPLUS_DEBUG_FMT(log_1,"冻结的保证金=[%lf]\n",pRspInvestorAccount->FrozenMargin);
	LOG4CPLUS_DEBUG_FMT(log_1,"冻结手续费=[%lf]\n",pRspInvestorAccount->FrozenFee);
	LOG4CPLUS_DEBUG_FMT(log_1,"手续费=[%lf]\n",pRspInvestorAccount->Fee);
	LOG4CPLUS_DEBUG_FMT(log_1,"平仓盈亏=[%lf]\n",pRspInvestorAccount->CloseProfit);
	LOG4CPLUS_DEBUG_FMT(log_1,"持仓盈亏=[%lf]\n",pRspInvestorAccount->PositionProfit);
	LOG4CPLUS_DEBUG_FMT(log_1,"可用资金=[%lf]\n",pRspInvestorAccount->Available);
	LOG4CPLUS_DEBUG_FMT(log_1,"多头冻结的保证金=[%lf]\n",pRspInvestorAccount->LongFrozenMargin);
	LOG4CPLUS_DEBUG_FMT(log_1,"空头冻结的保证金=[%lf]\n",pRspInvestorAccount->ShortFrozenMargin);
	LOG4CPLUS_DEBUG_FMT(log_1,"多头保证金=[%lf]\n",pRspInvestorAccount->LongMargin);
	LOG4CPLUS_DEBUG_FMT(log_1,"空头保证金=[%lf]\n",pRspInvestorAccount->ShortMargin);
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");

}

void AtkCTraderSpi::OnRspQryUserInvestor(CDINGFtdcRspUserInvestorField *pUserInvestor, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		LOG4CPLUS_DEBUG_FMT(log_1,"查询可用投资者失败 错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
		return;
	}
	if (pUserInvestor==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"No data\n");
		return ;
	}
	
	LOG4CPLUS_DEBUG_FMT(log_1,"InvestorID=[%s]\n",pUserInvestor->InvestorID);
}

void AtkCTraderSpi::Show(CDINGFtdcRspInstrumentField *pRspInstrument)
{
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"交易所代码=[%s]\n",pRspInstrument->ExchangeID);
	LOG4CPLUS_DEBUG_FMT(log_1,"品种代码=[%s]\n",pRspInstrument->ProductID);
	LOG4CPLUS_DEBUG_FMT(log_1,"品种名称=[%s]\n",pRspInstrument->ProductName);
	LOG4CPLUS_DEBUG_FMT(log_1,"合约代码=[%s]\n",pRspInstrument->InstrumentID);
	LOG4CPLUS_DEBUG_FMT(log_1,"合约名称=[%s]\n",pRspInstrument->InstrumentName);
	LOG4CPLUS_DEBUG_FMT(log_1,"交割年份=[%d]\n",pRspInstrument->DeliveryYear);
	LOG4CPLUS_DEBUG_FMT(log_1,"交割月=[%d]\n",pRspInstrument->DeliveryMonth);
	LOG4CPLUS_DEBUG_FMT(log_1,"限价单最大下单量=[%d]\n",pRspInstrument->MaxLimitOrderVolume);
	LOG4CPLUS_DEBUG_FMT(log_1,"限价单最小下单量=[%d]\n",pRspInstrument->MinLimitOrderVolume);
	LOG4CPLUS_DEBUG_FMT(log_1,"市价单最大下单量=[%d]\n",pRspInstrument->MaxMarketOrderVolume);
	LOG4CPLUS_DEBUG_FMT(log_1,"市价单最小下单量=[%d]\n",pRspInstrument->MinMarketOrderVolume);
	
	LOG4CPLUS_DEBUG_FMT(log_1,"数量乘数=[%d]\n",pRspInstrument->VolumeMultiple);
	LOG4CPLUS_DEBUG_FMT(log_1,"报价单位=[%lf]\n",pRspInstrument->PriceTick);
	LOG4CPLUS_DEBUG_FMT(log_1,"币种=[%c]\n",pRspInstrument->Currency);
	LOG4CPLUS_DEBUG_FMT(log_1,"多头限仓=[%d]\n",pRspInstrument->LongPosLimit);
	LOG4CPLUS_DEBUG_FMT(log_1,"空头限仓=[%d]\n",pRspInstrument->ShortPosLimit);
	LOG4CPLUS_DEBUG_FMT(log_1,"跌停板价=[%lf]\n",pRspInstrument->LowerLimitPrice);
	LOG4CPLUS_DEBUG_FMT(log_1,"涨停板价=[%lf]\n",pRspInstrument->UpperLimitPrice);
	LOG4CPLUS_DEBUG_FMT(log_1,"昨结算=[%lf]\n",pRspInstrument->PreSettlementPrice);
	LOG4CPLUS_DEBUG_FMT(log_1,"合约交易状态=[%c]\n",pRspInstrument->InstrumentStatus);
	
	LOG4CPLUS_DEBUG_FMT(log_1,"创建日=[%s]\n",pRspInstrument->CreateDate);
	LOG4CPLUS_DEBUG_FMT(log_1,"上市日=[%s]\n",pRspInstrument->OpenDate);
	LOG4CPLUS_DEBUG_FMT(log_1,"到期日=[%s]\n",pRspInstrument->ExpireDate);
	LOG4CPLUS_DEBUG_FMT(log_1,"开始交割日=[%s]\n",pRspInstrument->StartDelivDate);
	LOG4CPLUS_DEBUG_FMT(log_1,"最后交割日=[%s]\n",pRspInstrument->EndDelivDate);
	LOG4CPLUS_DEBUG_FMT(log_1,"挂牌基准价=[%lf]\n",pRspInstrument->BasisPrice);
	LOG4CPLUS_DEBUG_FMT(log_1,"当前是否交易=[%d]\n",pRspInstrument->IsTrading);
	LOG4CPLUS_DEBUG_FMT(log_1,"基础商品代码=[%s]\n",pRspInstrument->UnderlyingInstrID);
	LOG4CPLUS_DEBUG_FMT(log_1,"持仓类型=[%c]\n",pRspInstrument->PositionType);
	LOG4CPLUS_DEBUG_FMT(log_1,"执行价=[%lf]\n",pRspInstrument->StrikePrice);
	LOG4CPLUS_DEBUG_FMT(log_1,"期权类型=[%c]\n",pRspInstrument->OptionsType);
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	
}
void AtkCTraderSpi::OnRspQryInstrument(CDINGFtdcRspInstrumentField *pRspInstrument, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"查询交易编码失败 错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		return;
	}
	
	if (pRspInstrument==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有查询到合约数据\n");
		return ;
	}
	
	Show(pRspInstrument);
	return ;
}

void AtkCTraderSpi::OnRspQryTradingCode(CDINGFtdcRspTradingCodeField *pTradingCode, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"查询交易编码失败 错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		return;
	}
	
	if (pTradingCode==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有查询到交易编码\n");
		return ;
	}
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"交易所代码=[%s]\n",pTradingCode->ExchangeID);
	LOG4CPLUS_DEBUG_FMT(log_1,"经纪公司编号=[%s]\n",pTradingCode->BrokerID);
	LOG4CPLUS_DEBUG_FMT(log_1,"投资者编号=[%s]\n",pTradingCode->InvestorID);
	LOG4CPLUS_DEBUG_FMT(log_1,"客户代码=[%s]\n",pTradingCode->ClientID);
	LOG4CPLUS_DEBUG_FMT(log_1,"客户代码权限=[%d]\n",pTradingCode->ClientRight);
	LOG4CPLUS_DEBUG_FMT(log_1,"是否活跃=[%c]\n",pTradingCode->IsActive);
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	return ;
}

void AtkCTraderSpi::OnRspQryInvestorPosition(CDINGFtdcRspInvestorPositionField *pRspInvestorPosition, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"查询投资者持仓 错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		return;
	}
	
	if (pRspInvestorPosition==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有查询到投资者持仓\n");
		return ;
	}
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"交易所代码=[%s]\n",pRspInvestorPosition->ExchangeID);
	LOG4CPLUS_DEBUG_FMT(log_1,"经纪公司编号=[%s]\n",pRspInvestorPosition->BrokerID);
	LOG4CPLUS_DEBUG_FMT(log_1,"投资者编号=[%s]\n",pRspInvestorPosition->InvestorID);
	LOG4CPLUS_DEBUG_FMT(log_1,"客户代码=[%s]\n",pRspInvestorPosition->ClientID);
	LOG4CPLUS_DEBUG_FMT(log_1,"合约代码=[%s]\n",pRspInvestorPosition->InstrumentID);
	LOG4CPLUS_DEBUG_FMT(log_1,"买卖方向=[%c]\n",pRspInvestorPosition->Direction);
	LOG4CPLUS_DEBUG_FMT(log_1,"今持仓量=[%d]\n",pRspInvestorPosition->Position);
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	return ;

}

	///投资者手续费率查询应答
void AtkCTraderSpi::OnRspQryInvestorFee(CDINGFtdcInvestorFeeField *pInvestorFee, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"查询投资者手续费率失败 错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		return;
	}
	
	if (pInvestorFee==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有查询到投资者手续费率\n");
		return ;
	}
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"经纪公司编号=[%s]\n",pInvestorFee->BrokerID);
	LOG4CPLUS_DEBUG_FMT(log_1,"合约代码=[%s]\n",pInvestorFee->InstrumentID);
	LOG4CPLUS_DEBUG_FMT(log_1,"客户代码=[%s]\n",pInvestorFee->ClientID);
	LOG4CPLUS_DEBUG_FMT(log_1,"开仓手续费按比例=[%f]\n",pInvestorFee->OpenFeeRate);
	LOG4CPLUS_DEBUG_FMT(log_1,"开仓手续费按手数=[%f]\n",pInvestorFee->OpenFeeAmt);
	LOG4CPLUS_DEBUG_FMT(log_1,"平仓手续费按比例=[%f]\n",pInvestorFee->OffsetFeeRate);
	LOG4CPLUS_DEBUG_FMT(log_1,"平仓手续费按手数=[%f]\n",pInvestorFee->OffsetFeeAmt);
	LOG4CPLUS_DEBUG_FMT(log_1,"平今仓手续费按比例=[%f]\n",pInvestorFee->OTFeeRate);
	LOG4CPLUS_DEBUG_FMT(log_1,"平今仓手续费按手数=[%f]\n",pInvestorFee->OTFeeAmt);
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	return ;

}

	///投资者保证金率查询应答
void AtkCTraderSpi::OnRspQryInvestorMargin(CDINGFtdcInvestorMarginField *pInvestorMargin, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"查询投资者保证金率失败 错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		return;
	}
	
	if (pInvestorMargin==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有查询到投资者保证金率\n");
		return ;
	}
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"经纪公司编号=[%s]\n",pInvestorMargin->BrokerID);
	LOG4CPLUS_DEBUG_FMT(log_1,"合约代码=[%s]\n",pInvestorMargin->InstrumentID);
	LOG4CPLUS_DEBUG_FMT(log_1,"客户代码=[%s]\n",pInvestorMargin->ClientID);
	LOG4CPLUS_DEBUG_FMT(log_1,"多头占用保证金按比例=[%f]\n",pInvestorMargin->LongMarginRate);
	LOG4CPLUS_DEBUG_FMT(log_1,"多头保证金按手数=[%f]\n",pInvestorMargin->LongMarginAmt);
	LOG4CPLUS_DEBUG_FMT(log_1,"空头占用保证金按比例=[%f]\n",pInvestorMargin->ShortMarginRate);
	LOG4CPLUS_DEBUG_FMT(log_1,"空头保证金按手数=[%f]\n",pInvestorMargin->ShortMarginAmt);
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	return ;

}


void AtkCTraderSpi::OnRspQryComplianceParam(CDINGFtdcRspComplianceParamField *pRspComplianceParam, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"查询合规参数失败 错误原因：%s\n", UTF8(pRspInfo->ErrorMsg));
		return;
	}
	
	if (pRspComplianceParam==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有查询到合规参数\n");
		return ;
	}
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"经纪公司编号=[%s]\n",pRspComplianceParam->BrokerID);
	LOG4CPLUS_DEBUG_FMT(log_1,"客户代码=[%s]\n",pRspComplianceParam->ClientID);
	LOG4CPLUS_DEBUG_FMT(log_1,"每日最大报单笔=[%d]\n",pRspComplianceParam->DailyMaxOrder);
	LOG4CPLUS_DEBUG_FMT(log_1,"每日最大撤单笔=[%d]\n",pRspComplianceParam->DailyMaxOrderAction);
	LOG4CPLUS_DEBUG_FMT(log_1,"每日最大错单笔=[%d]\n",pRspComplianceParam->DailyMaxErrorOrder);
	LOG4CPLUS_DEBUG_FMT(log_1,"每日最大报单手=[%d]\n",pRspComplianceParam->DailyMaxOrderVolume);
	LOG4CPLUS_DEBUG_FMT(log_1,"每日最大撤单手=[%d]\n",pRspComplianceParam->DailyMaxOrderActionVolume);
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	return ;


}


int static icount=0;
void AtkCTraderSpi::OnRtnInstrumentStatus(CDINGFtdcInstrumentStatusField *pInstrumentStatus)
{
	if (pInstrumentStatus==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有合约状态信息\n");
		return ;
	}
	icount++;
	
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"交易所代码=[%s]\n",pInstrumentStatus->ExchangeID);
	LOG4CPLUS_DEBUG_FMT(log_1,"品种代码=[%s]\n",pInstrumentStatus->ProductID);
	LOG4CPLUS_DEBUG_FMT(log_1,"品种名称=[%s]\n",pInstrumentStatus->ProductName);
	LOG4CPLUS_DEBUG_FMT(log_1,"合约代码=[%s]\n",pInstrumentStatus->InstrumentID);
	LOG4CPLUS_DEBUG_FMT(log_1,"合约名称=[%s]\n",pInstrumentStatus->InstrumentName);
	LOG4CPLUS_DEBUG_FMT(log_1,"交割年份=[%d]\n",pInstrumentStatus->DeliveryYear);
	LOG4CPLUS_DEBUG_FMT(log_1,"交割月=[%d]\n",pInstrumentStatus->DeliveryMonth);
	LOG4CPLUS_DEBUG_FMT(log_1,"限价单最大下单量=[%d]\n",pInstrumentStatus->MaxLimitOrderVolume);
	LOG4CPLUS_DEBUG_FMT(log_1,"限价单最小下单量=[%d]\n",pInstrumentStatus->MinLimitOrderVolume);
	LOG4CPLUS_DEBUG_FMT(log_1,"市价单最大下单量=[%d]\n",pInstrumentStatus->MaxMarketOrderVolume);
	LOG4CPLUS_DEBUG_FMT(log_1,"市价单最小下单量=[%d]\n",pInstrumentStatus->MinMarketOrderVolume);
	
	LOG4CPLUS_DEBUG_FMT(log_1,"数量乘数=[%d]\n",pInstrumentStatus->VolumeMultiple);
	LOG4CPLUS_DEBUG_FMT(log_1,"报价单位=[%lf]\n",pInstrumentStatus->PriceTick);
	LOG4CPLUS_DEBUG_FMT(log_1,"币种=[%c]\n",pInstrumentStatus->Currency);
	LOG4CPLUS_DEBUG_FMT(log_1,"多头限仓=[%d]\n",pInstrumentStatus->LongPosLimit);
	LOG4CPLUS_DEBUG_FMT(log_1,"空头限仓=[%d]\n",pInstrumentStatus->ShortPosLimit);
	LOG4CPLUS_DEBUG_FMT(log_1,"跌停板价=[%lf]\n",pInstrumentStatus->LowerLimitPrice);
	LOG4CPLUS_DEBUG_FMT(log_1,"涨停板价=[%lf]\n",pInstrumentStatus->UpperLimitPrice);
	LOG4CPLUS_DEBUG_FMT(log_1,"昨结算=[%lf]\n",pInstrumentStatus->PreSettlementPrice);
	LOG4CPLUS_DEBUG_FMT(log_1,"合约交易状态=[%c]\n",pInstrumentStatus->InstrumentStatus);
	
	LOG4CPLUS_DEBUG_FMT(log_1,"创建日=[%s]\n",pInstrumentStatus->CreateDate);
	LOG4CPLUS_DEBUG_FMT(log_1,"上市日=[%s]\n",pInstrumentStatus->OpenDate);
	LOG4CPLUS_DEBUG_FMT(log_1,"到期日=[%s]\n",pInstrumentStatus->ExpireDate);
	LOG4CPLUS_DEBUG_FMT(log_1,"开始交割日=[%s]\n",pInstrumentStatus->StartDelivDate);
	LOG4CPLUS_DEBUG_FMT(log_1,"最后交割日=[%s]\n",pInstrumentStatus->EndDelivDate);
	LOG4CPLUS_DEBUG_FMT(log_1,"挂牌基准价=[%lf]\n",pInstrumentStatus->BasisPrice);
	LOG4CPLUS_DEBUG_FMT(log_1,"当前是否交易=[%d]\n",pInstrumentStatus->IsTrading);
	LOG4CPLUS_DEBUG_FMT(log_1,"基础商品代码=[%s]\n",pInstrumentStatus->UnderlyingInstrID);
	LOG4CPLUS_DEBUG_FMT(log_1,"持仓类型=[%c]\n",pInstrumentStatus->PositionType);
	LOG4CPLUS_DEBUG_FMT(log_1,"执行价=[%lf]\n",pInstrumentStatus->StrikePrice);
	LOG4CPLUS_DEBUG_FMT(log_1,"期权类型=[%c]\n",pInstrumentStatus->OptionsType);

	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"[%d]",icount);
	return ;

}


void AtkCTraderSpi::OnRtnInvestorAccountDeposit(CDINGFtdcInvestorAccountDepositResField *pInvestorAccountDepositRes)
{
	if (pInvestorAccountDepositRes==NULL)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"没有资金推送信息\n");
		return ;
	}

	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	LOG4CPLUS_DEBUG_FMT(log_1,"经纪公司编号=[%s]\n",pInvestorAccountDepositRes->BrokerID);
	LOG4CPLUS_DEBUG_FMT(log_1,"用户代码＝[%s]\n",pInvestorAccountDepositRes->UserID);
	LOG4CPLUS_DEBUG_FMT(log_1,"投资者编号=[%s]\n",pInvestorAccountDepositRes->InvestorID);
	LOG4CPLUS_DEBUG_FMT(log_1,"资金账号=[%s]\n",pInvestorAccountDepositRes->AccountID);
	LOG4CPLUS_DEBUG_FMT(log_1,"资金流水号＝[%s]\n",pInvestorAccountDepositRes->AccountSeqNo);
	LOG4CPLUS_DEBUG_FMT(log_1,"金额＝[%lf]\n",pInvestorAccountDepositRes->Amount);
	LOG4CPLUS_DEBUG_FMT(log_1,"出入金方向＝[%c]\n",pInvestorAccountDepositRes->AmountDirection);
	LOG4CPLUS_DEBUG_FMT(log_1,"可用资金＝[%lf]\n",pInvestorAccountDepositRes->Available);
	LOG4CPLUS_DEBUG_FMT(log_1,"结算准备金＝[%lf]\n",pInvestorAccountDepositRes->Balance);
	LOG4CPLUS_DEBUG_FMT(log_1,"-----------------------------\n");
	return ;

}

void AtkCTraderSpi::ReqOrderInsertReady( TDINGFtdcInstrumentIDType instId, TDINGFtdcDirectionType dir, TDINGFtdcOffsetFlagType kpp, TDINGFtdcPriceType price, TDINGFtdcVolumeType vol,TDINGFtdcExchangeIDType exchangeID, CDINGFtdcInputOrderField& req )
{
	memset(&req, 0, sizeof(CDINGFtdcInputOrderField));
	strcpy(req.BrokerID, appId);  //应用单元代码	
	strcpy(req.UserID, userId); //投资者代码
    strcpy(req.InvestorID, investorId); //投资者代码
	strcpy(req.InstrumentID, instId); //合约代码	
	strcpy(req.UserOrderLocalID, orderRef);  //报单引用
	strcpy(req.ExchangeID, exchangeID);  //报单引用
	int nextOrderRef = atoi(orderRef);
	sprintf(orderRef, "%012d", ++nextOrderRef);

	req.LimitPrice = price;	//价格

	req.OrderPriceType = DING_FTDC_OPT_LimitPrice;//价格类型=限价	
	req.TimeCondition = DING_FTDC_TC_GFD;  //有效期类型:当日有效
	req.Direction = dir;  //买卖方向	
	req.OffsetFlag = kpp; //组合开平标志:开仓
	req.HedgeFlag = DING_FTDC_CHF_Speculation;	  //组合投机套保标志	
	req.Volume = vol;	///数量		
	req.VolumeCondition = DING_FTDC_VC_AV; //成交量类型:任何数量
	req.MinVolume = 1;	//最小成交量:1	
	//req.ContingentConditionType = DING_FTDC_CC_Immediately;  //触发条件:立即

	//TDINGFtdcPriceType	StopPrice;  //止损价
	req.ForceCloseReason = DING_FTDC_FCR_NotForceClose;	//强平原因:非强平	
	req.IsAutoSuspend = 0;  //自动挂起标志:否	
	//throw std::exception("The method or operation is not implemented.");
}

void AtkCTraderSpi::OnRspError( CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	printf("The method or operation is not implemented.");
}

void AtkCTraderSpi::OnFrontDisconnected( int nReason )
{
	printf("The method or operation is not implemented.");
}
