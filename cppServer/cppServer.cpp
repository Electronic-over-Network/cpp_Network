#include <iostream>
#include <string>
#include <vector>
#include <WinSock2.h>
#include <random>
#include <thread>
#include <chrono>
#pragma comment (lib , "ws2_32.lib")
using namespace std;


int makeRand(void)
{
	// ����?
	random_device rd;
	mt19937 rng(rd());
	uniform_int_distribution<int> ud(1, 1024);

	return ud(rng);
}

int main(void)
{
	WSADATA wsaData;
	SOCKADDR_IN serverAdress, clientAdress;
	int err;

	err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0) {
		cout << "WSAStartup error " << WSAGetLastError() << endl;
		WSACleanup();
		return false;
	}

	serverAdress.sin_family = AF_INET; // AF_INET�� ����??
	serverAdress.sin_port = htons(11235);
	serverAdress.sin_addr.s_addr = inet_addr("127.0.0.1"); // s_addr�� �׳� ������ ��?

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serverSocket == INVALID_SOCKET)
	{
		cout << "Socket Error" << WSAGetLastError() << endl;
		WSACleanup();
		return false; // ���� ���� ���н� ����
	}

	if (bind(serverSocket, reinterpret_cast<SOCKADDR*> (&serverAdress), sizeof(serverAdress)))
	{
		// reinterpret_cast���� cast�� ����?
		cout << "Binding failed. Error code : " << WSAGetLastError() << endl;
		WSACleanup();
		return false;
	}
	
	cout << "client�� ��ٸ��ϴ�." << endl;

	listen(serverSocket, 5); // backlog(5)�� ����?
	int xx = sizeof(clientAdress);
	SOCKET clientSocket = accept(serverSocket, reinterpret_cast<SOCKADDR*> (&clientAdress), &xx); // bind������ �׳� sizeof ������ �� ��� &(�ּҰ�)����?
	cout << "Connection Complete" << "���ο� Socket�� " << clientSocket << endl;

	int receiveNumber;
	char communicationBuffer[100];
	int iRand;
	int bytesSent;
	while (TRUE)
	{
		receiveNumber = recv(clientSocket, communicationBuffer, 100, 0); // flag�� ����?
		if (receiveNumber <= 0) { cout << "Got nothing" << endl; break; }
		communicationBuffer[receiveNumber] = 0;

		if (!strcmp(communicationBuffer, "Hello"))
		{
			iRand = makeRand();
			auto sRand = to_string(iRand);
			cout << "Sending Random Number " << iRand << "to the client" << endl;
			bytesSent = send(clientSocket, sRand.c_str(), sRand.length(), 0);

			continue;
		}
		string sNumber(communicationBuffer);
		try
		{
			iRand = stoi(sNumber);
			cout << "Server got " << "\"" << iRand << "\"" << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Sending \"" << iRand++ << "\"" << " to Client" << endl;
			auto sRand = to_string(iRand);
			bytesSent = send(clientSocket, sRand.c_str(), sRand.length(), 0);
		}
		catch (const invalid_argument& ex)
		{
			cerr << "Invalid argument while converting string to number" << endl;
			cerr << "Error : " << ex.what() << endl;
			break;
		}
		catch (const out_of_range& ex)
		{
			cerr << "Invalid argument while converting string to number" << endl;
			cerr << "Error : " << ex.what() << endl;
			break;
		}
	}
	
	closesocket(serverSocket);
	WSACleanup();

	return 0;
}