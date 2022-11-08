

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
			SOCKET connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
			if (connectSocket == INVALID_SOCKET) {
				cout << "Can't create listening socket" << endl;
				break;
			}

			// Set address
			sockaddr_in connectSocketAddress;
			connectSocketAddress.sin_family = AF_INET;
			connectSocketAddress.sin_port = htons(80);

			cout << "--------------------------------" << endl;
			cout << "Get: " << fileName << endl << "Host: " << domainName << endl << "IP: " << ipAddress << endl << endl;
			connectSocketAddress.sin_addr.s_addr = inet_addr(ipAddress.c_str());
			// connect to server
			if (connect(connectSocket, (sockaddr*)&connectSocketAddress, sizeof(connectSocketAddress)) == 0) {
				cout << ">> Client connected to server. " << endl;
				string requestToServer = "GET " + path + " HTTP/1.1\r\nHost: " + domainName + "\r\n\r\n";
				send(connectSocket, requestToServer.c_str(), (int)requestToServer.size() + 1, 0);

				ofstream ofs(fileName, ios::binary);
				string headerMsg = readHeaderMsg(connectSocket);
				string dataMsg = readMsgData(connectSocket, headerMsg);
				ofs.write(dataMsg.c_str(), dataMsg.size());
				cout << "Loaded file" << endl << endl;
				ofs.close();
				int iResult = closesocket(connectSocket);
				if (iResult == 0) cout << "Closed connection" << endl;
				
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