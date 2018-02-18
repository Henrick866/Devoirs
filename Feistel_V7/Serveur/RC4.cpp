#include <cstddef>
#include <string>
#include <vector>
#include "RC4.h"
using namespace std;

RC4::RC4()
{
	i = j = 0;
}


RC4::~RC4()
{
}

void RC4::GenererClef(vector<unsigned char> Key, unsigned int Key_Length) {
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
string RC4::Encrypt(string clear) {
	string Cipher;
	for (int y = 0; y < clear.length(); y++) {
		Cipher += (clear[y] ^ Output());
	}
	return Cipher;
}
string RC4::Decrypt(string cipher) {
	string Clear;
	
	for (int y = 0; y < cipher.length(); y++) {
		Clear += (cipher[y] ^ Output());
	}
	return Clear;
}
