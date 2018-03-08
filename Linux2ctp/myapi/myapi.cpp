#include "myapi.h"
#include "string.h"



CMyTraderApi * CMyTraderApi::CreateFtdcTraderApi( const char *pszFlowPath /*= ""*/ )
{
	CMyTraderApi* temp= new CMyTraderApi();
	temp->realapi = CThostFtdcTraderApi::CreateFtdcTraderApi(pszFlowPath);
	return temp;
}

const char * CMyTraderApi::GetVersion( int &nMajorVersion, int &nMinorVersion )
{
	//return CMyTraderApi::GetVersion(nMajorVersion,nMinorVersion);
	return "test";
}

void CMyTraderApi::Release()
{
	realapi->Release();
}

void CMyTraderApi::Init()
{
	realapi->Init();
}

int CMyTraderApi::Join()
{
	return realapi->Join();
}

const char * CMyTraderApi::GetTradingDay()
{
	return realapi->GetTradingDay();
}

void CMyTraderApi::RegisterFront( char *pszFrontAddress )
{
	realapi->RegisterFront(pszFrontAddress);
}

void CMyTraderApi::RegisterNameServer( char *pszNsAddress )
{
	//realapi->RegisterNameServer(pszNsAddress);
}

void CMyTraderApi::RegisterSpi( CMyTraderSpi* pSpi )
{
	realapi->RegisterSpi(pSpi);
}

int CMyTraderApi::RegisterCertificateFile( const char *pszCertFileName, const char *pszKeyFileName, const char *pszCaFileName, const char *pszKeyFilePassword )
{
	//return realapi->RegisterCertificateFile(pszCertFileName,pszKeyFileName,pszCaFileName,pszKeyFilePassword);
	return 1;
}

void CMyTraderApi::SubscribePrivateTopic( DING_TE_RESUME_TYPE nResumeType )
{
	//realapi->SubscribePrivateTopic(nResumeType);
	realapi->SubscribePrivateTopic((THOST_TE_RESUME_TYPE)nResumeType);

}

void CMyTraderApi::SubscribePublicTopic( DING_TE_RESUME_TYPE nResumeType )
{
	realapi->SubscribePublicTopic((THOST_TE_RESUME_TYPE)nResumeType);
	//realapi->SubscribePublicTopic(nResumeType);
}

void CMyTraderApi::SubscribeUserTopic( DING_TE_RESUME_TYPE nResumeType )
{
	//realapi->SubscribeUserTopic(nResumeType);
}

void CMyTraderApi::SetHeartbeatTimeout( unsigned int timeout )
{
	//realapi->SetHeartbeatTimeout(timeout);
}

int CMyTraderApi::OpenRequestLog( const char *pszReqLogFileName )
{
	//return realapi->OpenRequestLog(pszReqLogFileName);
	return 1;
}

int CMyTraderApi::OpenResponseLog( const char *pszRspLogFileName )
{
	//return realapi->OpenResponseLog(pszRspLogFileName);
	return 1;
}

int CMyTraderApi::ReqUserLogin( CDINGFtdcReqUserLoginField *pReqUserLogin, int nRequestID )
{
	CThostFtdcReqUserLoginField reqUserLoginField={0};
	strcpy(reqUserLoginField.BrokerID,pReqUserLogin->BrokerID);
	strcpy(reqUserLoginField.Password,pReqUserLogin->Password);
	strcpy(reqUserLoginField.UserID,pReqUserLogin->UserID);
	return realapi->ReqUserLogin(&reqUserLoginField,nRequestID);
	//return 1;
}

int CMyTraderApi::ReqUserLogout( CDINGFtdcReqUserLogoutField *pReqUserLogout, int nRequestID1 )
{
	CThostFtdcUserLogoutField UserLogout={0};
	strcpy(UserLogout.BrokerID,pReqUserLogout->BrokerID);
	strcpy(UserLogout.UserID,pReqUserLogout->UserID);
	return realapi->ReqUserLogout(&UserLogout,nRequestID1);
	//return 1;
}

