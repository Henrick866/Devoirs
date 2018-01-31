#include "DES.h"


DES::DES(long long key)
{
	DES::key = key;
}

DES::~DES()
{
}

void DES::GenerateSubKeys()
{
	long long permutedKey = DES::PC1_Permutation(key);

	int cBase = 0;
	int dBase = 0;

	DES::KeySplitToMini(permutedKey, cBase, dBase);

	// Produce 16 subkeys
	int c[16];
	int d[16];

	c[0] = cBase;
	d[0] = dBase;
	DES::MiniKeyShift(c[0], shiftTable[0]);
	DES::MiniKeyShift(d[0], shiftTable[0]);
	subkeys[0] = DES::JoinMiniKeys(c[0], d[0]);

	for (size_t i = 1; i < 16; i++)
	{
		c[i] = c[i - 1];
		d[i] = d[i - 1];
		DES::MiniKeyShift(c[i], shiftTable[i]);
		DES::MiniKeyShift(d[i], shiftTable[i]);
		subkeys[i] = DES::JoinMiniKeys(c[i], d[i]);
	}
	for (size_t i = 0; i < 16; i++)
	{
		subkeys[i] = DES::PC2_Permutation(subkeys[i]);
	}
	subKeysGenerated = true;
}

long long DES::EncodeBlock(long long message)
{
	if (!subKeysGenerated)
		GenerateSubKeys();

	long long ip = DES::IP_Permutation(message);

	int lBase = 0;
	int rBase = 0;
	DES::LongLongSplit(ip, lBase, rBase);

	int l[16];
	int r[16];

	l[0] = rBase;
	r[0] = REncoding(lBase, rBase, subkeys[0]);

	for (size_t i = 1; i < 16; i++)
	{
		l[i] = r[i - 1];
		r[i] = REncoding(l[i - 1], r[i - 1], subkeys[i]);
	}

	long long result = l[15];
	result |= (long long)r[15] << 32;
	result = IPInv_Permutation(result);

	return result;
}

int DES::P_Permutation(int value)
{
	long long result = 0;

	int tableI = 0;
	for (int i = 32 - 1; i >= 0; i--)
	{
		int originalBitIndex = 31 - (pTable[tableI] - 1);

		DES::SetBit(result, i, DES::GetBit(value, originalBitIndex));
		tableI++;
	}

	return result;
}

long long DES::PC1_Permutation(long long value)
{
	long long result = 0;

	int tableI = 0;
	for (int i = 56 - 1; i >= 0; i--)
	{
		int originalBitIndex = 63 - (pc1Table[tableI] - 1);

		DES::SetBit(result, i, DES::GetBit(value, originalBitIndex));
		tableI++;
	}

	return result;
}

long long DES::PC2_Permutation(long long value)
{
	long long result = 0;

	int tableI = 0;
	for (int i = 48 - 1; i >= 0; i--)
	{
		int originalBitIndex = 55 - (pc2Table[tableI] - 1);

		DES::SetBit(result, i, DES::GetBit(value, originalBitIndex));
		tableI++;
	}

	return result;
}

long long DES::IP_Permutation(long long value)
{
	long long result = 0;

	int tableI = 0;
	for (int i = 64 - 1; i >= 0; i--)
	{
		int originalBitIndex = 63 - (ipTable[tableI] - 1);

		DES::SetBit(result, i, DES::GetBit(value, originalBitIndex));
		tableI++;
	}

	return result;
}

long long DES::IPInv_Permutation(long long value)
{
	long long result = 0;

	int tableI = 0;
	for (int i = 64 - 1; i >= 0; i--)
	{
		int originalBitIndex = 63 - (ipInvTable[tableI] - 1);

		DES::SetBit(result, i, DES::GetBit(value, originalBitIndex));
		tableI++;
	}

	return result;
}

void DES::LongLongSplit(long long value, int & l, int & r)
{
	r = value;
	l = value >> 32;
}

int DES::REncoding(int previousL, int previousR, long long currentSubKey)
{
	return previousL ^ DES::FOperation(previousR, currentSubKey);
}

int DES::FOperation(int data, long long key)
{
	long long eData = EOperation(data);

	long long comp = eData ^ key;

	short b[8];

	for (size_t i = 0; i < 8; i++)
	{
		b[i] = (comp >> ((7 - i) * 6)) & 63;
	}

	int result = 0;

	for (size_t i = 0; i < 8; i++)
	{
		short row = GetBit(b[i], 0);
		SetBit(row, 1, GetBit(b[i], 5));
		short column = GetBit(b[i], 1);
		SetBit(column, 3, GetBit(b[i], 4));
		SetBit(column, 2, GetBit(b[i], 3));
		SetBit(column, 1, GetBit(b[i], 2));
		short number = sBoxes[i][row][column];
		result |= number << (7 - i) * 4;
	}

	return P_Permutation(result);
}

long long DES::EOperation(int value)
{
	long long result = 0;

	int tableI = 0;
	for (int i = 48 - 1; i >= 0; i--)
	{
		int originalBitIndex = 31 - (eTable[tableI] - 1);

		DES::SetBit(result, i, DES::GetBit(value, originalBitIndex));
		tableI++;
	}

	return result;
}

void DES::KeySplitToMini(long long value, int & c, int & d)
{
	c = value >> 28;
	d = (value << 64 - 28) >> 64 - 28;
}

void DES::MiniKeyShift(int & key, int leftShifts)
{
	for (size_t i = 0; i < leftShifts; i++)
	{
		bool leftmostBit = DES::GetBit(key, 27);
		key <<= 1;
		DES::SetBit(key, 0, leftmostBit);
	}
}

long long DES::JoinMiniKeys(int c, int d)
{
	d &= 268435455;
	c &= 268435455;
	long long result = (long long)c << 28;
	result |= (long long)d;
	return result;
}
