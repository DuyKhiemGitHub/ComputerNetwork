#pragma once

#include "WebClient.h"


int contentLength(string headerMsg);

string readMsgData(int socket, string headerMsg);
