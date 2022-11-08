#include "WebClient.h"





string getDomainName(ifstream& ifs) {
	string res = "";
	getline(ifs, res, '/');
	return res;
}

string getPath(ifstream& ifs) {
	string path = "";
	getline(ifs, path, '\n');
	return path;
}

string getFileName(string path) {
	int i = path.size();
	while (path[--i] != '/');
	return path.substr(i + 1,path.size());
}

string getIpAddress(ifstream& ifs) {
	string ipAddress = "";
	getline(ifs, ipAddress, '\n');
	return ipAddress;
}