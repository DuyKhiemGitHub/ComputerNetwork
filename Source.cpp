

#include "ReadMsg.h"
#include "WebClient.h"
#include "ContentLength.h"
#include "Chunked.h"



int main() {

	//Initialize winsock
	WSADATA wsData;
	if (WSAStartup(MAKEWORD(2, 2), &wsData)) {
		cout << "Can't initialize winsock!" << endl;
		return 1;
	}



	// Test functions 
	// Hard code IP Address

								   //  Reveive message by Content-Length - Test case 1

	{
		ifstream ifs("Testcase1.txt");
		while (!ifs.eof()) {
			string domainName = getDomainName(ifs);
			string path = "/" + getPath(ifs);
			string fileName = getFileName(path);
			string ipAddress = getIpAddress(ifs);

			//Create a Socket to Listen
			SOCKET connectSocket = createSocket();
			if (connectSocket == INVALID_SOCKET) {
				cout << ">> Can't create listening socket" << endl;
				break;
			}

			// Set address
			sockaddr_in connectSocketAddress;
			connectSocketAddress.sin_family = AF_INET;
			connectSocketAddress.sin_port = htons(80);
			connectSocketAddress.sin_addr.s_addr = inet_addr(ipAddress.c_str());

			cout << "--------------------------------" << endl;
			cout << "Get: " << fileName << endl << "Host: " << domainName << endl << "IP: " << ipAddress << endl;

			// connect to server
			if (connect(connectSocket, (sockaddr*)&connectSocketAddress, sizeof(connectSocketAddress)) == 0) {
				cout << ">> Client connected to server. " << endl;


				if (fileName == "") {
					if (path == "/") receiveAFile(connectSocket, domainName, path, domainName + "_" + "index.html");
					else {
						// to do sth...
					}
				}
				else {
					receiveAFile(connectSocket, domainName, path, domainName + "_" + fileName);
				}

				int iResult = closesocket(connectSocket);
				if (iResult == 0) cout << ">> Closed connection" << endl;

				Sleep(2000);
			}
			else cout << ">> Couldn't connect to server." << endl << endl;
			Sleep(2000);

		}
		ifs.close();
	}






	WSACleanup();

	return 0;
}