int CMyTraderApi::ReqUserPasswordUpdate( CDINGFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID )
{
	//return realapi->ReqUserPasswordUpdate(pUserPasswordUpdate,nRequestID);
	return 1;
}

int CMyTraderApi::ReqOrderInsert( CDINGFtdcInputOrderField *pInputOrder, int nRequestID )
{
	CThostFtdcInputOrderField req={0};
	strcpy(req.BrokerID, pInputOrder->BrokerID);  //应用单元代码	
	strcpy(req.InvestorID, pInputOrder->UserID); //投资者代码	
	strcpy(req.UserID, pInputOrder->UserID); //投资者代码
	strcpy(req.InstrumentID, pInputOrder->InstrumentID); //合约代码	
	strcpy(req.OrderRef, pInputOrder->UserOrderLocalID);  //报单引用

	req.LimitPrice = pInputOrder->LimitPrice;	//价格

	req.OrderPriceType = pInputOrder->OrderPriceType;//价格类型=限价	
	req.TimeCondition = pInputOrder->TimeCondition;  //有效期类型:当日有效
	req.Direction = pInputOrder->Direction;  //买卖方向	
	req.CombOffsetFlag[0] = pInputOrder->OffsetFlag;	  //组合投机套保标志
	req.CombHedgeFlag[0] = pInputOrder->HedgeFlag;	  //组合投机套保标志
	req.VolumeTotalOriginal = pInputOrder->Volume;	///数量		
	req.MinVolume = 1;	//最小成交量:1	
	req.ContingentCondition = THOST_FTDC_CC_Immediately;  //触发条件:立即
	req.VolumeCondition = THOST_FTDC_VC_AV; //成交量类型:任何数量
	//TThostFtdcPriceType	StopPrice;  //止损价
	req.ForceCloseReason = pInputOrder->ForceCloseReason;	//强平原因:非强平	
	req.IsAutoSuspend = 0;  //自动挂起标志:否	
	//throw std::exception("The method or operation is not implemented.");
	return realapi->ReqOrderInsert(&req,nRequestID);
	return 1;
}

int CMyTraderApi::ReqOrderAction( CDINGFtdcOrderActionField *pOrderAction, int nRequestID )
{
	CThostFtdcInputOrderActionField inputOrderAction;
	memset(&inputOrderAction, 0, sizeof(CThostFtdcInputOrderActionField));
	strcpy(inputOrderAction.BrokerID, pOrderAction->BrokerID);   //经纪公司代码	
	strcpy(inputOrderAction.InvestorID, pOrderAction->UserID); //投资者代码
	//strcpy(req.OrderRef, pOrderRef); //报单引用	
	//req.FrontID = frontId;           //前置编号	
	//req.SessionID = sessionId;       //会话编号
	strcpy(inputOrderAction.InstrumentID, pOrderAction->InstrumentID); //投资者代码
	strcpy(inputOrderAction.ExchangeID, pOrderAction->ExchangeID);
	strcpy(inputOrderAction.OrderSysID, pOrderAction->OrderSysID);
	inputOrderAction.ActionFlag = THOST_FTDC_AF_Delete;  //操作标志 
	return realapi->ReqOrderAction(&inputOrderAction,nRequestID++);
	//return 1;
}

int CMyTraderApi::ReqQryOrder( CDINGFtdcQryOrderField *pQryOrder, int nRequestID )
{
	CThostFtdcQryOrderField qryOrder={0};
	strcpy(qryOrder.BrokerID,pQryOrder->BrokerID);
	strcpy(qryOrder.ExchangeID,pQryOrder->ExchangeID);
	strcpy(qryOrder.InstrumentID,pQryOrder->InstrumentID);
	strcpy(qryOrder.InvestorID,pQryOrder->InvestorID);
	strcpy(qryOrder.OrderSysID,pQryOrder->OrderSysID);
	return realapi->ReqQryOrder(&qryOrder, nRequestID);
	//return 1;
}

