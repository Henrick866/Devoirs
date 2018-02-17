#include <stdio.h>
#include <winsock.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <Shellapi.h>
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include "Winsocket.h"

using namespace std;

void client();
string EncryptMessageFeistel(Winsocket Client1, string message);

int main()
{
	thread Client(client);
	Client.join();
 	system("pause");
	return 0;
}

void client()
{
	string message;
	Winsocket Client1(2);
	Client1.CreateSocket();
	Client1.Connect();
	cout << "\nEntrer message a envoyer :" << endl;
	cin>>message;
	//cout << "Message to send: " << message << endl;
	message = EncryptMessageFeistel(Client1,message);
	Client1.Send(message);
	Client1.CloseSocket();
}

string EncryptMessageFeistel(Winsocket Client1, string message)
{
	string messageSend;
	bitset<16> IV = Client1.StringTo16BitInvert("0101010101010101");
	bitset<16> key = Client1.StringTo16BitInvert("0101101010101001");
	messageSend = (Client1.EncryptCipherBlockChaining(message, IV, key));
	//cout << "Encrypted message send : " << messageSend << endl;
	return messageSend;
}