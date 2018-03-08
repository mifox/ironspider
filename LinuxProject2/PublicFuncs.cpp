// PublicFuncs.cpp: implementation of the PublicFuncs class.
//
//////////////////////////////////////////////////////////////////////

#include "PublicFuncs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PublicFuncs::PublicFuncs()
{

}



PublicFuncs::~PublicFuncs()
{

}

int PublicFuncs::SendCommonSignal( int sv )
{
	for (int i=0;i<this->carlist.size();i++)
	{
		carlist[i]->sendSignal(sv);
	}
	return 1;
}

CDINGFtdcOrderField classMapper::toCDINGFtdcOrderField(CDINGFtdcInputOrderField& a)
{
	CDINGFtdcOrderField* b=new CDINGFtdcOrderField;
	memcpy(b,&a,sizeof(CDINGFtdcInputOrderField));
	return *b; 
}

CDINGFtdcOrderFieldEX classMapper::toCDINGFtdcOrderFieldEX(CDINGFtdcInputOrderField& a)
{
	CDINGFtdcOrderFieldEX* b=new CDINGFtdcOrderFieldEX;
	memcpy(b,&a,sizeof(CDINGFtdcInputOrderField));
	return *b; 
}

CDINGFtdcOrderFieldEX* classMapper::toCDINGFtdcOrderFieldEX2(CDINGFtdcInputOrderField& a)
{
	CDINGFtdcOrderFieldEX* b=new CDINGFtdcOrderFieldEX;
	memset(b,0,sizeof(CDINGFtdcInputOrderField));
	memcpy(b,&a,sizeof(CDINGFtdcInputOrderField));
	b->main.OrderStatus='A';
	return b; 
}

CDINGFtdcOrderFieldEX classMapper::toCDINGFtdcOrderFieldEX(CDINGFtdcOrderField& a)
{
	CDINGFtdcOrderFieldEX* b=new CDINGFtdcOrderFieldEX;
	memcpy(b,&a,sizeof(CDINGFtdcOrderField));
	return *b; 
}


typedef struct 
{
	int nordspeed;
	int nordloop;
} stordspeed;

/*---------------全局变量区---------------*/

const char* TEST_API_INI_NAME="./config/TestApi.ini";
char* g_pFlowPath="./flow/";
char* g_pProductInfo="演示测试工具V1.0";
char* g_pProgramName="TestApi";
int g_choose;

extern int g_nSwitch;
extern FILE * g_fpSend;

//1.登录信息
///经纪公司代码
TDINGFtdcBrokerIDType g_BrokerID;
///交易用户代码
TDINGFtdcUserIDType	g_UserID;
///密码
TDINGFtdcPasswordType	g_Password;

//2.地址信息
char g_frontaddr[BUFLEN];

//3.报单信息
int g_nOrdLocalID=0;
CMyTraderApi * g_puserapi=NULL;
CMyMdApi * g_puserMdapi=NULL;
#ifdef WIN32
#else
int code_convert(char *from_charset, char *to_charset, char *inbuf, int inlen, char *outbuf, int outlen)
{
	iconv_t cd;
	int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);
	if (cd == 0)
		return -1;
	memset(outbuf, 0, outlen);
	if (iconv(cd, pin, (size_t*) &inlen, pout, (size_t*)&outlen) == -1)
		return -1;
	iconv_close(cd);
	return 0;
}

int u2g(char *inbuf, int inlen, char *outbuf, int outlen)
{
	return code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, outlen);
}

char* u2gc(char *inbuf, int inlen, char *outbuf, int outlen)
{
	code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, outlen);
	return outbuf;
}

int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
      //  return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
	return code_convert("gbk", "utf-8", inbuf, inlen, outbuf, outlen);
}

char* g2uc(char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
      //  return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
	code_convert("gbk", "utf-8", inbuf, inlen, outbuf, outlen);
	return outbuf;
}
#endif



#ifdef _WIN32
#include <time.h>
#else
#include <sys/time.h>
#endif

unsigned long long GetCurrentTimeMsec()
{
#ifdef _WIN32
	struct timeval tv;
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;

	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tv.tv_sec = clock;
	tv.tv_usec = wtm.wMilliseconds * 1000;
    //return ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000);
	return wtm.wMilliseconds;
#else
	struct timeval tv;
	gettimeofday(&tv,NULL);
	//return ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000);
	return (unsigned long long)tv.tv_usec / 1000;
#endif
}