int CMyTraderApi::ReqQryTrade( CDINGFtdcQryTradeField *pQryTrade, int nRequestID )
{
	//return realapi->ReqQryTrade(pQryTrade, nRequestID) ;
	return 1;
}

int CMyTraderApi::ReqQryUserInvestor( CDINGFtdcQryUserInvestorField *pQryUserInvestor, int nRequestID )
{
	//return realapi->ReqQryUserInvestor(pQryUserInvestor, nRequestID) ;
	return 1;
}

int CMyTraderApi::ReqQryTradingCode( CDINGFtdcQryTradingCodeField *pQryTradingCode, int nRequestID )
{
	//return realapi->ReqQryTradingCode(pQryTradingCode,nRequestID);
	return 1;
}

int CMyTraderApi::ReqQryInvestorAccount( CDINGFtdcQryInvestorAccountField *pQryInvestorAccount, int nRequestID )
{
	CThostFtdcQryTradingAccountField qryInvestorAccount={0};
	strcpy(qryInvestorAccount.BrokerID,pQryInvestorAccount->BrokerID);
	strcpy(qryInvestorAccount.InvestorID,pQryInvestorAccount->InvestorID);
	int ret= realapi->ReqQryTradingAccount(&qryInvestorAccount, nRequestID++);
	return ret;
	return 1;
}

int CMyTraderApi::ReqQryInstrument( CDINGFtdcQryInstrumentField *pQryInstrument, int nRequestID )
{
	CThostFtdcQryInstrumentField qryInstrument={0};
	strcpy(qryInstrument.InstrumentID,pQryInstrument->InstrumentID);
	return realapi->ReqQryInstrument(&qryInstrument,nRequestID);
	return 1;
}

int CMyTraderApi::ReqQryExchange( CDINGFtdcQryExchangeField *pQryExchange, int nRequestID )
{
	//return realapi->ReqQryExchange(pQryExchange,nRequestID);
	return 1;
}

int CMyTraderApi::ReqQryInvestorPosition( CDINGFtdcQryInvestorPositionField *pQryInvestorPosition, int nRequestID )
{
	CThostFtdcQryInvestorPositionField  qryInvestorPosition={0};
	strcpy(qryInvestorPosition.BrokerID,pQryInvestorPosition->BrokerID);
	/*strcpy(qryInvestorPosition,pQryInvestorPosition->ExchangeID);*/
	strcpy(qryInvestorPosition.InstrumentID,pQryInvestorPosition->InstrumentID);
	strcpy(qryInvestorPosition.InvestorID,pQryInvestorPosition->InvestorID);
	/*strcpy(qryInvestorPosition.UserID,pQryInvestorPosition->UserID);*/
	return realapi->ReqQryInvestorPosition(&qryInvestorPosition,nRequestID);
	//return 1;
}

int CMyTraderApi::ReqSettlementInfoConfirm(CDINGFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID)
{
	CThostFtdcSettlementInfoConfirmField settlementInfoConfirm;
	strcpy(settlementInfoConfirm.BrokerID,pSettlementInfoConfirm->BrokerID);
	strcpy(settlementInfoConfirm.ConfirmDate,pSettlementInfoConfirm->ConfirmDate);
	strcpy(settlementInfoConfirm.ConfirmTime,pSettlementInfoConfirm->ConfirmTime);
	strcpy(settlementInfoConfirm.InvestorID,pSettlementInfoConfirm->InvestorID);
	return realapi->ReqSettlementInfoConfirm(&settlementInfoConfirm, nRequestID);
}

int CMyTraderApi::ReqSubscribeTopic( CDINGFtdcDisseminationField *pDissemination, int nRequestID )
{
	//return realapi->ReqSubscribeTopic(pDissemination,nRequestID);
	return 1;
}

