#include "Chunked.h"


string readChunkedData(SOCKET socket) {
	int bytesReceived = 0;
	string Msg = "";
	string string_chunkedSize = "";
	int chunkedSize = 0;
	do {
		string_chunkedSize = readALine(socket);
		chunkedSize = convertHexToDec(string_chunkedSize);
		Msg += readData(socket, chunkedSize + 2); // extra "\r\n"
	} while (chunkedSize > 0);

	//char buffer[2000];
	//memset(buffer, 0, 2000);
	//int n = 0;
	//while ((n = recv(socket, buffer, 2000, 0)) > 0) {
	//	memset(buffer, 0, 2000);
	//}
	return Msg;
}