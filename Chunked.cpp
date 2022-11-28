#include "Chunked.h"


void readChunkedDataAndSave(SOCKET socket, string path, string fileName) {
	int bytesReceived = 0;
	string Msg = "";
	string string_chunkedSize = "";
	int chunkedSize = 0;
	ofstream ofs(path + fileName, ios::binary);
	do {
		string_chunkedSize = readALine(socket);
		chunkedSize = convertHexToDec(string_chunkedSize);
		ofs.write(readData(socket, chunkedSize + 2).c_str(), chunkedSize + 2); // extra "\r\n
	} while (chunkedSize > 0);

	ofs.close();

	cout << ">> Loaded file " << fileName << " successfully" << endl;

}

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

	return Msg;
}