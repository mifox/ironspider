#include "atk.h"
#include <iostream>
#include "AtkTraderSpi.h"
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/sleep.h>
#include <log4cplus/loggingmacros.h>
#include "Platform.h"
#include "calcstr.h"
#include "MdSpi.h"


tm * getTimeInfo()
{
	time_t rawtime;
	struct tm * timeinfo;	
	time ( &rawtime );
	//rawtime+=timedif;
	timeinfo = localtime ( &rawtime );
	return timeinfo;
};

int  getTimeInfotimeT()
{
	time_t rawtime;
	struct tm * timeinfo;	
	time ( &rawtime );
	//rawtime+=timedif;
    timeinfo = localtime ( &rawtime );
	int ret=0;
	ret = timeinfo->tm_hour*3600+ timeinfo->tm_min*60+timeinfo->tm_sec;
	return ret;
};

int CtpAtkMd::getAtkInputOrder( char* s,CDINGFtdcInputOrderField& m )
{
	map<string, CDINGFtdcDepthMarketDataField>::iterator iter;
	iter = mapMd.find(string(s));
	memset(&m,0,sizeof(CDINGFtdcInputOrderField));
	if(iter != mapMd.end())
	{
		CDINGFtdcDepthMarketDataField n;
		std::cout<<"value"<<((CDINGFtdcDepthMarketDataField)(iter->second)).InstrumentID<<endl;
		memcpy(&n,&iter->second,sizeof(CDINGFtdcDepthMarketDataField));
		strcpy(m.InstrumentID,n.InstrumentID);
		if (1)
		{
			m.LimitPrice=n.LowerLimitPrice;
			m.Direction=DING_FTDC_D_Buy;
			m.Volume=1;
			m.OffsetFlag='0';//m.CombOffsetFlag[1]='0';
		}
		else
		{
			m.LimitPrice=n.HighestPrice;
			m.Direction=DING_FTDC_D_Sell;
			m.Volume=1;
			m.OffsetFlag='0';//m.CombOffsetFlag[1]='1';

		}

	}else
	{
		std::cout<<"no found"<<endl;
	}
	return 0;
}

#include <iostream>
#include <string>

struct TimeCalc {  
	int detaSecondsFromR2L;  
	int Milliseconds;  
}; 

struct Tmm {  
	long seconds;  
	long wMilliseconds;  
}; 

int nRequestID=0;

