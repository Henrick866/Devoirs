#include <thread>
#include "Winsocket.h"
#include "DES.h"
#include "MACGen.h"
#include "RC4.h"
#include "ChatUser.h"
using namespace std;

void serveur();

int main()
{
	thread Serveur(serveur);
	Serveur.join();
	system("pause");
	return 0;
}

void serveur()
{
	//Paramètrer la console pour correctement afficher les é, à, ù, etc.
	SetConsoleOutputCP(1252);
	SetConsoleCP(1252);

	Winsocket* Serveur1 = new Winsocket(1);
	Serveur1->CreateSocket();
	Serveur1->Bind();
	Serveur1->Listen();
	Serveur1->Accept();

	cout << endl;

	ChatUser user = ChatUser(Serveur1);

	while (true)
	{
		if (!user.SendLoop())
			break;
		if (!user.ReceiveLoop())
			break;
	}
	Serveur1->CloseSocket();
	delete Serveur1;
}