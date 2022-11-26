#pragma once



 // Main header



#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <direct.h>



using namespace std;
#define WINSOCK_DEPRECATED_NO_WARNINGS
#define BUFFER_SIZE 20000

#pragma comment (lib,"ws2_32.lib")	
#pragma warning(disable : 4996)



SOCKET createSocket();

void parseURLString(string URL, string& domainName, string& path, string& fileName);

string getIpAddressFromDomainName(string domainName);
string receiveAFile(SOCKET socket);
bool receiveSubFolder(vector<string> vector_fileName, string domainName, string IP, string path, string subFolderName);
void saveFile(string path, string fileName, string data);
bool sendRequestToServer(SOCKET socket, string request);
void handleSocket(string URL);