/*---------------全局函数区---------------*/
void ShowManu()
{
	printf("**********************\n"
	       "选择需要执行的操作\n"
	       "1-报单申报\n"
	       "2-报单操作\n"
	       "3-报单查询\n"
	       "4-成交查询\n"
	       "5-投资者账户查询\n"
	       "6-交易编码查询\n"
	       "7-交易所查询\n"
	       "8-合约信息查询\n"
	       "9-可用投资者查询\n"
	       "10-客户持仓查询\n"
	       "11-修改用户密码\n"
		   "12-投资者手续费率查询\n"
		   "13-投资者保证金率查询\n"
		   "14-合规参数查询\n"
	       "0-退出\n"
	       "**********************\n"
	       "请选择："
	);
	scanf("%d",&g_choose);
	
}


void StartInputOrder ()
{
	if (g_puserapi==NULL)
	{
		printf("StartInputOrder  USERAPI未创建");
		return ;
	}
	//CDINGFtdcInputOrderField* pord=g_pOrd->at(1);
	CDINGFtdcInputOrderField ord;
	memset(&ord,0,sizeof(CDINGFtdcInputOrderField));
	printf("请输入报单信息\n");
	
	printf("input InvestorID\n");
	scanf("%s",(ord.InvestorID));
	printf("InvestorID=[%s]\n",ord.InvestorID);
	
	printf("input InstrumentID\n");
	scanf("%s",(ord.InstrumentID));
	printf("InstrumentID=[%s]\n",(ord.InstrumentID));
	getchar();
	printf("input OrderPriceType －－[1:市价 2:限价]－－\n");
	scanf("%c",&(ord.OrderPriceType));
	printf("OrderPriceType=[%c]\n",(ord.OrderPriceType));
	getchar();
	printf("input Direction －－－[0:买 1:卖]－－\n");
	scanf("%c",&(ord.Direction));
	printf("Direction=[%c]\n",(ord.Direction));
	getchar();
	printf("input OffsetFlag －－[0:开仓 1:平仓]－－\n");
	scanf("%c",&(ord.OffsetFlag));
	printf("OffsetFlag=[%c]\n",(ord.OffsetFlag));
	getchar();
	printf("input HedgeFlag －－[1:投机 2:套保 3:套利]－－\n");
	scanf("%c",&(ord.HedgeFlag));
	printf("HedgeFlag=[%c]\n",(ord.HedgeFlag));
	getchar();
	printf("input LimitPrice\n");
	scanf("%lf",&(ord.LimitPrice));
	printf("LimitPrice=[%lf]\n",(ord.LimitPrice));
	
	printf("input Volume\n");
	scanf("%d",&(ord.Volume));
	printf("Volume=[%d]\n",(ord.Volume));
	getchar();
	printf("input TimeCondition －－[1:IOC  2:本节有效 3:当日有效]－－\n");
	scanf("%c",&(ord.TimeCondition));
	printf("InstrumentID=[%c]\n",(ord.TimeCondition));	

	
	strcpy(ord.BrokerID,g_BrokerID);
	strcpy(ord.UserID,g_UserID);
	strcpy(ord.ExchangeID,"CFFEX");
	ord.VolumeCondition='1';
	ord.ForceCloseReason='0';
	sprintf(ord.UserOrderLocalID,"%012d",g_nOrdLocalID++);
		
	g_puserapi->ReqOrderInsert(&ord,g_nOrdLocalID++);


	return ;
}


