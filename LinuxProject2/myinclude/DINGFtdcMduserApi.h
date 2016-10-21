/////////////////////////////////////////////////////////////////////////
///@system 风控前置系统
///@company 上海金融期货信息技术有限公司
///@file DINGFtdcMduserApi.h
///@brief 定义了客户端接口
///@history 
///20130520	徐忠华	创建该文件
/////////////////////////////////////////////////////////////////////////

#if !defined(DING_FTDCMDUSERAPI_H)
#define DING_FTDCMDUSERAPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DINGFtdcUserApiStruct.h"

#if defined(ISLIB) && defined(WIN32)
#ifdef LIB_MDUSER_API_EXPORT
#define MDUSER_API_EXPORT __declspec(dllexport)
#else
#define MDUSER_API_EXPORT __declspec(dllimport)
#endif
#else
#define MDUSER_API_EXPORT 
#endif

class CDINGFtdcMduserSpi
{
public:
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected(){};
	
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason){};
		
	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse){};
	
	///报文回调开始通知。当API收到一个报文后，首先调用本方法，然后是各数据域的回调，最后是报文回调结束通知。
	///@param nTopicID 主题代码（如私有流、公共流、行情流等）
	///@param nSequenceNo 报文序号
	virtual void OnPackageStart(int nTopicID, int nSequenceNo){};
	
	///报文回调结束通知。当API收到一个报文后，首先调用报文回调开始通知，然后是各数据域的回调，最后调用本方法。
	///@param nTopicID 主题代码（如私有流、公共流、行情流等）
	///@param nSequenceNo 报文序号
	virtual void OnPackageEnd(int nTopicID, int nSequenceNo){};


	///错误应答
	virtual void OnRspError(CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///风控前置系统用户登录应答
	virtual void OnRspUserLogin(CDINGFtdcRspUserLoginField *pRspUserLogin, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///用户退出应答
	virtual void OnRspUserLogout(CDINGFtdcRspUserLogoutField *pRspUserLogout, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///订阅主题应答
	virtual void OnRspSubscribeTopic(CDINGFtdcDisseminationField *pDissemination, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///主题查询应答
	virtual void OnRspQryTopic(CDINGFtdcDisseminationField *pDissemination, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///深度行情通知
	virtual void OnRtnDepthMarketData(CDINGFtdcDepthMarketDataField *pDepthMarketData) {};

	///订阅合约的相关信息
	virtual void OnRspSubMarketData(CDINGFtdcSpecificInstrumentField *pSpecificInstrument, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///退订合约的相关信息
	virtual void OnRspUnSubMarketData(CDINGFtdcSpecificInstrumentField *pSpecificInstrument, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
};


#endif
