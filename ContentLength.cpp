#include "ContentLength.h"
#include "ReadMsg.h"



int contentLength(string headerMsg) {
	int pos = (int)headerMsg.find("Content-Length: ");
	pos += 16;
	string length = "";
	while ('0' <= headerMsg[pos] && headerMsg[pos] <= '9')
		length += headerMsg[pos++];
	return stoi(length);
}

string readMsgData(SOCKET socket, string headerMsg) {
	int length = contentLength(headerMsg);
	return readData(socket,length);
}