#include "myapi.h"
#include <string.h>


CMyTraderApi * CMyTraderApi::CreateFtdcTraderApi( const char *pszFlowPath /*= ""*/ )
{
	CMyTraderApi* temp= new CMyTraderApi();
	temp->realapi = CUstpFtdcTraderApi::CreateFtdcTraderApi(pszFlowPath);
	return temp;
}

const char * CMyTraderApi::GetVersion( int &nMajorVersion, int &nMinorVersion )
{
	return CUstpFtdcTraderApi::GetVersion(nMajorVersion,nMinorVersion);
}

void CMyTraderApi::Release()
{
	realapi->Release();
}

void CMyTraderApi::Init()
{
	realapi->Init();
}

int CMyTraderApi::Join()
{
	return realapi->Join();
}

const char * CMyTraderApi::GetTradingDay()
{
	return realapi->GetTradingDay();
}


void CMyTraderApi::RegisterFront( char *pszFrontAddress )
{
	realapi->RegisterFront(pszFrontAddress);
}

void CMyTraderApi::RegisterNameServer( char *pszNsAddress )
{
	realapi->RegisterNameServer(pszNsAddress);
}

void CMyTraderApi::RegisterSpi( CMyTraderSpi *pSpi )
{
	//CMyTraderSpi myspi(pSpi);
	//realapi->RegisterSpi(pSpi);
	realapi->RegisterSpi(pSpi);
}

int CMyTraderApi::RegisterCertificateFile( const char *pszCertFileName, const char *pszKeyFileName, const char *pszCaFileName, const char *pszKeyFilePassword )
{
	return realapi->RegisterCertificateFile(pszCertFileName,pszKeyFileName,pszCaFileName,pszKeyFilePassword);
}

void CMyTraderApi::SubscribePrivateTopic( DING_TE_RESUME_TYPE nResumeType )
{
	realapi->SubscribePrivateTopic((USTP_TE_RESUME_TYPE)nResumeType);
}

void CMyTraderApi::SubscribePublicTopic( DING_TE_RESUME_TYPE nResumeType )
{
	realapi->SubscribePublicTopic((USTP_TE_RESUME_TYPE)nResumeType);
}

void CMyTraderApi::SubscribeUserTopic( DING_TE_RESUME_TYPE nResumeType )
{
	realapi->SubscribeUserTopic((USTP_TE_RESUME_TYPE)nResumeType);
}

void CMyTraderApi::SetHeartbeatTimeout( unsigned int timeout )
{
	realapi->SetHeartbeatTimeout(timeout);
}

int CMyTraderApi::OpenRequestLog( const char *pszReqLogFileName )
{
	return realapi->OpenRequestLog(pszReqLogFileName);
}

int CMyTraderApi::OpenResponseLog( const char *pszRspLogFileName )
{
	return realapi->OpenResponseLog(pszRspLogFileName);
}

int CMyTraderApi::ReqUserLogin( CDINGFtdcReqUserLoginField *pReqUserLogin, int nRequestID )
{
	CUstpFtdcReqUserLoginField* pReqUserLogin0 =(CUstpFtdcReqUserLoginField*) pReqUserLogin;
// 	CUstpFtdcReqUserLoginField reqUserLogin;
// 	memset(&reqUserLogin,0,sizeof(CUstpFtdcReqUserLoginField));		
// 	strcpy(reqUserLogin.BrokerID,pReqUserLogin->BrokerID);
// 	strcpy(reqUserLogin.UserID, pReqUserLogin->UserID);
// 	strcpy(reqUserLogin.Password, pReqUserLogin->Password);	
// 	strcpy(reqUserLogin.UserProductInfo,"xxx111");
	return realapi->ReqUserLogin(pReqUserLogin0,nRequestID);
}

int CMyTraderApi::ReqUserLogout( CDINGFtdcReqUserLogoutField *pReqUserLogout, int nRequestID )
{
	CUstpFtdcReqUserLogoutField *pReqUserLogout0 = (CUstpFtdcReqUserLogoutField *)pReqUserLogout;
	return realapi->ReqUserLogout(pReqUserLogout0,nRequestID);
}

int CMyTraderApi::ReqUserPasswordUpdate( CDINGFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID )
{
	CUstpFtdcUserPasswordUpdateField * pUserPasswordUpdate0=(CUstpFtdcUserPasswordUpdateField *) pUserPasswordUpdate;
	return realapi->ReqUserPasswordUpdate(pUserPasswordUpdate0,nRequestID);
}

int CMyTraderApi::ReqOrderInsert( CDINGFtdcInputOrderField *pInputOrder, int nRequestID )
{
	CUstpFtdcInputOrderField* pInputOrder0 = (CUstpFtdcInputOrderField*)pInputOrder;
	return realapi->ReqOrderInsert(pInputOrder0,nRequestID);
}

