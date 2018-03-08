#ifndef ATK_H_
#define ATK_H_
#include "myapi.h"
#include "config.h"
#include "PublicFuncs.h"
#include "AtkTraderSpi.h"
class CtpAtkMd //:public CThostFtdcTraderSpi
{

public:
	static int getAtkInputOrder(char* s,CDINGFtdcInputOrderField& m);
	static CtpAtkMd* CreateAtkApi();

	static void oneKeyCleanPostion( AtkCTraderSpi* pTdSpi );

	static int TimeConfirm();
	static int getexchId( int atkexchange,TDINGFtdcExchangeIDType exchageID);
	static void getmaxvol( AtkCTraderSpi* pTdSpi, CDINGFtdcInputOrderField &attackReq );


	static TDINGFtdcExchangeIDType exchageID;
	//static void ReqOrderInsertReady( TDINGFtdcInstrumentIDType instId, TDINGFtdcDirectionType dir, TDINGFtdcOffsetFlagType kpp, TDINGFtdcPriceType price, TDINGFtdcVolumeType vol, CDINGFtdcInputOrderField& req );
};

#endif