#include "../../LinuxProject2/myinclude/DINGFtdcTraderApi.h"
#include "include/USTPFtdcTraderApi.h"

#ifndef MY_API_H_
#define MY_API_H_
class CMyTraderSpi:public  CUstpFtdcTraderSpi
{
public:
	CDINGFtdcTraderSpi * realspi;
	CMyTraderSpi(CDINGFtdcTraderSpi* spi);

	virtual void OnRspUserLogin( CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );

	virtual void OnRspUserLogout( CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );

	virtual void OnRspOrderInsert( CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );

	virtual void OnRspOrderAction( CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );

	virtual void OnRtnTrade( CUstpFtdcTradeField *pTrade );

	virtual void OnRtnOrder( CUstpFtdcOrderField *pOrder );

	virtual void OnRspQryOrder( CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );



};
//-----
class CMyTraderApi
{
private:
	CMyTraderApi()
	{};
public:
	CUstpFtdcTraderApi * realapi;
	static CMyTraderApi * CreateFtdcTraderApi(const char *pszFlowPath = "");


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
	virtual void RegisterSpi(CMyTraderSpi *pSpi);

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

	///订阅私有流。
	///@param nResumeType 私有流重传方式  
	///        DING_TERT_RESTART:从本交易日开始重传
	///        DING_TERT_RESUME:从上次收到的续传
	///        DING_TERT_QUICK:只传送登录后私有流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到私有流的数据。
	virtual void SubscribePrivateTopic(DING_TE_RESUME_TYPE nResumeType);

	///订阅公共流。
	///@param nResumeType 公共流重传方式  
	///        DING_TERT_RESTART:从本交易日开始重传
	///        DING_TERT_RESUME:从上次收到的续传
	///        DING_TERT_QUICK:只传送登录后公共流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到公共流的数据。
	virtual void SubscribePublicTopic(DING_TE_RESUME_TYPE nResumeType);

	///订阅交易员流。
	///@param nResumeType 交易员流重传方式  
	///        DING_TERT_RESTART:从本交易日开始重传
	///        DING_TERT_RESUME:从上次收到的续传
	///        DING_TERT_QUICK:只传送登录后交易员流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到交易员流的数据。
	virtual void SubscribeUserTopic(DING_TE_RESUME_TYPE nResumeType);

	///设置心跳超时时间。
	///@param timeout 心跳超时时间(秒)  
	virtual void SetHeartbeatTimeout(unsigned int timeout);

	///打开请求日志文件
	///@param pszReqLogFileName 请求日志文件名  
	///@return 0 操作成功
	///@return -1 打开日志文件失败
	virtual int OpenRequestLog(const char *pszReqLogFileName);

	///打开应答日志文件
	///@param pszRspLogFileName 应答日志文件名  
	///@return 0 操作成功
	///@return -1 打开日志文件失败
	virtual int OpenResponseLog(const char *pszRspLogFileName);


	///风控前置系统用户登录请求
	virtual int ReqUserLogin(CDINGFtdcReqUserLoginField *pReqUserLogin, int nRequestID);

	///用户退出请求
	virtual int ReqUserLogout(CDINGFtdcReqUserLogoutField *pReqUserLogout, int nRequestID);

	///用户密码修改请求
	virtual int ReqUserPasswordUpdate(CDINGFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID);

	///报单录入请求
	virtual int ReqOrderInsert(CDINGFtdcInputOrderField *pInputOrder, int nRequestID);

	///报单操作请求
	virtual int ReqOrderAction(CDINGFtdcOrderActionField *pOrderAction, int nRequestID);

	///报单查询请求
	virtual int ReqQryOrder(CDINGFtdcQryOrderField *pQryOrder, int nRequestID);
	///成交单查询请求
	virtual int ReqQryTrade(CDINGFtdcQryTradeField *pQryTrade, int nRequestID);

	///可用投资者账户查询请求
	virtual int ReqQryUserInvestor(CDINGFtdcQryUserInvestorField *pQryUserInvestor, int nRequestID);

	///交易编码查询请求
	virtual int ReqQryTradingCode(CDINGFtdcQryTradingCodeField *pQryTradingCode, int nRequestID);

	///投资者资金账户查询请求
	virtual int ReqQryInvestorAccount(CDINGFtdcQryInvestorAccountField *pQryInvestorAccount, int nRequestID);

	///合约查询请求
	virtual int ReqQryInstrument(CDINGFtdcQryInstrumentField *pQryInstrument, int nRequestID);

	///交易所查询请求
	virtual int ReqQryExchange(CDINGFtdcQryExchangeField *pQryExchange, int nRequestID);
	
	///投资者持仓查询请求
	virtual int ReqQryInvestorPosition(CDINGFtdcQryInvestorPositionField *pQryInvestorPosition, int nRequestID);

	///订阅主题请求
	virtual int ReqSubscribeTopic(CDINGFtdcDisseminationField *pDissemination, int nRequestID);

	///合规参数查询请求
	virtual int ReqQryComplianceParam(CDINGFtdcQryComplianceParamField *pQryComplianceParam, int nRequestID);

	///主题查询请求
	virtual int ReqQryTopic(CDINGFtdcDisseminationField *pDissemination, int nRequestID);

	///投资者手续费率查询请求
	virtual int ReqQryInvestorFee(CDINGFtdcQryInvestorFeeField *pQryInvestorFee, int nRequestID);

	///投资者保证金率查询请求
	virtual int ReqQryInvestorMargin(CDINGFtdcQryInvestorMarginField *pQryInvestorMargin, int nRequestID);

	virtual int ReqSettlementInfoConfirm(CDINGFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID);
protected:
	~CMyTraderApi(){};
};

#endif