int CMyTraderApi::ReqOrderAction( CDINGFtdcOrderActionField *pOrderAction, int nRequestID )
{
	CUstpFtdcOrderActionField *pOrderAction0 = (CUstpFtdcOrderActionField *)pOrderAction;
	pOrderAction0->ActionFlag = USTP_FTDC_AF_Delete;
	return realapi->ReqOrderAction(pOrderAction0,nRequestID);
}

int CMyTraderApi::ReqQryOrder( CDINGFtdcQryOrderField *pQryOrder, int nRequestID )
{
	CUstpFtdcQryOrderField* pQryOrder0 = (CUstpFtdcQryOrderField*) pQryOrder;
	return realapi->ReqQryOrder(pQryOrder0, nRequestID);
}

int CMyTraderApi::ReqQryTrade( CDINGFtdcQryTradeField *pQryTrade, int nRequestID )
{
	CUstpFtdcQryTradeField* pQryTrade0 =(CUstpFtdcQryTradeField*) pQryTrade;
	return realapi->ReqQryTrade(pQryTrade0, nRequestID) ;
}

int CMyTraderApi::ReqQryUserInvestor( CDINGFtdcQryUserInvestorField *pQryUserInvestor, int nRequestID )
{
	CUstpFtdcQryUserInvestorField* pQryUserInvestor0 = (CUstpFtdcQryUserInvestorField*) pQryUserInvestor;
	return realapi->ReqQryUserInvestor(pQryUserInvestor0, nRequestID) ;
}

int CMyTraderApi::ReqQryTradingCode( CDINGFtdcQryTradingCodeField *pQryTradingCode, int nRequestID )
{
	CUstpFtdcQryTradingCodeField* pQryTradingCode0 = (CUstpFtdcQryTradingCodeField*) pQryTradingCode;
	return realapi->ReqQryTradingCode(pQryTradingCode0,nRequestID);
}

int CMyTraderApi::ReqQryInvestorAccount( CDINGFtdcQryInvestorAccountField *pQryInvestorAccount, int nRequestID )
{
	CUstpFtdcQryInvestorAccountField* pQryInvestorAccount0 = (CUstpFtdcQryInvestorAccountField* )pQryInvestorAccount;
	return realapi->ReqQryInvestorAccount(pQryInvestorAccount0,nRequestID);
}

int CMyTraderApi::ReqQryInstrument( CDINGFtdcQryInstrumentField *pQryInstrument, int nRequestID )
{
	CUstpFtdcQryInstrumentField* pQryInstrument0 = (CUstpFtdcQryInstrumentField*) pQryInstrument;
	return realapi->ReqQryInstrument(pQryInstrument0,nRequestID);
}

int CMyTraderApi::ReqQryExchange( CDINGFtdcQryExchangeField *pQryExchange, int nRequestID )
{
	CUstpFtdcQryExchangeField* pQryExchange0 = (CUstpFtdcQryExchangeField*) pQryExchange;
	return realapi->ReqQryExchange(pQryExchange0,nRequestID);
}

int CMyTraderApi::ReqQryInvestorPosition( CDINGFtdcQryInvestorPositionField *pQryInvestorPosition, int nRequestID )
{
	CUstpFtdcQryInvestorPositionField* pQryInvestorPosition0 = (CUstpFtdcQryInvestorPositionField* )pQryInvestorPosition;
	return realapi->ReqQryInvestorPosition(pQryInvestorPosition0,nRequestID);
}

int CMyTraderApi::ReqSubscribeTopic( CDINGFtdcDisseminationField *pDissemination, int nRequestID )
{
	CUstpFtdcDisseminationField* pDissemination0 = (CUstpFtdcDisseminationField*) pDissemination;
	return realapi->ReqSubscribeTopic(pDissemination0,nRequestID);
}

int CMyTraderApi::ReqQryComplianceParam( CDINGFtdcQryComplianceParamField *pQryComplianceParam, int nRequestID )
{
	CUstpFtdcQryComplianceParamField *pQryComplianceParam0 = (CUstpFtdcQryComplianceParamField *)pQryComplianceParam;
	return realapi->ReqQryComplianceParam(pQryComplianceParam0,nRequestID);
}

int CMyTraderApi::ReqQryTopic( CDINGFtdcDisseminationField *pDissemination, int nRequestID )
{
	CUstpFtdcDisseminationField *pDissemination0 = (CUstpFtdcDisseminationField *)pDissemination;
	return realapi->ReqQryTopic(pDissemination0,nRequestID);
}

int CMyTraderApi::ReqQryInvestorFee( CDINGFtdcQryInvestorFeeField *pQryInvestorFee, int nRequestID )
{
	CUstpFtdcQryInvestorFeeField* pQryInvestorFee0 = (CUstpFtdcQryInvestorFeeField*) pQryInvestorFee;
	return realapi->ReqQryInvestorFee(pQryInvestorFee0,nRequestID);
}

int CMyTraderApi::ReqQryInvestorMargin( CDINGFtdcQryInvestorMarginField *pQryInvestorMargin, int nRequestID )
{
	CUstpFtdcQryInvestorMarginField* pQryInvestorMargin0 = (CUstpFtdcQryInvestorMarginField*) pQryInvestorMargin;
	return realapi->ReqQryInvestorMargin(pQryInvestorMargin0,nRequestID);
}

