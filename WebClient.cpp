#include "WebClient.h"
#include "Chunked.h"
#include "ContentLength.h"
#include "ReadMsg.h"



SOCKET createSocket() {
	SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	return Socket;
}

void deleteAFile(string path) {
	wstring wFileName(path.begin(), path.end());
	DeleteFile(wFileName.c_str());
}


void parseURLString(string URL, string& domainName, string& path, string& fileName) {
	int k = 0;
	while (URL[k++] == ' ') URL = URL.substr(1);
	k = (int)URL.length() - 1;
	while (URL[k--] == ' ') URL.pop_back();
	if (URL[0] == 'h' && URL[1] == 't' && URL[2] == 't' && URL[3] == 'p' && URL[4] == ':' && URL[5] == '/' && URL[6] == '/') URL = URL.substr(7);
	if (URL[0] == 'h' && URL[1] == 't' && URL[2] == 't' && URL[3] == 'p' && URL[4] == 's' && URL[5] == ':' && URL[6] == '/' && URL[7] == '/') URL = URL.substr(8);


	int i = 0;
	int length = (int)URL.length();
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

	if (dwRetval != 0)
		return "";

	for (addrinfo* ptr = result; ptr != NULL;ptr = ptr->ai_next) {
		if (ptr->ai_family == AF_INET) {
			struct sockaddr_in* sockaddr_ipv4;
			sockaddr_ipv4 = (sockaddr_in*)ptr->ai_addr;
			ipAddress = inet_ntoa(sockaddr_ipv4->sin_addr);
			return ipAddress;
		}
	}

	freeaddrinfo(result);
	return "";
}


bool sendRequestToServer(SOCKET socket, string request) {
	if (send(socket, request.c_str(), (int)request.size() + 1, 0) < 0)
		return false;
	return true;
}

void receiveAFile(SOCKET socket, string path, string fileName, string domainName) {
	string headerMsg = readHeaderMsg(socket);
	int i = 9;
	string temp = "";
	while (headerMsg[i] != ' ') temp += headerMsg[i++];
	if (temp != "200") {
		string error = "";
		while (headerMsg[++i] != '\r') error += headerMsg[i];
		cout << ">> Coudldn't receive file from host " << domainName << ". Error: " << temp << " - " << error << endl;
		return;
	}
	if (return_ContentLength_Or_ChunkedTranferEncoding(headerMsg) == "Content-Length")
		readMsgDataAndSave(socket, headerMsg, path, fileName);
	else readChunkedDataAndSave(socket, path, fileName);

}



bool receiveSubFolder(vector<string> vector_fileName, string domainName, string IP, string path, string subFolderName) {
	string directoryName = domainName + "_" + subFolderName;
	int status = _mkdir(directoryName.c_str());
	// TO DO sth...


	SOCKET connectSocket = createSocket();
	if (connectSocket == INVALID_SOCKET) {
		cout << ">> Couldn't create listening socket to host " << domainName << " to get folder " << subFolderName << endl;
		cout << "Error: " << WSAGetLastError << endl;
		return false;
	}

	// Set address
	sockaddr_in connectSocketAddress;
	connectSocketAddress.sin_family = AF_INET;
	connectSocketAddress.sin_port = htons(DEFAULT_PORT);
	connectSocketAddress.sin_addr.s_addr = inet_addr(IP.c_str());



	char* currentPath = NULL;

	if (!(currentPath = _getcwd(NULL, 0))) {
		cout << ">> Couldn't get path to binary file!" << endl;
		return false;
	}

	string requestToServer = "";
	if (connect(connectSocket, (sockaddr*)&connectSocketAddress, sizeof(connectSocketAddress)) == 0) {
		cout << ">> Client connected to host: " << domainName << " to get all file in folder " << subFolderName << endl;
		for (int i = 0;i < vector_fileName.size();i++) {
			requestToServer = requestToServer + "GET " + path + vector_fileName[i] + " HTTP/1.1\r\nHost: " + domainName + " \r\n\r\n";
		}
		if (!sendRequestToServer(connectSocket, requestToServer)) {
			cout << ">> Server " << domainName << " couldn't receive your request!" << endl;
			return false;
		};

		for (int i = 0;i < vector_fileName.size();i++) {
			receiveAFile(connectSocket, string(currentPath, strlen(currentPath)) + "\\" + directoryName + "\\", vector_fileName[i],domainName);
		}
	}
	else {
		cout << ">> Couldn't connect to host: " << domainName << endl;
		cout << "Error: " << WSAGetLastError << endl;
		return false;
	}
	if (closesocket(connectSocket) != 0) {
		cout << ">> Couldn't close connection to host " << domainName << endl;
		cout << "Error: " << WSAGetLastError << endl;
	}
	else cout << ">> Closed connection to host " << domainName << endl;
	return true;
}




