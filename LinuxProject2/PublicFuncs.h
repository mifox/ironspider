// PublicFuncs.h: interface for the PublicFuncs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUBLICFUNCS_H__F565A242_2630_4908_B2B8_011FDAA45C89__INCLUDED_)
#define AFX_PUBLICFUNCS_H__F565A242_2630_4908_B2B8_011FDAA45C89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <memory.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <vector>
#include <queue>
#include <string>
#include <stack>


#include "myinclude/DINGFtdcTraderApi.h"
#include "myinclude/DINGFtdcMduserApi.h"
#include "myinclude/DINGFtdcUserApiStruct.h"
#include "myapi.h"
#ifdef WIN32
#include <windows.h>
typedef HANDLE THREAD_HANDLE ;
#else
#include <unistd.h>
#include <pthread.h>
#include <iconv.h>
typedef pthread_t THREAD_HANDLE ;
#endif
const int BUFLEN=512;
#include <myMdapi.h>
#include <myMdapi.h>

#ifndef WIN32
 
void Sleep(int x);;
#endif

#include "zthread/Thread.h"
#include "zthread/Mutex.h"
#include "zthread/Guard.h"
#include "zthread/Condition.h"
#include "zthread/ThreadedExecutor.h"
using namespace ZThread;
using namespace std;

class Car 
{
	//Ыјзг
	Mutex lock;
	// the base of trehad cooperation
	Condition condition;
	//flag for wax
	bool waxOn;
	int stateValue;
	int  targetStatus;
public:
	Car() : condition(lock), waxOn(false),stateValue(0),targetStatus(100) {}    //lock nad not wax
	//waxing 
	void waxed();
	//liggting
	void buffed();
	void waitForState(unsigned long timeout,int sv);
	void sendSignal(int sv);
	void waitForWaxing();
	void waitForBuffing();
};


class PublicFuncs  
{
public:
	vector<CountedPtr<Car>> carlist;
	PublicFuncs();
	int SendCommonSignal(int sv);
	virtual ~PublicFuncs();
};

class classMapper
{
public:
	static CDINGFtdcOrderField toCDINGFtdcOrderField(CDINGFtdcInputOrderField& a); 
	static CDINGFtdcOrderFieldEX toCDINGFtdcOrderFieldEX(CDINGFtdcInputOrderField& a); 
	static CDINGFtdcOrderFieldEX* toCDINGFtdcOrderFieldEX2(CDINGFtdcInputOrderField& a); 
	static CDINGFtdcOrderFieldEX toCDINGFtdcOrderFieldEX(CDINGFtdcOrderField& a);
};

extern TDINGFtdcBrokerIDType g_BrokerID;
extern TDINGFtdcUserIDType	g_UserID;
extern TDINGFtdcPasswordType	g_Password;
extern char g_frontaddr[BUFLEN];
extern int g_nOrdLocalID;
extern char* g_pProductInfo;
extern CMyTraderApi * g_puserapi;
extern CMyMdApi * g_puserMdapi;
extern bool StartAutoOrder();
extern void StartTest();

int code_convert(char *from_charset, char *to_charset, char *inbuf, int inlen, char *outbuf, int outlen);

int u2g(char *inbuf, int inlen, char *outbuf, int outlen);

int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

char* g2uc(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

char* u2gc(char *inbuf, int inlen, char *outbuf, int outlen);

unsigned long long GetCurrentTimeMsec();





#endif // !defined(AFX_PUBLICFUNCS_H__F565A242_2630_4908_B2B8_011FDAA45C89__INCLUDED_)
