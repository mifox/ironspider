#include "myMdapi.h"
#include "string.h"


CMyMdApi * CMyMdApi::CreateFtdcMdApi(const char *pszFlowPath /*= ""*/)
{
	CMyMdApi* temp= new CMyMdApi();	
	temp->realapi = CThostFtdcMdApi::CreateFtdcMdApi(pszFlowPath);
	return temp;
}

const char * CMyMdApi::GetVersion( int &nMajorVersion, int &nMinorVersion )
{
	//return CDINGFtdcMduserApi::GetVersion(nMajorVersion,nMinorVersion);
	return "test";
}

void CMyMdApi::Release()
{
	realapi->Release();
}

void CMyMdApi::Init()
{
	realapi->Init();
}

int CMyMdApi::Join()
{
	return realapi->Join();
}

const char * CMyMdApi::GetTradingDay()
{
	return realapi->GetTradingDay();
}

void CMyMdApi::RegisterFront( char *pszFrontAddress )
{
	realapi->RegisterFront(pszFrontAddress);
}

void CMyMdApi::RegisterNameServer( char *pszNsAddress )
{
	realapi->RegisterNameServer(pszNsAddress);
}

void CMyMdApi::RegisterSpi( CMyMdSpi *pSpi )
{
	//CMyMdSpi myspi(pSpi);
	//realapi->RegisterSpi(pSpi);
	realapi->RegisterSpi(pSpi);
}

int CMyMdApi::RegisterCertificateFile( const char *pszCertFileName, const char *pszKeyFileName, const char *pszCaFileName, const char *pszKeyFilePassword )
{
	//return realapi->RegisterCertificateFile(pszCertFileName,pszKeyFileName,pszCaFileName,pszKeyFilePassword);
	return 1;
}


void CMyMdApi::SetHeartbeatTimeout( unsigned int timeout )
{
	//realapi->SetHeartbeatTimeout(timeout);
}

// int CMyMdApi::OpenRequestLog( const char *pszReqLogFileName )
// {
// 	return realapi->OpenRequestLog(pszReqLogFileName);
// }
// 
// int CMyMdApi::OpenResponseLog( const char *pszRspLogFileName )
// {
// 	return realapi->OpenResponseLog(pszRspLogFileName);
// }

int CMyMdApi::ReqUserLogin( CDINGFtdcReqUserLoginField *pReqUserLogin, int nRequestID )
{
	//CDINGFtdcReqUserLoginField myReqUserLogin;
	CThostFtdcReqUserLoginField myReqUserLoginField={0};
	strcpy(myReqUserLoginField.BrokerID,pReqUserLogin->BrokerID);
	strcpy(myReqUserLoginField.UserID,pReqUserLogin->UserID);
	strcpy(myReqUserLoginField.Password,pReqUserLogin->Password);

	return realapi->ReqUserLogin(&myReqUserLoginField,nRequestID);
}

int CMyMdApi::ReqUserLogout( CDINGFtdcReqUserLogoutField *pReqUserLogout, int nRequestID )
{
	//CDINGFtdcReqUserLogoutField myReqUserLogout;
	CThostFtdcUserLogoutField  mypReqUserLogout;
	return realapi->ReqUserLogout(&mypReqUserLogout,nRequestID);
}




int CMyMdApi::ReqSubscribeTopic( CDINGFtdcDisseminationField *pDissemination, int nRequestID )
{
	//return realapi->ReqSubscribeTopic(pDissemination,nRequestID);
	//realapi->SubscribeForQuoteRsp()
	return 1;
}



int CMyMdApi::ReqQryTopic( CDINGFtdcDisseminationField *pDissemination, int nRequestID )
{
	//return realapi->ReqQryTopic(pDissemination,nRequestID);
	//realapi->SubscribeForQuoteRsp()
	return 0;
}


CMyMdSpi::CMyMdSpi( CDINGFtdcMduserSpi* spi )
{
	realspi = spi;
}