void StartOrderAction()
{
	/*支持系统报单号和本地报单号两种撤单方式*/
	//-----------------------系统报单号撤单----------------------------//
	int SysID;
	CDINGFtdcOrderActionField OrderAction;
	memset(&OrderAction,0,sizeof(CDINGFtdcOrderActionField));
	if (g_puserapi==NULL)
	{
		printf("StartOrderAction  USERAPI未创建");
		return ;
	}

	strcpy(OrderAction.ExchangeID,"CFFEX");
	strcpy(OrderAction.BrokerID,g_BrokerID);
	strcpy(OrderAction.UserID,g_UserID);

	printf("input InvestorID\n");
	scanf("%s",(OrderAction.InvestorID));
	printf("InvestorID=[%s]\n",OrderAction.InvestorID);
	
	printf("请输入系统报单号：");
	scanf("%d",&SysID);
	sprintf(OrderAction.OrderSysID,"%12d",SysID);
	printf("撤销系统报单号[%s]\n",OrderAction.OrderSysID);
	strcpy(OrderAction.UserOrderLocalID,"");
	OrderAction.ActionFlag=DING_FTDC_AF_Delete;
	sprintf(OrderAction.UserOrderActionLocalID,"%012d",g_nOrdLocalID++);

	g_puserapi->ReqOrderAction(&OrderAction,g_nOrdLocalID++);
	
	return ;
	//-----------------------系统报单号撤单----------------------------//

 
	//-----------------------本地报单号撤单----------------------------//
    /*
         int LocalID;
         CDINGFtdcOrderActionField OrderAction;
         memset(&OrderAction,0,sizeof(CDINGFtdcOrderActionField));
         if (g_puserapi==NULL)
         {
                   printf("StartOrderAction  USERAPI未创建");
                   return ;
         }
         strcpy(OrderAction.ExchangeID,"CFFEX");
         strcpy(OrderAction.BrokerID,g_BrokerID);
         strcpy(OrderAction.UserID,g_UserID);
		 
		 printf("input InvestorID\n");
		 scanf("%s",(OrderAction.InvestorID));
		 printf("InvestorID=[%s]\n",OrderAction.InvestorID);
         
         printf("请输入本地报单号:");
         scanf("%d",&LocalID);
         sprintf(OrderAction.UserOrderLocalID,"%012d",LocalID);

         printf("撤销本地报单号[%s]\n",OrderAction.UserOrderLocalID );
         strcpy(OrderAction.OrderSysID,"");
         
         OrderAction.ActionFlag=DING_FTDC_AF_Delete;
         sprintf(OrderAction.UserOrderActionLocalID,"%012d",g_nOrdLocalID++);
         
         g_puserapi->ReqOrderAction(&OrderAction,g_nOrdLocalID++);
         
         return ;
    */
	//-----------------------本地报单号撤单----------------------------//

}

void StartQueryExchange()
{
	CDINGFtdcQryExchangeField QryExchange;
	if (g_puserapi==NULL)
	{
		printf("StartQueryExchange  USERAPI未创建");
		return ;
	}

	strcpy(QryExchange.ExchangeID,"CFFEX");
	g_puserapi->ReqQryExchange(&QryExchange,g_nOrdLocalID++);
	return ;
}

void StartQryTrade()
{
	CDINGFtdcQryTradeField QryTrade;
	memset(&QryTrade,0,sizeof(CDINGFtdcQryTradeField));
	if (g_puserapi==NULL)
	{
		printf("StartQryTrade  USERAPI未创建");
		return ;

	}
	strcpy(QryTrade.ExchangeID,"CFFEX");
	strcpy(QryTrade.BrokerID,g_BrokerID);
	strcpy(QryTrade.UserID,g_UserID);
	//strcpy(QryTrade.TradeID,"");
	g_puserapi->ReqQryTrade(&QryTrade,g_nOrdLocalID++);
	return ;
}
void StartQryOrder()
{
	CDINGFtdcQryOrderField QryOrder;
	memset(&QryOrder,0,sizeof(CDINGFtdcQryOrderField));
	if (g_puserapi==NULL)
	{
		printf("StartQryOrder  USERAPI未创建");
		return ;

	}
	strcpy(QryOrder.ExchangeID,"CFFEX");
	strcpy(QryOrder.BrokerID,g_BrokerID);
	strcpy(QryOrder.UserID,g_UserID);
	//strcpy(QryOrder.OrderSysID,"");
	g_puserapi->ReqQryOrder(&QryOrder,g_nOrdLocalID++);
	return ;
}

void StartQryInvestorAccount()
{
	CDINGFtdcQryInvestorAccountField QryInvestorAcc;
	if (g_puserapi==NULL)
	{
		printf("StartQryInvestorAccount  USERAPI未创建");
		return ;

	}
	memset(&QryInvestorAcc,0,sizeof(CDINGFtdcQryInvestorAccountField));
	strcpy(QryInvestorAcc.BrokerID,g_BrokerID);
//	strcpy(QryInvestorAcc.InvestorID,"8");
	g_puserapi->ReqQryInvestorAccount(&QryInvestorAcc,g_nOrdLocalID++);

	return ;
}

void StartQryUserInvestor()
{
	CDINGFtdcQryUserInvestorField QryUserInvestor;
	if (g_puserapi==NULL)
	{
		printf("StartQryInvestorAccount  USERAPI未创建");
		return ;

	}
	memset(&QryUserInvestor,0,sizeof(CDINGFtdcQryUserInvestorField));
	strcpy(QryUserInvestor.BrokerID,g_BrokerID);
	strcpy(QryUserInvestor.UserID,g_UserID);
	g_puserapi->ReqQryUserInvestor(&QryUserInvestor,g_nOrdLocalID++);
	return ;
}

