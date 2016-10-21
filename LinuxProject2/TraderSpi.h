// TraderSpi.h: interface for the CTraderSpi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRADERSPI_H__4B26E1C3_9EEE_4412_8C08_71BFB55CE6FF__INCLUDED_)
#define AFX_TRADERSPI_H__4B26E1C3_9EEE_4412_8C08_71BFB55CE6FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PublicFuncs.h"
#include "myapi.h"
class CTraderSpi : public CDINGFtdcTraderSpi 
{
public:
	CTraderSpi(CMyTraderApi *pTrader);
	virtual ~CTraderSpi();

	virtual void OnFrontConnected();
	virtual void OnRspUserLogin(CDINGFtdcRspUserLoginField *pRspUserLogin, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
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
	void Show(CDINGFtdcOrderField *pOrder);
	void Show(CDINGFtdcTradeField *pTrade);
	void Show(CDINGFtdcRspInstrumentField *pRspInstrument);
private:	
	CMyTraderApi *m_pUserApi;
};

#endif // !defined(AFX_TRADERSPI_H__4B26E1C3_9EEE_4412_8C08_71BFB55CE6FF__INCLUDED_)