int CMyTraderApi::ReqSettlementInfoConfirm( CDINGFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID )
{
	return 0;
}

CMyTraderSpi::CMyTraderSpi( CDINGFtdcTraderSpi* spi )
{
	realspi = spi;
}


void CMyTraderSpi::OnFrontConnected()
{
	realspi->OnFrontConnected();
	//throw std::exception("The method or operation is not implemented.");
}
void CMyTraderSpi::OnRspUserLogin( CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CDINGFtdcRspUserLoginField rspUserLogin1 ={0};
	//CDINGFtdcRspUserLoginField *pRspUserLogin1 = (CDINGFtdcRspUserLoginField*) pRspUserLogin;
	CDINGFtdcRspInfoField *pRspInfo1 = (CDINGFtdcRspInfoField *)pRspInfo;
	memcpy(&rspUserLogin1,pRspUserLogin,sizeof(CDINGFtdcRspUserLoginField));
	strcpy(rspUserLogin1.CZCETime,pRspUserLogin->LoginTime);
	strcpy(rspUserLogin1.SHFETime,pRspUserLogin->LoginTime);
	strcpy(rspUserLogin1.DCETime,pRspUserLogin->LoginTime);
	strcpy(rspUserLogin1.FFEXTime,pRspUserLogin->LoginTime);
	realspi->OnRspUserLogin(&rspUserLogin1, pRspInfo1, nRequestID,bIsLast );
	return;
}

void CMyTraderSpi::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CDINGFtdcRspUserLogoutField *pRspUserLogout1=(CDINGFtdcRspUserLogoutField *)pRspUserLogout;
	CDINGFtdcRspInfoField* pRspInfo1 =(CDINGFtdcRspInfoField*) pRspInfo;
	realspi->OnRspUserLogout( pRspUserLogout1,pRspInfo1, nRequestID, bIsLast );
	return;
}

void CMyTraderSpi::OnRspOrderInsert( CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CDINGFtdcInputOrderField *pInputOrder1 = (CDINGFtdcInputOrderField*)pInputOrder;
	CDINGFtdcRspInfoField* pRspInfo1 =(CDINGFtdcRspInfoField*) pRspInfo;
	realspi->OnRspOrderInsert(pInputOrder1,pRspInfo1,nRequestID,bIsLast);
	return;
}

void CMyTraderSpi::OnRspOrderAction( CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CDINGFtdcOrderActionField* pOrderAction1 =(CDINGFtdcOrderActionField*) pOrderAction;
	CDINGFtdcRspInfoField* pRspInfo1 =(CDINGFtdcRspInfoField*) pRspInfo;
	realspi->OnRspOrderAction(pOrderAction1,pRspInfo1,nRequestID,bIsLast);
	return;
}

void CMyTraderSpi::OnRtnTrade( CUstpFtdcTradeField *pTrade )
{
	CDINGFtdcTradeField *pTrade1 = (CDINGFtdcTradeField*)pTrade;
	realspi->OnRtnTrade(pTrade1);
	return;
}

void CMyTraderSpi::OnRtnOrder( CUstpFtdcOrderField *pOrder )
{
	CDINGFtdcOrderField *pOrder1 = (CDINGFtdcOrderField*)pOrder;
	realspi->OnRtnOrder(pOrder1);
	return;
}

void CMyTraderSpi::OnRspQryOrder( CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CDINGFtdcOrderField * pOrder1=(CDINGFtdcOrderField *)pOrder;
	CDINGFtdcRspInfoField * pRspInfo1= (CDINGFtdcRspInfoField *)pRspInfo;
	realspi->OnRspQryOrder(pOrder1,pRspInfo1,nRequestID,bIsLast);
	return;
}

void CMyTraderSpi::OnPackageStart( int nTopicID, int nSequenceNo )
{
	printf("The method or operation is not implemented.\n");
}

void CMyTraderSpi::OnPackageEnd( int nTopicID, int nSequenceNo )
{
	printf("The method or operation is not implemented.\n");
}

void CMyTraderSpi::OnErrRtnOrderInsert( CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo )
{
	CDINGFtdcInputOrderField  * pOrder1=(CDINGFtdcInputOrderField  *)pInputOrder;
	CDINGFtdcRspInfoField * pRspInfo1= (CDINGFtdcRspInfoField *)pRspInfo;
	realspi->OnErrRtnOrderInsert(pOrder1,pRspInfo1);
	//throw std::exception("The method or operation is not implemented.");
}

void CMyTraderSpi::OnRspQryInstrument( CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CDINGFtdcRspInfoField * pRspInfo1= (CDINGFtdcRspInfoField *)pRspInfo;
	CDINGFtdcRspInstrumentField *pRspInstrument1 = (CDINGFtdcRspInstrumentField *)pRspInstrument;
	realspi->OnRspQryInstrument(pRspInstrument1,pRspInfo1, nRequestID, bIsLast);
}

