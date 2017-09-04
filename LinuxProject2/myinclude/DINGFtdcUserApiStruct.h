/////////////////////////////////////////////////////////////////////////
///@system 风控前置系统
///@company CFFEX
///@file DINGFtdcUserApiStruct.h
///@brief 定义了客户端接口使用的业务数据结构
///@history 
/////////////////////////////////////////////////////////////////////////

#if !defined(_DING_FTDCSTRUCT_H)
#define _DING_FTDCSTRUCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DINGFtdcUserApiDataType.h"
#include <map>
///系统用户登录请求
struct CDINGFtdcReqUserLoginField
{
	///交易日
	TDINGFtdcDateType	TradingDay;
	///交易用户代码
	TDINGFtdcUserIDType	UserID;
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///密码
	TDINGFtdcPasswordType	Password;
	///用户端产品信息
	TDINGFtdcProductInfoType	UserProductInfo;
	///接口端产品信息
	TDINGFtdcProductInfoType	InterfaceProductInfo;
	///协议信息
	TDINGFtdcProtocolInfoType	ProtocolInfo;
	///IP地址
	TDINGFtdcIPAddressType	IPAddress;
	///Mac地址
	TDINGFtdcMacAddressType	MacAddress;
	///数据中心代码
	TDINGFtdcDataCenterIDType	DataCenterID;
};
///系统用户登录应答
struct CDINGFtdcRspUserLoginField
{
	///交易日
	TDINGFtdcDateType	TradingDay;
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///交易用户代码
	TDINGFtdcUserIDType	UserID;
	///登录成功时间
	TDINGFtdcTimeType	LoginTime;
	///用户最大本地报单号
	TDINGFtdcUserOrderLocalIDType	MaxOrderLocalID;
	///交易系统名称
	TDINGFtdcTradingSystemNameType	TradingSystemName;
	///数据中心代码
	TDINGFtdcDataCenterIDType	DataCenterID;
	///会员私有流当前长度
	TDINGFtdcSequenceNoType	PrivateFlowSize;
	///交易员私有流当前长度
	TDINGFtdcSequenceNoType	UserFlowSize;
	///业务发生日期
	TDINGFtdcDateType	ActionDay;
////------added
	TDINGFtdcTimeType	SHFETime;
	///大商所时间
	TDINGFtdcTimeType	DCETime;
	///郑商所时间
	TDINGFtdcTimeType	CZCETime;
	///中金所时间
	TDINGFtdcTimeType	FFEXTime;
	///能源中心时间
	TDINGFtdcTimeType	INETime;
};
///用户登出请求
struct CDINGFtdcReqUserLogoutField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///交易用户代码
	TDINGFtdcUserIDType	UserID;
};
///用户登出请求
struct CDINGFtdcRspUserLogoutField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///交易用户代码
	TDINGFtdcUserIDType	UserID;
};
///强制用户退出
struct CDINGFtdcForceUserExitField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///交易用户代码
	TDINGFtdcUserIDType	UserID;
};
///用户口令修改
struct CDINGFtdcUserPasswordUpdateField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///交易用户代码
	TDINGFtdcUserIDType	UserID;
	///旧密码
	TDINGFtdcPasswordType	OldPassword;
	///新密码
	TDINGFtdcPasswordType	NewPassword;
};



