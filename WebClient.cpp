#include "WebClient.h"
#include "Chunked.h"
#include "ContentLength.h"
#include "ReadMsg.h"



SOCKET createSocket() {
	SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	return Socket;
}


void parseURLString(string URL, string& domainName, string& path, string& fileName) {

	if (URL[0] == 'h' && URL[1] == 't' && URL[2] == 't' && URL[3] == 'p' && URL[4] == ':' && URL[5] == '/' && URL[6] == '/') URL = URL.substr(7);
	if (URL[0] == 'h' && URL[1] == 't' && URL[2] == 't' && URL[3] == 'p' && URL[4] == 's' && URL[5] == ':' && URL[6] == '/' && URL[7] == '/') URL = URL.substr(8);


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

	if (dwRetval != 0)
		return "";

	struct sockaddr_in* sockaddr_ipv4;
	sockaddr_ipv4 = (sockaddr_in*)result->ai_addr;
	ipAddress = inet_ntoa(sockaddr_ipv4->sin_addr);

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
	int i = 9;
	string temp = "";
	while (headerMsg[i] != ' ') temp += headerMsg[i++];

	if (temp != "200") {
		string error = "";
		while (headerMsg[++i] != '\r') error += headerMsg[i];
		cout << ">> Error: " << temp << " - " << error << endl;
		return "";
	}


	string bodyMsg = "";
	if (return_ContentLength_Or_ChunkedTranferEncoding(headerMsg) == "Content-Length")
		bodyMsg = readMsgData(socket, headerMsg);
	else bodyMsg = readChunkedData(socket);

	return bodyMsg;
}



bool receiveSubFolder(vector<string> vector_fileName, string domainName, string IP, string path, string subFolderName) {
	string directoryName = domainName + "_" + subFolderName;
	int status = _mkdir(directoryName.c_str());
	// TO DO sth...


	SOCKET connectSocket = createSocket();
	if (connectSocket == INVALID_SOCKET) {
		cout << ">> Can't create listening socket" << endl;
		return false;
	}

	// Set address
	sockaddr_in connectSocketAddress;
	connectSocketAddress.sin_family = AF_INET;
	connectSocketAddress.sin_port = htons(80);
	connectSocketAddress.sin_addr.s_addr = inet_addr(IP.c_str());



	char* currentPath = NULL;

	if (!(currentPath = _getcwd(NULL, 0))) {
		cout << ">> Can't get path to binary file!" << endl;
		return false;
	}

	string requestToServer = "";
	if (connect(connectSocket, (sockaddr*)&connectSocketAddress, sizeof(connectSocketAddress)) == 0) {
		cout << ">> Client connected to server has IP: " << IP << endl;
		for (int i = 0;i < vector_fileName.size();i++) {
			requestToServer = requestToServer + "GET " + path + vector_fileName[i] + " HTTP/1.1\r\nHost: " + domainName + " \r\n\r\n";
		}
		if (!sendRequestToServer(connectSocket, requestToServer)) {
			cout << ">> Server couldn't receive your request!" << endl;
			return false;
		};

		for (int i = 0;i < vector_fileName.size();i++) {
			string data = receiveAFile(connectSocket);
			if (data != "")
				saveFile(string(currentPath, strlen(currentPath)) + "\\" + directoryName + "\\", vector_fileName[i], data);
			else cout << ">> Couldn't receive file " << vector_fileName[i] << endl;
		}
	}
	else cout << "";
	if (closesocket(connectSocket) != 0) {
		cout << ">> Couldn't close connection has IP " << IP << endl;
	}
	return true;
}

void saveFile(string path, string fileName, string data) {
	ofstream ofs(path + fileName, ios::binary);
	ofs.write(data.c_str(), data.size());
	cout << ">> Loaded file " << fileName << " successfully" << endl;
	ofs.close();
}



void handleSocket(string URL) {
	string domainName = "";
	string path = "";
	string fileName = "";
	parseURLString(URL, domainName, path, fileName);
	string IpAddress = getIpAddressFromDomainName(domainName);

	if (IpAddress == "") {
		cout << ">> Couldn't get IP from host " << domainName << endl;
		return;
	}

	//Create a Socket to Listen
	SOCKET connectSocket = createSocket();
	if (connectSocket == INVALID_SOCKET) {
		cout << ">> Can't create listening socket to server has IP: " << IpAddress << endl;
		return;
	}

	// Set address
	sockaddr_in connectSocketAddress;
	connectSocketAddress.sin_family = AF_INET;
	connectSocketAddress.sin_port = htons(80);
	connectSocketAddress.sin_addr.s_addr = inet_addr(IpAddress.c_str());
	if (connect(connectSocket, (sockaddr*)&connectSocketAddress, sizeof(connectSocketAddress)) == 0) {
		string requestToServer = "GET " + path + " HTTP/1.1\r\nHost: " + domainName + "\r\n\r\n";
		if (!sendRequestToServer(connectSocket, requestToServer)) {
			cout << ">> Server can't receive your request!" << endl;
			return;
		};

		string data = receiveAFile(connectSocket);
		if (data == "") {
			cout << ">> Couldn't receive file!" << endl;
			return;
		}
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
				saveFile("", domainName + fileNameToSave, data);
			else
				saveFile("", domainName + fileNameToSave + "_" + fileName, data);
		}
		else {
			string subFolderName = "";
			int i = path.size() - 1;
			while (path[--i] != '/');
			for (i++;i < path.size() - 2;i++)
				subFolderName += path[i];

			vector<string> vector_fileName = returnFileNameInSubfolder(data);
			if (receiveSubFolder(vector_fileName, domainName, IpAddress, path, subFolderName)) {
				cout << "Received folder " << subFolderName << " successfully " << endl;
			}
			else cout << "Couldn't receive folder " << subFolderName << endl;
		}
	}


	else cout << ">> Couldn't connect to server has IP " << IpAddress << endl;

	if (closesocket(connectSocket) != 0) {
		cout << ">> Couldn't close connection to IP " << IpAddress << endl;
	}

	return;
}