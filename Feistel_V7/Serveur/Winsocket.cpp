#include "Winsocket.h"

Winsocket::Winsocket()
{
}

Winsocket::Winsocket(int type)
{
	if (type == 1)//Serveur
	{
		TypeSocket = 1;
		nPort = PORT;
		nRet = WSAStartup(wVersionRequested, &wsaData);

		if (wsaData.wVersion != wVersionRequested)
		{
			cout << "Wrong version";
			return;
		}
	}
	else if (type == 2)//Client
	{
		TypeSocket = 2;
		cout << "Enter Pc name of Server" << endl;
		cin.getline(PC_Name, 100);
		nPort = PORT;
		nRet = WSAStartup(wVersionRequested, &wsaData);

		if (wsaData.wVersion != wVersionRequested)
		{
			cout << "Wrong version";
			return;
		}
	}
}

Winsocket::~Winsocket()
{
}

void Winsocket::CreateSocket()
{
	if (TypeSocket == 1)//Serveur
	{
		listenSocket = socket(AF_INET,			// Address family
			SOCK_STREAM,		// Socket type
			IPPROTO_TCP);		// Protocol

		if (listenSocket == INVALID_SOCKET)
		{
			PRINTERROR("socket()");
			return;
		}
		saServer.sin_family = AF_INET;
		saServer.sin_addr.s_addr = INADDR_ANY;	// Let WinSock supply address
		saServer.sin_port = htons(nPort);		// Use port from command line

	}
	else if (TypeSocket == 2)//Client
	{
		cout << "\nStream Client connecting to server: " << PC_Name << " on port: " << nPort;
		cout << endl;

		lpHostEntry = gethostbyname(PC_Name);
		if (lpHostEntry == NULL)
		{
			PRINTERROR("gethostbyname()");
			return;
		}

		theSocket = socket(AF_INET,			// Address family
			SOCK_STREAM,        // Socket type
			IPPROTO_TCP);		// Protocol

		if (theSocket == INVALID_SOCKET)
		{
			PRINTERROR("socket()");
			return;
		}

		saServer.sin_family = AF_INET;
		saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
		saServer.sin_port = htons(nPort);				// Port number from command line
	}
}

void Winsocket::Bind()
{
	nRet = bind(listenSocket,				// Socket
		(LPSOCKADDR)&saServer,		// Our address
		sizeof(struct sockaddr));	// Size of address structure

	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("bind()");
		closesocket(listenSocket);
		return;
	}

	nLen = sizeof(SOCKADDR);

	nRet = gethostname(szBuf, sizeof(szBuf));
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("gethostname()");
		closesocket(listenSocket);
		return;
	}
	cout << "\nServer named " << szBuf << " waiting on port " << nPort;
}

void Winsocket::Listen()
{
	cout << "\nlisten()";
	nRet = listen(listenSocket,		// Bound socket
		SOMAXCONN);			// Number connection request queue

	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("listen()");
		closesocket(listenSocket);
		return;
	}
}

void Winsocket::Accept()
{
	cout << "\nBlocking at accept()";
	remoteSocket = accept(listenSocket,	// Listening socket
		NULL,			// Optional client address
		NULL);

	if (remoteSocket == INVALID_SOCKET)
	{
		PRINTERROR("accept()");
		closesocket(listenSocket);
		return;
	}
}

void Winsocket::Connect()
{
	nRet = connect(theSocket,					// Socket
		(LPSOCKADDR)&saServer,		// Server address
		sizeof(struct sockaddr));	// Length of server address 
									// structure
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("socket()");
		closesocket(theSocket);
		return;
	}
}

void Winsocket::Send(string message)
{
	memset(szBuf_Receive, 0, sizeof(szBuf_Receive));

	strcpy_s(szBuf_send, message.c_str());

	if (TypeSocket == 1)//Serveur
	{
		nRet = send(remoteSocket,				// Connected socket
			szBuf_send,					// Data buffer
			strlen(szBuf_send),			// Length of data
			0);							// Flags
	}
	else if (TypeSocket == 2)//Client
	{
		nRet = send(theSocket,					// Connected socket
			szBuf_send,					// Data buffer
			strlen(szBuf_send),			// Length of data
			0);							// Flags
	}
}

string Winsocket::Receive()
{
	memset(szBuf_Receive, 0, sizeof(szBuf_Receive));

	if (TypeSocket == 1)//Serveur
	{
		nRet = recv(remoteSocket,				// Connected socket
			szBuf_Receive,				// Receive buffer
			sizeof(szBuf_Receive),		// Size of receive buffer
			0);							// Flags

	}
	else if (TypeSocket == 2)//Client
	{
		nRet = recv(theSocket,					// Connected socket
			szBuf_Receive,				// Receive buffer
			sizeof(szBuf_Receive),		// Size of receive buffer
			0);							// Flags
	}
	message = szBuf_Receive;
	return message;
}

