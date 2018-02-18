#pragma once
#include "DES.h"
#include <string>
using namespace std;

class MACGen
{

public:
	MACGen(long long key);
	~MACGen();
	int GetCodeForMessage_Int(string message);
	string GetCodeForMessage_Str(string message);

private:
	DES* des;
};