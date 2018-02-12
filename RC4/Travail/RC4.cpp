#include "stdafx.h"
#include <cstddef>
#include <string>
#include "RC4.h"


RC4::RC4(char * Message)
{
	Taille = strlen(Message);
	BlocCrypt = new char[Taille + 1];
	BlocMessage = Message;
	i = j = 0;
}


RC4::~RC4()
{
}

void RC4::GenererClef(unsigned char *Key, unsigned int Key_Length) {
	for (i = 0; i < 256; i++) {
		S[i] = i;
	}
	for (i = 0, j = 0; i < 256; i++) {
		j = (j + S[i] + Key[i % Key_Length]) % 256;
		Swap(S, i, j);
	}
	i = j = 0;
}
void RC4::Swap(unsigned char *s, unsigned int i, unsigned int j) {
	unsigned char temp = s[i];
	s[i] = s[j];
	s[j] = temp;
}

unsigned char RC4::Output() {
	i = (i + 1) % 256;
	j = (j + S[i]) % 256;

	Swap(S, i, j);
	return S[(S[i] + S[j]) % 256];
}
char* RC4::Encrypt() {
	for (int y = 0; y < Taille; y++) {
		BlocCrypt[y] = (BlocMessage[y] ^ Output());
	}
	return BlocCrypt;
}
char* RC4::Decrypt() {
	char * BlocDecrypt = new char[Taille + 1];
	
	for (int y = 0; y < Taille; y++) {
		BlocDecrypt[y] = (BlocCrypt[y] ^ Output());
	}
	return BlocDecrypt;
}
