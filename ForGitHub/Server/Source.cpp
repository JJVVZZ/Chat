#include<iostream>
#include<math.h>
#include<iomanip>
#include<string>
#include<ctime>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#include<new>
#include<fstream>
#include<ostream>
#include<memory>
#include<exception>
#include<vector>
#include<list>
#include<forward_list>
#include<array>
#include<deque>
#include<stack>
#include<queue>
#include<set>
#include<map>
#include<thread>
#include<chrono> 
#include<functional>
#include<algorithm>
#include<numeric>
#include<mutex>
#include<regex>
#include<unordered_map>
#include<unordered_set>
#include<cstring>
#include <cstdlib>
#include <typeinfo>
#include<future>
#include <type_traits>
#include<ranges>
#include<cassert>



using namespace std;
#pragma pack(push,1)
class Message
{
public:
	char mess[512];
	char name[512] = "Someone";
	static int id;

};
int Message::id = 0;
#pragma pack(pop)
void GetMess(SOCKET clientSocket)
{
	Message msg;
	while (true)
	{
		

		int br = recv(clientSocket, (char*)&msg, sizeof(msg), 0);

		if (br <= 0)
		{
			cerr << "[Server] Client lost" << endl;
			break;
		}

		cout << "["<<msg.name<<" client wrote] " << msg.mess << endl;

	
	}
}
int main()
{
	setlocale(LC_ALL, "Rus");
	srand(time(NULL));

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cerr << "[Server] Error WSAStartup" << endl;
		return 1;
	}
	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET)
	{
		cerr << "[Server] Error to create a socket" << endl;
		WSACleanup();
		return 1;
	}
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(5678);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (::bind(ServerSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		cerr << "[Server] Error to bind" << endl;
		closesocket(ServerSocket);
		WSACleanup();
		return 1;
	}


	if (listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cerr << "[Server] Error while listening" << endl;
		closesocket(ServerSocket);
		WSACleanup();
		return 1;
	}

	cout << "[Server] Waiting for the client" << endl;

	sockaddr_in ClientAddr{};
	int clientSize = sizeof(ClientAddr);
	SOCKET clientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &clientSize);

	if (clientSocket == INVALID_SOCKET)
	{
		cerr << "[Server] Error to connect to client" << endl;
		closesocket(ServerSocket);
		WSACleanup();
		return 1;

	}
	cout << "Client connected" << endl;


	
	thread t(GetMess, clientSocket);
	while (true)
	{
		
		string reply;
		getline(cin, reply);

		send(clientSocket, reply.c_str(), reply.size(), 0);
	}
	t.join();
	system("pause");
	return 0;
}
