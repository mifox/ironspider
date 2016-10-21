// TraderSpi.h: interface for the CTraderSpi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDSPI_H__4B26E1C3_9EEE_4412_8C08_71BFB55CE6FF__INCLUDED_)
#define AFX_MDSPI_H__4B26E1C3_9EEE_4412_8C08_71BFB55CE6FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PublicFuncs.h"
class CMdSpi : public CDINGFtdcMduserSpi  
{
public:
	CMdSpi(CMyMdApi *pMd);
	virtual ~CMdSpi();
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///���Ļص���ʼ֪ͨ����API�յ�һ�����ĺ����ȵ��ñ�������Ȼ���Ǹ�������Ļص�������Ǳ��Ļص�����֪ͨ��
	///@param nTopicID ������루��˽���������������������ȣ�
	///@param nSequenceNo �������
	virtual void OnPackageStart(int nTopicID, int nSequenceNo);

	///���Ļص�����֪ͨ����API�յ�һ�����ĺ����ȵ��ñ��Ļص���ʼ֪ͨ��Ȼ���Ǹ�������Ļص��������ñ�������
	///@param nTopicID ������루��˽���������������������ȣ�
	///@param nSequenceNo �������
	virtual void OnPackageEnd(int nTopicID, int nSequenceNo);


	///����Ӧ��
	virtual void OnRspError(CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///���ǰ��ϵͳ�û���¼Ӧ��
	virtual void OnRspUserLogin(CDINGFtdcRspUserLoginField *pRspUserLogin, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�û��˳�Ӧ��
	virtual void OnRspUserLogout(CDINGFtdcRspUserLogoutField *pRspUserLogout, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspSubscribeTopic(CDINGFtdcDisseminationField *pDissemination, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯӦ��
	virtual void OnRspQryTopic(CDINGFtdcDisseminationField *pDissemination, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CDINGFtdcDepthMarketDataField *pDepthMarketData);

	///���ĺ�Լ�������Ϣ
	virtual void OnRspSubMarketData(CDINGFtdcSpecificInstrumentField *pSpecificInstrument, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�˶���Լ�������Ϣ
	virtual void OnRspUnSubMarketData(CDINGFtdcSpecificInstrumentField *pSpecificInstrument, CDINGFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);;
	void ReqUserLogin(TDINGFtdcBrokerIDType appId, TDINGFtdcUserIDType userId, TDINGFtdcPasswordType passwd);
	void SubscribeMarketData(char* instIdList);
private:	
	CMyMdApi *m_pUserApi;
};

#endif // !defined(AFX_TRADERSPI_H__4B26E1C3_9EEE_4412_8C08_71BFB55CE6FF__INCLUDED_)