///报单
struct CDINGFtdcOrderField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///系统报单编号
	TDINGFtdcOrderSysIDType	OrderSysID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
	///用户代码
	TDINGFtdcUserIDType	UserID;
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
	///用户本地报单号
	TDINGFtdcUserOrderLocalIDType	UserOrderLocalID;
	///报单类型
	TDINGFtdcOrderPriceTypeType	OrderPriceType;
	///买卖方向
	TDINGFtdcDirectionType	Direction;
	///开平标志
	TDINGFtdcOffsetFlagType	OffsetFlag;
	///投机套保标志
	TDINGFtdcHedgeFlagType	HedgeFlag;
	///价格
	TDINGFtdcPriceType	LimitPrice;
	///数量

	
	TDINGFtdcVolumeType	Volume;
	///有效期类型
	TDINGFtdcTimeConditionType	TimeCondition;
	///GTD日期
	TDINGFtdcDateType	GTDDate;
	///成交量类型
	TDINGFtdcVolumeConditionType	VolumeCondition;
	///最小成交量
	TDINGFtdcVolumeType	MinVolume;
	///止损价
	TDINGFtdcPriceType	StopPrice;
	///强平原因
	TDINGFtdcForceCloseReasonType	ForceCloseReason;
	///自动挂起标志
	TDINGFtdcBoolType	IsAutoSuspend;
	///业务单元
	TDINGFtdcBusinessUnitType	BusinessUnit;
	///用户自定义域
	TDINGFtdcCustomType	UserCustom;
	///本地业务标识
	TDINGFtdcBusinessLocalIDType	BusinessLocalID;
	///业务发生日期
	TDINGFtdcDateType	ActionDay;
	///交易日
	TDINGFtdcTradingDayType	TradingDay;
	///会员编号
	TDINGFtdcParticipantIDType	ParticipantID;
	///客户号
	TDINGFtdcClientIDType	ClientID;
	///下单席位号
	TDINGFtdcSeatIDType	SeatID;
	///插入时间
	TDINGFtdcTimeType	InsertTime;
	///本地报单编号
	TDINGFtdcOrderLocalIDType	OrderLocalID;
	///报单来源
	TDINGFtdcOrderSourceType	OrderSource;
	///报单状态
	TDINGFtdcOrderStatusType	OrderStatus;
	///撤销时间
	TDINGFtdcTimeType	CancelTime;
	///撤单用户编号
	TDINGFtdcUserIDType	CancelUserID;
	///今成交数量
	TDINGFtdcVolumeType	VolumeTraded;
	///剩余数量
	TDINGFtdcVolumeType	VolumeRemain;

	TDINGFtdcContingentConditionType ContingentCondition;

    TDINGFtdcSessionIDType SessionID;
};
///输入报单
struct CDINGFtdcInputOrderField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///系统报单编号
	TDINGFtdcOrderSysIDType	OrderSysID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
	///用户代码
	TDINGFtdcUserIDType	UserID;
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
	///用户本地报单号
	TDINGFtdcUserOrderLocalIDType	UserOrderLocalID;
	///报单类型
	TDINGFtdcOrderPriceTypeType	OrderPriceType;
	///买卖方向
	TDINGFtdcDirectionType	Direction;
	///开平标志
	TDINGFtdcOffsetFlagType	OffsetFlag;
	///投机套保标志
	TDINGFtdcHedgeFlagType	HedgeFlag;
	///价格
	TDINGFtdcPriceType	LimitPrice;
	///数量
	TDINGFtdcVolumeType	Volume;
	///有效期类型
	TDINGFtdcTimeConditionType	TimeCondition;
	///GTD日期
	TDINGFtdcDateType	GTDDate;
	///成交量类型
	TDINGFtdcVolumeConditionType	VolumeCondition;
	///最小成交量
	TDINGFtdcVolumeType	MinVolume;
	///止损价
	TDINGFtdcPriceType	StopPrice;
	///强平原因
	TDINGFtdcForceCloseReasonType	ForceCloseReason;
	///自动挂起标志
	TDINGFtdcBoolType	IsAutoSuspend;
	///业务单元
	TDINGFtdcBusinessUnitType	BusinessUnit;
	///用户自定义域
	TDINGFtdcCustomType	UserCustom;
	///本地业务标识
	TDINGFtdcBusinessLocalIDType	BusinessLocalID;
	///业务发生日期
	TDINGFtdcDateType	ActionDay;
};



