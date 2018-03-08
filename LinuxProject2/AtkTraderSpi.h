// TraderSpi.h: interface for the CTraderSpi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATKTRADERSPI_H__4B26E1C3_9EEE_4412_8C08_71BFB55CE6FF__INCLUDED_)
#define AFX_ATKTRADERSPI_H__4B26E1C3_9EEE_4412_8C08_71BFB55CE6FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PublicFuncs.h"
#include "myapi.h"
class AtkCTraderSpi : public CDINGFtdcTraderSpi
{
public:
	int setCar(CountedPtr<Car>& pcar);
	AtkCTraderSpi(CMyTraderApi *pTrader);
	virtual ~AtkCTraderSpi();

	virtual void OnFrontConnected();
	virtual void OnRspUserLogin(CDINGFtdcRspUserLoginField *pRspUserLogin, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUserLogout(CDINGFtdcRspUserLogoutField *pRspUserLogout, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
	//traderSPI
	virtual void OnRspOrderInsert(CDINGFtdcInputOrderField *pInputOrder, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspOrderAction(CDINGFtdcOrderActionField *pOrderAction, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUserPasswordUpdate(CDINGFtdcUserPasswordUpdateField *pUserPasswordUpdate, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRtnTrade(CDINGFtdcTradeField *pTrade);
	virtual void OnRtnOrder(CDINGFtdcOrderField *pOrder);
	virtual void OnRtnInstrumentStatus(CDINGFtdcInstrumentStatusField *pInstrumentStatus);
	virtual void OnRtnInvestorAccountDeposit(CDINGFtdcInvestorAccountDepositResField *pInvestorAccountDepositRes);

	virtual void OnErrRtnOrderInsert(CDINGFtdcInputOrderField *pInputOrder, CDINGFtdcRspInfoField *pRspInfo);
	virtual void OnErrRtnOrderAction(CDINGFtdcOrderActionField *pOrderAction, CDINGFtdcRspInfoField *pRspInfo);

	//QuerySPI
	virtual void OnRspQryOrder(CDINGFtdcOrderField *pOrder, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryTrade(CDINGFtdcTradeField *pTrade, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryExchange(CDINGFtdcRspExchangeField *pRspExchange, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryInvestorAccount(CDINGFtdcRspInvestorAccountField *pRspInvestorAccount, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryUserInvestor(CDINGFtdcRspUserInvestorField *pUserInvestor, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryInstrument(CDINGFtdcRspInstrumentField *pRspInstrument, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryTradingCode(CDINGFtdcRspTradingCodeField *pTradingCode, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryInvestorPosition(CDINGFtdcRspInvestorPositionField *pRspInvestorPosition, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///合规参数查询应答
	virtual void OnRspQryComplianceParam(CDINGFtdcRspComplianceParamField *pRspComplianceParam, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///投资者手续费率查询应答
	virtual void OnRspQryInvestorFee(CDINGFtdcInvestorFeeField *pInvestorFee, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///投资者保证金率查询应答
	virtual void OnRspQryInvestorMargin(CDINGFtdcInvestorMarginField *pInvestorMargin, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void ReqOrderInsertReady( TDINGFtdcInstrumentIDType instId, TDINGFtdcDirectionType dir, TDINGFtdcOffsetFlagType kpp, TDINGFtdcPriceType price, TDINGFtdcVolumeType vol,TDINGFtdcExchangeIDType	exchangeID, CDINGFtdcInputOrderField& req );
	void ReqSettlementInfoConfirm();
	///请求查询合约
	void ReqQryInstrument(TDINGFtdcInstrumentIDType instId);
	///请求查询资金账户
	void ReqQryTradingAccount();

	void ReqQryMarginRate(CDINGFtdcQryInvestorMarginField *req);
	///请求查询投资者持仓
	void ReqQryInvestorPosition(TDINGFtdcInstrumentIDType instId);

	void Show(CDINGFtdcOrderField *pOrder);
	void Show(CDINGFtdcTradeField *pTrade);
	void Show(CDINGFtdcRspInstrumentField *pRspInstrument);

	virtual void OnRspError( CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );
	virtual void OnFrontDisconnected( int nReason );
	CDINGFtdcRspUserLoginField rspUserLogin;
	//void ReqUserLogin()
private:
  
  CMyTraderSpi* pTdSpi;
  CMyTraderSpi* realspi;
public:
  CMyTraderApi *m_pUserApi;
  CountedPtr<Car> pCar;
 
	
};

#endif // !defined(AFX_TRADERSPI_H__4B26E1C3_9EEE_4412_8C08_71BFB55CE6FF__INCLUDED_)
