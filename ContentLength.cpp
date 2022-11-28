#include "ContentLength.h"
#include "ReadMsg.h"
#include "WebClient.h"



int contentLength(string headerMsg) {
	int pos = (int)headerMsg.find("Content-Length: ");
	pos += 16;
	string length = "";
	while ('0' <= headerMsg[pos] && headerMsg[pos] <= '9')
		length += headerMsg[pos++];
	return stoi(length);
}

void readMsgDataAndSave(SOCKET socket, string headerMsg, string path, string fileName) {
	int length = contentLength(headerMsg);

	char* buffer = new char[BUFFER_SIZE];
	ofstream ofs(path + fileName, ios::binary);
	int totalBytes = 0;
	do {
		int bytesReceived = recv(socket, buffer, min(BUFFER_SIZE, length - totalBytes), 0);
		totalBytes += bytesReceived;
		if (bytesReceived < 0 && totalBytes < length) {
			deleteAFile(path + fileName);
			cout << ">> Couldn't load file " << fileName << endl;
		}
		ofs.write(buffer, bytesReceived);
	} while (totalBytes < length);
	delete[] buffer;
	ofs.close();
	cout << ">> Loaded file " << fileName << " successfully" << endl;
}