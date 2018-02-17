#include <stdio.h>
#include <winsock.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <windows.h>
#include <vector>
#include "Winsocket.h"

using namespace std;

void serveur();
string DecryptMessageFeistel(Winsocket Serveur1, string message);

int main()
{
	thread Serveur(serveur);
	Serveur.join();
 	system("pause");
	return 0;
}

void serveur()
{
	string message;
	Winsocket Serveur1(1);
	Serveur1.CreateSocket();
	Serveur1.Bind();
	Serveur1.Listen();
	Serveur1.Accept();
	message = Serveur1.Receive();
	cout << "\nMessage recu :" << message << endl;
	message = DecryptMessageFeistel(Serveur1, message);
	Serveur1.CloseSocket();
}

string DecryptMessageFeistel(Winsocket Serveur1, string message)
{
	string messageReceive;
	bitset<16> IV = Serveur1.StringTo16BitInvert("0101010101010101");
	bitset<16> key = Serveur1.StringTo16BitInvert("0101101010101001");
	messageReceive = Serveur1.DecryptCipherBlockChaining(message,IV, key);
	//cout << "Decrypted message receive : " << messageReceive << endl;
	return messageReceive;
}