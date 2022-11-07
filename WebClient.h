#pragma once



 // Main header



#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <fstream>
#include <sstream>
#include <vector>



using namespace std;
#define WINSOCK_DEPRECATED_NO_WARNINGS
#define BUFFER_SIZE 20000

#pragma comment (lib,"ws2_32.lib")	
#pragma warning(disable : 4996)