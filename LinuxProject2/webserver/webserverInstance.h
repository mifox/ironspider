#include "webserver.h"
#include "socket.h"
#include <iostream>
using namespace std;

void StringReplace(string &strBase, string strSrc, string strDes);
void Get(webserver::http_request* r);