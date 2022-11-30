#pragma once



 // Main header



#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <Windows.h>
#include <direct.h>
#include <time.h>



using namespace std;
#define WINSOCK_DEPRECATED_NO_WARNINGS
#define BUFFER_SIZE 20000

#pragma comment (lib,"ws2_32.lib")	
#pragma warning(disable : 4996)

const int DEFAULT_PORT = 80;



SOCKET createSocket();

void parseURLString(string URL, string& domainName, string& path, string& fileName);

string getIpAddressFromDomainName(string domainName);
bool receiveAFile(SOCKET socket,string path, string fileName,string domainName);
bool receiveSubFolder(vector<string> vector_fileName, string domainName, string IP, string path, string subFolderName);
bool sendRequestToServer(SOCKET socket, string request);
void handleSocket(string URL);


void deleteAFile(string path);