void handleSocket(string URL) {
	string domainName = "";
	string path = "";
	string fileName = "";
	parseURLString(URL, domainName, path, fileName);
	string IpAddress = getIpAddressFromDomainName(domainName);

	if (IpAddress == "") {
		cout << ">> Couldn't get IP address of host " << domainName << endl;
		cout << "Error: " << WSAGetLastError << endl;
		return;
	}

	//Create a Socket to Listen
	SOCKET connectSocket = createSocket();
	if (connectSocket == INVALID_SOCKET) {
		cout << ">> Couldn't create listening socket to host " << domainName << endl;
		cout << "Error: " << WSAGetLastError << endl;
		return;
	}

	// Set address
	sockaddr_in connectSocketAddress;
	connectSocketAddress.sin_family = AF_INET;
	connectSocketAddress.sin_port = htons(DEFAULT_PORT);
	connectSocketAddress.sin_addr.s_addr = inet_addr(IpAddress.c_str());
	if (connect(connectSocket, (sockaddr*)&connectSocketAddress, sizeof(connectSocketAddress)) == 0) {
		cout << ">> Client connected to host " << domainName << endl;
		string requestToServer = "GET " + path + " HTTP/1.1\r\nHost: " + domainName + "\r\n\r\n";
		if (!sendRequestToServer(connectSocket, requestToServer)) {
			cout << ">> Host " << domainName << " couldn't receive your request!" << endl;
			cout << "Error: " << WSAGetLastError << endl;
			return;
		};


		if (fileName != "") {

			string fileNameToSave = path;
			bool check = false;
			for (auto& i : fileNameToSave) {
				if (i == '.') {
					check = true;
					break;
				}
				if (i == '/')
					i = '_';
			}
			if (fileNameToSave[fileNameToSave.length() - 1] == '_') fileNameToSave.pop_back();
			if (check)
				receiveAFile(connectSocket, "", domainName + fileNameToSave,domainName);
			else
				receiveAFile(connectSocket, "", domainName + fileNameToSave + "_" + fileName,domainName);
		}
		else {
			string subFolderName = "";
			int i = (int)path.size() - 1;
			while (path[--i] != '/');
			for (i++;i < path.size() - 2;i++)
				subFolderName += path[i];

			string headerOfResponse = readHeaderMsg(connectSocket);
			string bodyOfResponse = "";
			if (return_ContentLength_Or_ChunkedTranferEncoding(headerOfResponse) == "Content-Length")
				bodyOfResponse = readData(connectSocket, contentLength(headerOfResponse));
			else bodyOfResponse = readChunkedData(connectSocket);
			vector<string> vector_fileName = returnFileNameInSubfolder(bodyOfResponse);
			if (receiveSubFolder(vector_fileName, domainName, IpAddress, path, subFolderName)) {
				cout << ">> Received folder " << subFolderName << " successfully " << endl;
			}
			else cout << "Couldn't receive folder " << subFolderName << endl;
		}
	}


	else {
		cout << ">> Couldn't connect to host " << domainName << endl;
		cout << "Error: " << WSAGetLastError << endl;
	}

	if (closesocket(connectSocket) != 0) {
		cout << ">> Couldn't close connection to host " << domainName << endl;
		cout << "Error: " << WSAGetLastError << endl;
	}
	else cout << ">> Closed connection to host " << domainName << endl;

	return;
}