int CMyTraderApi::ReqQryComplianceParam( CDINGFtdcQryComplianceParamField *pQryComplianceParam, int nRequestID )
{
	//return realapi->ReqQryComplianceParam(pQryComplianceParam,nRequestID);
	return 1;
}

int CMyTraderApi::ReqQryTopic( CDINGFtdcDisseminationField *pDissemination, int nRequestID )
{
	//return realapi->ReqQryTopic(pDissemination,nRequestID);
	return 1;
}

int CMyTraderApi::ReqQryInvestorFee( CDINGFtdcQryInvestorFeeField *pQryInvestorFee, int nRequestID )
{
	//return realapi->ReqQryInvestorFee(pQryInvestorFee,nRequestID);
	return 1;
}

int CMyTraderApi::ReqQryInvestorMargin( CDINGFtdcQryInvestorMarginField *pQryInvestorMargin, int nRequestID )
{
	CThostFtdcQryInstrumentMarginRateField qryInstrumentMarginRate={0};
	strcpy(qryInstrumentMarginRate.BrokerID,pQryInvestorMargin->BrokerID);
	strcpy(qryInstrumentMarginRate.InstrumentID,pQryInvestorMargin->InstrumentID);
	strcpy(qryInstrumentMarginRate.InvestorID,pQryInvestorMargin->UserID);
	//req.
	qryInstrumentMarginRate.HedgeFlag = '1';
	return realapi->ReqQryInstrumentMarginRate(&qryInstrumentMarginRate,nRequestID++);
	//return realapi->ReqQryInvestorMargin(pQryInvestorMargin,nRequestID);
	return 1;
}


 

CMyTraderSpi::CMyTraderSpi( CDINGFtdcTraderSpi* spi )
{
	realspi = spi;
}

void CMyTraderSpi::OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	//throw std::exception("The method or operation is not implemented.");
	CDINGFtdcRspUserLoginField mypRspUserLogin={0};
	CDINGFtdcRspInfoField mypRspInfo={0};
	strcpy(mypRspUserLogin.ActionDay,pRspUserLogin->TradingDay);
	strcpy(mypRspUserLogin.BrokerID,pRspUserLogin->BrokerID);
	strcpy(mypRspUserLogin.LoginTime,pRspUserLogin->LoginTime);
	strcpy(mypRspUserLogin.CZCETime,pRspUserLogin->CZCETime);
	strcpy(mypRspUserLogin.DCETime,pRspUserLogin->DCETime);
	strcpy(mypRspUserLogin.SHFETime,pRspUserLogin->SHFETime);
	strcpy(mypRspUserLogin.FFEXTime,pRspUserLogin->FFEXTime);
	strcpy(mypRspUserLogin.INETime,pRspUserLogin->INETime);
	strcpy(mypRspUserLogin.MaxOrderLocalID,pRspUserLogin->MaxOrderRef);

	mypRspInfo.ErrorID=pRspInfo->ErrorID;
	strcpy(mypRspInfo.ErrorMsg,pRspInfo->ErrorMsg);
	//int nRequestID; bool bIsLast;
	realspi->OnRspUserLogin(&mypRspUserLogin,&mypRspInfo,nRequestID,bIsLast);
}

void CMyTraderSpi::OnFrontConnected()
{
	realspi->OnFrontConnected();
	//throw std::exception("The method or operation is not implemented.");
}

void CMyTraderSpi::OnFrontDisconnected( int nReason )
{
	realspi->OnFrontDisconnected(nReason);
	//throw std::exception("The method or operation is not implemented.");
}

void CMyTraderSpi::OnHeartBeatWarning( int nTimeLapse )
{
	realspi->OnHeartBeatWarning(nTimeLapse);
	//throw std::exception("The method or operation is not implemented.");
}

