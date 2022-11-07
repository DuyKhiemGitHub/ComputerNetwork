#pragma once


#include "WebClient.h"

string readData(int socket, int size) ;


unsigned int convertHexToDec(const std::string& hex) ;


string readALine(int socket);

string return_ContentLength_Or_ChunkedTranferEncoding(string headerMsg);

string readHeaderMsg(int socket);