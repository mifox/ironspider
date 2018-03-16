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



//2.地址信息
char g_frontaddr[BUFLEN];

//3.报单信息
int g_nOrdLocalID=0;

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
	if (autoreset==1)
	{
		stateValue=0;
		autoreset=0;
	}
	    
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

void Car::sendSignalAuto( int sv )
{
	autoreset=1;
	Guard<Mutex> g(lock);
	stateValue = sv;
	//weaken 
	condition.signal();
}

void Car::sendSignal( int sv )
{
	Guard<Mutex> g(lock);
	stateValue = sv;
	//weaken 
	condition.signal();
}
