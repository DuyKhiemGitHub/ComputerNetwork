#include "Chunked.h"


bool readChunkedDataAndSave(SOCKET socket, string path, string fileName) {
	int bytesReceived = 0;
	string Msg = "";
	string string_chunkedSize = "";
	int chunkedSize = 0;
	ofstream ofs(path + fileName, ios::binary);
	time_t t = clock();
	do {
		string_chunkedSize = readALine(socket);
		chunkedSize = convertHexToDec(string_chunkedSize);
		string msg = readData(socket, chunkedSize + 2);
		if (msg == "") {
			ofs.close();
			deleteAFile(path + fileName);
			cout << ">> Couldn't load file " << fileName << endl;
			return false;
		}
		ofs.write(msg.c_str(), chunkedSize + 2); // extra "\r\n
	} while (chunkedSize > 0);

	ofs.close();

	cout << ">> Loaded file " << fileName << " successfully. Time: " << (double)(clock() - t) / CLOCKS_PER_SEC << endl;
	return true;
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