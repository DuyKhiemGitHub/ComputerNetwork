#pragma once


#include "WebClient.h"
#include <regex>

string readData(SOCKET socket, int size) ;



unsigned int convertHexToDec(const std::string& hex) ;


string readALine(SOCKET socket);

string return_ContentLength_Or_ChunkedTranferEncoding(string headerMsg);

string readHeaderMsg(SOCKET socket);

vector<string> returnFileNameInSubfolder(string Msg);

string returnNameOfFile(string source);