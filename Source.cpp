

#include "ReadMsg.h"
#include "WebClient.h"
#include "ContentLength.h"
#include "Chunked.h"
#include <thread>

static const int THREAD_SIZE = 4;


int main(int argv, char** argc) {


	//Initialize winsock
	WSADATA wsData;
	if (WSAStartup(MAKEWORD(2, 2), &wsData)) {
		cout << "Can't initialize winsock!" << endl;
		return 1;
	}

	if (argv == 1) {
		return 1;
	}

	if (argv == 2) {
		handleSocket(argc[1]);
	}

	if (argv > 2) {
		int numOfRequest = argv - 1;
		vector<thread> threads;
		threads.resize(0);
		int i = 1;
		while (numOfRequest > 0) {
			while (threads.size() < numOfRequest && threads.size() < THREAD_SIZE) {
				threads.push_back(thread(handleSocket, argc[i]));
				i++;
			}
			for (auto& thread : threads)
				if (thread.joinable()) thread.join();

			while (!threads.empty())
				threads.pop_back();
			numOfRequest = numOfRequest - THREAD_SIZE;
		}
	}
	WSACleanup();
	return 0;
}