bool SortByMilliseconds( const TimeCalc &v1, const TimeCalc &v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致  
{  
	return v1.Milliseconds < v2.Milliseconds;//升序排列  
} 


double getdff()
{
#ifdef WIN32
	LARGE_INTEGER  large_interger;
	QueryPerformanceFrequency(&large_interger);
	return (double)large_interger.QuadPart;
#else
	return 1000000;
#endif
}

__int64 getQuadPart()
{
	__int64 ret=0;
#ifdef WIN32
	LARGE_INTEGER  large_interger;
	QueryPerformanceCounter(&large_interger);
	ret = large_interger.QuadPart;
#else
	struct  timeval    tv;

	struct  timezone   tz;

	gettimeofday(&tv,&tz);
	ret = tv.tv_sec*1000000+tv.tv_usec;
	//printf("tv_sec:%d\n",tv.tv_sec);
	//printf("tv_usec:%d\n",tv.tv_usec);
#endif
	return ret;
	//throw std::exception("The method or operation is not implemented.");
}

CtpAtkMd* CtpAtkMd::CreateAtkApi()
{
	CountedPtr<Car> car(new Car);
	pb.carlist.push_back(car);
	onRtnOrderLogFlag = 1;//需要主动处理回报
	configFromIniFile();


	CMyMdApi *pMd = CMyMdApi::CreateFtdcMdApi("");	

	g_puserMdapi=pMd;
	CMdSpi spiMd((CMyMdApi *)pMd);
 	pMd->RegisterFront(mdFront);
	CMyMdSpi* pMdSpi = new CMyMdSpi((CDINGFtdcMduserSpi*)&spiMd);
 	pMd->RegisterSpi(pMdSpi);
	pMd->Init();	
	//pMd->Join();

	CMyTraderApi *pTrader = CMyTraderApi::CreateFtdcTraderApi("");	
	AtkCTraderSpi spi((CMyTraderApi *)pTrader);
	spi.setCar(car);
	CMyTraderSpi* pTdSpi = new CMyTraderSpi((CDINGFtdcTraderSpi*)&spi);
	pTrader->RegisterFront(tradeFront);	
	pTrader->SubscribePrivateTopic(DING_TERT_QUICK);
	pTrader->SubscribePublicTopic(DING_TERT_QUICK);
	pTrader->RegisterSpi(pTdSpi);
	
	//car->waitForState(10000,EVENT_TD_CONNECT);

	vector<tm> localstart,localend; //标准
	vector<Tmm> localstartTmm,localendTmm;//系统毫秒
	//vector<SYSTEMTIME> tmSysB,tmSysC;
	vector<tm> tmCZCE;
	vector<tm> tmCFFEX;
	vector<tm> tmDCE;
	vector<tm> tmSHFE;

	vector<TimeCalc> vTimeCalc;
	tm a,b;
	int loginedTimes;
	int iClockUpdateWithSHFE=1;
	Tmm sys;
	TimeCalc resultTimeCalc={0};
	time_t timep;
	pTrader->Init();
	CDINGFtdcInputOrderField attackReq={0};
	//----1.0----登录计时
	for (int i=1;i<25;i++)
	{
		loginedTimes=i;
		//pTdSpi->ReqUserLogin(appId,userId,passwd);
		//myWait(EVENT_TD_CONNECT);
		//
		car->waitForState(10000,EVENT_TD_CONNECT);
		Sleep(200);
		
		time(&timep); /*获取time_t类型的当前时间*/
		memcpy(&b,getTimeInfo(),sizeof(tm));
		localstart.push_back(b);//登陆前

		//GetLocalTime(&sys);
		sys.seconds = getTimeInfotimeT();
		sys.wMilliseconds = GetCurrentTimeMsec();
		localstartTmm.push_back(sys);

		CDINGFtdcReqUserLoginField reqUserLogin={0};
		strcpy(reqUserLogin.BrokerID,appId);
		strcpy(reqUserLogin.UserID	,userId);
		strcpy(reqUserLogin.Password,passwd);
		int ret=pTrader->ReqUserLogin(&reqUserLogin,nRequestID++);
		printf("------[ret]-----%d\n",ret);
		if (ret!=0) //返回值=0,成功
		{
			continue;
		}
		car->waitForState(10000 ,EVENT_TD_LOGIN);
		time(&timep); /*获取time_t类型的当前时间*/
		memcpy(&b,getTimeInfo(),sizeof(tm));
		localend.push_back(b);//登陆后

		//GetLocalTime(&sys);
		sys.seconds = getTimeInfotimeT();
		sys.wMilliseconds = GetCurrentTimeMsec();
		localendTmm.push_back(sys);

		
		LOG4CPLUS_DEBUG(log_1, "logsuccess");
		
		CDINGFtdcRspUserLoginField* pRspUserLogin = &(spi.rspUserLogin);

		if (strncmp(pRspUserLogin->SHFETime,"--:--:--",8))
		{
			memcpy(&a,getTimeInfo(),sizeof(tm));
			sscanf(pRspUserLogin->SHFETime,"%d:%d:%d",&a.tm_hour,&a.tm_min,&a.tm_sec);
			tmSHFE.push_back(a);
			//memcpy(&b,getTimeInfo(),sizeof(tm));
		}

		if (strncmp(pRspUserLogin->DCETime,"--:--:--",8))
		{
			memcpy(&a,getTimeInfo(),sizeof(tm));
			sscanf(pRspUserLogin->DCETime,"%d:%d:%d",&a.tm_hour,&a.tm_min,&a.tm_sec);
			tmDCE.push_back(a);
			//memcpy(&b,getTimeInfo(),sizeof(tm));
		}

		if (strncmp(pRspUserLogin->CZCETime,"--:--:--",8))
		{
			memcpy(&a,getTimeInfo(),sizeof(tm));	
			sscanf(pRspUserLogin->CZCETime,"%d:%d:%d",&a.tm_hour,&a.tm_min,&a.tm_sec);
			tmCZCE.push_back(a);
			//memcpy(&b,getTimeInfo(),sizeof(tm));
		}

		if (strncmp(pRspUserLogin->FFEXTime,"--:--:--",8))
		{
			memcpy(&a,getTimeInfo(),sizeof(tm));	
			sscanf(pRspUserLogin->FFEXTime,"%d:%d:%d",&a.tm_hour,&a.tm_min,&a.tm_sec);
			tmCFFEX.push_back(a);
			//memcpy(&b,getTimeInfo(),sizeof(tm));
		}

		long long myMillisecondsnow = sys.seconds*1000;
		long long targetAcc = ((8*60+50)*60)*1000;
		printf("%d\n",myMillisecondsnow);
		int logginWaitTime=20333;
		if (myMillisecondsnow > targetAcc)
		{
			logginWaitTime =1333;
		}
		//Sleep(logginWaitTime);

		car->waitForState(logginWaitTime,EVENT_TD_WEBACCESS);
		
		//map<string,string>::iterator it;
		map<string,string>::iterator it=common_params_maps.find("instrument1");
		if (it!=common_params_maps.end())
		{
			strcpy(attackReq.InstrumentID,common_params_maps["instrument1"].data());
		}

		it=common_params_maps.find("direction1");
		if (it!=common_params_maps.end())
		{
			attackReq.Direction=common_params_maps["direction1"].data()[0];
		}

		it=common_params_maps.find("price1");
		if (it!=common_params_maps.end())
		{
			attackReq.LimitPrice=atof((common_params_maps["price1"]).data());
		}

		it=common_params_maps.find("vol1");
		if (it!=common_params_maps.end())
		{
			attackReq.Volume=atof((common_params_maps["vol1"]).data());
			attackvol=atof((common_params_maps["vol1"]).data());
		}
		double closeprice=0;
		it=common_params_maps.find("closeprice1");
		if (it!=common_params_maps.end())
		{
			closeprice=atof((common_params_maps["closeprice1"]).data());

		}
		LOG4CPLUS_DEBUG(log_1,"closeprice1"<<closeprice<<endl);
		if (attackReq.Direction == DING_FTDC_D_Buy && attackReq.LimitPrice < closeprice)
		{
			LOG4CPLUS_DEBUG(log_1,"check pass"<<closeprice<<endl);
			printf("check pass\n");
			common_params_maps["closepricecheck"]="check success!!!";
		}else if (attackReq.Direction == DING_FTDC_D_Sell && attackReq.LimitPrice > closeprice)
		{
			LOG4CPLUS_DEBUG(log_1,"check pass"<<closeprice<<endl);
			printf("check pass\n");
			common_params_maps["closepricecheck"]="check success!!!";
		}
		else
		{
			LOG4CPLUS_DEBUG(log_1,"check failed"<<closeprice<<endl);
			printf("check failed\n");
			common_params_maps["closepricecheck"]="check failure!!!";
		}
		car->waitForState(10000 ,EVENT_TD_LOGOUT);
		CDINGFtdcReqUserLogoutField reqlogout={0};
		strcpy(reqlogout.BrokerID,appId);
		strcpy(reqlogout.UserID	,userId);
		nRequestID++;
		ret=pTrader->ReqUserLogout(&reqlogout,nRequestID);

	}
	car->waitForState(10000,EVENT_TD_CONNECT);
	//---2.0---计算时间差异------
	for(unsigned int i=0; i<localstart.size(); i++){
		int localstart_i=mktime(&localstart[i]);
		int localend_i=mktime(&localend[i]);
		int remote_i;
		if (tmCZCE.size() > 1 && atkexchange==0)
		{
			remote_i=mktime(&tmCZCE[i]);
		}
		else if(tmDCE.size()>1 && atkexchange==2)
		{
			remote_i = mktime(&tmDCE[i]);
		}
		else
		{
			remote_i=mktime(&tmSHFE[i]);
		}

		int wMillisecondslocalstart_i=localstartTmm[i].wMilliseconds;
		int wMillisecondslocalendTmm_i=localendTmm[i].wMilliseconds;
		int detaSecondsFromR2L=localstart_i-remote_i;

		TimeCalc tmepTimeCalc;
		tmepTimeCalc.detaSecondsFromR2L=detaSecondsFromR2L;
		tmepTimeCalc.Milliseconds=wMillisecondslocalstart_i;

		vTimeCalc.push_back(tmepTimeCalc);		
		printf("%d  %d %d =%d\n",localstart_i,remote_i,detaSecondsFromR2L,wMillisecondslocalstart_i);

	}
	int j=1;
	std::sort(vTimeCalc.begin(),vTimeCalc.end(),SortByMilliseconds); 

	resultTimeCalc=vTimeCalc[0];
	for(unsigned int i=0; i<vTimeCalc.size(); i++)
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"%d =%d\n",vTimeCalc[i].detaSecondsFromR2L,vTimeCalc[i].Milliseconds);
	}
	for(unsigned int i=0; i<vTimeCalc.size(); i++)
	{
		//printf("%d =%d\n",vTimeCalc[i].deta,vTimeCalc[i].Milliseconds);
		resultTimeCalc=vTimeCalc[i];
		if (i<vTimeCalc.size()-1)
		{
			if (vTimeCalc[i].detaSecondsFromR2L != vTimeCalc[i+1].detaSecondsFromR2L)
			{			
				break;
			}
		}

	}
	LOG4CPLUS_DEBUG_FMT(log_1,"[resultTimeCalc] %d = %d\n",resultTimeCalc.detaSecondsFromR2L,resultTimeCalc.Milliseconds);

	//------3-------登录 等待
	
	CDINGFtdcReqUserLoginField reqUserLogin={0};
	strcpy(reqUserLogin.BrokerID,appId);
	strcpy(reqUserLogin.UserID	,userId);
	strcpy(reqUserLogin.Password,passwd);
	int ret=pTrader->ReqUserLogin(&reqUserLogin,nRequestID++);
	printf("------[ret]-----%d\n",ret);
	if (ret!=0) //返回值=0,成功
	{
		LOG4CPLUS_DEBUG_FMT(log_1,"login successed 1");
	}
	car->waitForState(100000 ,EVENT_TD_LOGIN);

	CDINGFtdcSettlementInfoConfirmField settlementInfoConfirm={0};
	strcpy(settlementInfoConfirm.BrokerID,appId);
	strcpy(settlementInfoConfirm.InvestorID,userId);
	pTrader->ReqSettlementInfoConfirm(&settlementInfoConfirm, nRequestID++);

	//------4------数据准备
	int breakFlag=0;
	long timedeta855=0;
	long timedeta859=0;
	long targetSeconds=0;
	char exchangeM[10]="0";
	long myMilliseconds=1;

	attackReq.Volume = 1;
	attackReq.OffsetFlag='0';
	LOG4CPLUS_DEBUG_FMT(log_1,"Attack exchangeM %s",exchangeM);
	

	LOG4CPLUS_DEBUG_FMT(log_1,"%s ,%c ,%c ,%f ,%d\n",attackReq.InstrumentID,attackReq.Direction
		,attackReq.OffsetFlag,attackReq.LimitPrice,attackReq.Volume);
	TDINGFtdcInstrumentIDType instId={0};
	TDINGFtdcDirectionType dir;
	TDINGFtdcOffsetFlagType kpp='0'; 
	TDINGFtdcPriceType price=0.0;
	TDINGFtdcVolumeType vol=1;

	//----------精确记时--------
	
	double dff;
	__int64  c1, c2;

	dff = getdff();
	//QueryPerformanceCounter(&large_interger);
	//c1 = large_interger.QuadPart;
	static __int64 qpc855=0;
	static __int64 qpc855for900=0;//新增参数
	static __int64 qpc859=0;
	static __int64 diff_859_855=0;
	static __int64 diff_900_859=0;
	static __int64 diff_900_855=0;

	long testTime2_l=0;
	long attackTime_l=0;
	map<string,string>::iterator it;

	while(1)
		{
			if (breakFlag==1000)
			{
				LOG4CPLUS_DEBUG_FMT(log_1,"breakFlag==1000\n");
				break;
			}
			Sleep(3000);
			static int i=0;
			printf("cicle %d\n",i++);
			memcpy(&a,getTimeInfo(),sizeof(tm));
			printf("%d:%d:%d",a.tm_hour,a.tm_min,a.tm_sec);
			
			Tmm sys;
			//GetLocalTime( &sys );
			tm * t1 = getTimeInfo();
			sys.seconds = getTimeInfotimeT();
			sys.wMilliseconds = GetCurrentTimeMsec();
			printf("[sys.wMilliseconds1]:%d\n",sys.wMilliseconds);

			//printf( "M/%d/%d%d %d:%d:%d.%d 星期\n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,sys.wDayOfWeek);
			//printf( "%d/%d%d %d:%d:%d.%d ",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
			printf("[P]%d = %d\n",-resultTimeCalc.detaSecondsFromR2L,-resultTimeCalc.Milliseconds);
			myMilliseconds = (sys.seconds-resultTimeCalc.detaSecondsFromR2L)*1000+sys.wMilliseconds-resultTimeCalc.Milliseconds;
			printf("%d\n",myMilliseconds);



			map<string,string>::iterator it=common_params_maps.find("exchangeM");
			if (it!=common_params_maps.end())
			{
				strcpy(exchangeM,common_params_maps["exchangeM"].data());
				if (strcmp(exchangeM,"0") == 0)
				{
					atkexchange=0;
					LOG4CPLUS_DEBUG_FMT(log_1,"Attack exchangeM SHFE");
					LOG4CPLUS_DEBUG_FMT(log_1,"Attack exchangeM %s",exchangeM);

				}
				else if (strcmp(exchangeM,"1") == 0)
				{
					atkexchange=1;
					LOG4CPLUS_DEBUG_FMT(log_1,"Attack exchangeM CZCE");
					LOG4CPLUS_DEBUG_FMT(log_1,"Attack exchangeM %s",exchangeM);

				}
				else if (strcmp(exchangeM,"2") == 0)
				{
					atkexchange=2;
					LOG4CPLUS_DEBUG_FMT(log_1,"Attack exchangeM DCE");
					LOG4CPLUS_DEBUG_FMT(log_1,"Attack exchangeM %s",exchangeM);

				}
			}


			CDINGFtdcInputOrderField test_attackReq;
			int ret = CtpAtkMd::getAtkInputOrder(testAtkInstlist[atkexchange],test_attackReq); //IF1412
			if (fabs(test_attackReq.LimitPrice)<0.001 )
			{
				test_attackReq.LimitPrice =1;
			}

			CDINGFtdcInputOrderField attackReq={0};
			it=common_params_maps.find("instrument1");
			if (it!=common_params_maps.end())
			{
				strcpy(attackReq.InstrumentID,common_params_maps["instrument1"].data());
			}
			

			it=common_params_maps.find("direction1");
			if (it!=common_params_maps.end())
			{
				attackReq.Direction=common_params_maps["direction1"].data()[0];
			}

			it=common_params_maps.find("price1");
			if (it!=common_params_maps.end())
			{
				attackReq.LimitPrice=atof((common_params_maps["price1"]).data());
			}

			it=common_params_maps.find("vol1");
			if (it!=common_params_maps.end())
			{
				attackReq.Volume=atof((common_params_maps["vol1"]).data());
				attackvol=atof((common_params_maps["vol1"]).data());
			}
			double closeprice=0;
			it=common_params_maps.find("closeprice1");
			if (it!=common_params_maps.end())
			{
				closeprice=atof((common_params_maps["closeprice1"]).data());

			}
			else
			{
				memcpy(&attackReq,&test_attackReq,sizeof(CDINGFtdcInputOrderField));
				attackvol  = 1;
				closeprice = test_attackReq.LimitPrice+10;
				LOG4CPLUS_DEBUG(log_1,"No web input"<<endl);
			}

			LOG4CPLUS_DEBUG(log_1,"closeprice1"<<closeprice<<endl);
			if (attackReq.Direction == DING_FTDC_D_Buy && attackReq.LimitPrice < closeprice)
			{
				LOG4CPLUS_DEBUG(log_1,"check pass"<<closeprice<<endl);
				printf("check pass\n");
				common_params_maps["closepricecheck"]="check success!!!";
			}else if (attackReq.Direction == DING_FTDC_D_Sell && attackReq.LimitPrice > closeprice)
			{
				LOG4CPLUS_DEBUG(log_1,"check pass"<<closeprice<<endl);
				printf("check pass\n");
				common_params_maps["closepricecheck"]="check success!!!";
			}
			else
			{
				LOG4CPLUS_DEBUG(log_1,"check failed"<<closeprice<<endl);
				printf("check failed\n");
				common_params_maps["closepricecheck"]="check failure!!!";
			}
			//common_params_maps["closepricecheck"]="<a herf=setAttckInstrument.html>check</a>";

			attackReq.Volume = 1;
			attackReq.OffsetFlag='0';
			LOG4CPLUS_DEBUG_FMT(log_1,"Attack exchangeM %s",exchangeM);
			LOG4CPLUS_DEBUG_FMT(log_1,"%s ,%c ,%c ,%f ,%d\n",test_attackReq.InstrumentID,test_attackReq.Direction
				,test_attackReq.OffsetFlag,test_attackReq.LimitPrice,test_attackReq.Volume);

			LOG4CPLUS_DEBUG_FMT(log_1,"%s ,%c ,%c ,%f ,%d\n",attackReq.InstrumentID,attackReq.Direction
				,attackReq.OffsetFlag,attackReq.LimitPrice,attackReq.Volume);
			TDINGFtdcInstrumentIDType instId={0};
			TDINGFtdcDirectionType dir;
			TDINGFtdcOffsetFlagType kpp='0'; 
			TDINGFtdcPriceType price=0.0;
			TDINGFtdcVolumeType vol=1;
			//---------


			//LOG4CPLUS_DEBUG_FMT(log_1,"%s ,%c ,%c ,%f ,%d\n",instId,dir,kpp,price,vol);

			//map<string,string>::iterator it;
			it=common_params_maps.find("price1");
			if (it!=common_params_maps.end())
			{
				price=atof((common_params_maps["price1"]).data());
			}

			int debug=0;
			it=common_params_maps.find("debug");

			if (it!=common_params_maps.end())
			{
				debug=atoi((common_params_maps["debug"]).data());
			}

			char programchoose[20];
			it=common_params_maps.find("programchoose");

			if (it!=common_params_maps.end())
			{
				strcpy(programchoose,(common_params_maps["programchoose"]).data());
			}



			if (!strcmp(programchoose,"debug") && price>100)
			{
				CDINGFtdcInputOrderField req;
				memset(&req, 0, sizeof(req));	
				spi.ReqOrderInsertReady(instId,dir,kpp,price,vol,req);
	            int ret = pTrader->ReqOrderInsert(&req, nRequestID++);
				
				//pTrader->ReqOrderInsert()
			}

			long testTime_l;
			if (atkexchange == 0)//上海期货粗略记时
			{
				testTime_l=(long)(good(testAttackTime));
			}
			else if (atkexchange == 1)//郑州商品交易所
			{

				testTime_l=(long)(good(testAttackTime));
				LOG4CPLUS_DEBUG_FMT(log_1,"testTime_l=%d\n",testTime_l);
			}
			else if (atkexchange == 2)//大连商品交易所
			{

				testTime_l=(long)(good(testAttackTime));
				LOG4CPLUS_DEBUG_FMT(log_1,"testTime_l=%d\n",testTime_l);
			}
			testTime2_l=(long)(good(testAttackTime2));
			attackTime_l=(long)(good(attackTime));
			//测试攻击时间
			it=common_params_maps.find("TestTime");
			if (it!=common_params_maps.end())
			{
				long testTime_t = (long)good(const_cast<char*>(common_params_maps["TestTime"].data()));
				if (testTime_t>0)
				{
					testTime_l=testTime_t;
				}

			}

			it=common_params_maps.find("TestTime2");
			if (it!=common_params_maps.end())
			{
				long testTime_t = (long)good(const_cast<char*>(common_params_maps["TestTime2"].data()));
				if (testTime_t>0)
				{
					testTime2_l=testTime_t;
				}

			}

			it=common_params_maps.find("AttackTime");
			if (it!=common_params_maps.end())
			{
				long attackTime_t = (long)good(const_cast<char*>(common_params_maps["AttackTime"].data()));
				if (attackTime_t>0)
				{
					attackTime_l=attackTime_t;
				}

			}

			LOG4CPLUS_DEBUG_FMT(log_1,"\ntestTime %d testTime2 %d attackTime %d\n",testTime_l,testTime2_l,attackTime_l);

			if (atkmode != 2) //mode2只需要进攻
			{		
				//long targetSeconds =  ((0*60+0)*60+0)*1000; 
				//[s0]test attack time 
				LOG4CPLUS_DEBUG_FMT(log_1,"[s0]test attack time out\n");
				targetSeconds = testTime_l;


				if (atkexchange == 0) //上海期货精度计时
				{
					diff_859_855=dff*(4*60) + dff*shfe_ms_859_855/1000;
					diff_900_859=dff*(1*60) + dff*shfe_ws_900_859/1000000;
					diff_900_855=dff*(5*60) - dff*shfe_ws_900_855/1000000;//++++++
					LOG4CPLUS_DEBUG_FMT(log_1,"[dff]:%f\n",dff);
					LOG4CPLUS_DEBUG_FMT(log_1,"[diff_859_855]:%lld\n",diff_859_855);
					LOG4CPLUS_DEBUG_FMT(log_1,"[diff_900_859]:%lld\n",diff_900_859);
					LOG4CPLUS_DEBUG_FMT(log_1,"[diff_900_855]:%lld\n",diff_900_855);//++++
				}
				else if(atkexchange == 1)//郑州商品
				{
					diff_859_855=dff*(4*60) + dff*zce_ms_859_855/1000;
					diff_900_859=dff*(1*60) + dff*zce_ws_900_859/1000000;//-dff*1*98/100/1000;
					diff_900_855=dff*(5*60) + dff*zce_ws_900_855/1000000;
					LOG4CPLUS_DEBUG_FMT(log_1,"[zce_ms_859_855]:%d\n",zce_ms_859_855);
					LOG4CPLUS_DEBUG_FMT(log_1,"[zce_ms_900_859]:%d\n",zce_ws_900_859);
					LOG4CPLUS_DEBUG_FMT(log_1,"[zce_ws_900_855]:%d\n",zce_ws_900_855);//+++++
					LOG4CPLUS_DEBUG_FMT(log_1,"[dff]:%f\n",dff);
					LOG4CPLUS_DEBUG_FMT(log_1,"[diff_859_855]:%lld\n",diff_859_855);
					LOG4CPLUS_DEBUG_FMT(log_1,"[diff_900_859]:%lld\n",diff_900_859);
					LOG4CPLUS_DEBUG_FMT(log_1,"[diff_900_855]:%lld\n",diff_900_855);//++++
				}
				else if(atkexchange == 2)//大连商品
				{
					diff_859_855=dff*(4*60) + dff*dce_ms_859_855/1000;
					diff_900_859=dff*(1*60) + dff*dce_ws_900_859/1000000;
					diff_900_855=dff*(5*60) + dff*dce_ws_900_855/1000000;
					LOG4CPLUS_DEBUG_FMT(log_1,"[dce_ms_859_855]:%d\n",dce_ms_859_855);
					LOG4CPLUS_DEBUG_FMT(log_1,"[dce_ws_900_859]:%d\n",dce_ws_900_859);
					LOG4CPLUS_DEBUG_FMT(log_1,"[dce_ws_900_855]:%d\n",dce_ws_900_855);
					LOG4CPLUS_DEBUG_FMT(log_1,"[dff]:%f\n",dff);
					LOG4CPLUS_DEBUG_FMT(log_1,"[diff_859_855]:%lld\n",diff_859_855);
					LOG4CPLUS_DEBUG_FMT(log_1,"[diff_900_859]:%lld\n",diff_900_859);
					LOG4CPLUS_DEBUG_FMT(log_1,"[diff_900_855]:%lld\n",diff_900_855);//++++
				}

				int t= diff_900_859/dff;

				//LOG4CPLUS_DEBUG_FMT(log_1,"[diff_859_855]:%lld\n",diff_859_855);
				//LOG4CPLUS_DEBUG_FMT(log_1,"[diff_900_859]:%lld\n",diff_900_859);


				//----------精确记时----------
				LOG4CPLUS_DEBUG_FMT(log_1,"[s0]test attack time 　[myMilliseconds]:%ld | [targetSeconds]:%ld\n",myMilliseconds,targetSeconds);

				if (myMilliseconds > targetSeconds - 6000 && myMilliseconds < targetSeconds + 3000)
				{
					LOG4CPLUS_DEBUG_FMT(log_1,"[s0]test attack time in\n");
					strcpy(instId,test_attackReq.InstrumentID);
					dir=test_attackReq.Direction;
					kpp=test_attackReq.OffsetFlag;		
					price=test_attackReq.LimitPrice;		
					vol=test_attackReq.Volume;

					while(1)
					{
						if (breakFlag>=20)
						{
							if (onRtnOrderLogFlag == 0)
							{
								LOG4CPLUS_DEBUG_FMT(log_1,"breakFlag>=20\n");
								CDINGFtdcQryOrderField qryOrder={0};
								strcpy(qryOrder.BrokerID,appId);
								strcpy(qryOrder.UserID,userId);
								pTrader->ReqQryOrder(&qryOrder,nRequestID++);
							}
							LOG4CPLUS_DEBUG_FMT(log_1,"breakFlag=%d\n",breakFlag);
							break;
						}
						Tmm sys;
						//GetLocalTime( &sys );
						tm * t1 = getTimeInfo();
						sys.seconds = getTimeInfotimeT();
						sys.wMilliseconds = GetCurrentTimeMsec();
						printf("[sys.wMilliseconds1]:%d\n",sys.wMilliseconds);
						//printf( "%d/%d%d %d:%d:%d.%d ",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
						printf("[P]%d = %d\n",-resultTimeCalc.detaSecondsFromR2L,-resultTimeCalc.Milliseconds);
						LOG4CPLUS_DEBUG_FMT(log_1,"%d\n",myMilliseconds);
						myMilliseconds = (sys.seconds-resultTimeCalc.detaSecondsFromR2L)*1000+sys.wMilliseconds-resultTimeCalc.Milliseconds;
						LOG4CPLUS_DEBUG_FMT(log_1,"[myMilliseconds]:%d\n",myMilliseconds);
						LOG4CPLUS_DEBUG_FMT(log_1,"[myMilliseconds.wMilliseconds]:%d\n",sys.wMilliseconds);


						if (myMilliseconds > targetSeconds + 10000)
						{
							if (onRtnOrderLogFlag == 0)
							{
								CDINGFtdcQryOrderField qryOrder={0};
								strcpy(qryOrder.BrokerID,appId);
								strcpy(qryOrder.UserID,userId);
								pTrader->ReqQryOrder(&qryOrder,nRequestID++);
							}
							LOG4CPLUS_DEBUG_FMT(log_1,"breakFlag=%d\n",breakFlag);
							break;
						}

						if (myMilliseconds > targetSeconds)
						{
							for(int i=1;i<51;i++)
							{

								CDINGFtdcInputOrderField& req=*(new CDINGFtdcInputOrderField);
								memset(&req,0,sizeof(CDINGFtdcInputOrderField));
								spi.ReqOrderInsertReady(instId,dir,kpp,price,vol,req);
								//pTdSpi->pUserApi->ReqOrderInsert(req);

								CDINGFtdcOrderFieldEX* reqEx = classMapper::toCDINGFtdcOrderFieldEX2(req);

								reqEx->exmaps["test"]=(void *) new string("test");
								reqEx->exmaps["seq"]=(void *) new int(i); //顺序
								reqEx->main.OrderStatus = 'T';
// 								SYSTEMTIME* point_sys=(new SYSTEMTIME);
// 								GetLocalTime( point_sys );
								Tmm* point_sys=(new Tmm);
								tm * t1 = getTimeInfo();
								point_sys->seconds = getTimeInfotimeT();
								point_sys->wMilliseconds = GetCurrentTimeMsec();
								reqEx->exmaps["sys_time"]=(void *) point_sys; //sys_time


								//QueryPerformanceCounter(&large_interger);
								//c2 = large_interger.QuadPart;							
								__int64* qpc =new __int64;
								*qpc=getQuadPart();//large_interger.QuadPart;

								//LOG4CPLUS_DEBUG_FMT(log_1,"[sys_time_qpc_org] :%lld\n",*qpc);

								reqEx->exmaps["sys_time_qpc"]=(void *)qpc; //sys_time

								atkorderList.push_back(reqEx);
								pTrader->ReqOrderInsert(&req,nRequestID++);
								LOG4CPLUS_DEBUG_FMT(log_1,"atkorderList.size() %d\n",atkorderList.size());

								LOG4CPLUS_DEBUG_FMT(log_1,"insertOrder TESTORDER------------1\n");
								/*
								for(int j=1;j<540;j++)							   
								{
								printf("insertOrder------------2\n");
								}*/
								Sleep(32);
								//printf("insertOrder------------3\n");
								//printf("insertOrder------------4\n");
								//printf("insertOrder------------5\n");
								//printf("insertOrder------------6\n");
								//printf("insertOrder------------7\n");

								//printf("insertOrder------------8\n");
								//printf("insertOrder------------9\n");
								//printf("insertOrder------------1\n");
								//printf("insertOrder------------2\n");
								//printf("insertOrder------------3\n");
								//printf("insertOrder------------4\n");
								//printf("insertOrder------------5\n");
							}
							if (onRtnOrderLogFlag == 0)
							{
								CDINGFtdcQryOrderField qryOrder={0};
								strcpy(qryOrder.BrokerID,appId);
								strcpy(qryOrder.UserID,userId);
								LOG4CPLUS_DEBUG_FMT(log_1,"test ReqQryOrder",breakFlag);
								pTrader->ReqQryOrder(&qryOrder,nRequestID++);
							}
							breakFlag=20;
							LOG4CPLUS_DEBUG_FMT(log_1,"breakFlag=%d\n",breakFlag);
							break;
							
						}//if (myMilliseconds > targetSeconds) breakout
					}//while(1) 


				}//if (myMilliseconds > targetSeconds - 6000 && myMilliseconds < targetSeconds + 3000) 




				//map<string,string>::iterator it;


				for(it=common_params_maps.begin();it!= common_params_maps.end();++it)
				{			 
					cout<<"key: "<<it->first <<" value: "+it->second<<endl;
				}


				{
					bool founded=false;   
					int i=0;
					for(i=0; i<atkorderList.size(); i++){



						LOG4CPLUS_DEBUG_FMT(log_1,"Seq%d \n OrderRef %s\n OrderStatus %c\n",
							(int)atkorderList[i]->exmaps["seq"],
							atkorderList[i]->main.UserOrderLocalID,
							atkorderList[i]->main.OrderStatus
							);

						Tmm sys;
						sys.seconds = getTimeInfotimeT();
						sys.wMilliseconds = GetCurrentTimeMsec();
						memcpy(&sys,atkorderList[i]->exmaps["sys_time"],sizeof(Tmm));
						LOG4CPLUS_DEBUG_FMT(log_1,"[sys.wMilliseconds1]:%u.%u\n",sys.seconds,sys.wMilliseconds);
					}
				}
				int allReturnFlag=1;
				LOG4CPLUS_DEBUG_FMT(log_1,"breakFlag=%d\n",breakFlag);
				if(breakFlag==20)
					//Sleep(3000);
					//while(allReturnFlag)
				{
					for(i=0; i<atkorderList.size(); i++){					
						if (atkorderList[i]->main.OrderStatus == DING_FTDC_OS_NoTradeQueueing)
						{
							LOG4CPLUS_DEBUG_FMT(log_1,"IF OrderStatus [%c]\n",atkorderList[i]->main.OrderStatus);
						}		
						else
						{
							LOG4CPLUS_DEBUG_FMT(log_1,"ELSE OrderStatus [%c]\n",atkorderList[i]->main.OrderStatus);
						}
					}
					//Sleep(3000);
					//breakFlag=2;

				}
				allReturnFlag=1;
				LOG4CPLUS_DEBUG_FMT(log_1,"breakFlag=%d\n",breakFlag);
				if(breakFlag==20)
				{
					for(i=0; i<atkorderList.size(); i++){					
						if (atkorderList[i]->main.OrderStatus == DING_FTDC_OS_NoTradeQueueing )
						{
							LOG4CPLUS_DEBUG_FMT(log_1,"IF OrderStatus [%c]\n",atkorderList[i]->main.OrderStatus);
						}		
						else
						{
							LOG4CPLUS_DEBUG_FMT(log_1,"ELSE OrderStatus [%c]\n",atkorderList[i]->main.OrderStatus);
						}
					}
					//Sleep(3000);
					//breakFlag=2;

				}

				
				//[s1]cancel order time 
				LOG4CPLUS_DEBUG_FMT(log_1,"[s1]cancel order time out\n");
				targetSeconds = testTime_l + 60000;
				LOG4CPLUS_DEBUG_FMT(log_1,"[s1]cancel order time  [myMilliseconds]:%ld | [targetSeconds]:%ld\n",myMilliseconds,targetSeconds);
				if (myMilliseconds > targetSeconds - 4000 && myMilliseconds < targetSeconds + 4000)
				{	
					LOG4CPLUS_DEBUG_FMT(log_1,"[s1]cancel order time in\n");
					for(i=0; i<atkorderList.size()-1; i++){

						//计算最后一张无效效单时间

						if (i < atkorderList.size())
						{
							__int64 t1=*(__int64*)atkorderList[i]->exmaps["sys_time_qpc"];
							LOG4CPLUS_DEBUG_FMT(log_1,"[sys_time_qpc %d] :[t1]:%lld\n",i,t1);



							LOG4CPLUS_DEBUG_FMT(log_1,"[calc last no inserted order] :i < atkorderList.size()+1");
							if (atkorderList[i]->main.OrderStatus == DING_FTDC_OS_Canceled
								&& atkorderList[i+1]->main.OrderStatus == DING_FTDC_OS_NoTradeQueueing)
							{
								LOG4CPLUS_DEBUG_FMT(log_1,"OrderStatus : = %d;OrderStatus+1 : = %d",atkorderList[i]->main.OrderStatus,atkorderList[i+1]->main.OrderStatus);
								//----粗记时方法begin----
								Tmm sys;
								sys.seconds = getTimeInfotimeT();
								sys.wMilliseconds = GetCurrentTimeMsec();
								memcpy(&sys,atkorderList[i+1]->exmaps["sys_time"],sizeof(Tmm));
								LOG4CPLUS_DEBUG_FMT(log_1,"[855lastNoUseOrderwMilliseconds1]:%u.%u\n",sys.seconds,sys.wMilliseconds);
								long milliseconds855local = (sys.seconds)*1000+sys.wMilliseconds;
								LOG4CPLUS_DEBUG_FMT(log_1,"[milliseconds855local]:%d\n",milliseconds855local);
								long milliseconds855remote = (testTime_l+800)/1000*1000;
								LOG4CPLUS_DEBUG_FMT(log_1,"[milliseconds855remote]:%d\n",milliseconds855remote);
								timedeta855 = (milliseconds855local - milliseconds855remote);

								//----粗记时方法end----

								//----精确时方法begin----


								qpc855=*(__int64*)atkorderList[i+1]->exmaps["sys_time_qpc"];
								qpc855for900 = qpc855;
								LOG4CPLUS_DEBUG_FMT(log_1,"[qpc855]:%lld\n",qpc855);
								//---------专门为流控设计
								if (i<50)
								{
									//qpc855+=1*dff/1000;
									qpc855for900+=1*dff/1000;
								}else
								{
									//qpc855+=337*dff/1000;
									qpc855for900+=345*dff/1000;
								}

								LOG4CPLUS_DEBUG_FMT(log_1,"[qpc855]:%lld\n",qpc855);
								LOG4CPLUS_DEBUG_FMT(log_1,"[qpc855for900]:%lld\n",qpc855for900);
								//----精确时方法end----

							}
							//加上本地超时调整系数
						}


					}
					LOG4CPLUS_DEBUG_FMT(log_1,"[timedeta855]:%d\n",timedeta855);
					if (timedeta855==0) //全部一样的OrderStatus
					{
						if(atkorderList.size()>0)
							if (atkorderList[0]->main.OrderStatus == DING_FTDC_OS_Canceled)
							{
								Tmm sys;
								memcpy(&sys,atkorderList[atkorderList.size()-1]->exmaps["sys_time"],sizeof(Tmm));
								LOG4CPLUS_DEBUG_FMT(log_1,"[855lastNoUseOrderwMilliseconds1]:%u.%u\n",sys.seconds,sys.wMilliseconds);
								long milliseconds855local = (sys.seconds)*1000+sys.wMilliseconds;
								LOG4CPLUS_DEBUG_FMT(log_1,"[milliseconds855local]:%d\n",milliseconds855local);
								//long milliseconds855remote = ((8*60+55)*60+0)*1000+0;
								long milliseconds855remote =(testTime_l+800)/1000*1000;
								LOG4CPLUS_DEBUG_FMT(log_1,"[milliseconds855remote]:%d\n",milliseconds855remote);
								timedeta855 = (milliseconds855local - milliseconds855remote);


								//精确
								qpc855=*(__int64*)atkorderList[atkorderList.size()-1]->exmaps["sys_time_qpc"];
								qpc855for900 = qpc855;
								LOG4CPLUS_DEBUG_FMT(log_1,"[atkorderList.size()-1][qpc855]:%lld\n",qpc855);
								//qpc855+= 400*dff/1000;//----流控专用
								qpc855for900+= 450*dff/1000;//----流控专用
								LOG4CPLUS_DEBUG_FMT(log_1,"[atkorderList.size()-1][qpc855]:%lld\n",qpc855);
								LOG4CPLUS_DEBUG_FMT(log_1,"[atkorderList.size()-1][qpc855for900]:%lld\n",qpc855for900);
							}
							else if (atkorderList[0]->main.OrderStatus == DING_FTDC_OS_NoTradeQueueing)
							{
								Tmm sys;
								memcpy(&sys,atkorderList[0]->exmaps["sys_time"],sizeof(Tmm));
								LOG4CPLUS_DEBUG_FMT(log_1,"[855lastNoUseOrderwMilliseconds1]:%u.%u\n",sys.seconds,sys.wMilliseconds);
								long milliseconds855local = (sys.seconds)*1000+sys.wMilliseconds;
								LOG4CPLUS_DEBUG_FMT(log_1,"[milliseconds855local]:%d\n",milliseconds855local);
								//long milliseconds855remote = ((8*60+55)*60+0)*1000+0;
								long milliseconds855remote = (testTime_l+800)/1000*1000;
								LOG4CPLUS_DEBUG_FMT(log_1,"[milliseconds855remote]:%d\n",milliseconds855remote);
								timedeta855 = (milliseconds855local - milliseconds855remote);

								//
								qpc855=*(__int64*)atkorderList[0]->exmaps["sys_time_qpc"];
								LOG4CPLUS_DEBUG_FMT(log_1,"[atkorderList[0]][qpc855]:%lld\n",qpc855);
								qpc855for900 = qpc855;
								//qpc855+= -100*dff/1000;//----流控专用 
								qpc855for900+= -100*dff/1000;//----流控专用 
								LOG4CPLUS_DEBUG_FMT(log_1,"[atkorderList[0]][qpc855]:%lld\n",qpc855);
								LOG4CPLUS_DEBUG_FMT(log_1,"[atkorderList[0]][qpc855for900]:%lld\n",qpc855for900);
							}
					}


					LOG4CPLUS_DEBUG_FMT(log_1,"[timedeta855]:%d\n",timedeta855);
					//testTime2_l = testTime2_l + timedeta855;

					//撤单
					for(i=0; i<atkorderList.size(); i++){
						//
						if (atkorderList[i]->main.OrderStatus == DING_FTDC_OS_NoTradeQueueing ||atkorderList[i]->main.OrderStatus==DING_FTDC_OST_Unknown)
						{
							CDINGFtdcOrderField* main = &(atkorderList[i]->main);
							CDINGFtdcOrderActionField orderAction;
							strcpy(orderAction.BrokerID,appId);
							strcpy(orderAction.UserOrderLocalID,main->UserOrderLocalID);
							strcpy(orderAction.InvestorID,userId);
							strcpy(orderAction.InstrumentID,main->InstrumentID);
							strcpy(orderAction.ExchangeID,main->ExchangeID);
							strcpy(orderAction.OrderSysID,main->OrderSysID);
							pTrader->ReqOrderAction(&orderAction,nRequestID++);
							Sleep(500);
						}	
					}
					//清理攻击列表
					atkorderList.clear();//[0]
					LOG4CPLUS_DEBUG_FMT(log_1,"atkorderList.size:%d\n",atkorderList.size());
			}		
		}

		//[s1.5]test attack time 
				LOG4CPLUS_DEBUG_FMT(log_1,"[s1.5]test attack time out\n");
				targetSeconds = testTime2_l+timedeta855;

				LOG4CPLUS_DEBUG_FMT(log_1,"[s1.5]test attack time [myMilliseconds]:%ld | [targetSeconds]:%ld\n",myMilliseconds,targetSeconds);
				if (myMilliseconds > testTime2_l - 4000 && myMilliseconds < testTime2_l + 4000)
				{
					LOG4CPLUS_DEBUG_FMT(log_1,"[s1.5]test attack time in\n");
					strcpy(instId,test_attackReq.InstrumentID);
					dir=test_attackReq.Direction;
					kpp=test_attackReq.OffsetFlag;		
					price=test_attackReq.LimitPrice;		
					vol=test_attackReq.Volume;



					while(1)
					{
						if (breakFlag>=30)
						{
							if (onRtnOrderLogFlag == 0)
							{
								CDINGFtdcQryOrderField qryOrder={0};
								strcpy(qryOrder.BrokerID,appId);
								strcpy(qryOrder.UserID,userId);
								pTrader->ReqQryOrder(&qryOrder,nRequestID++);
							}
							LOG4CPLUS_DEBUG_FMT(log_1,"breakFlag=%d\n",breakFlag);
							break;
							
						}
						Tmm sys;
						sys.seconds = getTimeInfotimeT();
						sys.wMilliseconds = GetCurrentTimeMsec();
						//GetLocalTime( &sys );
						printf("[sys.wMilliseconds1]:%u\n",sys.wMilliseconds);
						//printf( "%d/%d%d %d:%d:%d.%u ",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
						printf("[P]%d = %d\n",-resultTimeCalc.detaSecondsFromR2L,-resultTimeCalc.Milliseconds);
						//LOG4CPLUS_DEBUG_FMT(log_1,"%d\n",myMilliseconds);
						long myMillisecondsMS = (sys.seconds)*1000+sys.wMilliseconds;
						myMilliseconds = (sys.seconds-resultTimeCalc.detaSecondsFromR2L)*1000+sys.wMilliseconds-resultTimeCalc.Milliseconds;
						LOG4CPLUS_DEBUG_FMT(log_1,"myMillisecondsMS %d\n",myMillisecondsMS);
						LOG4CPLUS_DEBUG_FMT(log_1,"myMilliseconds %d\n",myMilliseconds);

						sys.seconds = getTimeInfotimeT();
						sys.wMilliseconds = GetCurrentTimeMsec();
						LOG4CPLUS_DEBUG_FMT(log_1,"[sys.wMilliseconds2]:%d\n",sys.wMilliseconds);



						if (myMillisecondsMS > targetSeconds + 10000)
						{
							if (onRtnOrderLogFlag == 0)
							{
								CDINGFtdcQryOrderField qryOrder={0};
								strcpy(qryOrder.BrokerID,appId);
								strcpy(qryOrder.UserID,userId);
								pTrader->ReqQryOrder(&qryOrder,nRequestID++);
							}
							LOG4CPLUS_DEBUG_FMT(log_1,"breakFlag=%d\n",breakFlag);
							break;
						}
						//-------精确计时-------

						//QueryPerformanceCounter(&large_interger);
						c2 = getQuadPart();//large_interger.QuadPart;
						LOG4CPLUS_DEBUG_FMT(log_1,"[c2]:%lld\n [qpc855]:%lld\n [diff_859_855]:%lld\n [qpc855+diff_859_855]:%lld\n",c2,qpc855,diff_859_855,qpc855+diff_859_855);
						if(c2>qpc855+diff_859_855)
							//if (myMillisecondsMS > targetSeconds)// - 150) //提早100ms进攻 改为50ms 应该改为150ms
						{
							//LARGE_INTEGER litmp;
							double dft,dff,dfm;
							//QueryPerformanceFrequency(&litmp);//获得时钟频率
							dff=getdff();//(double)litmp.QuadPart;
							int cpuSleepCount = dff*1/1000;
							LOG4CPLUS_DEBUG(log_1,"myMillisecondsMS > targetSeconds------------1\n"<<myMillisecondsMS<<"|"<<targetSeconds<<endl);
							for(int i=1;i<31;i++)
							{

								CDINGFtdcInputOrderField& req=*(new CDINGFtdcInputOrderField);
								spi.ReqOrderInsertReady(instId,dir,kpp,price,vol,req);
								//pTdSpi->pUserApi->ReqOrderInsert(req);

								CDINGFtdcOrderFieldEX* reqEx= classMapper::toCDINGFtdcOrderFieldEX2(req);
								reqEx->exmaps["test"]=(void *) new string("test");
								reqEx->exmaps["seq"]=(void *) new int(i); //顺序
								Tmm* point_sys=(new Tmm);
								point_sys->seconds = getTimeInfotimeT();
								point_sys->wMilliseconds = GetCurrentTimeMsec();

								//LOG4CPLUS_DEBUG_FMT(log_1,"%d\n",myMilliseconds);
								myMilliseconds = (point_sys->seconds-resultTimeCalc.detaSecondsFromR2L)*1000+point_sys->wMilliseconds-resultTimeCalc.Milliseconds;
								myMillisecondsMS = (point_sys->seconds)*1000+point_sys->wMilliseconds;
								LOG4CPLUS_DEBUG_FMT(log_1,"[myMillisecondsMS]=%d\n",myMillisecondsMS);
								LOG4CPLUS_DEBUG_FMT(log_1,"[myMilliseconds]=%d\n",myMilliseconds);
								reqEx->exmaps["sys_time"]=(void *) point_sys; //sys_time

								//QueryPerformanceCounter(&large_interger);
								//c2 = large_interger.QuadPart;							
								__int64* qpc =new __int64;
								*qpc=getQuadPart();
								reqEx->exmaps["sys_time_qpc"]=(void *)qpc; //sys_time
								//LOG4CPLUS_DEBUG_FMT(log_1,"[sys_time_qpc 1.5]:%lld\n",*qpc);



								atkorderList.push_back(reqEx);
								int ret = pTrader->ReqOrderInsert(&req, nRequestID++);
								//Sleep(2);
								
								//sleepcpu(cpuSleepCount);
								int j=0;
								for (int i=0;i<900;i++)
								{   j=j+1;
								for(int k=0;k<900;k++)
								{j=j+1-1;}
								}

								/*LOG4CPLUS_DEBUG_FMT(log_1,"insertOrder------------1\n");
								printf("insertOrder2------------2\n");
								printf("insertOrder2------------3\n");
								printf("insertOrder2------------4\n");
								printf("insertOrder2------------5\n");
								printf("insertOrder2------------6\n");
								printf("insertOrder2------------7\n");
								printf("insertOrder2------------8\n");
								printf("insertOrder2------------9\n");
								printf("insertOrder2------------1\n");
								printf("insertOrder2------------2\n");
								printf("insertOrder2------------3\n");
								printf("insertOrder2------------4\n");
								printf("insertOrder2------------5\n");*/
							}
							if (onRtnOrderLogFlag == 0)
							{
								CDINGFtdcQryOrderField qryOrder={0};
								strcpy(qryOrder.BrokerID,appId);
								strcpy(qryOrder.UserID,userId);
								pTrader->ReqQryOrder(&qryOrder,nRequestID++);
							}
							breakFlag=40;
							LOG4CPLUS_DEBUG_FMT(log_1,"breakFlag=%d\n",breakFlag);
							break;
							
						}
					}
				}


				{
					bool founded=false;   
					int i=0;
					LOG4CPLUS_DEBUG_FMT(log_1,"atkorderList.size()=%d",atkorderList.size());
					for(i=0; i<atkorderList.size(); i++){



						LOG4CPLUS_DEBUG_FMT(log_1,"Seq%d \n OrderRef %s\n OrderStatus %c\n",
							(int)atkorderList[i]->exmaps["seq"],
							atkorderList[i]->main.UserOrderLocalID,
							atkorderList[i]->main.OrderStatus
							);

						Tmm sys;
						memcpy(&sys,atkorderList[i]->exmaps["sys_time"],sizeof(Tmm));
						LOG4CPLUS_DEBUG_FMT(log_1,"[sys.wMilliseconds2]:%u.%u\n",sys.seconds,sys.wMilliseconds);
					}
				}




				//[s1.6]not cancel but calc time 
				targetSeconds = testTime2_l + 15000;
				LOG4CPLUS_DEBUG_FMT(log_1,"[s1.6]not cancel but calc time out\n");
				LOG4CPLUS_DEBUG_FMT(log_1,"[s1.6]not cancel but calc time  [myMilliseconds]:%ld | [targetSeconds]:%ld\n",myMilliseconds,targetSeconds);
				if (myMilliseconds > targetSeconds - 4000 && myMilliseconds < targetSeconds + 4000)
				{	
					LOG4CPLUS_DEBUG_FMT(log_1,"[s1.6]not cancel but calc time in\n");
					for(i=0; i<atkorderList.size(); i++){

						//calc last inserted order

						if (i < atkorderList.size()-1)
						{
							LOG4CPLUS_DEBUG_FMT(log_1,"atkorderList.size()=%d",atkorderList.size());
							LOG4CPLUS_DEBUG_FMT(log_1,"[calc last inserted order] :i < atkorderList.size()+1");
							if (atkorderList[i]->main.OrderStatus == DING_FTDC_OS_NoTradeQueueing
								&& atkorderList[i+1]->main.OrderStatus == DING_FTDC_OS_Canceled)
							{
								LOG4CPLUS_DEBUG_FMT(log_1,"OrderStatus : = %d;OrderStatus+1 : = %d",atkorderList[i]->main.OrderStatus,atkorderList[i+1]->main.OrderStatus);
								Tmm sys;
								memcpy(&sys,atkorderList[i]->exmaps["sys_time"],sizeof(Tmm));
								LOG4CPLUS_DEBUG_FMT(log_1,"[859lastUseOrderwMilliseconds1]:%u.%u\n",sys.seconds,sys.wMilliseconds);
								long milliseconds859local = (sys.seconds)*1000+sys.wMilliseconds;
								LOG4CPLUS_DEBUG_FMT(log_1,"[milliseconds859local]:%d\n",milliseconds859local);
								long milliseconds859remote = (testTime2_l+800) /1000 *1000;
								LOG4CPLUS_DEBUG_FMT(log_1,"[milliseconds859remote]:%d\n",milliseconds859remote);
								timedeta859 = milliseconds859local - milliseconds859remote;

								//----精确时方法begin----
								qpc859=*(__int64*)atkorderList[i]->exmaps["sys_time_qpc"];
								LOG4CPLUS_DEBUG_FMT(log_1,"[qpc859]:%lld\n",qpc859);
								//----精确时方法end----


							}
						}										
					}

					LOG4CPLUS_DEBUG_FMT(log_1,"[timedeta859]:%ld\n",timedeta859);
					if (timedeta859==0) //全部一样的OrderStatus
					{
						if(atkorderList.size()>0)
							if (atkorderList[0]->main.OrderStatus == DING_FTDC_OS_PartTradedNotQueueing)
							{
								Tmm sys;
								memcpy(&sys,atkorderList[atkorderList.size()-1]->exmaps["sys_time"],sizeof(Tmm));
								LOG4CPLUS_DEBUG_FMT(log_1,"[859lastNoUseOrderwMilliseconds1_DING_FTDC_OST_NoTradeQueueing]:%u.%u\n",sys.seconds,sys.wMilliseconds);
								long milliseconds859local = (sys.seconds)*1000+sys.wMilliseconds;
								LOG4CPLUS_DEBUG_FMT(log_1,"[milliseconds859local]:%d\n",milliseconds859local);
								long milliseconds859remote = (testTime2_l+800) /1000 *1000;
								LOG4CPLUS_DEBUG_FMT(log_1,"[milliseconds859remote]:%d\n",milliseconds859remote);
								timedeta859 = (milliseconds859local - milliseconds859remote);

								//----精确时方法begin----
								qpc859=*(__int64*)atkorderList[atkorderList.size()-1]->exmaps["sys_time_qpc"];
								LOG4CPLUS_DEBUG_FMT(log_1,"[qpc859]:%lld\n",qpc859);
								//----精确时方法end----
							}
							else if (atkorderList[0]->main.OrderStatus == DING_FTDC_OS_Canceled) //第一张就失败
							{
								Tmm sys;
								memcpy(&sys,atkorderList[0]->exmaps["sys_time"],sizeof(Tmm));
								LOG4CPLUS_DEBUG_FMT(log_1,"[859lastNoUseOrderwMilliseconds1_DING_FTDC_OST_Canceled]:%u.%u\n",sys.seconds,sys.wMilliseconds);
								long milliseconds859local = (sys.seconds)*1000+sys.wMilliseconds;
								LOG4CPLUS_DEBUG_FMT(log_1,"[milliseconds859local]:%d\n",milliseconds859local);
								//long milliseconds859remote = ((8*60+59)*60+0)*1000+0;
								long milliseconds859remote = (testTime2_l+800)  /1000 *1000;
								LOG4CPLUS_DEBUG_FMT(log_1,"[milliseconds859remote]:%d\n",milliseconds859remote);
								timedeta859 = (milliseconds859local - milliseconds859remote-200);
								//----精确时方法begin----
								qpc859=*(__int64*)atkorderList[0]->exmaps["sys_time_qpc"];
								LOG4CPLUS_DEBUG_FMT(log_1,"[qpc859]:%lld\n",qpc859);
								//----精确时方法end----
							}
					}

					//attackTime_l = attackTime_l + timedeta859; //加上859本地超时调整系数
					LOG4CPLUS_DEBUG_FMT(log_1,"[timedeta859]:%d\n",timedeta859);
					LOG4CPLUS_DEBUG_FMT(log_1,"[timedeta855]:%d\n",timedeta855);
					LOG4CPLUS_DEBUG_FMT(log_1,"[attackTime_l]:%d\n",attackTime_l);


				}
							//if (atkmode != 1) //mode=1 计时状态 //atkmode+++++++20151129 去掉 mode1要点火
			if(myMilliseconds > testTime2_l)//太早进入容易没有收到输入参数
			{
				LOG4CPLUS_DEBUG_FMT(log_1,"[s2]imform attack time out\n");
				targetSeconds = attackTime_l + timedeta859; // attackTime_l此值每个循环会被自动刷新
				LOG4CPLUS_DEBUG_FMT(log_1,"[s2]imform attack time [myMilliseconds] %d [targetSeconds - 4000] %d\n",myMilliseconds,targetSeconds - 4000);
				if (((myMilliseconds > attackTime_l - 4000 && myMilliseconds < attackTime_l + 4000)) || atkmode == 2) //atkmode+++++++20151129
				{
					LOG4CPLUS_DEBUG_FMT(log_1,"[s2]imform attack time in\n");
					strcpy(instId,attackReq.InstrumentID);
					dir=attackReq.Direction;
					kpp=attackReq.OffsetFlag;		
					price=attackReq.LimitPrice;		
					vol=attackReq.Volume;
					int mark=0;
					while(1)
					{
						if (breakFlag>=50)
						{
							LOG4CPLUS_DEBUG_FMT(log_1,"breakFlag=%d\n",breakFlag);
							break;
						}
						Tmm sys;
						//GetLocalTime( &sys );
						sys.seconds = getTimeInfotimeT();
						sys.wMilliseconds = GetCurrentTimeMsec();
						//printf("[sys.wMilliseconds2]:%u\n",sys.wMilliseconds);
						//LOG4CPLUS_DEBUG_FMT(log_1, "%d/%d/%d %d:%d:%d.%u ",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
						//printf("[P]%d = %d\n",-resultTimeCalc.deta,-resultTimeCalc.Milliseconds);
						long myMillisecondsMS = (sys.seconds)*1000+sys.wMilliseconds;
						myMilliseconds = (sys.seconds-resultTimeCalc.detaSecondsFromR2L)*1000+sys.wMilliseconds-resultTimeCalc.Milliseconds;
						if (0&&mark == 0)
						{
							LOG4CPLUS_DEBUG_FMT(log_1,"[myMilliseconds]:%u\n",myMillisecondsMS);
							LOG4CPLUS_DEBUG_FMT(log_1,"[myMilliseconds]:%u\n",myMilliseconds);
							printf("[myMilliseconds2]:%u\n",myMilliseconds);
						}
						//mark++;
						//LOG4CPLUS_DEBUG_FMT(log_1,"[myMilliseconds]:%u\n",myMillisecondsMS);
						//LOG4CPLUS_DEBUG_FMT(log_1,"[myMilliseconds]:%u\n",myMilliseconds);
						//printf("[myMilliseconds2]:%u\n",myMilliseconds);
						//GetLocalTime( &sys );

						//LOG4CPLUS_DEBUG_FMT(log_1,"[sys.wMilliseconds2]:%u\n",sys.wMilliseconds);
// 						if (atkmode == 2)
// 						{
// 							LOG4CPLUS_DEBUG_FMT(log_1,"Sleep(30000);------------1\n");
// 							Sleep(30000);
// 						}
						if (myMillisecondsMS > targetSeconds + 10000)
						{
							LOG4CPLUS_DEBUG_FMT(log_1,"break------------1\n");
							break;
						}


						//-------精确计时-------

						
						c2 = getQuadPart();
						LOG4CPLUS_DEBUG_FMT(log_1,"[c2]:%lld\n [qpc859+diff_900_859]:%lld\n",c2,qpc859+diff_900_859);
						if(c2>qpc859+diff_900_859 || atkmode == 2) //+++++20151126更新分离式进攻
							//if(c2>qpc855+diff_900_855)//+++++++++
							//if(c2>qpc855for900+diff_900_855)//+++++++++20151103新更
							//if (myMillisecondsMS > targetSeconds )//- 100)
						{
							if (atkmode == 2) //++++++++++++++20151126更新分离式进攻
							{
								LOG4CPLUS_DEBUG_FMT(log_1,"waiting for signal\n");
								//WaitForSingleObject(attackEvent, INFINITE);
							}
							else
							{
								LOG4CPLUS_DEBUG_FMT(log_1,"send signal\n");
								//SetEvent(attackEvent);
							}
							int cpuSleepCount = dff*0.1/1000;
							if (atkmode != 1) //+20151126更新分离式进攻点火完成 atkmode ==1 不进攻
								for(int i=1;i<21;i++)
								{
									onRtnOrderLogFlag=1;int j=0;
									{
										//sleepcpu(cpuSleepCount);
										for (int x=0;x<470;x++)
										{   j=j+1;
											for(int k=0;k<470;k++)
											{j=j+1-1;}
										}
										CDINGFtdcInputOrderField& req=*(new CDINGFtdcInputOrderField);
										spi.ReqOrderInsertReady(instId,dir,kpp,price,attackvol,req);
										req.TimeCondition=DING_FTDC_TC_IOC;
										pTrader->ReqOrderInsert(&req,nRequestID++);

										CDINGFtdcOrderFieldEX* reqEx= classMapper::toCDINGFtdcOrderFieldEX2(req);
										reqEx->exmaps["test"]=(void *) new string("test");
										reqEx->exmaps["seq"]=(void *) new int(i); //顺序
										
										Tmm* point_sys=(new Tmm);
										point_sys->seconds = getTimeInfotimeT();
										point_sys->wMilliseconds = GetCurrentTimeMsec();
									
										reqEx->exmaps["sys_time"]=(void *) point_sys; //sys_time
										atkorderList.push_back(reqEx);
									
									}									

								}
								breakFlag=50;
								LOG4CPLUS_DEBUG_FMT(log_1,"breakFlag=%d\n",breakFlag);
								break;
						}
					}
				}
			}


			//[s3]cancel order time 
			targetSeconds = attackTime_l + 15000;
			LOG4CPLUS_DEBUG_FMT(log_1,"[s3]cancel order time out\n");
			LOG4CPLUS_DEBUG_FMT(log_1,"[s3]cancel order time [myMilliseconds]:%ld | [targetSeconds]:%ld\n",myMilliseconds,targetSeconds);
			if (myMilliseconds > targetSeconds - 7000 && myMilliseconds < targetSeconds + 7000)
			{	
				LOG4CPLUS_DEBUG_FMT(log_1,"[s3]cancel order time in\n");
				for(i=0; i<atkorderList.size(); i++){					
					if (atkorderList[i]->main.OrderStatus == DING_FTDC_OS_NoTradeQueueing ||atkorderList[i]->main.OrderStatus==DING_FTDC_OST_Unknown)
					{
						CDINGFtdcOrderField* main = &(atkorderList[i]->main);
						CDINGFtdcOrderActionField orderAction;
						strcpy(orderAction.BrokerID,main->BrokerID);
						strcpy(orderAction.UserOrderLocalID,main->UserOrderLocalID);
						strcpy(orderAction.InvestorID,main->InvestorID);
						strcpy(orderAction.InstrumentID,main->InstrumentID);
						strcpy(orderAction.ExchangeID,main->ExchangeID);
						strcpy(orderAction.OrderSysID,main->OrderSysID);
						pTrader->ReqOrderAction(&orderAction,nRequestID++);
						Sleep(500);
					}					
				}
				int atkorderListSize=atkorderList.size();
				for(i=0; i< atkorderListSize; i++){
					{
						atkorderList.pop_back();
					}					
				}

			}
		}
	pTrader->Join();
	pTrader->Release();

	return &CtpAtkMd();
}

int CtpAtkMd::TimeConfirm()
{
	return 0;
}
