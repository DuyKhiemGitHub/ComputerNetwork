#include "WebClient.h"
#include "Chunked.h"
#include "ContentLength.h"
#include "ReadMsg.h"



SOCKET createSocket() {
	SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	return Socket;
}


void parseURLString(string URL, string& domainName, string& path, string& fileName) {
	int i = 0;
	int length = URL.length();
	while (i < length && URL[i] != '/') domainName += URL[i++];
	if (i == length || i == length - 1) {
		path = "/";
		fileName = "index.html";
		return;
	}

	while (i < length) path += URL[i++];
	if (path[path.size() - 1] == '/') {
		return;
	}


	i = (int)path.size();
	while (path[--i] != '/');
	for (i++;i < path.size();i++)
		fileName += path[i];

	bool isFileName = false;
	for (i = 0;i < fileName.size();i++)
		if (fileName[i] == '.') {
			isFileName = true;
			break;
		}

	if (!isFileName) fileName = "index.html";

}


string getIpAddressFromDomainName(string domainName) {
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


bool sendRequestToServer(SOCKET socket, string request) {
	if (send(socket, request.c_str(), (int)request.size() + 1, 0) < 0)
		return false;
	return true;
}

string receiveAFile(SOCKET socket) {

	string headerMsg = readHeaderMsg(socket);
	string bodyMsg = "";
	if (return_ContentLength_Or_ChunkedTranferEncoding(headerMsg) == "Content-Length")
		bodyMsg = readMsgData(socket, headerMsg);
	else bodyMsg = readChunkedData(socket);
	return bodyMsg;
}



void receiveSubFolder(vector<string> vector_fileName, string domainName, string IP, string path, string subFolderName) {
	string stringPath = domainName + "_" + subFolderName;
	int status = _mkdir(stringPath.c_str());
	// TO DO sth...


	SOCKET connectSocket = createSocket();
	if (connectSocket == INVALID_SOCKET) {
		cout << ">> Can't create listening socket" << endl;
		return;
	}

	// Set address
	sockaddr_in connectSocketAddress;
	connectSocketAddress.sin_family = AF_INET;
	connectSocketAddress.sin_port = htons(80);
	connectSocketAddress.sin_addr.s_addr = inet_addr(IP.c_str());



	char* currentPath = NULL;

	if ((currentPath = _getcwd(NULL, 0))) {
	}

	string requestToServer = "";
	if (connect(connectSocket, (sockaddr*)&connectSocketAddress, sizeof(connectSocketAddress)) == 0) {
		for (int i = 0;i < vector_fileName.size();i++) {
			requestToServer = requestToServer + "GET " + path + vector_fileName[i] + " HTTP/1.1\r\nHost: " + domainName + " \r\n\r\n";
		}
		sendRequestToServer(connectSocket, requestToServer);

		for (int i = 0;i < vector_fileName.size();i++) {
			string data = receiveAFile(connectSocket);
			saveFile(string(currentPath, strlen(currentPath)) + "\\" + stringPath + "\\" + vector_fileName[i], data);
		}
	}
	if (closesocket(connectSocket) == 0) {};
}

void saveFile(string path, string data) {
	ofstream ofs(path, ios::binary);
	ofs.write(data.c_str(), data.size());
	cout << ">> Loaded file : " << "\"" << path << "\" successful" << endl;
	ofs.close();
}



void handleSocket(string URL) {
	string domainName = "";
	string path = "";
	string fileName = "";
	parseURLString(URL, domainName, path, fileName);
	string IpAddress = getIpAddressFromDomainName(domainName);

	cout << domainName << " " << path << " " << fileName << " " << IpAddress << endl;
	//Create a Socket to Listen
	SOCKET connectSocket = createSocket();
	if (connectSocket == INVALID_SOCKET) {
		cout << ">> Can't create listening socket" << endl;
		return;
	}

	// Set address
	sockaddr_in connectSocketAddress;
	connectSocketAddress.sin_family = AF_INET;
	connectSocketAddress.sin_port = htons(80);
	connectSocketAddress.sin_addr.s_addr = inet_addr(IpAddress.c_str());
	if (connect(connectSocket, (sockaddr*)&connectSocketAddress, sizeof(connectSocketAddress)) == 0) {
		cout << ">> Client connected to server. " << endl;
		string requestToServer = "GET " + path + " HTTP/1.1\r\nHost: " + domainName + "\r\n\r\n";
		cout << requestToServer;
		sendRequestToServer(connectSocket, requestToServer);
		string data = receiveAFile(connectSocket);
		if (fileName != "") {
			saveFile(domainName + "_" + fileName, data);
		}
		else {
			string subFolderName = "";
			int i = path.size() - 1;
			while (path[--i] != '/');
			for (i++;i < path.size() - 2;i++)
				subFolderName += path[i];

			vector<string> vector_fileName = returnFileNameInSubfolder(data);
			receiveSubFolder(vector_fileName,domainName,IpAddress,path,subFolderName);
		}

	}

	else cout << ">> Couldn't connect to server." << endl << endl;

	if (closesocket(connectSocket) == 0) cout << ">> Closed connection" << endl;

	return;
}