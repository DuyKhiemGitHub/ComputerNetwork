#pragma once

#include "WebClient.h"


int contentLength(string headerMsg);

void readMsgDataAndSave(SOCKET socket, string headerMsg, string path, string fileName);
