#define  CONFIG_CPP
#define  readconfig(x,y) cfg.GetConfigString(temp, string(y));strcpy(x,temp.c_str());LOG4CPLUS_DEBUG_FMT(log_1,"%s=[%s]\n",#x,x);
#include "config.h"
#include "calcstr.h"
#include <string.h>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/sleep.h>
#include <log4cplus/loggingmacros.h>
#include "webserver/webserverInstance.h"

using namespace std;

extern log4cplus::Logger log_1;

HANDLE g_hEvent;
// 前置地址
char mdFront []   = "tcp://222.66.235.70:61213";
char tradeFront[100];

char tradeFront2[100];

char instIdList[1024 * 128] = "IF1412";
char testAtkInst[1024] = "IF1412";
char appId[11];		// 应用单元
char userId[13];		// 投资者代码
char investorId[13];		// 投资者代码
char passwd[13];

char appId2[11];		// 应用单元
char userId2[13];		// 投资者代码
char passwd2[13];

char attackTime[512];
char testAttackTime[512];
char testAttackTime2[512];
char rollInstrumentID[512] = {0};			// 滚单列表
char webport[10];
int  onRtnOrderLogFlag = 0;//不主动处理回报
// 会话参数
int	 frontId;	//前置编号
int	 sessionId;	//会话编号
double riskrate = 0;
double riskrate2 = 0;
char orderRef[13];
int attackvol = 1;
int atkexchange = 0;
int atkmode = 0;

std::map<std::string, std::string> common_params_maps;
char testAtkInstlist[20][20] = {0};
int functionNO = 0;
int trademiniFlag = 1;
int zce_ms_859_855 = -1;
int zce_ws_900_859 = -1000;
int zce_ws_900_855 = -1;//++++
int shfe_ms_859_855 = -1;
int shfe_ws_900_859 = -1000;
int shfe_ws_900_855 = -1;//++++
int dce_ms_859_855 = -1;
int dce_ws_900_859 = -1000;
int dce_ws_900_855 = -1;//++++
int cffex_ms_859_855 = -1;
int cffex_ws_900_859 = -1000;
int cffex_ws_900_855 = -1;//++++

double	jiangebase854ms=-1;
double	jiangebase858ms=-1;
double	jiangebase900ms=-1;

CDINGFtdcRspInvestorAccountField *pTdaccount;
//std::map<std::string, CThostFtdcDepthMarketDataField> map;
int requestId = 0;
//CtpTraderMiniSpi* orderMini=NULL;

std::map<std::string, CDINGFtdcDepthMarketDataField> mapMd; 
vector<CDINGFtdcOrderField*> orderList;
vector<CDINGFtdcRspInstrumentField> instrumentList;

vector<CDINGFtdcRspInvestorPositionField> postionList;


int configFromIniFile() 
{
	ConfigParser cfg;
	cfg.Load("config.ini");
	string test1;
	int u = cfg.GetFileSize();
	string temp;
	readconfig(mdFront, "FRONT_MdADDR");
	readconfig(tradeFront, "FRONT_TraderADDR");

	readconfig(tradeFront2, "FRONT_TraderADDR2");

	readconfig(appId, "BROKER_ID");
	readconfig(userId, "INVESTOR_ID");
	readconfig(investorId, "INVESTOR_ID_FEMAS");
	readconfig(passwd, "PASSWORD");

	readconfig(appId2, "BROKER_ID2");
	readconfig(userId2, "INVESTOR_ID2");
	readconfig(passwd2, "PASSWORD2");

	readconfig(testAttackTime, "TESTATTACKTIME");
	readconfig(testAttackTime2, "TESTATTACKTIME2");
	readconfig(attackTime, "ACTATTACKTIME");				
	readconfig(instIdList, "ppInstrumentID");
	readconfig(testAtkInst, "MD_TESTINSTRUMENT");
	char str_zce_ms_859_855[20];
	char str_zce_ws_900_859[20];
	char str_zce_ws_900_855[20];
	char str_shfe_ms_859_855[20];
	char str_shfe_ws_900_859[20];
	char str_shfe_ws_900_855[20];
	char str_dce_ms_859_855[20];
	char str_dce_ws_900_859[20];
	char str_dce_ws_900_855[20];
	char str_cffex_ms_859_855[20];
	char str_cffex_ws_900_859[20];
	char str_cffex_ws_900_855[20];

	char str_jiangebase854ms[256];
	char str_jiangebase858ms[256];
	char str_jiangebase900ms[256];

	char str_attackvol[20];
	char str_trademini[20];
	char str_atkexchange[20];
	readconfig(str_atkexchange, "defaultexchange");
	atkexchange = atoi(str_atkexchange);
	char str_atkmode[20];
	readconfig(str_atkmode, "atkmode");
	atkmode = atoi(str_atkmode);

	readconfig(str_zce_ms_859_855, "zce_ms_859_855");
	readconfig(str_zce_ws_900_859, "zce_ws_900_859");
	readconfig(str_zce_ws_900_855, "zce_ws_900_855");//+++
	readconfig(str_shfe_ms_859_855, "shfe_ms_859_855");
	readconfig(str_shfe_ws_900_859, "shfe_ws_900_859");
	readconfig(str_shfe_ws_900_855, "shfe_ws_900_855");//+++
	readconfig(str_dce_ms_859_855, "dce_ms_859_855");
	readconfig(str_dce_ws_900_859, "dce_ws_900_859");
	readconfig(str_dce_ws_900_855, "dce_ws_900_855");//+++
	readconfig(str_cffex_ms_859_855, "cffex_ms_859_855");
	readconfig(str_cffex_ws_900_859, "cffex_ws_900_859");
	readconfig(str_cffex_ws_900_855, "cffex_ws_900_855");//+++

	readconfig(str_attackvol, "attackvol");
	readconfig(str_trademini, "TRADEMINI");

	trademiniFlag = atoi(str_trademini);

	zce_ms_859_855 = atoi(str_zce_ms_859_855);
	zce_ws_900_859 = atoi(str_zce_ws_900_859);
	zce_ws_900_855 = atoi(str_zce_ws_900_855);//+++++
	shfe_ms_859_855 = atoi(str_shfe_ms_859_855);
	shfe_ws_900_859 = atoi(str_shfe_ws_900_859);
	shfe_ws_900_855 = atoi(str_shfe_ws_900_855);//+++++
	dce_ms_859_855 = atoi(str_dce_ms_859_855);
	dce_ws_900_859 = atoi(str_dce_ws_900_859);
	dce_ws_900_855 = atoi(str_dce_ws_900_855);//+++++
	cffex_ms_859_855 = atoi(str_cffex_ms_859_855);
	cffex_ws_900_859 = atoi(str_cffex_ws_900_859);
	cffex_ws_900_855 = atoi(str_cffex_ws_900_855);//+++++
	attackvol = atoi(str_attackvol);

	readconfig(str_jiangebase854ms,"jiangebase854ms");
	readconfig(str_jiangebase858ms,"jiangebase858ms");//+++
	readconfig(str_jiangebase900ms,"jiangebase900ms");

	jiangebase854ms = atof(str_jiangebase854ms);
	jiangebase858ms = atof(str_jiangebase858ms);
	jiangebase900ms = atof(str_jiangebase900ms);
	if (attackvol == 0)
	{
		attackvol = 1;
	}

	char *token = strtok(testAtkInst, ",");
	int i = 0;
	while (token != NULL) {
		strcpy(testAtkInstlist[i++], token);
		token = strtok(NULL, ",");
	}


	readconfig(rollInstrumentID, "rollInstrumentID")
		readconfig(webport, "WEB_PORT");
	if (strlen(webport) == 0)
	{
		strcpy(webport, "2223");
	}
	//readconfig(attackTime,"TIME");


	VS_StrTrim(testAttackTime);
	VS_StrTrim(testAttackTime2);
	VS_StrTrim(attackTime);
	sprintf(testAttackTime, "%d", (int)good(testAttackTime));	
	sprintf(testAttackTime2, "%d", (int)good(testAttackTime2));

	sprintf(attackTime, "%d", (int)good(attackTime));
	//LOG4CPLUS_DEBUG(log_1, "%s",passwd);

	return 1;
}







