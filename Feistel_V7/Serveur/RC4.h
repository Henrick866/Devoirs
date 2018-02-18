#pragma once
#include <vector>
using namespace std;
class RC4
{
private:
	unsigned char S[256], T[256];
	unsigned int i, j;
	void Swap(unsigned char *s, unsigned int i, unsigned int j);
	unsigned char Output();
	
public:
	RC4();
	~RC4();
	void GenererClef(vector<unsigned char> Key, unsigned int Key_Length);
	string Encrypt(string clear);
	string Decrypt(string cipher);
};

