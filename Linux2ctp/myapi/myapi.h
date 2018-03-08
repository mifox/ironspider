#ifndef ctpzctp
	#include "../ctpapi/ThostFtdcTraderApi.h"
#endif

#ifdef ctpzctp
	#include "../ctpapizctp/ThostFtdcTraderApi.h"
#endif

#include "../../LinuxProject2/myinclude/DINGFtdcTraderApi.h"
#ifndef MY_API_H_
#define MY_API_H_



class CMyTraderSpi:public  CThostFtdcTraderSpi
{
public:
	 CThostFtdcTraderApi * m;
	 CDINGFtdcTraderSpi * realspi;
	 CMyTraderSpi(CDINGFtdcTraderSpi* spi);

	 virtual void OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );

	 virtual void OnFrontConnected();

	 virtual void OnFrontDisconnected( int nReason );

	 virtual void OnHeartBeatWarning( int nTimeLapse );

	 virtual void OnRspUserLogout( CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );

	 virtual void OnRspOrderInsert( CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );

	 virtual void OnRspOrderAction( CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );

	 virtual void OnRspSettlementInfoConfirm( CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );

	 virtual void OnRtnOrder( CThostFtdcOrderField *pOrder );

	 virtual void OnRtnTrade( CThostFtdcTradeField *pTrade );

	 ///�����ѯ��Լ��Ӧ
	 virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	 ///�����ѯ�ʽ��˻���Ӧ
	 virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	 virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	 virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;


private:
	CThostFtdcTraderApi* pUserApi;
};
//-----
class CMyTraderApi
{
private:
	CMyTraderApi()
	{};
public:
	CThostFtdcTraderApi * realapi;
	static CMyTraderApi * CreateFtdcTraderApi(const char *pszFlowPath = "");


	///����TraderApi
	///@param pszFlowPath ����������Ϣ�ļ���Ŀ¼��Ĭ��Ϊ��ǰĿ¼
	///@return ��������UserApi
	//static CMyTraderApi *CreateFtdcTraderApi(const char *pszFlowPath = "");

	///��ȡϵͳ�汾��
	///@param nMajorVersion ���汾��
	///@param nMinorVersion �Ӱ汾��
	///@return ϵͳ��ʶ�ַ���
	static const char *GetVersion(int &nMajorVersion, int &nMinorVersion);

	///ɾ���ӿڶ�����
	///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
	virtual void Release();

	///��ʼ��
	///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
	virtual void Init();

	///�ȴ��ӿ��߳̽�������
	///@return �߳��˳�����
	virtual int Join();

	///��ȡ��ǰ������
	///@retrun ��ȡ���Ľ�����
	///@remark ֻ�е�¼�ɹ���,���ܵõ���ȷ�Ľ�����
	virtual const char *GetTradingDay();

	///ע��ǰ�û������ַ
	///@param pszFrontAddress��ǰ�û������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	virtual void RegisterFront(char *pszFrontAddress);

	///ע�����ַ����������ַ
	///@param pszNsAddress�����ַ����������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:12001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����12001������������˿ںš�
	///@remark RegisterFront������RegisterNameServer
	virtual void RegisterNameServer(char *pszNsAddress);

	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	virtual void RegisterSpi(CMyTraderSpi* pSpi );

	///����֤��
	///@param pszCertFileName �û�֤���ļ���
	///@param pszKeyFileName �û�˽Կ�ļ���
	///@param pszCaFileName ������CA֤���ļ���
	///@param pszKeyFilePassword �û�˽Կ�ļ�����
	///@return 0 �����ɹ�
	///@return -1 ������CA֤������ʧ��
	///@return -2 �û�֤������ʧ��
	///@return -3 �û�˽Կ����ʧ��	
	///@return -4 �û�֤��У��ʧ��
	virtual int RegisterCertificateFile(const char *pszCertFileName, const char *pszKeyFileName, 
		const char *pszCaFileName, const char *pszKeyFilePassword);

	///����˽������
	///@param nResumeType ˽�����ش���ʽ  
	///        DING_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        DING_TERT_RESUME:���ϴ��յ�������
	///        DING_TERT_QUICK:ֻ���͵�¼��˽����������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ�˽���������ݡ�
	virtual void SubscribePrivateTopic(DING_TE_RESUME_TYPE nResumeType);

