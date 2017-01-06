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
	realapi->RegisterNameServer(pszNsAddress);
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
	strcpy(req.BrokerID, pInputOrder->BrokerID);  //Ӧ�õ�Ԫ����	
	strcpy(req.InvestorID, pInputOrder->UserID); //Ͷ���ߴ���	
	strcpy(req.UserID, pInputOrder->UserID); //Ͷ���ߴ���
	strcpy(req.InstrumentID, pInputOrder->InstrumentID); //��Լ����	
	strcpy(req.OrderRef, pInputOrder->UserOrderLocalID);  //��������

	req.LimitPrice = pInputOrder->LimitPrice;	//�۸�

	req.OrderPriceType = pInputOrder->OrderPriceType;//�۸�����=�޼�	
	req.TimeCondition = pInputOrder->TimeCondition;  //��Ч������:������Ч
	req.Direction = pInputOrder->Direction;  //��������	
	req.CombOffsetFlag[0] = pInputOrder->OffsetFlag;	  //���Ͷ���ױ���־
	req.CombHedgeFlag[0] = pInputOrder->HedgeFlag;	  //���Ͷ���ױ���־
	req.VolumeTotalOriginal = pInputOrder->Volume;	///����		
	req.MinVolume = 1;	//��С�ɽ���:1	
	req.ContingentCondition = THOST_FTDC_CC_Immediately;  //��������:����
	req.VolumeCondition = THOST_FTDC_VC_AV; //�ɽ�������:�κ�����
	//TThostFtdcPriceType	StopPrice;  //ֹ���
	req.ForceCloseReason = pInputOrder->ForceCloseReason;	//ǿƽԭ��:��ǿƽ	
	req.IsAutoSuspend = 0;  //�Զ������־:��	
	//throw std::exception("The method or operation is not implemented.");
	return realapi->ReqOrderInsert(&req,nRequestID);
	return 1;
}

int CMyTraderApi::ReqOrderAction( CDINGFtdcOrderActionField *pOrderAction, int nRequestID )
{
	CThostFtdcInputOrderActionField inputOrderAction;
	memset(&inputOrderAction, 0, sizeof(CThostFtdcInputOrderActionField));
	strcpy(inputOrderAction.BrokerID, pOrderAction->BrokerID);   //���͹�˾����	
	strcpy(inputOrderAction.InvestorID, pOrderAction->UserID); //Ͷ���ߴ���
	//strcpy(req.OrderRef, pOrderRef); //��������	
	//req.FrontID = frontId;           //ǰ�ñ��	
	//req.SessionID = sessionId;       //�Ự���
	strcpy(inputOrderAction.ExchangeID, pOrderAction->ExchangeID);
	strcpy(inputOrderAction.OrderSysID, pOrderAction->OrderSysID);
	inputOrderAction.ActionFlag = THOST_FTDC_AF_Delete;  //������־ 
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
	//return realapi->ReqQryInvestorAccount(pQryInvestorAccount,nRequestID);
	return 1;
}

int CMyTraderApi::ReqQryInstrument( CDINGFtdcQryInstrumentField *pQryInstrument, int nRequestID )
{
	//return realapi->ReqQryInstrument(pQryInstrument,nRequestID);
	return 1;
}

int CMyTraderApi::ReqQryExchange( CDINGFtdcQryExchangeField *pQryExchange, int nRequestID )
{
	//return realapi->ReqQryExchange(pQryExchange,nRequestID);
	return 1;
}

int CMyTraderApi::ReqQryInvestorPosition( CDINGFtdcQryInvestorPositionField *pQryInvestorPosition, int nRequestID )
{
	//return realapi->ReqQryInvestorPosition(pQryInvestorPosition,nRequestID);
	return 1;
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
	CDINGFtdcOrderActionField myOrderAction;
	CDINGFtdcRspInfoField mypRspInfo={0};
	mypRspInfo.ErrorID=pRspInfo->ErrorID;
	strcpy(mypRspInfo.ErrorMsg,pRspInfo->ErrorMsg);
	realspi->OnRspOrderAction(&myOrderAction,&mypRspInfo,nRequestID,bIsLast);
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
	///���͹�˾���
	TDINGFtdcBrokerIDType	BrokerID;
	///����������
	TDINGFtdcExchangeIDType	ExchangeID;
	///������
	TDINGFtdcTradingDayType	TradingDay;
	///��Ա���
	TDINGFtdcParticipantIDType	ParticipantID;
	///�µ�ϯλ��
	TDINGFtdcSeatIDType	SeatID;
	///Ͷ���߱��
	TDINGFtdcInvestorIDType	InvestorID;
	///�ͻ���
	TDINGFtdcClientIDType	ClientID;
	///�û����
	TDINGFtdcUserIDType	UserID;
	///�ɽ����
	TDINGFtdcTradeIDType	TradeID;
	///�������
	TDINGFtdcOrderSysIDType	OrderSysID;
	///���ر������
	TDINGFtdcUserOrderLocalIDType	UserOrderLocalID;
	///��Լ����
	TDINGFtdcInstrumentIDType	InstrumentID;
	///��������
	TDINGFtdcDirectionType	Direction;
	///��ƽ��־
	TDINGFtdcOffsetFlagType	OffsetFlag;
	///Ͷ���ױ���־
	TDINGFtdcHedgeFlagType	HedgeFlag;
	///�ɽ��۸�
	TDINGFtdcPriceType	TradePrice;
	///�ɽ�����
	TDINGFtdcVolumeType	TradeVolume;
	///�ɽ�ʱ��
	TDINGFtdcTimeType	TradeTime;
	///�����Ա���
	TDINGFtdcParticipantIDType	ClearingPartID;
	///����ҵ���ʶ
	TDINGFtdcBusinessLocalIDType	BusinessLocalID;
	///ҵ��������
	TDINGFtdcDateType	ActionDay;
	//strcpy_s(myTrade.BusinessLocalID,pTrade->BusinessUnit);*/

	realspi->OnRtnTrade(&myTrade);
	//throw std::exception("The method or operation is not implemented.");
}