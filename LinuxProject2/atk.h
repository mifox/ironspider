#ifndef ATK_H_
#define ATK_H_
#include "myapi.h"
#include "config.h"
#include "PublicFuncs.h"
class CtpAtkMd //:public CThostFtdcTraderSpi
{

public:
	static int getAtkInputOrder(char* s,CDINGFtdcInputOrderField& m);
	static CtpAtkMd* CreateAtkApi();
	static int TimeConfirm();
	//static void ReqOrderInsertReady( TDINGFtdcInstrumentIDType instId, TDINGFtdcDirectionType dir, TDINGFtdcOffsetFlagType kpp, TDINGFtdcPriceType price, TDINGFtdcVolumeType vol, CDINGFtdcInputOrderField& req );
};

#endif