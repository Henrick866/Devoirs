#pragma once
#include "Winsocket.h"
#include "MACGen.h"
#include "RC4.h"

class ChatUser
{
public:
	ChatUser(Winsocket * socket);
	~ChatUser();
	bool SendLoop();
	bool ReceiveLoop();

private:
	Winsocket * socket;

	void SendMessage_(string message);
	void SendAlgoChange();
	void SendQuit();
	void SendNothing();

	void ShowMessage(string message);
	void SetAlgoChoice(char algo);

	void SendData(string data);
	string ReceiveData();
	void GetUserInput(string& result);

	string DecryptFeistel(string s);
	string DecryptRC4(string s);
	string EncryptFeistel(string s);
	string EncryptRC4(string s);

	MACGen* macGen;
	RC4* rc4;
	char cryptoAlgo;
	vector<unsigned char> RC4Key;
};