void CMyTraderSpi::OnRspUserLogout( CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CDINGFtdcRspUserLogoutField myRspUserLogout;
	CDINGFtdcRspInfoField mypRspInfo={0};
	mypRspInfo.ErrorID=pRspInfo->ErrorID;
	strcpy(mypRspInfo.ErrorMsg,pRspInfo->ErrorMsg);
	realspi->OnRspUserLogout(&myRspUserLogout, &mypRspInfo, nRequestID, bIsLast) ;
	//throw std::exception("The method or operation is not implemented.");
}

void CMyTraderSpi::OnRspOrderInsert( CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CDINGFtdcInputOrderField myInputOrder; 
	CDINGFtdcRspInfoField mypRspInfo={0};
	mypRspInfo.ErrorID=pRspInfo->ErrorID;
	strcpy(mypRspInfo.ErrorMsg,pRspInfo->ErrorMsg);
	realspi->OnRspOrderInsert(&myInputOrder, &mypRspInfo, nRequestID, bIsLast );
	//throw std::exception("The method or operation is not implemented.");
}

void CMyTraderSpi::OnRspOrderAction( CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CDINGFtdcRspInfoField mypRspInfo={0};
	if (pRspInfo)
	{		
		mypRspInfo.ErrorID=pRspInfo->ErrorID;
		strcpy(mypRspInfo.ErrorMsg,pRspInfo->ErrorMsg);
	}
	CDINGFtdcOrderActionField myOrderAction;
	if (pInputOrderAction)
	{
		realspi->OnRspOrderAction(&myOrderAction,&mypRspInfo,nRequestID,bIsLast);
	}
	else
	{
		realspi->OnRspOrderAction(0,&mypRspInfo,nRequestID,bIsLast);
	}
	
	//throw std::exception("The method or operation is not implemented.");
}

void CMyTraderSpi::OnRspSettlementInfoConfirm( CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	//realspi->onrspset
	//throw std::exception("The method or operation is not implemented.");
}

void CMyTraderSpi::OnRtnOrder( CThostFtdcOrderField *pOrder )
{
	CDINGFtdcOrderField myOrder;
	strcpy(myOrder.ActionDay,pOrder->TradingDay);
	strcpy(myOrder.BrokerID,pOrder->BrokerID);
	strcpy(myOrder.InvestorID,pOrder->InvestorID);
	strcpy(myOrder.InstrumentID,pOrder->InstrumentID);
	strcpy(myOrder.UserOrderLocalID,pOrder->OrderRef);
	sprintf(myOrder.OrderLocalID,"%d",pOrder->BrokerOrderSeq);
	strcpy(myOrder.UserID,pOrder->UserID);
	myOrder.OrderPriceType=pOrder->OrderPriceType;
	myOrder.Direction=pOrder->Direction;
	myOrder.OffsetFlag=pOrder->CombOffsetFlag[0];
	myOrder.HedgeFlag=pOrder->CombHedgeFlag[0];
	myOrder.LimitPrice=pOrder->LimitPrice;
	myOrder.Volume = pOrder->VolumeTotalOriginal;
	myOrder.TimeCondition = pOrder->TimeCondition;
	strcpy(myOrder.GTDDate,pOrder->GTDDate);
	myOrder.VolumeCondition = pOrder->VolumeCondition;
	myOrder.MinVolume = pOrder->MinVolume;
	myOrder.ContingentCondition = pOrder->ContingentCondition;
	myOrder.StopPrice = pOrder->StopPrice;
	myOrder.VolumeTraded = pOrder->VolumeTraded;
	myOrder.VolumeRemain = pOrder->VolumeTotal;
	myOrder.ForceCloseReason = pOrder->ForceCloseReason;
	myOrder.IsAutoSuspend = pOrder->IsAutoSuspend;
	myOrder.OrderStatus = pOrder->OrderStatus;
	strcpy(myOrder.BusinessUnit , pOrder->BusinessUnit);
	myOrder.SessionID=pOrder->SessionID;
	strcpy(myOrder.OrderSysID , pOrder->OrderSysID);
	strcpy(myOrder.ExchangeID , pOrder->ExchangeID);
	realspi->OnRtnOrder(&myOrder);
	//throw std::exception("The method or operation is not implemented.");
}

