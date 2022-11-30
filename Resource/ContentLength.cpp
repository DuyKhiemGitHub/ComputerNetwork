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

bool readMsgDataAndSave(SOCKET socket, string headerMsg, string path, string fileName) {
	int length = contentLength(headerMsg);

	char* buffer = new char[BUFFER_SIZE];
	ofstream ofs(path + fileName, ios::binary);
	int totalBytes = 0;
	time_t t = clock();
	do {
		int bytesReceived = recv(socket, buffer, min(BUFFER_SIZE, length - totalBytes), 0);
		if (bytesReceived == SOCKET_ERROR && totalBytes < length) {
			ofs.close();
			deleteAFile(path + fileName);
			cout << ">> Couldn't load file " << fileName << endl;
			return false;
		}
		totalBytes += bytesReceived;
		ofs.write(buffer, bytesReceived);
	} while (totalBytes < length);
	delete[] buffer;
	ofs.close();
	cout << ">> Loaded file " << fileName << " successfully. Time: " << (double)(clock() - t) / CLOCKS_PER_SEC << endl;
	return true;
}