	///���Ĺ�������
	///@param nResumeType �������ش���ʽ  
	///        DING_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        DING_TERT_RESUME:���ϴ��յ�������
	///        DING_TERT_QUICK:ֻ���͵�¼�󹫹���������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ������������ݡ�
	virtual void SubscribePublicTopic(DING_TE_RESUME_TYPE nResumeType);

	///���Ľ���Ա����
	///@param nResumeType ����Ա���ش���ʽ  
	///        DING_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        DING_TERT_RESUME:���ϴ��յ�������
	///        DING_TERT_QUICK:ֻ���͵�¼����Ա��������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ�����Ա�������ݡ�
	virtual void SubscribeUserTopic(DING_TE_RESUME_TYPE nResumeType);

	///����������ʱʱ�䡣
	///@param timeout ������ʱʱ��(��)  
	virtual void SetHeartbeatTimeout(unsigned int timeout);

	///��������־�ļ�
	///@param pszReqLogFileName ������־�ļ���  
	///@return 0 �����ɹ�
	///@return -1 ����־�ļ�ʧ��
	virtual int OpenRequestLog(const char *pszReqLogFileName);

	///��Ӧ����־�ļ�
	///@param pszRspLogFileName Ӧ����־�ļ���  
	///@return 0 �����ɹ�
	///@return -1 ����־�ļ�ʧ��
	virtual int OpenResponseLog(const char *pszRspLogFileName);


	///���ǰ��ϵͳ�û���¼����
	virtual int ReqUserLogin(CDINGFtdcReqUserLoginField *pReqUserLogin, int nRequestID);

	///�û��˳�����
	virtual int ReqUserLogout(CDINGFtdcReqUserLogoutField *pReqUserLogout, int nRequestID);

	///�û������޸�����
	virtual int ReqUserPasswordUpdate(CDINGFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID);

	///����¼������
	virtual int ReqOrderInsert(CDINGFtdcInputOrderField *pInputOrder, int nRequestID);

	///������������
	virtual int ReqOrderAction(CDINGFtdcOrderActionField *pOrderAction, int nRequestID);

	///������ѯ����
	virtual int ReqQryOrder(CDINGFtdcQryOrderField *pQryOrder, int nRequestID);
	///�ɽ�����ѯ����
	virtual int ReqQryTrade(CDINGFtdcQryTradeField *pQryTrade, int nRequestID);

	///����Ͷ�����˻���ѯ����
	virtual int ReqQryUserInvestor(CDINGFtdcQryUserInvestorField *pQryUserInvestor, int nRequestID);

	///���ױ����ѯ����
	virtual int ReqQryTradingCode(CDINGFtdcQryTradingCodeField *pQryTradingCode, int nRequestID);

	///Ͷ�����ʽ��˻���ѯ����
	virtual int ReqQryInvestorAccount(CDINGFtdcQryInvestorAccountField *pQryInvestorAccount, int nRequestID);

	///��Լ��ѯ����
	virtual int ReqQryInstrument(CDINGFtdcQryInstrumentField *pQryInstrument, int nRequestID);

	///��������ѯ����
	virtual int ReqQryExchange(CDINGFtdcQryExchangeField *pQryExchange, int nRequestID);
	
	///Ͷ���ֲֲ߳�ѯ����
	virtual int ReqQryInvestorPosition(CDINGFtdcQryInvestorPositionField *pQryInvestorPosition, int nRequestID);

	///������������
	virtual int ReqSubscribeTopic(CDINGFtdcDisseminationField *pDissemination, int nRequestID);

	///�Ϲ������ѯ����
	virtual int ReqQryComplianceParam(CDINGFtdcQryComplianceParamField *pQryComplianceParam, int nRequestID);

	///�����ѯ����
	virtual int ReqQryTopic(CDINGFtdcDisseminationField *pDissemination, int nRequestID);

	///Ͷ�����������ʲ�ѯ����
	virtual int ReqQryInvestorFee(CDINGFtdcQryInvestorFeeField *pQryInvestorFee, int nRequestID);

	///Ͷ���߱�֤���ʲ�ѯ����
	virtual int ReqQryInvestorMargin(CDINGFtdcQryInvestorMarginField *pQryInvestorMargin, int nRequestID);

	virtual int ReqSettlementInfoConfirm(CDINGFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID);


	//void ReqOrderInsertGo( CDINGFtdcInputOrderField& req );
protected:
	~CMyTraderApi(){};
	


};

#endif
