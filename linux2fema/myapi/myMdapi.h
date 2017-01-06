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


	/// 是否使用多播方式接收行情
	/// @param bUseMulti 默认为false，表示不使用多播接收行情
	/// @remark 如果使用多播，则无需登录，也使用不了需要登录才能使用的接口
	virtual void SetUseMultiChannel(bool bUseMulti = false);

	///注册多播通道
	///@param pMultiChannel 多播通道地址地址，
	///@remark 网络地址的例子：”multi://172.25.125@232.0.0.1:5131”。 
	///@remark “multi”代表使用多播传输；“172.25.125”用来确定接收多播的网卡；”232.0.0.1:5131”代表多播通道及端口。
	///@remark 可以多次调用，注册多个通道，接收多个通道中速度最优的行情
	virtual void RegisterMultiChannel(const char *pMultiChannel);

	///创建TraderApi
	///@param pszFlowPath 存贮订阅信息文件的目录，默认为当前目录
	///@return 创建出的UserApi
	//static CDINGFtdcTraderApi *CreateFtdcTraderApi(const char *pszFlowPath = "");

	///获取系统版本号
	///@param nMajorVersion 主版本号
	///@param nMinorVersion 子版本号
	///@return 系统标识字符串
	static const char *GetVersion(int &nMajorVersion, int &nMinorVersion);;

	///删除接口对象本身
	///@remark 不再使用本接口对象时,调用该函数删除接口对象
	virtual void Release();

	///初始化
	///@remark 初始化运行环境,只有调用后,接口才开始工作
	virtual void Init();

	///等待接口线程结束运行
	///@return 线程退出代码
	virtual int Join();

	///获取当前交易日
	///@retrun 获取到的交易日
	///@remark 只有登录成功后,才能得到正确的交易日
	virtual const char *GetTradingDay();

	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	virtual void RegisterFront(char *pszFrontAddress);

	///注册名字服务器网络地址
	///@param pszNsAddress：名字服务器网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:12001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”12001”代表服务器端口号。
	///@remark RegisterFront优先于RegisterNameServer
	virtual void RegisterNameServer(char *pszNsAddress);

	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	virtual void RegisterSpi(CMyMdSpi *pSpi);

	///加载证书
	///@param pszCertFileName 用户证书文件名
	///@param pszKeyFileName 用户私钥文件名
	///@param pszCaFileName 可信任CA证书文件名
	///@param pszKeyFilePassword 用户私钥文件密码
	///@return 0 操作成功
	///@return -1 可信任CA证书载入失败
	///@return -2 用户证书载入失败
	///@return -3 用户私钥载入失败	
	///@return -4 用户证书校验失败
	virtual int RegisterCertificateFile(const char *pszCertFileName, const char *pszKeyFileName, 
		const char *pszCaFileName, const char *pszKeyFilePassword);

	///订阅市场行情。
	///@param nTopicID 市场行情主题  
	///@param nResumeType 市场行情重传方式  
	///        DING_TERT_RESTART:从本交易日开始重传
	///        DING_TERT_RESUME:从上次收到的续传(非订阅全部合约时，不支持续传模式)
	///        DING_TERT_QUICK:先传送当前行情快照,再传送登录后市场行情的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到私有流的数据。
	virtual void SubscribeMarketDataTopic(int nTopicID, DING_TE_RESUME_TYPE nResumeType);

	///订阅合约行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	virtual int SubMarketData(char *ppInstrumentID[], int nCount);

	///退订合约行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	virtual int UnSubMarketData(char *ppInstrumentID[], int nCount);		

	///设置心跳超时时间。
	///@param timeout 心跳超时时间(秒)  
	virtual void SetHeartbeatTimeout(unsigned int timeout);


	///风控前置系统用户登录请求
	virtual int ReqUserLogin(CDINGFtdcReqUserLoginField *pReqUserLogin, int nRequestID);

	///用户退出请求
	virtual int ReqUserLogout(CDINGFtdcReqUserLogoutField *pReqUserLogout, int nRequestID);

	///订阅主题请求
	virtual int ReqSubscribeTopic(CDINGFtdcDisseminationField *pDissemination, int nRequestID);

	///主题查询请求
	virtual int ReqQryTopic(CDINGFtdcDisseminationField *pDissemination, int nRequestID);

	///订阅合约的相关信息
	virtual int ReqSubMarketData(CDINGFtdcSpecificInstrumentField *pSpecificInstrument, int nRequestID);

	///退订合约的相关信息
	virtual int ReqUnSubMarketData(CDINGFtdcSpecificInstrumentField *pSpecificInstrument, int nRequestID);
protected:
	~CMyMdApi(){};
};
#endif