void Winsocket::CloseSocket()
{
	if (TypeSocket == 1)//Serveur
	{
		closesocket(remoteSocket);
		closesocket(listenSocket);
	}
	else if (TypeSocket == 2)//Client
	{
		closesocket(theSocket);
	}
	WSACleanup();
}

bool Winsocket::PairNumber(int item1)
{
	if (item1 % 2 == 0)
		return true;
	else
		return false;
}

bitset<16> Winsocket::StringTo16Bit(string message)
{
	bitset<16> tmp;

	for (int i = 0; i < 16;i++)
	{
		if (message[i] == '0')
		{
			tmp[i] = 0;
		}
		else if (message[i] == '1')
		{
			tmp[i] = 1;
		}
	}
	return tmp;
}

bitset<16> Winsocket::StringTo16BitInvert(string message)//ok
{
	bitset<16> tmp;

	for (int i = 0; i < 16;i ++) 
	{
		if (message[i] == '0')
		{
			tmp[15-i] = 0;
		}
		else if (message[i] == '1')
		{
			tmp[15-i] = 1;
		}
	}
	return tmp;
}

string Winsocket::_8BitToString(bitset<8> item1)
{
	string tmp1 = item1.to_string();
	return tmp1;
}

string Winsocket::_16BitToString(bitset<16> item1)
{
	string tmp1 = item1.to_string();
	return tmp1;
}

bitset<16> Winsocket::ConcatenateBitset(bitset<8> item1, bitset<8> item2) 
{
	string tmp1, tmp2, tmp3;
	bitset<16> temp4;

	tmp1 = _8BitToString(item1);
	tmp2 = _8BitToString(item2);
	tmp3 += tmp1; //HS : Je ne sais pas pourquoi tu a concaténé la deuxième lettre en premier alors j'ai remis dans le bon ordre.
	tmp3 += tmp2; //HS

	return (temp4 = (StringTo16Bit(tmp3)));
}

bitset<8> Winsocket::ResizeBitsetToRight(bitset<16> item1)
{
	bitset<8> tmp;
	for (int i = 0; i < 8;i++)
	{
		tmp[7 - i] = item1[i];//HS: inverse les bits car le constructeur de bitset inverse l'endian original (BigEndian->LittleEndian)
	}
	return tmp;
}

bitset<8> Winsocket::ResizeBitsetToLeft(bitset<16> item1)
{
	bitset<8> tmp;
	for (int i = 8; i < 16;i++)
	{
		tmp[7 - (i - 8)] = item1[i];//HS: inverse les bits car le constructeur de bitset inverse l'endian original (BigEndian->LittleEndian)
	}
	return tmp;
}

bitset<8> Winsocket::Xor8Bit(bitset<8> item1, bitset<8> item2)
{
	bitset<8> tmp;
	tmp = item1 ^ item2;
	return tmp;
}

bitset<16> Winsocket::Xor16Bit(bitset<16> item1, bitset<16> item2)
{
	bitset<16> tmp;
	tmp = item1 ^ item2;
	return tmp;
}

void Winsocket::SwapRight(bitset<8>& item1, bitset<8>& item2)
{
	bitset<8> temp;
	temp = item2;
	item2 = item1;
	item1 = temp;
}

void Winsocket::RightCircularShift(bitset<16>& item) //bitset[access] = 4,3,2,1,0
{
	bitset<1>  tmp0;
	bitset<16> tmp1;
	tmp1 = item;

	tmp0[0] = tmp1[0]; //copie

	for (int i = 0;i < 15;i++) //shift
	{
		tmp1[i] = tmp1[i + 1];
	}
	tmp1[15] = tmp0[0];
	item = tmp1;
}

void Winsocket::LeftCircularShift(bitset<16>& item) 
{
	bitset<1>  tmp0;
	bitset<16> tmp1;
	tmp1 = item;

	tmp0[0] = tmp1[15]; //copie

	for (int i = 15;i > 0;i--) //shift
	{
		tmp1[i] = tmp1[i-1];
	}
	tmp1[0] = tmp0[0];
	item = tmp1;
}

