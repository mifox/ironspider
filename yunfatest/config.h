#ifndef MD_CONFIG_H_
#define MD_CONFIG_H_


#define EVENT_TD_CONNECT 80001
#define EVENT_TD_LOGIN 80002
#define EVENT_TD_CONFIRM 80003
#define EVENT_TD_QRYINSTRUMENT 80004
#define EVENT_TD_LOGOUT 80005
#define EVENT_TD_REQQRYORDER 80006
#define EVENT_TD_WEBACCESS 80007

#define EVENT_MD_INIT 90000
#define EVENT_MD_CONNECT 90001
#define EVENT_MD_LOGIN 90002
#define EVENT_MD_SUBMD 90003
#define EVENT_MD_UNSUBMD 90004
#define EVENT_MD_DISCONNECT 90005

#define EVENT_BS_READORDER 70004

#define SubInstrumentNo 8000
#define InstrumentLineNo SubInstrumentNo*50

#define FUNCTION_CLOSETODAY 10001

#define MDLINENO 2000
#ifndef CONFIG_CPP
#define CONFIG_CPP

//请求编号
//extern int requestId;

// 前置地址
extern char mdFront [];
extern char tradeFront [];
extern char tradeFront2 [];
extern char instIdList [];
extern char testAtkInst [];
extern char webport [];

extern char rollInstrumentID [];


extern char appId [];		// 应用单元
extern char userId [];		// 投资者代码
extern char passwd [];

extern char appId2 [];		// 应用单元2
extern char userId2 [];		// 投资者代码2
extern char passwd2 [];		//

extern char attackTime [];
extern char testAttackTime [];
extern char testAttackTime2 [];

extern int requestId; 
extern int g_hEvent_flag;
extern int g_hEvent_flagforMD;
extern int functionNO;
extern int trademiniFlag;


// 会话参数
extern int	 frontId;	//前置编号
extern int	 sessionId;	//会话编号
extern char orderRef [];

extern double riskrate;
extern double riskrate2;

using namespace std;



extern char testAtkInstlist [][20];
extern int atkexchange;
extern int atkmode;

extern int zce_ms_859_855;
extern int zce_ws_900_859;
extern int zce_ws_900_855;
extern int shfe_ms_859_855;
extern int shfe_ws_900_859;
extern int shfe_ws_900_855;
extern int dce_ms_859_855;
extern int dce_ws_900_859;
extern int dce_ws_900_855;
extern int cffex_ms_859_855;
extern int cffex_ws_900_859;
extern int cffex_ws_900_855;

extern int attackvol;


#endif

char * GetInitKey(char *filename, char *title, char *key);
int configFromIniFile();