//字符串替换函数
/********************************************************************
*  Function：  my_strstr()
*  Description: 在一个字符串中查找一个子串;
*  Input：      ps: 源;      pd：子串
*  Return :    0：源字符串中没有子串; 1：源字符串中有子串;
*********************************************************************/
char * my_strstr(char * ps, char *pd)
{
	char *pt = pd;
	int c = 0;
	while (*ps != '\0')
	{
		if (*ps == *pd)
		{
			while (*ps == *pd && *pd != '\0')
			{
				ps++;
				pd++;
				c++;
			}
		}
		else
		{
			ps++;
		}
		if (*pd == '\0')
		{
			return (ps - c);
		}
		c = 0;
		pd = pt;
	}
	return 0;
}


/********************************************************************
*  Function：  str_replace()
*  Description: 在一个字符串中查找一个子串，并且把所有符合的子串用
另一个替换字符串替换。
*  Input：      p_source:要查找的母字符串； p_seach要查找的子字符串;
p_repstr：替换的字符串;
*  Output：      p_result:存放结果;
*  Return :      返回替换成功的子串数量;
*  Others:      p_result要足够大的空间存放结果，所以输入参数都要以\0结束;
*********************************************************************/
int str_replace(char *p_result, char* p_source, char* p_seach, char *p_repstr)
{
	int c = 0;
	int repstr_leng = 0;
	int searchstr_leng = 0;

	char *p1;
	char *presult = p_result;
	char *psource = p_source;
	char *prep = p_repstr;
	char *pseach = p_seach;
	int nLen = 0;

	repstr_leng = strlen(prep);
	searchstr_leng = strlen(pseach);

	do { 
		p1 = my_strstr(psource, p_seach);

		if (p1 == 0)
		{
			strcpy(presult, psource);
			return c;
		}
		c++;  //匹配子串计数加1;
		printf("结果:%s\r\n", p_result);
		printf("源字符:%s\r\n", p_source);

		// 拷贝上一个替换点和下一个替换点中间的字符串
		nLen = p1 - psource;
		memcpy(presult, psource, nLen);

		// 拷贝需要替换的字符串
		memcpy(presult + nLen, p_repstr, repstr_leng);

		psource = p1 + searchstr_leng;
		presult = presult + nLen + repstr_leng;
	} while (p1);

	return c;
}
#define MAX 200
int main2test(void)
{
	int i = 0;
	char s[MAX] = {0};        //存放源字串
	char s1[MAX] = {0};        //存放子字串
	char s2[MAX] = {0};        //存放替换字串
	char result_a[2000] = {0};//存放替换结果;
	char *p, *ptm, *pr;
	puts("Please input the string for s:");
	scanf("%s", s);
	puts("Please input the string for s1:");
	scanf("%s", s1);
	puts("Please input the string for s2:");
	scanf("%s", s2);
	ptm = s;
	pr = result_a;
	i = str_replace(pr, ptm, s1, s2);
	printf("替换%d个子字符串;\r\n", i); 
	printf("替换后结果:%s\r\n", result_a);
	system("pause");
	return 0;
}

PublicFuncs pb;
vector<CDINGFtdcOrderFieldEX*> atkorderList;
vector<CDINGFtdcOrderFieldEX*> atkorderList0000;