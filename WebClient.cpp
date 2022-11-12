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

string getIpAddress(string domainName) {
	string ipAddress = "";


	DWORD dwRetval;
	// Setup the hints address info structure
// which is passed to the getaddrinfo() function

	struct addrinfo* result = NULL;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	dwRetval = getaddrinfo(domainName.c_str(), NULL, &hints, &result);

	if (dwRetval != 0) {
		cout << "getaddrinfo failed with error: " << dwRetval << endl;
		return "";
	}

	printf("getaddrinfo returned success\n");

	struct sockaddr_in* sockaddr_ipv4;
	sockaddr_ipv4 = (sockaddr_in*)result->ai_addr;
	ipAddress = inet_ntoa(sockaddr_ipv4->sin_addr);
	cout << "IPv4 address " << ipAddress << endl;

	freeaddrinfo(result);
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
	ofs.write(bodyMsg.c_str(), bodyMsg.size());
	cout << ">> Loaded file : " << "\"" << fileName << "\" successful" << endl;
	ofs.close();
}

void receiveSubFolder( SOCKET socket, string domainName, string path) {
	//string headerMsg = readHeaderMsg(socket);
	//string bodyMsg = "";
	//bodyMsg = readMsgData(socket, headerMsg);
	//for (auto x : vector_FileName) {
	//	receiveAFile(socket, domainName, path + "/" + x, x); 
	//}
}