void StartQryInstrument()
{
	CDINGFtdcQryInstrumentField QryInstrument;
	if (g_puserapi==NULL)
	{
		printf("StartQryInvestorAccount  USERAPI未创建");
		return ;

	}
	memset(&QryInstrument,0,sizeof(CDINGFtdcQryInstrumentField));
	strcpy(QryInstrument.ExchangeID,"CFFEX");
	//strcpy(QryInstrument.InstrumentID,"IF1206");
	g_puserapi->ReqQryInstrument(&QryInstrument,g_nOrdLocalID++);
	return ;
}

void StartQryTradingCode()
{
	CDINGFtdcQryTradingCodeField QryTradingCode;
	if (g_puserapi==NULL)
	{
		printf("StartQryInvestorAccount  USERAPI未创建");
		return ;

	}
	strcpy(QryTradingCode.ExchangeID,"CFFEX");
	strcpy(QryTradingCode.BrokerID,g_BrokerID);
	strcpy(QryTradingCode.UserID,g_UserID);
	//strcpy(QryTradingCode.InvestorID,"1");
	g_puserapi->ReqQryTradingCode(&QryTradingCode,g_nOrdLocalID++);
	return ;
}

void StartQryInvestorPosition()
{
	CDINGFtdcQryInvestorPositionField QryInvestorPosition;
	memset(&QryInvestorPosition,0,sizeof(CDINGFtdcQryInvestorPositionField));
	if (g_puserapi==NULL)
	{
		printf("StartQryInvestorPosition  USERAPI未创建");
		return ;
	}
	strcpy(QryInvestorPosition.ExchangeID,"CFFEX");
	strcpy(QryInvestorPosition.BrokerID,g_BrokerID);
	printf("请投资者编号：");
	scanf("%s",(QryInvestorPosition.InvestorID));
//	strcpy(QryInvestorPosition.InstrumentID,"");
	g_puserapi->ReqQryInvestorPosition(&QryInvestorPosition,g_nOrdLocalID++);
	return;
}

void StartUpdatePassword()
{
	CDINGFtdcUserPasswordUpdateField PasswordUpd;
	memset(&PasswordUpd,0,sizeof(CDINGFtdcUserPasswordUpdateField));
	if (g_puserapi==NULL)
	{
		printf("StartQryInvestorPosition  USERAPI未创建");
		return ;
	}
	strcpy(PasswordUpd.BrokerID,g_BrokerID);
	strcpy(PasswordUpd.UserID,g_UserID);
	printf("请输入密码：");
	scanf("%s",(PasswordUpd.OldPassword));
	printf("请输入新密码：");
	scanf("%s",(PasswordUpd.NewPassword));
	g_puserapi->ReqUserPasswordUpdate(&PasswordUpd,g_nOrdLocalID++);
	return;
}

void StartLogOut()
{
	CDINGFtdcReqUserLogoutField UserLogOut;
	memset(&UserLogOut,0,sizeof(CDINGFtdcReqUserLogoutField));
	strcpy(UserLogOut.BrokerID,g_BrokerID);
	strcpy(UserLogOut.UserID,g_UserID);
	
	g_puserapi->ReqUserLogout(&UserLogOut,g_nOrdLocalID++);
}

void StartQueryFee()
{
	printf("在查询手续费率\n");
	CDINGFtdcQryInvestorFeeField InvestorFee;
	memset(&InvestorFee,0,sizeof(CDINGFtdcQryInvestorFeeField));
	strcpy(InvestorFee.BrokerID,g_BrokerID);
	strcpy(InvestorFee.UserID,g_UserID);
	g_puserapi->ReqQryInvestorFee(&InvestorFee,g_nOrdLocalID++);
}

void StartQueryMargin()
{
	printf("在查询保证金率\n");
	CDINGFtdcQryInvestorMarginField InvestorMargin;
	memset(&InvestorMargin,0,sizeof(CDINGFtdcQryInvestorMarginField));
	strcpy(InvestorMargin.BrokerID,g_BrokerID);
	strcpy(InvestorMargin.UserID,g_UserID);
	g_puserapi->ReqQryInvestorMargin(&InvestorMargin,g_nOrdLocalID++);
}

