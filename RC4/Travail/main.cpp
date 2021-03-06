// Travail.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "RC4.h"
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
using namespace std;

vector<unsigned char> intToBytes(int paramInt);
vector<unsigned char> GenerateKey(int Seed);
int main()
{
	vector<unsigned char> Key = GenerateKey(5); //Génération de clé aléatoire avec graine
	cout << sizeof(Key);
	string MessageOrig = "I LOVE TOAST!";//le méssage
	char * MessageChar = (char *)MessageOrig.c_str();//conversion de string à char
	RC4* ProtocoleRC4 = new RC4(MessageChar);//construit l'obj RC4


	/*Code à faire dans la méthode mère*/
	ProtocoleRC4->GenererClef(Key, sizeof(Key));//pour réinitialiser la clé
	char* Encrypted = ProtocoleRC4->Encrypt();
	for (int i = 0; i < MessageOrig.size(); i++) {
		cout << Encrypted[i];//on peut faire un string
	}
	cout << std::endl;//et on envoie le string

	ProtocoleRC4->GenererClef(Key, sizeof(Key));
	char* Decrypted = ProtocoleRC4->Decrypt();
	for (int i = 0; i < MessageOrig.size(); i++) {
		cout << Decrypted[i];
	}
    return 0;
}
vector<unsigned char> intToBytes(int paramInt)
{
	vector<unsigned char> arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[3 - i] = (paramInt >> (i * 8));
	return arrayOfByte;
}
vector<unsigned char> GenerateKey(int Seed) {
	vector<unsigned char> Key(16); //la clé
	srand(Seed);
	for (int i = 0; i < 4; i++) {
		int B = rand();
		vector<unsigned char> Nombre = intToBytes(B);
		for (int j = 0; j < 4; j++) {
			Key[(i * 4) + j] = Nombre[j];
		}
	}
	return Key;
}