///报单操作
struct CDINGFtdcOrderActionField
{
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///报单编号
	TDINGFtdcOrderSysIDType	OrderSysID;
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
	///用户代码
	TDINGFtdcUserIDType	UserID;
	///本次撤单操作的本地编号
	TDINGFtdcUserOrderLocalIDType	UserOrderActionLocalID;
	///被撤订单的本地报单编号
	TDINGFtdcUserOrderLocalIDType	UserOrderLocalID;
	///报单操作标志
	TDINGFtdcActionFlagType	ActionFlag;
	///价格
	TDINGFtdcPriceType	LimitPrice;
	///数量变化
	TDINGFtdcVolumeType	VolumeChange;
	///本地业务标识
	TDINGFtdcBusinessLocalIDType	BusinessLocalID;
    TDINGFtdcInstrumentIDType InstrumentID;
};
///内存表导出
struct CDINGFtdcMemDbField
{
	///内存表名
	TDINGFtdcMemTableNameType	MemTableName;
};
///响应信息
struct CDINGFtdcRspInfoField
{
	///错误代码
	TDINGFtdcErrorIDType	ErrorID;
	///错误信息
	TDINGFtdcErrorMsgType	ErrorMsg;
};
///报单查询
struct CDINGFtdcQryOrderField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///用户代码
	TDINGFtdcUserIDType	UserID;
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
	///报单编号
	TDINGFtdcOrderSysIDType	OrderSysID;
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
};
///成交查询
struct CDINGFtdcQryTradeField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///用户代码
	TDINGFtdcUserIDType	UserID;
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
	///成交编号
	TDINGFtdcTradeIDType	TradeID;
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
};
///合约查询
struct CDINGFtdcQryInstrumentField
{
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///产品代码
	TDINGFtdcProductIDType	ProductID;
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
};
///合约查询应答
struct CDINGFtdcRspInstrumentField
{
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///品种代码
	TDINGFtdcProductIDType	ProductID;
	///品种名称
	TDINGFtdcProductNameType	ProductName;
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
	///合约名称
	TDINGFtdcInstrumentNameType	InstrumentName;
	///交割年份
	TDINGFtdcYearType	DeliveryYear;
	///交割月
	TDINGFtdcMonthType	DeliveryMonth;
	///限价单最大下单量
	TDINGFtdcVolumeType	MaxLimitOrderVolume;
	///限价单最小下单量
	TDINGFtdcVolumeType	MinLimitOrderVolume;
	///市价单最大下单量
	TDINGFtdcVolumeType	MaxMarketOrderVolume;
	///市价单最小下单量
	TDINGFtdcVolumeType	MinMarketOrderVolume;
	///数量乘数
	TDINGFtdcVolumeMultipleType	VolumeMultiple;
	///报价单位
	TDINGFtdcPriceTickType	PriceTick;
	///币种
	TDINGFtdcCurrencyType	Currency;
	///多头限仓
	TDINGFtdcVolumeType	LongPosLimit;
	///空头限仓
	TDINGFtdcVolumeType	ShortPosLimit;
	///跌停板价
	TDINGFtdcPriceType	LowerLimitPrice;
	///涨停板价
	TDINGFtdcPriceType	UpperLimitPrice;
	///昨结算
	TDINGFtdcPriceType	PreSettlementPrice;
	///合约交易状态
	TDINGFtdcInstrumentStatusType	InstrumentStatus;
	///创建日
	TDINGFtdcDateType	CreateDate;
	///上市日
	TDINGFtdcDateType	OpenDate;
	///到期日
	TDINGFtdcDateType	ExpireDate;
	///开始交割日
	TDINGFtdcDateType	StartDelivDate;
	///最后交割日
	TDINGFtdcDateType	EndDelivDate;
	///挂牌基准价
	TDINGFtdcPriceType	BasisPrice;
	///当前是否交易
	TDINGFtdcBoolType	IsTrading;
	///基础商品代码
	TDINGFtdcInstrumentIDType	UnderlyingInstrID;
	///基础商品乘数
	TDINGFtdcUnderlyingMultipleType	UnderlyingMultiple;
	///持仓类型
	TDINGFtdcPositionTypeType	PositionType;
	///执行价
	TDINGFtdcPriceType	StrikePrice;
	///期权类型
	TDINGFtdcOptionsTypeType	OptionsType;
	///币种代码
	TDINGFtdcCurrencyIDType	CurrencyID;
};
///合约状态
struct CDINGFtdcInstrumentStatusField
{
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///品种代码
	TDINGFtdcProductIDType	ProductID;
	///品种名称
	TDINGFtdcProductNameType	ProductName;
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
	///合约名称
	TDINGFtdcInstrumentNameType	InstrumentName;
	///交割年份
	TDINGFtdcYearType	DeliveryYear;
	///交割月
	TDINGFtdcMonthType	DeliveryMonth;
	///限价单最大下单量
	TDINGFtdcVolumeType	MaxLimitOrderVolume;
	///限价单最小下单量
	TDINGFtdcVolumeType	MinLimitOrderVolume;
	///市价单最大下单量
	TDINGFtdcVolumeType	MaxMarketOrderVolume;
	///市价单最小下单量
	TDINGFtdcVolumeType	MinMarketOrderVolume;
	///数量乘数
	TDINGFtdcVolumeMultipleType	VolumeMultiple;
	///报价单位
	TDINGFtdcPriceTickType	PriceTick;
	///币种
	TDINGFtdcCurrencyType	Currency;
	///多头限仓
	TDINGFtdcVolumeType	LongPosLimit;
	///空头限仓
	TDINGFtdcVolumeType	ShortPosLimit;
	///跌停板价
	TDINGFtdcPriceType	LowerLimitPrice;
	///涨停板价
	TDINGFtdcPriceType	UpperLimitPrice;
	///昨结算
	TDINGFtdcPriceType	PreSettlementPrice;
	///合约交易状态
	TDINGFtdcInstrumentStatusType	InstrumentStatus;
	///创建日
	TDINGFtdcDateType	CreateDate;
	///上市日
	TDINGFtdcDateType	OpenDate;
	///到期日
	TDINGFtdcDateType	ExpireDate;
	///开始交割日
	TDINGFtdcDateType	StartDelivDate;
	///最后交割日
	TDINGFtdcDateType	EndDelivDate;
	///挂牌基准价
	TDINGFtdcPriceType	BasisPrice;
	///当前是否交易
	TDINGFtdcBoolType	IsTrading;
	///基础商品代码
	TDINGFtdcInstrumentIDType	UnderlyingInstrID;
	///基础商品乘数
	TDINGFtdcUnderlyingMultipleType	UnderlyingMultiple;
	///持仓类型
	TDINGFtdcPositionTypeType	PositionType;
	///执行价
	TDINGFtdcPriceType	StrikePrice;
	///期权类型
	TDINGFtdcOptionsTypeType	OptionsType;
	///币种代码
	TDINGFtdcCurrencyIDType	CurrencyID;
	///进入本状态日期
	TDINGFtdcDateType	EnterDate;
};
///投资者资金查询
struct CDINGFtdcQryInvestorAccountField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///用户代码
	TDINGFtdcUserIDType	UserID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
};
///投资者资金应答
struct CDINGFtdcRspInvestorAccountField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
	///资金帐号
	TDINGFtdcAccountIDType	AccountID;
	///上次结算准备金
	TDINGFtdcMoneyType	PreBalance;
	///入金金额
	TDINGFtdcMoneyType	Deposit;
	///出金金额
	TDINGFtdcMoneyType	Withdraw;
	///冻结的保证金
	TDINGFtdcMoneyType	FrozenMargin;
	///冻结手续费
	TDINGFtdcMoneyType	FrozenFee;
	///冻结权利金
	TDINGFtdcMoneyType	FrozenPremium;
	///手续费
	TDINGFtdcMoneyType	Fee;
	///平仓盈亏
	TDINGFtdcMoneyType	CloseProfit;
	///持仓盈亏
	TDINGFtdcMoneyType	PositionProfit;
	///可用资金
	TDINGFtdcMoneyType	Available;
	///多头冻结的保证金
	TDINGFtdcMoneyType	LongFrozenMargin;
	///空头冻结的保证金
	TDINGFtdcMoneyType	ShortFrozenMargin;
	///多头占用保证金
	TDINGFtdcMoneyType	LongMargin;
	///空头占用保证金
	TDINGFtdcMoneyType	ShortMargin;
	///当日释放保证金
	TDINGFtdcMoneyType	ReleaseMargin;
	///动态权益
	TDINGFtdcMoneyType	DynamicRights;
	///今日出入金
	TDINGFtdcMoneyType	TodayInOut;
	///占用保证金
	TDINGFtdcMoneyType	Margin;
	///期权权利金收支
	TDINGFtdcMoneyType	Premium;
	///风险度
	TDINGFtdcMoneyType	Risk;
};
///可用投资者查询
struct CDINGFtdcQryUserInvestorField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///用户代码
	TDINGFtdcUserIDType	UserID;
};
///可用投资者
struct CDINGFtdcRspUserInvestorField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///交易用户代码
	TDINGFtdcUserIDType	UserID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
};
///交易编码查询
struct CDINGFtdcQryTradingCodeField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///用户代码
	TDINGFtdcUserIDType	UserID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
};
///交易编码查询
struct CDINGFtdcRspTradingCodeField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
	///客户代码
	TDINGFtdcClientIDType	ClientID;
	///客户代码权限
	TDINGFtdcTradingRightType	ClientRight;
	///是否活跃
	TDINGFtdcIsActiveType	IsActive;
};
///交易所查询请求
struct CDINGFtdcQryExchangeField
{
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
};
///交易所查询应答
struct CDINGFtdcRspExchangeField
{
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///交易所名称
	TDINGFtdcExchangeNameType	ExchangeName;
};
///投资者持仓查询请求
struct CDINGFtdcQryInvestorPositionField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///用户代码
	TDINGFtdcUserIDType	UserID;
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
};

