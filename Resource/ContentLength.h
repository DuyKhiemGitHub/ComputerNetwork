#pragma once

#include "WebClient.h"


int contentLength(string headerMsg);

bool readMsgDataAndSave(SOCKET socket, string headerMsg, string path, string fileName);