void CMyTraderSpi::OnRtnTrade( CThostFtdcTradeField *pTrade )
{
	CDINGFtdcTradeField myTrade={0};
	strcpy(myTrade.ActionDay,pTrade->TradeDate);
	strcpy(myTrade.BrokerID,pTrade->BrokerID);
	strcpy(myTrade.ExchangeID,pTrade->ExchangeID);
	strcpy(myTrade.TradingDay,pTrade->TradingDay);
	strcpy(myTrade.ParticipantID,pTrade->ParticipantID);
	/*
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///交易日
	TDINGFtdcTradingDayType	TradingDay;
	///会员编号
	TDINGFtdcParticipantIDType	ParticipantID;
	///下单席位号
	TDINGFtdcSeatIDType	SeatID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
	///客户号
	TDINGFtdcClientIDType	ClientID;
	///用户编号
	TDINGFtdcUserIDType	UserID;
	///成交编号
	TDINGFtdcTradeIDType	TradeID;
	///报单编号
	TDINGFtdcOrderSysIDType	OrderSysID;
	///本地报单编号
	TDINGFtdcUserOrderLocalIDType	UserOrderLocalID;
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
	///买卖方向
	TDINGFtdcDirectionType	Direction;
	///开平标志
	TDINGFtdcOffsetFlagType	OffsetFlag;
	///投机套保标志
	TDINGFtdcHedgeFlagType	HedgeFlag;
	///成交价格
	TDINGFtdcPriceType	TradePrice;
	///成交数量
	TDINGFtdcVolumeType	TradeVolume;
	///成交时间
	TDINGFtdcTimeType	TradeTime;
	///清算会员编号
	TDINGFtdcParticipantIDType	ClearingPartID;
	///本地业务标识
	TDINGFtdcBusinessLocalIDType	BusinessLocalID;
	///业务发生日期
	TDINGFtdcDateType	ActionDay;
	//strcpy_s(myTrade.BusinessLocalID,pTrade->BusinessUnit);*/

	realspi->OnRtnTrade(&myTrade);
	//throw std::exception("The method or operation is not implemented.");
}

