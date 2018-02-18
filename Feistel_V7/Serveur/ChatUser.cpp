#include "ChatUser.h"

vector<unsigned char> intToBytes(int paramInt)
{
	vector<unsigned char> arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[3 - i] = (paramInt >> (i * 8));
	return arrayOfByte;
}
vector<unsigned char> GenerateKey(int Seed)
{
	vector<unsigned char> Key(16); //la clé
	srand(Seed);
	for (int i = 0; i < 4; i++)
	{
		int B = rand();
		vector<unsigned char> Nombre = intToBytes(B);
		for (int j = 0; j < 4; j++)
		{
			Key[(i * 4) + j] = Nombre[j];
		}
	}
	return Key;
}

ChatUser::ChatUser(Winsocket * socket)
{
	cout << "\nCe programme d'échange de message fonctionne en tout-à-tour. "
		<< "Après avoir envoyé un message, vous devez donc attendre la réponse avant d'en envoyer un autre.\n\n";

	ChatUser::socket = socket;
	ChatUser::macGen = new MACGen(360648138711912ll);
	ChatUser::rc4 = new RC4();
	ChatUser::RC4Key = GenerateKey(16);
	ChatUser::SetAlgoChoice('F');
}

ChatUser::~ChatUser()
{
	delete macGen;
	delete rc4;
}

bool ChatUser::SendLoop()
{
	cout << "\n\n--------OPTIONS--------\nEnvoyé un message: le texte\nChanger d'algorithme: -a\nQuitter: -q\nNe rien faire:\n\n";

	string userInput;
	GetUserInput(userInput);
	if (userInput.length() == 0)
	{
		SendNothing();
	}
	else if (userInput == "-a")
	{
		SendAlgoChange();
	}
	else if (userInput == "-q")
	{
		SendQuit();
		return false;
	}
	else
	{
		ChatUser::SendMessage_(userInput);
	}

	return true;
}

bool ChatUser::ReceiveLoop()
{
	string message = ReceiveData();
	if (message.length() == 0)
	{
		return false;
	}
	else
	{
		switch (message[0])
		{
		case 'n':
			cout << "\n\n--------L'interlocuteur ne fait rien...--------\n\n";
			break;
		case 'm':
			cout << "\n\n--------Message--------\n";
			ShowMessage(message.substr(1));
			break;
		case 'a':
			cout << "\n\n--------Nouvel algorithme d'encryption--------\n";
			SetAlgoChoice(message[1]);
			break;
		case 'q':
			cout << "\n\n--------L'interlocuteur a quitté...--------\n";
			return false;
		default:
			break;
		}

		return true;
	}
}

void ChatUser::SendMessage_(string message)
{
	SendData(message.insert(0, 1, 'm'));
}

void ChatUser::SendAlgoChange()
{
	if (cryptoAlgo == 'F')
	{
		SendData("aR");
		SetAlgoChoice('R');
	}
	else
	{
		SendData("aF");
		SetAlgoChoice('F');
	}
}

void ChatUser::SendQuit()
{
	SendData("q");
}

void ChatUser::SendNothing()
{
	SendData("n");
}

void ChatUser::ShowMessage(string message)
{
	cout << message << endl << endl;
}

void ChatUser::SetAlgoChoice(char algo)
{
	cout << "NEW CRYPTO ALGO: ";
	if (algo == 'F')
		cout << "Feistel\n";
	else
		cout << "RC4\n";

	cryptoAlgo = algo;
}

void ChatUser::SendData(string data)
{
	if (data.length() == 0)
	{
		cout << "Erreur d'envoi, message trop court." << endl;
		return;
	}

	//------------------Encryption------------------//

	switch (cryptoAlgo)
	{
	case 'R':
		data = EncryptRC4(data);
		break;
	case 'F':
		data = EncryptFeistel(data);
		break;
	default:
		cout << "Erreur d'encryption, aucun algorithme spécifié" << endl;
		return;
	}
	cout << "Message encrypté: " << data << endl;

	//------------------MAC Append------------------//
	string mac = macGen->GetCodeForMessage_Str(data);
	cout << "MAC calculé: " << mac << endl;

	if (mac.length() != 4)
	{
		cout << "Erreur de génération de MAC. Il n'est pas de 32 bit." << endl;
		return;
	}
	data.append(mac);


	//------------------Send------------------//
	cout << "Message brut complet: " << data << endl;
	socket->Send(data);
	cout << "Message envoyé..." << endl;
}

string ChatUser::ReceiveData()
{
	//------------------Receive------------------//

	cout << "\n\nEn attente de message ..." << endl;
	string data = socket->Receive();
	cout << "Message recue: " << data << endl;

	if (data.length() < 5)
	{
		cout << "Erreur de réception, message trop court" << endl;
		return "";
	}
	else
	{
		//------------------MAC Comparison------------------//

		string mac = data.substr(data.length() - 4);
		string message = data.substr(0, data.length() - 4);
		string myMac = macGen->GetCodeForMessage_Str(message);
		if (myMac != mac)
		{
			cout << "Message non-authentifié! Les MAC ne correspondent pas: " << mac << " vs " << myMac << endl;
			return "";
		}

		cout << "Message authentifié! Les MAC correspondent: " << mac << endl;


		//------------------Decryption------------------//

		switch (cryptoAlgo)
		{
		case 'R':
			message = DecryptRC4(message);
			break;
		case 'F':
			message = DecryptFeistel(message);
			break;
		default:
			cout << "Erreur de décryption, aucun algorithme spécifié" << endl;
			return "";
		}

		cout << "Message décrypté avec succes: " << message << endl;

		return message;
	}
}

void ChatUser::GetUserInput(string& result)
{
	getline(cin, result);
}

string ChatUser::DecryptFeistel(string s)
{
	string messageReceive;
	bitset<16> IV = socket->StringTo16BitInvert("0101010101010101");
	bitset<16> key = socket->StringTo16BitInvert("0101101010101001");
	messageReceive = socket->DecryptCipherBlockChaining(s, IV, key);
	return messageReceive;
}

string ChatUser::DecryptRC4(string s)
{
	rc4->GenererClef(RC4Key, sizeof(RC4Key));
	return rc4->Decrypt(s);
}

string ChatUser::EncryptFeistel(string s)
{
	string messageSend;
	bitset<16> IV = socket->StringTo16BitInvert("0101010101010101");
	bitset<16> key = socket->StringTo16BitInvert("0101101010101001");
	messageSend = (socket->EncryptCipherBlockChaining(s, IV, key));

	return messageSend;
}

string ChatUser::EncryptRC4(string s)
{
	rc4->GenererClef(RC4Key, sizeof(RC4Key));//pour réinitialiser la clé
	return rc4->Encrypt(s);
}
