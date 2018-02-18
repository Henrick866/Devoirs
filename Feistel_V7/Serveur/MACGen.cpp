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
	if (message.length() < 64)
	{
		message.append(' ', 64 - message.length());
	}

	long long lastBlock = 0;

	for (size_t i = 0; i < 8; i++)
	{
		lastBlock |= ((long long)message[message.length() - 1 - i]) << 8 * i;
	}

	return des->EncodeBlock(lastBlock);
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