void CMyTraderSpi::OnRspQryInstrument( CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CDINGFtdcRspInfoField mypRspInfo={0};
	if (pRspInfo)
	{		
		mypRspInfo.ErrorID=pRspInfo->ErrorID;
		strcpy(mypRspInfo.ErrorMsg,pRspInfo->ErrorMsg);
	}

	CDINGFtdcRspInstrumentField rspInstrument={0};
	if (pInstrument)
	{
		strcpy(rspInstrument.ExchangeID,pInstrument->ExchangeID);
		strcpy(rspInstrument.ProductID,pInstrument->ProductID);
		//strcpy(rspInstrument.ProductName,pInstrument->;
		strcpy(rspInstrument.InstrumentID,pInstrument->InstrumentID);
		strcpy(rspInstrument.InstrumentName,pInstrument->InstrumentName);
		rspInstrument.DeliveryYear=pInstrument->DeliveryYear;
		rspInstrument.DeliveryMonth=pInstrument->DeliveryMonth;
		rspInstrument.MaxLimitOrderVolume=pInstrument->MaxLimitOrderVolume;
		rspInstrument.MinLimitOrderVolume=pInstrument->MinLimitOrderVolume;
		rspInstrument.MaxMarketOrderVolume=pInstrument->MaxMarketOrderVolume;
		rspInstrument.MinMarketOrderVolume=pInstrument->MinMarketOrderVolume;
		rspInstrument.VolumeMultiple=pInstrument->VolumeMultiple;
		rspInstrument.PriceTick=pInstrument->PriceTick;
		//rspInstrument.Currency=pInstrument->;
		// 	rspInstrument.LongPosLimit=pInstrument->;
		// 	rspInstrument.ShortPosLimit;
		// 	rspInstrument.LowerLimitPrice;
		// 	rspInstrument.UpperLimitPrice;
		// 	rspInstrument.PreSettlementPrice;
		// 	rspInstrument.InstrumentStatus;
		strcpy(rspInstrument.CreateDate,pInstrument->CreateDate);
		strcpy(rspInstrument.OpenDate,pInstrument->OpenDate);
		strcpy(rspInstrument.ExpireDate,pInstrument->ExpireDate);
		strcpy(rspInstrument.StartDelivDate,pInstrument->StartDelivDate);
		strcpy(rspInstrument.EndDelivDate,pInstrument->EndDelivDate);
		//strcpy(rspInstrument.BasisPrice,pInstrument->BasisPrice);

		rspInstrument.IsTrading=pInstrument->IsTrading;
		rspInstrument.UnderlyingInstrID;
		// 	rspInstrument.UnderlyingMultiple;
		// 	rspInstrument.PositionType;
		// 	rspInstrument.StrikePrice;
		// 	rspInstrument.OptionsType;
		// 	rspInstrument.CurrencyID;
		// 	rspInstrument.LongMarginRatio;
		// 	rspInstrument.ShortMarginRatio;
		realspi->OnRspQryInstrument(&rspInstrument,&mypRspInfo,nRequestID++,bIsLast);
	}
	else{
		realspi->OnRspQryInstrument(0,&mypRspInfo,nRequestID++,bIsLast);
	}

	
}

void CMyTraderSpi::OnRspQryTradingAccount( CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CDINGFtdcRspInfoField mypRspInfo={0};
	if (pRspInfo)
	{		
		mypRspInfo.ErrorID=pRspInfo->ErrorID;
		strcpy(mypRspInfo.ErrorMsg,pRspInfo->ErrorMsg);
	}

	CDINGFtdcRspInvestorAccountField rspInvestorAccount={0};
	if (pTradingAccount)
	{
		strcpy(rspInvestorAccount.BrokerID,pTradingAccount->BrokerID);
		//strcpy(rspInvestorAccount.InvestorID,pTradingAccount->i);
		strcpy(rspInvestorAccount.AccountID,pTradingAccount->AccountID);
		rspInvestorAccount.PreBalance=pTradingAccount->PreBalance;
		rspInvestorAccount.Deposit=pTradingAccount->Deposit;
		rspInvestorAccount.Withdraw=pTradingAccount->Withdraw;
		rspInvestorAccount.FrozenMargin=pTradingAccount->FrozenMargin;
		rspInvestorAccount.FrozenFee=pTradingAccount->FrozenCommission;
		//rspInvestorAccount.FrozenPremium=pTradingAccount->f;
		rspInvestorAccount.Fee=pTradingAccount->Commission;
		rspInvestorAccount.CloseProfit=pTradingAccount->CloseProfit;
		rspInvestorAccount.PositionProfit=pTradingAccount->PositionProfit;
		rspInvestorAccount.Available=pTradingAccount->Available;
		// 	rspInvestorAccount.LongFrozenMargin;
		// 	rspInvestorAccount.ShortFrozenMargin;
		// 	rspInvestorAccount.LongMargin;
		// 	rspInvestorAccount.ShortMargin;
		// 	rspInvestorAccount.ReleaseMargin;
		// 	rspInvestorAccount.DynamicRights;
		rspInvestorAccount.TodayInOut=pTradingAccount->CashIn;
		rspInvestorAccount.Margin=pTradingAccount->CurrMargin;
		//rspInvestorAccount.Premium;
		//rspInvestorAccount.Risk;
		realspi->OnRspQryInvestorAccount(&rspInvestorAccount,&mypRspInfo,nRequestID++,bIsLast);
	}
	else
	{
		realspi->OnRspQryInvestorAccount(0,&mypRspInfo,nRequestID++,bIsLast);

	}
	

	
}

