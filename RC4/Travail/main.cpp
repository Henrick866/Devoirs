// Travail.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include "RC4.h"
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

int main()
{
	
	unsigned char* Key = (unsigned char *)"abcd"; //la clé
	std::string MessageOrig = "I LOVE TOAST!";//le méssage
	char * MessageChar = (char *)MessageOrig.c_str();//conversion de string à char
	RC4* ProtocoleRC4 = new RC4(MessageChar);//construit l'obj RC4


	/*Code à faire dans la méthode mère*/
	ProtocoleRC4->GenererClef(Key, strlen((char*)Key));//pour réinitialiser la clé
	char* Encrypted = ProtocoleRC4->Encrypt();
	for (int i = 0; i < MessageOrig.size(); i++) {
		std::cout << Encrypted[i];//on peut faire un string
	}
	std::cout << std::endl;//et on envoie le string

	ProtocoleRC4->GenererClef(Key, strlen((char*)Key));
	char* Decrypted = ProtocoleRC4->Decrypt();
	for (int i = 0; i < MessageOrig.size(); i++) {
		std::cout << Decrypted[i];
	}
    return 0;
}

