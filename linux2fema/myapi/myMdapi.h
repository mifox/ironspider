#include "../../LinuxProject2/myinclude/DINGFtdcMduserApi.h"
#include "../include/USTPFtdcMduserApi.h"
#ifndef MYMD_API_H_
#define MYMD_API_H_
class CMyMdSpi:public  CUstpFtdcMduserSpi
{
public:
	CDINGFtdcMduserSpi * realspi;
	CMyMdSpi(CDINGFtdcMduserSpi* spi);

	virtual void OnFrontConnected();

	virtual void OnFrontDisconnected( int nReason );

	virtual void OnPackageStart( int nTopicID, int nSequenceNo );

	virtual void OnRspUserLogin( CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );

	virtual void OnRtnDepthMarketData( CUstpFtdcDepthMarketDataField *pDepthMarketData );

	virtual void OnRspSubMarketData( CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );

};
//-----
class CMyMdApi
{
private:
	CMyMdApi()
	{};
public:
	CUstpFtdcMduserApi * realapi;
	static CMyMdApi * CreateFtdcMdApi(const char *pszFlowPath = "");


	/// �Ƿ�ʹ�öಥ��ʽ��������
	/// @param bUseMulti Ĭ��Ϊfalse����ʾ��ʹ�öಥ��������
	/// @remark ���ʹ�öಥ���������¼��Ҳʹ�ò�����Ҫ��¼����ʹ�õĽӿ�
	virtual void SetUseMultiChannel(bool bUseMulti = false);

	///ע��ಥͨ��
	///@param pMultiChannel �ಥͨ����ַ��ַ��
	///@remark �����ַ�����ӣ���multi://172.25.125@232.0.0.1:5131���� 
	///@remark ��multi������ʹ�öಥ���䣻��172.25.125������ȷ�����նಥ����������232.0.0.1:5131������ಥͨ�����˿ڡ�
	///@remark ���Զ�ε��ã�ע����ͨ�������ն��ͨ�����ٶ����ŵ�����
	virtual void RegisterMultiChannel(const char *pMultiChannel);

	///����TraderApi
	///@param pszFlowPath ����������Ϣ�ļ���Ŀ¼��Ĭ��Ϊ��ǰĿ¼
	///@return ��������UserApi
	//static CDINGFtdcTraderApi *CreateFtdcTraderApi(const char *pszFlowPath = "");

	///��ȡϵͳ�汾��
	///@param nMajorVersion ���汾��
	///@param nMinorVersion �Ӱ汾��
	///@return ϵͳ��ʶ�ַ���
	static const char *GetVersion(int &nMajorVersion, int &nMinorVersion);;

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
	virtual void RegisterSpi(CMyMdSpi *pSpi);

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

	///�����г����顣
	///@param nTopicID �г���������  
	///@param nResumeType �г������ش���ʽ  
	///        DING_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        DING_TERT_RESUME:���ϴ��յ�������(�Ƕ���ȫ����Լʱ����֧������ģʽ)
	///        DING_TERT_QUICK:�ȴ��͵�ǰ�������,�ٴ��͵�¼���г����������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ�˽���������ݡ�
	virtual void SubscribeMarketDataTopic(int nTopicID, DING_TE_RESUME_TYPE nResumeType);

	///���ĺ�Լ���顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	virtual int SubMarketData(char *ppInstrumentID[], int nCount);

	///�˶���Լ���顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	virtual int UnSubMarketData(char *ppInstrumentID[], int nCount);		

	///����������ʱʱ�䡣
	///@param timeout ������ʱʱ��(��)  
	virtual void SetHeartbeatTimeout(unsigned int timeout);


	///���ǰ��ϵͳ�û���¼����
	virtual int ReqUserLogin(CDINGFtdcReqUserLoginField *pReqUserLogin, int nRequestID);

	///�û��˳�����
	virtual int ReqUserLogout(CDINGFtdcReqUserLogoutField *pReqUserLogout, int nRequestID);

	///������������
	virtual int ReqSubscribeTopic(CDINGFtdcDisseminationField *pDissemination, int nRequestID);

	///�����ѯ����
	virtual int ReqQryTopic(CDINGFtdcDisseminationField *pDissemination, int nRequestID);

	///���ĺ�Լ�������Ϣ
	virtual int ReqSubMarketData(CDINGFtdcSpecificInstrumentField *pSpecificInstrument, int nRequestID);

	///�˶���Լ�������Ϣ
	virtual int ReqUnSubMarketData(CDINGFtdcSpecificInstrumentField *pSpecificInstrument, int nRequestID);
protected:
	~CMyMdApi(){};
};
#endif