void CMyTraderSpi::OnRspQryInstrumentMarginRate( CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CDINGFtdcRspInfoField mypRspInfo={0};
	if (pRspInfo)
	{		
		mypRspInfo.ErrorID=pRspInfo->ErrorID;
		strcpy(mypRspInfo.ErrorMsg,pRspInfo->ErrorMsg);
	}
	
	CDINGFtdcInvestorMarginField investorMargin={0};
	if (pInstrumentMarginRate)
	{
		strcpy(investorMargin.BrokerID,pInstrumentMarginRate->BrokerID);
		strcpy(investorMargin.ClientID,pInstrumentMarginRate->InvestorID);
		//strcpy(investorMargin.ExchangeID,pInstrumentMarginRate-;
		strcpy(investorMargin.InstrumentID,pInstrumentMarginRate->InstrumentID);
		//strcpy(investorMargin.ProductID,pInstrumentMarginRate->;
		investorMargin.LongMarginRate=pInstrumentMarginRate->LongMarginRatioByMoney;
		investorMargin.LongMarginAmt=pInstrumentMarginRate->LongMarginRatioByVolume;
		investorMargin.ShortMarginRate=pInstrumentMarginRate->ShortMarginRatioByMoney;
		investorMargin.ShortMarginAmt=pInstrumentMarginRate->ShortMarginRatioByVolume;
		realspi->OnRspQryInvestorMargin(&investorMargin,&mypRspInfo,nRequestID++,bIsLast);
	}
	else
	{
		realspi->OnRspQryInvestorMargin(0,&mypRspInfo,nRequestID++,bIsLast);
	}
	
	
}

void CMyTraderSpi::OnRspQryInvestorPosition( CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CDINGFtdcRspInfoField mypRspInfo={0};
	if (pRspInfo)
	{		
		mypRspInfo.ErrorID=pRspInfo->ErrorID;
		strcpy(mypRspInfo.ErrorMsg,pRspInfo->ErrorMsg);
	}
	

	CDINGFtdcRspInvestorPositionField rspInvestorPosition={0};
	if (pInvestorPosition)
	{
		strcpy(rspInvestorPosition.InvestorID,pInvestorPosition->InvestorID);
		strcpy(rspInvestorPosition.BrokerID,pInvestorPosition->BrokerID);
		strcpy(rspInvestorPosition.ClientID,pInvestorPosition->InvestorID);

		strcpy(rspInvestorPosition.InstrumentID,pInvestorPosition->InstrumentID);
		rspInvestorPosition.Direction = pInvestorPosition->PosiDirection;
		rspInvestorPosition.HedgeFlag = pInvestorPosition->HedgeFlag;
		rspInvestorPosition.UsedMargin = pInvestorPosition->UseMargin;

		/*rspInvestorPosition.ExchangeID = pInvestorPosition->e*/
		rspInvestorPosition.Position  = pInvestorPosition->TodayPosition;
		rspInvestorPosition.PositionCost = pInvestorPosition->PositionCost;
		rspInvestorPosition.YdPosition = pInvestorPosition->YdPosition;

		rspInvestorPosition.FrozenMargin = pInvestorPosition->FrozenMargin;

// 		rspInvestorPosition.LastTradeID;
// 		rspInvestorPosition.LastOrderLocalID;
// 		rspInvestorPosition.Currency;


		
		realspi->OnRspQryInvestorPosition(&rspInvestorPosition,&mypRspInfo,nRequestID++,bIsLast);
	}
	else
	{
		realspi->OnRspQryInvestorPosition(0,&mypRspInfo,nRequestID++,bIsLast);
	}

}

