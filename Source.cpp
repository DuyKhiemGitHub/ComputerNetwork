

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

	//Create a Socket to Listen
	SOCKET connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (connectSocket == INVALID_SOCKET) {
		cout << "Can't create listening socket" << endl;
		return 1;
	}

	// Set address
	sockaddr_in connectSocketAddress;
	connectSocketAddress.sin_family = AF_INET;
	connectSocketAddress.sin_port = htons(80);
	connectSocketAddress.sin_addr.s_addr = inet_addr("191.236.16.12"); // Hard code


	// Test functions 

	vector<string> requestVector;
	requestVector.resize(0);
	vector<string> ipAddressVector;
	ipAddressVector.resize(0);

	//Test reveive Message by Content-Length
	requestVector.push_back("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");  // Host : example.com
	ipAddressVector.push_back("93.184.216.34");
	requestVector.push_back("GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n");
	ipAddressVector.push_back("93.184.216.34");
	requestVector.push_back("GET /dept/its/support/techtraining/techbriefing-media/Intro_Net_91407.ppt HTTP/1.1\r\nHost: web.stanford.edu\r\n\r\n");  // Host : web.stanford.edu
	ipAddressVector.push_back("171.67.215.200");
	requestVector.push_back("GET /class/cs224w/slides/01-intro.pdf HTTP/1.1\r\nHost: web.stanford.edu\r\n\r\n"); // Host : web.stanford.edu
	ipAddressVector.push_back("171.67.215.200");
	requestVector.push_back("GET /class/cs224w/slides/08-GNN-application.pdf HTTP/1.1\r\nHost: web.stanford.edu\r\n\r\n"); // Host : web.stanford.edu
	ipAddressVector.push_back("171.67.215.200");
	requestVector.push_back("GET /class/cs231a/assignments.html HTTP/1.1\r\nHost: web.stanford.edu\r\n\r\n"); // Host : web.stanford.edu
	ipAddressVector.push_back("171.67.215.200");
	requestVector.push_back("GET /class/cs231a/project.html HTTP/1.1\r\nHost: web.stanford.edu\r\n\r\n"); // Host : web.stanford.edu
	ipAddressVector.push_back("171.67.215.200");
	requestVector.push_back("GET /wireshark-labs/alice.txt HTTP/1.1\r\nHost: gaia.cs.umass.edu\r\n\r\n"); // Host: gaia.cs.umass.edu
	ipAddressVector.push_back("");
	requestVector.push_back("GET /wireshark-labs/Wireshark_Intro_v8.1.docx HTTP/1.1\r\nHost: www-net.cs.umass.edu\r\n\r\n"); //Host: www - net.cs.umass.edu
	ipAddressVector.push_back("");

	//Connect to Server
	if (connect(connectSocket, (sockaddr*)&connectSocketAddress, sizeof(connectSocketAddress)) == 0) {
		cout << "Client connected to server. " << endl << endl;
		// send request to server


		s = "GET /httpgallery/chunked/chunkedimage.aspx HTTP/1.1\r\nHost: www.httpwatch.com\r\n\r\n";
		send(connectSocket, str.c_str(), (int)str.size() + 1, 0);




		// open file to write into
		ofstream ofs("chunkedimage.aspx", ios::binary);

		// read header of Message
		string headerMsg = readHeaderMsg(connectSocket);
		cout << headerMsg;



		ofs.write(Msg.c_str(), Msg.size());

		//string Msg = readBodyMsg(connectSocket, headerMsg);
		//ofs.write(Msg.c_str(), Msg.size());





		ofs.close();




	}
	else
		cout << "Coudn't connect to server! Try again!" << endl;






	int iResult = closesocket(connectSocket);
	WSACleanup();

	return 0;
}