#include <thread>
#include "Winsocket.h"
#include "DES.h"
#include "MACGen.h"
#include "RC4.h"
#include "ChatUser.h"
using namespace std;

void client();

int main()
{
	thread Client(client);
	Client.join();
	system("pause");

	return 0;
}

void client()
{
	//Param�trer la console pour correctement afficher les �, �, �, etc.
	SetConsoleOutputCP(1252);
	SetConsoleCP(1252);

	Winsocket* Client1 = new Winsocket(2);
	Client1->CreateSocket();
	Client1->Connect();

	cout << endl;

	ChatUser user = ChatUser(Client1);
	
	while (true)
	{
		if (!user.SendLoop())
			break;
		if (!user.ReceiveLoop())
			break;
	}

	Client1->CloseSocket();
	delete Client1;
}