///投资者结算结果确认信息
struct CDINGFtdcSettlementInfoConfirmField
{
	///经纪公司代码
	TDINGFtdcBrokerIDType	BrokerID;
	///投资者代码
	TDINGFtdcInvestorIDType	InvestorID;
	///确认日期
	TDINGFtdcDateType	ConfirmDate;
	///确认时间
	TDINGFtdcTimeType	ConfirmTime;
};
///投资者持仓查询应答
struct CDINGFtdcRspInvestorPositionField
{
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///客户代码
	TDINGFtdcClientIDType	ClientID;
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
	///买卖方向
	TDINGFtdcDirectionType	Direction;
	///投机套保标志
	TDINGFtdcHedgeFlagType	HedgeFlag;
	///占用保证金
	TDINGFtdcMoneyType	UsedMargin;
	///今持仓量
	TDINGFtdcVolumeType	Position;
	///今日持仓成本
	TDINGFtdcPriceType	PositionCost;
	///昨持仓量
	TDINGFtdcVolumeType	YdPosition;
	///昨日持仓成本
	TDINGFtdcMoneyType	YdPositionCost;
	///冻结的保证金
	TDINGFtdcMoneyType	FrozenMargin;
	///开仓冻结持仓
	TDINGFtdcVolumeType	FrozenPosition;
	///平仓冻结持仓
	TDINGFtdcVolumeType	FrozenClosing;
	///平昨仓冻结持仓
	TDINGFtdcVolumeType	YdFrozenClosing;
	///冻结的权利金
	TDINGFtdcMoneyType	FrozenPremium;
	///最后一笔成交编号
	TDINGFtdcTradeIDType	LastTradeID;
	///最后一笔本地报单编号
	TDINGFtdcOrderLocalIDType	LastOrderLocalID;
	///币种
	TDINGFtdcCurrencyIDType	Currency;
};
///合规参数查询请求
struct CDINGFtdcQryComplianceParamField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///用户代码
	TDINGFtdcUserIDType	UserID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
};
///合规参数查询应答
struct CDINGFtdcRspComplianceParamField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///客户号
	TDINGFtdcClientIDType	ClientID;
	///每日最大报单笔
	TDINGFtdcVolumeType	DailyMaxOrder;
	///每日最大撤单笔
	TDINGFtdcVolumeType	DailyMaxOrderAction;
	///每日最大错单笔
	TDINGFtdcVolumeType	DailyMaxErrorOrder;
	///每日最大报单手
	TDINGFtdcVolumeType	DailyMaxOrderVolume;
	///每日最大撤单手
	TDINGFtdcVolumeType	DailyMaxOrderActionVolume;
};
///用户查询
struct CDINGFtdcQryUserField
{
	///交易用户代码
	TDINGFtdcUserIDType	StartUserID;
	///交易用户代码
	TDINGFtdcUserIDType	EndUserID;
};
///用户
struct CDINGFtdcUserField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///用户代码
	TDINGFtdcUserIDType	UserID;
	///用户登录密码
	TDINGFtdcPasswordType	Password;
	///是否活跃
	TDINGFtdcIsActiveType	IsActive;
	///用户名称
	TDINGFtdcUserNameType	UserName;
	///用户类型
	TDINGFtdcUserTypeType	UserType;
	///营业部
	TDINGFtdcDepartmentType	Department;
	///授权功能集
	TDINGFtdcGrantFuncSetType	GrantFuncSet;
	///修改用户编号
	TDINGFtdcUserIDType	SetUserID;
	///操作日期
	TDINGFtdcDateType	CommandDate;
	///操作时间
	TDINGFtdcTimeType	CommandTime;
};
///投资者手续费率查询
struct CDINGFtdcQryInvestorFeeField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///用户代码
	TDINGFtdcUserIDType	UserID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
};
///投资者手续费率
struct CDINGFtdcInvestorFeeField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///客户号
	TDINGFtdcClientIDType	ClientID;
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
	///品种代码
	TDINGFtdcProductIDType	ProductID;
	///开仓手续费按比例
	TDINGFtdcRatioType	OpenFeeRate;
	///开仓手续费按手数
	TDINGFtdcRatioType	OpenFeeAmt;
	///平仓手续费按比例
	TDINGFtdcRatioType	OffsetFeeRate;
	///平仓手续费按手数
	TDINGFtdcRatioType	OffsetFeeAmt;
	///平今仓手续费按比例
	TDINGFtdcRatioType	OTFeeRate;
	///平今仓手续费按手数
	TDINGFtdcRatioType	OTFeeAmt;
};
///投资者保证金率查询
struct CDINGFtdcQryInvestorMarginField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///用户代码
	TDINGFtdcUserIDType	UserID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
};
///投资者保证金率
struct CDINGFtdcInvestorMarginField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///客户号
	TDINGFtdcClientIDType	ClientID;
	///交易所代码
	TDINGFtdcExchangeIDType	ExchangeID;
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
	///品种代码
	TDINGFtdcProductIDType	ProductID;
	///多头占用保证金按比例
	TDINGFtdcRatioType	LongMarginRate;
	///多头保证金按手数
	TDINGFtdcRatioType	LongMarginAmt;
	///空头占用保证金按比例
	TDINGFtdcRatioType	ShortMarginRate;
	///空头保证金按手数
	TDINGFtdcRatioType	ShortMarginAmt;
};
///成交
struct CDINGFtdcTradeField
{
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
};