void Winsocket::StringToVectorBitset(string message, vector<bitset<16>>& vector)
{
	bitset<8> tmp1;
	bitset<8> tmp2;
	string S1;
	string S2;
	if (PairNumber(message.length()) == true)//message = paire
	{
		for (int i = 0; i < message.length() - 1;i += 2) //recherche dans string
		{
			tmp1 = bitset<8>(message[i]); //HS: ICI, l'endian des bits de message[i] est inversé, c'est-à-dire que les bits les plus faibles sont à gauche au lieu d'être à droite, l'habituel.
			tmp2 = bitset<8>(message[i + 1]);
			vector.push_back(ConcatenateBitset(tmp1, tmp2));
		}
	}
	else
	{
		for (int i = 0; i < message.length();i += 2) //recherche dans string
		{
			tmp1 = bitset<8>(message[i]);
			if (i == message.length() - 1)
			{
				tmp2 = (00000000);//bit remplissage
			}
			else
			{
				tmp2 = bitset<8>(message[i + 1]);
			}

			vector.push_back(ConcatenateBitset(tmp1, tmp2));
		}
	}
}

string Winsocket::VectorBitsetToString(vector<bitset<16>> vector)
{
	string tmp;
	string tmpLeft;
	string tmpRight;
	for (int i = 0; i < vector.size(); i++)
	{
		tmpLeft = char(ResizeBitsetToLeft(vector[i]).to_ulong());//HS: méthode fournis par StackOverflow
		tmpRight = char(ResizeBitsetToRight(vector[i]).to_ulong());
		tmp += tmpLeft + tmpRight;//HS: une simple concaténation operator+ marche

								  /*tmpLeft = ResizeBitsetToLeft(vector[i]).to_string();
								  tmpRight = ResizeBitsetToRight(vector[i]).to_string();
								  tmp.push_back((char)tmpLeft.c_str());
								  tmp.push_back((char)tmpRight.c_str());*/
	}
	return tmp;
}

string Winsocket::VectorBitsetToBinaryString(vector<bitset<16>> vector)
{
	string tmp;
	string tmpToSend;
	for (int i = 0; i < vector.size();i++)
	{
		tmp = vector[i].to_string();
		tmpToSend += tmp;
	}
	return tmpToSend;
}

void Winsocket::StringBinaryToVectorBiset(string message, vector<bitset<16>>& vector)
{
	string tmp;
	int round;
	round = message.length() / 16;

	for (int i = 0; i < round ;i ++)
	{
		for (int j = 0;j < 16;j++)
		{
			tmp.push_back(message[j]);
		}
		vector.push_back(StringTo16Bit(tmp));
		tmp.clear();
	}	
}

bitset<16> Winsocket::FeistelEncrypt(bitset<16> Block, bitset<16> key)
{
	//cout << "Encrypt original key " << key << endl;

	int round = 1;
	bitset<8> BlockLeft;
	bitset<8> BlockRight;
	bitset<8> SubKey;
	bitset<8> FonctionOutput;
	bitset<16> EncryptedText;

	//__________Division 2 blocks_____________

	//cout << "Division en 2 block" << endl;

	BlockLeft = ResizeBitsetToLeft(Block);		//[15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0]

	BlockRight = ResizeBitsetToRight(Block);

	while (round <= 16)
	{
		//cout << "\n_____________Encrypt Round "<< round << "_____________" << endl;
		//__________Circular rotation key___________

		if(round > 1)
		{
			RightCircularShift(key);
			SubKey = ResizeBitsetToRight(key);
			//cout << "Right circular shift of key" << endl;
		}

		else if (round == 1)
		{
			SubKey = ResizeBitsetToRight(key);
		}

		//cout << "Encrypt key round "<<round<<" " << key << endl;

		//__________Fonction XOR____________________

		FonctionOutput = Xor8Bit(BlockRight, SubKey);

		//cout << "Block gauche vers fonction" << endl;

		//__________XOR_____________________________

		BlockLeft = Xor8Bit(BlockLeft, FonctionOutput);

		//cout << "Block droit XOR sortie fonction " << endl;

		//__________Swap____________________________

		SwapRight(BlockLeft, BlockRight);
		round++;
		//cout << "swap block droite avec gauche" << endl;
	}
	SwapRight(BlockLeft, BlockRight); //HS:il manquait une dernière inversion ici.
	//cout << "\nswap final block droite avec gauche" << endl;
	EncryptedText = ConcatenateBitset(BlockLeft, BlockRight);

	return EncryptedText;
}

