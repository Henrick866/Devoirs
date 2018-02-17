#include <winsock.h>
#include <stdio.h>
#include <iostream>
#include <bitset>
#include <fstream>
#include <string>
#include <Shellapi.h>
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <vector>
#include<algorithm>

#pragma comment(lib,"ws2_32.lib")
#define BUFF_SIZE 1024
#define PORT 27000
#define PRINTERROR(s) cout<<"\n"<<s<<":"<<WSAGetLastError()

using namespace std;

#ifndef Winsocket_H
#define Winsocket_H

class Winsocket
{
private:

	//____________Socket___________________________________________

	int nRet;
	int nLen;
	int TypeSocket;
	short Port;
	short nPort;
	char szBuf_Receive[BUFF_SIZE];
	char szBuf_send[BUFF_SIZE];
	char PC_Name[100];
	char szBuf[256];
	string message;
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	SOCKET listenSocket;
	SOCKET remoteSocket;
	SOCKET theSocket;
	SOCKADDR_IN saServer;
	LPHOSTENT lpHostEntry;

	//____________Cryptographie___________________________________

	vector<bitset<16>> TexteBrute;
	vector<bitset<16>> EncryptedMessage;
	vector<bitset<16>> DecryptedMessage;
	bitset<16>		   Originalkey;

public:

	//____________Socket___________________________________________

	Winsocket();
	Winsocket(int type);
	~Winsocket();
	void CreateSocket();
	void Bind();
	void Listen();
	void Accept();
	void Connect();
	void Send(string message);
	string Receive();
	void CloseSocket();

	//____________Cryptographie___________________________________

	bool		PairNumber			(int item1);
	bitset<16>	StringTo16Bit		(string message);
	bitset<16>	StringTo16BitInvert	(string message);
	string		_8BitToString		(bitset<8> item1);
	string		_16BitToString		(bitset<16> item1);
	bitset<16>	ConcatenateBitset	(bitset<8> item1, bitset<8> item2);
	bitset<8>	ResizeBitsetToLeft	(bitset<16> item1);
	bitset<8>	ResizeBitsetToRight	(bitset<16> item1);
	bitset<8>	Xor8Bit				(bitset<8> item1, bitset<8> item2);
	bitset<16>	Xor16Bit			(bitset<16> item1, bitset<16> item2);
	void		SwapRight			(bitset<8>& item1, bitset<8>& item2);
	void		RightCircularShift	(bitset<16>& item);
	void		LeftCircularShift	(bitset<16>& item);
	void		StringToVectorBitset(string message, vector<bitset<16>> &vector);
	string		VectorBitsetToString(vector<bitset<16>> vector);
	string		VectorBitsetToBinaryString(vector<bitset<16>> vector);
	void		StringBinaryToVectorBiset(string message, vector<bitset<16>> &vector);

	bitset<16>	FeistelEncrypt		(bitset<16> block, bitset<16> key);	//block 16 bit,half-block 8bit, key 16 bit, 16 round
	bitset<16>	FeistelDecrypt		(bitset<16> block, bitset<16> key);
	string		EncryptCipherBlockChaining (string message,bitset<16> IV, bitset<16> key);
	string		DecryptCipherBlockChaining (string message,bitset<16> IV, bitset<16> key);
};
#endif