void CMyMdSpi::OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	//throw std::exception("The method or operation is not implemented.");
	CDINGFtdcRspUserLoginField rspUserLogin;
	strcpy(rspUserLogin.BrokerID,pRspUserLogin->BrokerID);
	strcpy(rspUserLogin.ActionDay,pRspUserLogin->TradingDay);
	strcpy(rspUserLogin.CZCETime,pRspUserLogin->CZCETime);
	strcpy(rspUserLogin.SHFETime,pRspUserLogin->SHFETime);
	strcpy(rspUserLogin.FFEXTime,pRspUserLogin->FFEXTime);
	strcpy(rspUserLogin.DCETime,pRspUserLogin->DCETime);
	strcpy(rspUserLogin.LoginTime,pRspUserLogin->LoginTime);
	strcpy(rspUserLogin.UserID,pRspUserLogin->UserID);
	CDINGFtdcRspInfoField rspInfo;
	rspInfo.ErrorID = pRspInfo->ErrorID;
	strcpy(rspInfo.ErrorMsg,pRspInfo->ErrorMsg);
	realspi->OnRspUserLogin(&rspUserLogin,&rspInfo,nRequestID,bIsLast);
}

void CMyMdSpi::OnRspUserLogout( CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CDINGFtdcRspUserLogoutField rspUserLogout;
	strcpy(rspUserLogout.BrokerID,pUserLogout->BrokerID);
	strcpy(rspUserLogout.UserID,pUserLogout->UserID);
    CDINGFtdcRspInfoField rspInfo;
	rspInfo.ErrorID = pRspInfo->ErrorID;
	strcpy(rspInfo.ErrorMsg,pRspInfo->ErrorMsg);
	//throw std::exception("The method or operation is not implemented.");
	realspi->OnRspUserLogout(&rspUserLogout,&rspInfo,nRequestID,bIsLast);
}

void CMyMdSpi::OnRtnDepthMarketData( CThostFtdcDepthMarketDataField *pDepthMarketData )
{
	//throw std::exception("The method or operation is not implemented.");
	CDINGFtdcDepthMarketDataField depthMarketData={0};
	strcpy(depthMarketData.ActionDay,pDepthMarketData->ActionDay);
	strcpy(depthMarketData.InstrumentID,pDepthMarketData->InstrumentID);
	depthMarketData.AskPrice1=pDepthMarketData->AskPrice1;
	depthMarketData.AskVolume1=pDepthMarketData->AskVolume1;
	depthMarketData.BidPrice1=pDepthMarketData->BidPrice1;
	depthMarketData.BidVolume1=pDepthMarketData->BidVolume1;
	depthMarketData.LowerLimitPrice=pDepthMarketData->LowerLimitPrice;
	depthMarketData.UpperLimitPrice=pDepthMarketData->UpperLimitPrice;
	realspi->OnRtnDepthMarketData(&depthMarketData);
}

void CMyMdSpi::OnFrontConnected()
{
	realspi->OnFrontConnected();
}


void CMyMdApi::SubscribeMarketDataTopic( int nTopicID, DING_TE_RESUME_TYPE nResumeType )
{

}

int CMyMdApi::SubMarketData( char *ppInstrumentID[], int nCount )
{
	return realapi->SubscribeMarketData(ppInstrumentID,nCount);
}

int CMyMdApi::UnSubMarketData( char *ppInstrumentID[], int nCount )
{
	return realapi->SubscribeMarketData(ppInstrumentID,nCount);
}

int CMyMdApi::ReqSubMarketData( CDINGFtdcSpecificInstrumentField *pSpecificInstrument, int nRequestID )
{
	return 1;
	//return realapi->ReqSubMarketData(pSpecificInstrument,nRequestID);
}

int CMyMdApi::ReqUnSubMarketData( CDINGFtdcSpecificInstrumentField *pSpecificInstrument, int nRequestID )
{
	//return realapi->(pSpecificInstrument,nRequestID);
	return 1;
}

void CMyMdApi::SetUseMultiChannel( bool bUseMulti /*= false*/ )
{
	//return realapi->SetUseMultiChannel(bUseMulti);
}

void CMyMdApi::RegisterMultiChannel( const char *pMultiChannel )
{
	//return realapi->RegisterMultiChannel(pMultiChannel);
}


