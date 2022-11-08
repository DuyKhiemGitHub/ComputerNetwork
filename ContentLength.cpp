#include "ContentLength.h"



int contentLength(string headerMsg) {
	int pos = headerMsg.find("Content-Length: ");
	pos += 16;
	string length = "";
	while ('0' <= headerMsg[pos] && headerMsg[pos] <= '9')
		length += headerMsg[pos++];
	return stoi(length);
}

string readMsgData(int socket, string headerMsg) {
	int length = contentLength(headerMsg);
	char* Msg = new char[BUFFER_SIZE];

	string resString = "";
	int totalBytesReceived = 0;
	do {
		int bytesReceived = recv(socket, Msg, BUFFER_SIZE, 0);
		totalBytesReceived += bytesReceived;
		resString += string(Msg, bytesReceived);
	} while (totalBytesReceived < length);
	resString.resize(length);
	delete[] Msg;
	return resString;
}