bitset<16> Winsocket::FeistelDecrypt(bitset<16> Block, bitset<16> key)
{
	//cout << "Decrypt original key "<< key << endl;
	int round = 1;
	bitset<8> BlockLeft;
	bitset<8> BlockRight;
	bitset<8> SubKey;
	bitset<8> FonctionOutput;
	bitset<16> DecryptedText;

	//__________Key 16e round________

	for (int i = 0; i < 15;i++) //Henrick: Proposition: LeftCircularShift(key);
	{
		RightCircularShift(key);
	}

	//__________Division 2 block_____________

	//cout << "Division en 2 block" << endl;

	BlockLeft = ResizeBitsetToLeft(Block);		//[15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0]

	BlockRight = ResizeBitsetToRight(Block);

	while (round <= 16)
	{
		//cout << "\n_____________Decrypt Round " << round <<"_____________" << endl;
		//__________Circular rotation key___________

		if (round > 1)
		{
			LeftCircularShift(key);
			SubKey = ResizeBitsetToRight(key);
			//cout << "Right circular shift of key" << endl;
		}

		else if (round == 1)
		{
			SubKey = ResizeBitsetToRight(key);
		}

		//cout << "Decrypt key round " << round << " " << key << endl;
		
		//__________Fonction XOR____________________

		FonctionOutput = Xor8Bit(BlockRight, SubKey);

		//cout << "Block gauche vers fonction" << endl;

		//__________XOR_____________________________

		BlockLeft = Xor8Bit(BlockLeft, FonctionOutput);

		//cout << "Block droit XOR sortie fonction " << endl;

		//__________Swap____________________________

		SwapRight(BlockLeft, BlockRight);
		round++;
		//cout << "swap block droite avec gauche" << endl;
	}
	SwapRight(BlockLeft, BlockRight); //HS:il manquait une dernière inversion ici.
	//cout << "\nswap final block droite avec gauche" << endl;
	DecryptedText = ConcatenateBitset(BlockLeft, BlockRight);

	return DecryptedText;
}

string Winsocket::EncryptCipherBlockChaining(string message, bitset<16> IV, bitset<16> key)
{
	//cout << "\nDebut encrypt cypher block chainning\n"<< endl;
	TexteBrute.clear(); //HS:Reset pour éviter une concaténation durant la conversation
	EncryptedMessage.clear(); //HS:Reset pour éviter une concaténation durant la conversation
	DecryptedMessage.clear(); //HS:Reset pour éviter une concaténation durant la conversation
	StringToVectorBitset(message,TexteBrute);
	Originalkey = key;

	/*for (int j = 0; j < TexteBrute.size();j++)
	{
		cout << "Texte brute : " << TexteBrute[j] << endl;
	}*/

	for (int i = 0;i < TexteBrute.size();i++)
	{
		if (i == 0 )
		{
			EncryptedMessage.push_back(FeistelEncrypt((Xor16Bit(TexteBrute[i], IV)), Originalkey));
		}
		else if (i >0 )
		{
			EncryptedMessage.push_back(FeistelEncrypt((Xor16Bit(TexteBrute[i], EncryptedMessage[i-1])), Originalkey));
		}
	}

	/*for (int k = 0; k < EncryptedMessage.size();k++)
	{
		cout << "Texte encrypted : " << EncryptedMessage[k] << endl;
	}

	cout << "\nFin encrypt cypher block chainning\n" << endl;*/

	return (VectorBitsetToString(EncryptedMessage));
}

string Winsocket::DecryptCipherBlockChaining(string cipher, bitset<16> IV, bitset<16> key)
{
	TexteBrute.clear(); //HS:Reset pour éviter une concaténation durant la conversation
	EncryptedMessage.clear(); //HS:Reset pour éviter une concaténation durant la conversation
	DecryptedMessage.clear(); //HS:Reset pour éviter une concaténation durant la conversation
	StringToVectorBitset(cipher, EncryptedMessage); //HS:car il n'est pas initialisé côté serveur.

	Originalkey = key;
	bitset<16> tmp;
	for (int i = 0; i < EncryptedMessage.size(); i++)
	{
		if (i == 0)
		{
			tmp = FeistelDecrypt(EncryptedMessage[i], Originalkey);
			DecryptedMessage.push_back(Xor16Bit(tmp, IV));
		}
		else if (i >0)
		{
			tmp = FeistelDecrypt(EncryptedMessage[i], Originalkey);
			DecryptedMessage.push_back(Xor16Bit(tmp, EncryptedMessage[i - 1]));
		}
	}

	/*for (int j = 0; j < DecryptedMessage.size(); j++)
	{
		cout << "Texte decrypted :   " << DecryptedMessage[j] << endl;
	}*/

	return (VectorBitsetToString(DecryptedMessage));
}