#include "ReadMsg.h"


// Receive data with size defined
string readData(SOCKET socket, int size) {
	if (size == 0) return "\r\n";

	char* buffer = new char[BUFFER_SIZE];
	std::string data = "";
	int totalBytes = 0;
	do {
		int bytesReceived = recv(socket, buffer, min(BUFFER_SIZE, size - totalBytes), 0);
		totalBytes += bytesReceived;
		if (bytesReceived < 0 && totalBytes < size) {
			delete[] buffer;
			return "";
		}
		data += string(buffer, bytesReceived);
	} while (totalBytes < size);
	data.resize(size);
	delete[] buffer;
	return data;
}




// convert hex to dec
unsigned int convertHexToDec(const std::string& hex) {

	unsigned int dec;
	std::stringstream ss;

	ss << std::hex << hex;
	ss >> dec;

	return dec;

}


// Receive a line of message until find \\r\n
string readALine(SOCKET socket) {
	int n = 0;
	string line;
	line.resize(0);
	char c = '\0';
	while ((n = recv(socket, &c, 1, 0)) > 0) {
		if (c == '\r') {
			line += c;
			// peak for \n
			n = recv(socket, &c, 1, MSG_PEEK);
			if ((n > 0) && (c == '\n')) {
				n = recv(socket, &c, 1, 0);
				line += c;
				break; // end of line
			}
		}
		line += c;
	}
	return line;
}

// read Header of Message
string readHeaderMsg(SOCKET socket) {
	string headerMsg = "";
	string tempMsg = "";
	do {
		tempMsg = readALine(socket);
		headerMsg += tempMsg;
	} while (tempMsg.compare("\r\n"));
	return headerMsg;
}

// return type of Message
string return_ContentLength_Or_ChunkedTranferEncoding(string headerMsg) {
	if (headerMsg.find("Content-Length: ") != -1)
		return "Content-Length";
	return "ChunkedTranferEncoding";
}



vector<string> returnFileNameInSubfolder(string Msg) {
	//regex expression for pattern to be searched 
	regex regexp("[href=\"]{6}[0-9a-zA-Z_-]+\.[a-z]+\"");
	smatch sm;

	// regex_search that searches pattern regexp in the string mystr  
	vector<string> resVector;
	resVector.resize(0);
	string temp = Msg;
	while (regex_search(temp, sm, regexp, regex_constants::match_default)) {
		string result = returnNameOfFile(sm.str());
		resVector.push_back(result);
		temp = temp.substr(sm.position() + sm.str().size());
	}

	return resVector;
}

string returnNameOfFile(string source) {
	source.pop_back();
	return source.substr(6);
}