void StartQueryComplianceParam()
{
	printf("在查询合规参数\n");
	CDINGFtdcQryComplianceParamField ComplianceParam;
	memset(&ComplianceParam,0,sizeof(CDINGFtdcQryComplianceParamField));
	strcpy(ComplianceParam.BrokerID,g_BrokerID);
	strcpy(ComplianceParam.UserID,g_UserID);
	printf("请投资者编号：");
	scanf("%s",(ComplianceParam.InvestorID));
	g_puserapi->ReqQryComplianceParam(&ComplianceParam,g_nOrdLocalID++);
}

#ifdef WIN32
int WINAPI OrderFunc(LPVOID pParam)
#else

void * OrderFunc(void *pParam)
#endif
{
	while(1){
		ShowManu();
	 	printf("g_choose=[%d]\n",g_choose);
	 	switch(g_choose)
	 	{
	 		case 0:
	 			StartLogOut();
	 			exit(0);
	 			
	 		case 1:
	 			StartInputOrder ();
	 			
	 			break;
	 		case 2:
	 			StartOrderAction();
	 			break;
	 		case 3:
	 			StartQryOrder();
	 			break;
	 		case 4:
	 			StartQryTrade();
	 			break;
	 		case 5:
	 			StartQryInvestorAccount();
	 			break;
	 		case 6:
	 			StartQryTradingCode();
	 			break;
	 		case 7:
	 			StartQueryExchange();
	 			break;
	 		case 8:
	 			StartQryInstrument();
	 			break;
	 		case 9:
	 			StartQryUserInvestor();
	 			break;
	 		case 10:
	 			StartQryInvestorPosition();
	 			break;
	 		case 11:
	 			StartUpdatePassword();
	 			break;
			case 12:
				StartQueryFee();
				break;
			case 13:
				StartQueryMargin();
				break;
			case 14:
				StartQueryComplianceParam();
				break;
	 		default:
	 			printf("Input Error\n");
	 			break;	
	 		
	 		
	 		
	 	}
#ifdef WIN32
		Sleep(200);
#else	 
		usleep(200 * 1000);
#endif
	}
	return 0;
}
bool StartAutoOrder()
{	
	//int dwIDThread;
	unsigned long dwIDThread;
	THREAD_HANDLE hThread;	/**< 线程句柄 */
	bool ret = true;
#ifdef WIN32
	hThread = ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)OrderFunc,NULL,0,&dwIDThread);
	if(hThread==NULL)
	{
		ret = false;
	}
	SetThreadPriority(hThread,THREAD_PRIORITY_TIME_CRITICAL);
	ResumeThread(hThread);
#else
	ret = (::pthread_create(&hThread,NULL,&OrderFunc , NULL) == 0);
	
#endif
	return ret;
}

void StartTest()
{
	switch(g_nSwitch){
		case 1:
		case 2:
		case 3:
		case 4:
			StartAutoOrder();
			break;
		case 5:
			printf("StartInputOrder \n");
			StartInputOrder();
			break;
		case 6:
			printf("StartOrderAction \n");
			StartOrderAction();
			break;
		case 7:
			printf("StartQueryExchange \n");
			StartQueryExchange();
			break;
		case 8:
			printf("StartQryInvestorAccount \n");
			StartQryInvestorAccount();
			break;
		case 9:
			printf("StartQryUserInvestor \n");
			StartQryUserInvestor();
			break;
		case 10:
			printf("StartQryInstrument \n");
			StartQryInstrument();
			break;
		case 11:
			printf("StartQryTradingCode \n");
			StartQryTradingCode();
			break;
		default :
			printf("Input arg Error\n");
			break;
	}

	return ;
}

#ifndef WIN32
void Sleep( int x )
{
	usleep(x*1000);
}
#endif



void Car::waxed()
{
	Guard<Mutex> g(lock);
	waxOn = true; // Ready to buff
	condition.signal();
}

void Car::buffed()
{
	Guard<Mutex> g(lock);
	waxOn = false; // Ready for another coat of wax
	//weaken 
	condition.signal();
}

void Car::waitForState( unsigned long timeout,int sv )
{
	Guard<Mutex> g(lock);
	targetStatus = sv;
	bool tO=true;
	while(tO && stateValue != targetStatus)
	{
		tO = condition.wait(timeout);//ms
		printf("xxxxx");
	}
	printf("xxxxxy");
	    
}

void Car::waitForWaxing()
{
	Guard<Mutex> g(lock);
	while(waxOn == false)
		condition.wait();
}

void Car::waitForBuffing()
{
	Guard<Mutex> g(lock);
	while(waxOn == true)
		condition.wait();
}

void Car::sendSignal( int sv )
{
	Guard<Mutex> g(lock);
	stateValue = sv;
	//weaken 
	condition.signal();
}
