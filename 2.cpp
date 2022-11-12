
#include <regex>
#include <direct.h>
#include <iostream> 
#include <string>
using namespace std;

int main()
{
	//char* a = NULL;
	//a = _getcwd(NULL, 0);
	//if (a)
	//	cout << string(a, strlen(a));
	//else perror("_getcwd error");
	//string stringpath = string(a,strlen(a)) + "\\" + "New Folder" + "\\" "newww";
	//int status = _mkdir(stringpath.c_str());
	std::string s= "asdasd";
	std::regex e ("[a-z]{2}");




	std::smatch sm;
	regex_search(s, sm, e);


	std::cout << "the matches were: ";
	for (unsigned i = 0; i < sm.size(); ++i) {
		std::cout << "[" << sm[i] << "] ";
	}

	return 0;

}