///数据流回退
struct CDINGFtdcFlowMessageCancelField
{
	///序列系列号
	TDINGFtdcSequenceSeriesType	SequenceSeries;
	///交易日
	TDINGFtdcDateType	TradingDay;
	///数据中心代码
	TDINGFtdcDataCenterIDType	DataCenterID;
	///回退起始序列号
	TDINGFtdcSequenceNoType	StartSequenceNo;
	///回退结束序列号
	TDINGFtdcSequenceNoType	EndSequenceNo;
};
///信息分发
struct CDINGFtdcDisseminationField
{
	///序列系列号
	TDINGFtdcSequenceSeriesType	SequenceSeries;
	///序列号
	TDINGFtdcSequenceNoType	SequenceNo;
};
///出入金结果
struct CDINGFtdcInvestorAccountDepositResField
{
	///经纪公司编号
	TDINGFtdcBrokerIDType	BrokerID;
	///用户代码
	TDINGFtdcUserIDType	UserID;
	///投资者编号
	TDINGFtdcInvestorIDType	InvestorID;
	///资金帐号
	TDINGFtdcAccountIDType	AccountID;
	///资金流水号
	TDINGFtdcAccountSeqNoType	AccountSeqNo;
	///金额
	TDINGFtdcMoneyType	Amount;
	///出入金方向
	TDINGFtdcAccountDirectionType	AmountDirection;
	///可用资金
	TDINGFtdcMoneyType	Available;
	///结算准备金
	TDINGFtdcMoneyType	Balance;
};
///行情基础属性
struct CDINGFtdcMarketDataBaseField
{
	///交易日
	TDINGFtdcDateType	TradingDay;
	///结算组代码
	TDINGFtdcSettlementGroupIDType	SettlementGroupID;
	///结算编号
	TDINGFtdcSettlementIDType	SettlementID;
	///昨结算
	TDINGFtdcPriceType	PreSettlementPrice;
	///昨收盘
	TDINGFtdcPriceType	PreClosePrice;
	///昨持仓量
	TDINGFtdcLargeVolumeType	PreOpenInterest;
	///昨虚实度
	TDINGFtdcRatioType	PreDelta;
};
///行情静态属性
struct CDINGFtdcMarketDataStaticField
{
	///今开盘
	TDINGFtdcPriceType	OpenPrice;
	///最高价
	TDINGFtdcPriceType	HighestPrice;
	///最低价
	TDINGFtdcPriceType	LowestPrice;
	///今收盘
	TDINGFtdcPriceType	ClosePrice;
	///涨停板价
	TDINGFtdcPriceType	UpperLimitPrice;
	///跌停板价
	TDINGFtdcPriceType	LowerLimitPrice;
	///今结算
	TDINGFtdcPriceType	SettlementPrice;
	///今虚实度
	TDINGFtdcRatioType	CurrDelta;
};
///行情最新成交属性
struct CDINGFtdcMarketDataLastMatchField
{
	///最新价
	TDINGFtdcPriceType	LastPrice;
	///数量
	TDINGFtdcVolumeType	Volume;
	///成交金额
	TDINGFtdcMoneyType	Turnover;
	///持仓量
	TDINGFtdcLargeVolumeType	OpenInterest;
};
///行情最优价属性
struct CDINGFtdcMarketDataBestPriceField
{
	///申买价一
	TDINGFtdcPriceType	BidPrice1;
	///申买量一
	TDINGFtdcVolumeType	BidVolume1;
	///申卖价一
	TDINGFtdcPriceType	AskPrice1;
	///申卖量一
	TDINGFtdcVolumeType	AskVolume1;
};
///行情申买二、三属性
struct CDINGFtdcMarketDataBid23Field
{
	///申买价二
	TDINGFtdcPriceType	BidPrice2;
	///申买量二
	TDINGFtdcVolumeType	BidVolume2;
	///申买价三
	TDINGFtdcPriceType	BidPrice3;
	///申买量三
	TDINGFtdcVolumeType	BidVolume3;
};
///行情申卖二、三属性
struct CDINGFtdcMarketDataAsk23Field
{
	///申卖价二
	TDINGFtdcPriceType	AskPrice2;
	///申卖量二
	TDINGFtdcVolumeType	AskVolume2;
	///申卖价三
	TDINGFtdcPriceType	AskPrice3;
	///申卖量三
	TDINGFtdcVolumeType	AskVolume3;
};
///行情申买四、五属性
struct CDINGFtdcMarketDataBid45Field
{
	///申买价四
	TDINGFtdcPriceType	BidPrice4;
	///申买量四
	TDINGFtdcVolumeType	BidVolume4;
	///申买价五
	TDINGFtdcPriceType	BidPrice5;
	///申买量五
	TDINGFtdcVolumeType	BidVolume5;
};
///行情申卖四、五属性
struct CDINGFtdcMarketDataAsk45Field
{
	///申卖价四
	TDINGFtdcPriceType	AskPrice4;
	///申卖量四
	TDINGFtdcVolumeType	AskVolume4;
	///申卖价五
	TDINGFtdcPriceType	AskPrice5;
	///申卖量五
	TDINGFtdcVolumeType	AskVolume5;
};
///行情更新时间属性
struct CDINGFtdcMarketDataUpdateTimeField
{
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
	///最后修改时间
	TDINGFtdcTimeType	UpdateTime;
	///最后修改毫秒
	TDINGFtdcMillisecType	UpdateMillisec;
	///业务发生日期
	TDINGFtdcDateType	ActionDay;
};
///深度行情
struct CDINGFtdcDepthMarketDataField
{
	///交易日
	TDINGFtdcDateType	TradingDay;
	///结算组代码
	TDINGFtdcSettlementGroupIDType	SettlementGroupID;
	///结算编号
	TDINGFtdcSettlementIDType	SettlementID;
	///昨结算
	TDINGFtdcPriceType	PreSettlementPrice;
	///昨收盘
	TDINGFtdcPriceType	PreClosePrice;
	///昨持仓量
	TDINGFtdcLargeVolumeType	PreOpenInterest;
	///昨虚实度
	TDINGFtdcRatioType	PreDelta;
	///今开盘
	TDINGFtdcPriceType	OpenPrice;
	///最高价
	TDINGFtdcPriceType	HighestPrice;
	///最低价
	TDINGFtdcPriceType	LowestPrice;
	///今收盘
	TDINGFtdcPriceType	ClosePrice;
	///涨停板价
	TDINGFtdcPriceType	UpperLimitPrice;
	///跌停板价
	TDINGFtdcPriceType	LowerLimitPrice;
	///今结算
	TDINGFtdcPriceType	SettlementPrice;
	///今虚实度
	TDINGFtdcRatioType	CurrDelta;
	///最新价
	TDINGFtdcPriceType	LastPrice;
	///数量
	TDINGFtdcVolumeType	Volume;
	///成交金额
	TDINGFtdcMoneyType	Turnover;
	///持仓量
	TDINGFtdcLargeVolumeType	OpenInterest;
	///申买价一
	TDINGFtdcPriceType	BidPrice1;
	///申买量一
	TDINGFtdcVolumeType	BidVolume1;
	///申卖价一
	TDINGFtdcPriceType	AskPrice1;
	///申卖量一
	TDINGFtdcVolumeType	AskVolume1;
	///申买价二
	TDINGFtdcPriceType	BidPrice2;
	///申买量二
	TDINGFtdcVolumeType	BidVolume2;
	///申买价三
	TDINGFtdcPriceType	BidPrice3;
	///申买量三
	TDINGFtdcVolumeType	BidVolume3;
	///申卖价二
	TDINGFtdcPriceType	AskPrice2;
	///申卖量二
	TDINGFtdcVolumeType	AskVolume2;
	///申卖价三
	TDINGFtdcPriceType	AskPrice3;
	///申卖量三
	TDINGFtdcVolumeType	AskVolume3;
	///申买价四
	TDINGFtdcPriceType	BidPrice4;
	///申买量四
	TDINGFtdcVolumeType	BidVolume4;
	///申买价五
	TDINGFtdcPriceType	BidPrice5;
	///申买量五
	TDINGFtdcVolumeType	BidVolume5;
	///申卖价四
	TDINGFtdcPriceType	AskPrice4;
	///申卖量四
	TDINGFtdcVolumeType	AskVolume4;
	///申卖价五
	TDINGFtdcPriceType	AskPrice5;
	///申卖量五
	TDINGFtdcVolumeType	AskVolume5;
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
	///最后修改时间
	TDINGFtdcTimeType	UpdateTime;
	///最后修改毫秒
	TDINGFtdcMillisecType	UpdateMillisec;
	///业务发生日期
	TDINGFtdcDateType	ActionDay;
};
///订阅合约的相关信息
struct CDINGFtdcSpecificInstrumentField
{
	///合约代码
	TDINGFtdcInstrumentIDType	InstrumentID;
};
///多播通道心跳
struct CDINGFtdcMultiChannelHeartBeatField
{
	///心跳超时时间（秒）
	TDINGFtdcVolumeType	DINGMultiChannelHeartBeatTimeOut;
};

struct CDINGFtdcDepthMarketDataFieldTMEX
{
	CDINGFtdcDepthMarketDataField main;
	long timeMark;
};

struct CDINGFtdcOrderFieldEX
{
	CDINGFtdcOrderField main;
	std::map<std::string, void *> exmaps;
	double closeprice;
};


#endif
