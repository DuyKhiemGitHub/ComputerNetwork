#pragma once
#include "WebClient.h"
#include "ReadMsg.h"


bool readChunkedDataAndSave(SOCKET socket, string path, string fileName);

string readChunkedData(SOCKET socket);