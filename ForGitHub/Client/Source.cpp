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
#include <commdlg.h>

using namespace std;
bool timers = true;
void Timer5Sec()
{
	this_thread::sleep_for(chrono::milliseconds(5000));
	timers = false;
}
void GetMess(SOCKET ClientSocket)
{
	char buffer[512];
	while(true)
	{
		ZeroMemory(buffer, sizeof(buffer));

		int res = recv(ClientSocket, buffer, sizeof(buffer), 0);
		if (res <= 0)
		{
			cerr << "[Client] The server has disconnected or there was an error receiving data." << endl;
			return;
		}
		cout << "[Server] " << buffer << endl;
	}
}
string tempFileName = "temp.txt";

bool ConnectVpn(wstring& command)
{
	STARTUPINFO si{};
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);

	wchar_t cmdLine[512];
	wcscpy_s(cmdLine, command.c_str());

	if (!CreateProcessW
	(
		NULL,
		cmdLine,
		NULL, NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si, &pi
	))
	{
		cerr << "[Error] Failed to run vpn connection command" << endl;
		return false;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return true;
}
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

int main()
{
	setlocale(LC_ALL, "Rus");


	wstring connect = L"rasdial NameOfYourVpn Login Pass";
	wstring disconnect = L"rasdial NameOfYourVpn /disconnect";

	if (ConnectVpn(connect))
	{
		cout << "Connect to VPN" << endl;

	}
	else
	{
		cout << "Error to connect to VPN" << endl;
		return 1;
	}


	WSADATA wsaData;

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(result !=0)
	{
		cout << "Error WSAStartup" << endl;
		return 1;
	}

	SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ClientSocket == INVALID_SOCKET)
	{
		cerr << "Error to create a socket" << endl;
		WSACleanup();
		return 1;
	}

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(5678);
	serverAddr.sin_addr.s_addr = inet_addr("Local ip server");
	thread timer(Timer5Sec);
	while (true)
	{
		if (timers == true)
		{
			if (::connect(ClientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) != SOCKET_ERROR)
			{
				cout << "Connect to server" << endl;
				break;
			}
		}
		else
		{
			cout << "Writing to the memory" << endl;
			ofstream file;
			file.open("temp.txt",ios::app);

			while(true)
			{
				string sendm;

				getline(cin, sendm);
				if (sendm == "exit")
				{
					ConnectVpn(disconnect);
					file.close();
					return 1;
				}
				file << sendm<<endl;

			}
			file.close();

		}		
		
	}
	timer.join();

	std::ifstream inputFile(tempFileName);
	if (inputFile.good())
	{
		
		std::string line;
		while (std::getline(inputFile, line)) 
		{
			if (!line.empty()) 
			{
				send(ClientSocket, line.c_str(), line.size(), 0);
				std::cout << "Send: " << line << std::endl;
			}
		}
		inputFile.close();

	
		std::remove(tempFileName.c_str());
		std::cout << "All message did send " << std::endl;
	}
	thread d(GetMess,ClientSocket);

	Message txt;
	while (true)
	{
	
		
		cin >> txt.mess;
		txt.id++;
		if (txt.mess == "exit")
		{
			ConnectVpn(disconnect);

			return 1;
		}
		send(ClientSocket, (char*)&txt, sizeof(txt), 0);
	}
	d.join();
	closesocket(ClientSocket);
	WSACleanup();

	ConnectVpn(disconnect);

	system("pause");
	return 0;
}