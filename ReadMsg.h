#pragma once


#include "WebClient.h"

string readData(SOCKET socket, int size) ;


unsigned int convertHexToDec(const std::string& hex) ;


string readALine(SOCKET socket);

string return_ContentLength_Or_ChunkedTranferEncoding(string headerMsg);

string readHeaderMsg(SOCKET socket);