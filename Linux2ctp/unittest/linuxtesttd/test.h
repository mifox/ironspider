#ifndef TEST_H_
#define TEST_H_
#include "PublicFuncs.h"
#include "config.h"
#include "api/trade/win/public/ThostFtdcTraderApi.h"

#include "order/traderspi.h"

#include <iostream>

using namespace std;

extern CountedPtr<Car> car;

void ShowTraderCommand(CtpTraderSpi* p, bool print=false);


#endif