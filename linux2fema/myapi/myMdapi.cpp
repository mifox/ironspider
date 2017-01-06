#include "myMdapi.h"




CMyMdApi * CMyMdApi::CreateFtdcMdApi( const char *pszFlowPath /*= ""*/ )
{
	CMyMdApi* temp= new CMyMdApi();
	temp->realapi = CUstpFtdcMduserApi::CreateFtdcMduserApi(pszFlowPath);
	return temp;
}

const char * CMyMdApi::GetVersion( int &nMajorVersion, int &nMinorVersion )
{
	return CUstpFtdcMduserApi::GetVersion(nMajorVersion,nMinorVersion);
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
	realapi->RegisterSpi(pSpi);
}

int CMyMdApi::RegisterCertificateFile( const char *pszCertFileName, const char *pszKeyFileName, const char *pszCaFileName, const char *pszKeyFilePassword )
{
	return realapi->RegisterCertificateFile(pszCertFileName,pszKeyFileName,pszCaFileName,pszKeyFilePassword);
}


void CMyMdApi::SetHeartbeatTimeout( unsigned int timeout )
{
	realapi->SetHeartbeatTimeout(timeout);
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
	return realapi->ReqUserLogin(( CUstpFtdcReqUserLoginField *)pReqUserLogin,nRequestID);
}

int CMyMdApi::ReqUserLogout( CDINGFtdcReqUserLogoutField *pReqUserLogout, int nRequestID )
{
	return realapi->ReqUserLogout((CUstpFtdcReqUserLogoutField *)pReqUserLogout,nRequestID);
}




int CMyMdApi::ReqSubscribeTopic( CDINGFtdcDisseminationField *pDissemination, int nRequestID )
{
	return realapi->ReqSubscribeTopic((CUstpFtdcDisseminationField *)pDissemination,nRequestID);
}



int CMyMdApi::ReqQryTopic( CDINGFtdcDisseminationField *pDissemination, int nRequestID )
{
	return realapi->ReqQryTopic((CUstpFtdcDisseminationField *)pDissemination,nRequestID);
}


CMyMdSpi::CMyMdSpi( CDINGFtdcMduserSpi* spi )
{
	realspi = spi;
}

void CMyMdSpi::OnFrontConnected()
{
	realspi->OnFrontConnected();
	//throw std::exception("The method or operation is not implemented.");
}

void CMyMdSpi::OnFrontDisconnected( int nReason )
{
	realspi->OnFrontDisconnected(nReason);
	//throw std::exception("The method or operation is not implemented.");
}

void CMyMdSpi::OnPackageStart( int nTopicID, int nSequenceNo )
{
	realspi->OnPackageStart(nTopicID,nSequenceNo);
	//throw std::exception("The method or operation is not implemented.");
}

void CMyMdSpi::OnRspUserLogin( CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	realspi->OnRspUserLogin( (CDINGFtdcRspUserLoginField *)pRspUserLogin, (CDINGFtdcRspInfoField *)pRspInfo,nRequestID, bIsLast );
	//throw std::exception("The method or operation is not implemented.");
}

void CMyMdSpi::OnRtnDepthMarketData( CUstpFtdcDepthMarketDataField *pDepthMarketData )
{
	realspi->OnRtnDepthMarketData((CDINGFtdcDepthMarketDataField *)pDepthMarketData);
	//throw std::exception("The method or operation is not implemented.");
}

void CMyMdSpi::OnRspSubMarketData( CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	realspi->OnRspSubMarketData((CDINGFtdcSpecificInstrumentField *)pSpecificInstrument,(CDINGFtdcRspInfoField *)pRspInfo,nRequestID,bIsLast);
	//throw std::exception("The method or operation is not implemented.");
}


void CMyMdApi::SubscribeMarketDataTopic( int nTopicID, DING_TE_RESUME_TYPE nResumeType )
{
	return realapi->SubscribeMarketDataTopic(nTopicID, (USTP_TE_RESUME_TYPE) nResumeType);
}

int CMyMdApi::SubMarketData( char *ppInstrumentID[], int nCount )
{
	return realapi->SubMarketData(ppInstrumentID,nCount);
}

int CMyMdApi::UnSubMarketData( char *ppInstrumentID[], int nCount )
{
	return realapi->UnSubMarketData(ppInstrumentID,nCount);
}

int CMyMdApi::ReqUnSubMarketData(CDINGFtdcSpecificInstrumentField  *pSpecificInstrument, int nRequestID )
{
	return realapi->ReqUnSubMarketData((CUstpFtdcSpecificInstrumentField *)pSpecificInstrument,nRequestID);
}

void CMyMdApi::SetUseMultiChannel( bool bUseMulti /*= false*/ )
{
	return realapi->SetUseMultiChannel(bUseMulti);
}

void CMyMdApi::RegisterMultiChannel( const char *pMultiChannel )
{
	return realapi->RegisterMultiChannel(pMultiChannel);
}

int CMyMdApi::ReqSubMarketData( CDINGFtdcSpecificInstrumentField *pSpecificInstrument, int nRequestID )
{
	return realapi->ReqSubMarketData((CUstpFtdcSpecificInstrumentField *)pSpecificInstrument,nRequestID);
}
