#include "MACGen.h"

MACGen::MACGen(long long key)
{
	des = new DES(key);
}

MACGen::~MACGen()
{
	delete des;
}

int MACGen::GetCodeForMessage_Int(string message)
{
	// On s'assure que le message est un multiple de 64 bit (long long)
	if (message.length() % 8 != 0)
	{
		int missingCharacters = 8 - (message.length() % 8);
		message.append(missingCharacters, ' ');
	}

	//On chiffre chaque block avec le XOR du chiffrement du block précédent. Voir algo CMAC
	long long _lastBlock = 0;
	for (size_t i = 0; i < message.length(); i += 8)
	{
		long long block = 0;
		for (size_t u = 0; u < 8; u++)
		{
			block |= ((long long)message[i + u]) << 8 * (7 - u);
		}
		_lastBlock = des->EncodeBlock(_lastBlock ^ block);
	}

	return (int)_lastBlock;
}

string MACGen::GetCodeForMessage_Str(string message)
{
	int code = GetCodeForMessage_Int(message);
	char c[4];
	for (size_t i = 0; i < 4; i++)
	{
		c[i] = (char)(code >> (3 - i) * 8);
	}


	return string(c, 4);
}