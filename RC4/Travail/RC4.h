#pragma once
class RC4
{
private:
	unsigned char S[256], T[256];
	int Taille;
	char* BlocMessage;
	char* BlocCrypt;
	unsigned int i, j;
	void Swap(unsigned char *s, unsigned int i, unsigned int j);
	unsigned char Output();
	
public:
	RC4(char * Message);
	~RC4();
	void GenererClef(unsigned char *Key, unsigned int Key_Length);
	char* Encrypt();
	char* Decrypt();
};

