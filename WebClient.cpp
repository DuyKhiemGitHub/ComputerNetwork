#include "WebClient.h"
#include "Chunked.h"
#include "ContentLength.h"
#include "ReadMsg.h"


SOCKET createSocket() {
	SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	return Socket;
}


string getDomainName(ifstream& ifs) {
	string res = "";
	getline(ifs, res, '/');
	return res;
}

string getPath(ifstream& ifs) {
	string path = "";
	getline(ifs, path, '\n');
	return path;
}

string getFileName(string path) {
	int i = (int)path.size();
	if (i == 0) return "";
	while (path[--i] != '/');
	if (i == path.size() - 1) return "";
	return path.substr(i + 1, path.size() - 1);
}

string getIpAddress(ifstream& ifs) {
	string ipAddress = "";
	getline(ifs, ipAddress, '\n');
	return ipAddress;
}

void receiveAFile(SOCKET socket, string domainName, string path, string fileName) {
	string requestToServer = "GET " + path + " HTTP/1.1\r\nHost: " + domainName + "\r\n\r\n";
	send(socket, requestToServer.c_str(), (int)requestToServer.size() + 1, 0);
	string headerMsg = readHeaderMsg(socket);
	string bodyMsg = "";
	if (return_ContentLength_Or_ChunkedTranferEncoding(headerMsg) == "Content-Length")
		bodyMsg = readMsgData(socket, headerMsg);
	else bodyMsg = readChunkedData(socket);
	ofstream ofs(fileName, ios::binary);
	ofs.write(bodyMsg.c_str(), bodyMsg.size());	ofs.close();
	cout << ">> Loaded file : " << "\"" << fileName << "\" successful" << endl;
}

void receiveSubFolder(vector<string> vector_FileName,SOCKET socket, string domainName, string path) {
	for (auto x : vector_FileName) {
		receiveAFile(socket, domainName, path + "/" + x, x);
	}
}