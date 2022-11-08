#pragma once

#include "WebClient.h"


int contentLength(string headerMsg);

string readMsgData(SOCKET socket, string headerMsg);
