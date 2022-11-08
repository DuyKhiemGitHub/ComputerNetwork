#include "Chunked.h"


string readChunkedData(SOCKET socket) {
	int bytesReceived = 0;
	string Msg = "";
	char* buffer = new char[BUFFER_SIZE];
	string string_chunkedSize = "";
	int chunkedSize = 0;
	do {
		string_chunkedSize = readALine(socket);
		chunkedSize = convertHexToDec(string_chunkedSize) + 2;
		Msg += readData(socket, chunkedSize);
	} while (chunkedSize != 2);
	delete[] buffer;
	return Msg;
}