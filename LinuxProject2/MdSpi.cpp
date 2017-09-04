// TraderSpi.cpp: implementation of the CMdSpi class.
//
//////////////////////////////////////////////////////////////////////
#include "PublicFuncs.h"
#include "MdSpi.h"
#include <myMdapi.h>
#include "config.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


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
CMdSpi::CMdSpi(CMyMdApi *pMd):m_pUserApi(pMd)
{

}
CMdSpi::~CMdSpi()
{

}

void CMdSpi::OnFrontConnected()
{
	printf("test");
	ReqUserLogin(appId,userId,passwd); 
}


void CMdSpi::ReqUserLogin(TDINGFtdcBrokerIDType	appId,
	TDINGFtdcUserIDType	userId,	TDINGFtdcPasswordType	passwd)
{
	CDINGFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, appId);
	strcpy(req.UserID, userId);
	strcpy(req.Password, passwd);
	int ret = m_pUserApi->ReqUserLogin(&req, ++requestId);
	LOG4CPLUS_DEBUG(log_1," 请求 | 发送登录..."<<((ret == 0) ? "成功" :"失败") << endl);	
	//SetEvent(g_hEvent);
	//sendForWait(EVENT_MD_LOGIN);
}

void CMdSpi::SubscribeMarketData(char* instIdList)
{
	vector<char*> list;
	char *token = strtok(instIdList, ",");
	while( token != NULL ){
		char * t =new char[10];
		memset(t,0,10);
		strcpy(t,token);
		list.push_back(t); 
		token = strtok(NULL, ",");
	}
	unsigned int len = list.size();
	char** pInstId = new char* [len];  
	for(unsigned int i=0; i<len;i++)  pInstId[i]=list[i]; 
	int ret=m_pUserApi->SubMarketData(pInstId, len);
	LOG4CPLUS_DEBUG(log_1," 请求 | 发送行情订阅... "<<((ret == 0) ? "成功" : "失败")<< endl);
	//SetEvent(g_hEvent);
}
void CMdSpi::OnFrontDisconnected( int nReason )
{
	printf("test");
}

void CMdSpi::OnHeartBeatWarning( int nTimeLapse )
{

}

void CMdSpi::OnPackageStart( int nTopicID, int nSequenceNo )
{

}

void CMdSpi::OnPackageEnd( int nTopicID, int nSequenceNo )
{

}

void CMdSpi::OnRspError( CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void CMdSpi::OnRspUserLogin( CDINGFtdcRspUserLoginField *pRspUserLogin, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	//char ist[] = md;
	SubscribeMarketData(instIdList);
}

void CMdSpi::OnRspUserLogout( CDINGFtdcRspUserLogoutField *pRspUserLogout, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void CMdSpi::OnRspSubscribeTopic( CDINGFtdcDisseminationField *pDissemination, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void CMdSpi::OnRspQryTopic( CDINGFtdcDisseminationField *pDissemination, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void CMdSpi::OnRtnDepthMarketData( CDINGFtdcDepthMarketDataField *pDepthMarketData )
{
	mapMd[string(pDepthMarketData->InstrumentID)]=*pDepthMarketData;
}

void CMdSpi::OnRspSubMarketData( CDINGFtdcSpecificInstrumentField *pSpecificInstrument, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	printf("test\n");
}

void CMdSpi::OnRspUnSubMarketData( CDINGFtdcSpecificInstrumentField *pSpecificInstrument, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	
}





