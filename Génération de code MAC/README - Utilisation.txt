Afin de cr�er un code mac pour un message donn�e, il faut:

1.Se cr�er une instance de MACGen avec une cl� de 64 bit:

MACGen macGen = MACGen(cl�);



2.Appeler la m�thode pour obtenir un code MAC en passant le message en param�tre:

string codeMac = GetCodeForMessage_Str(message);
OU
int codeMac = GetCodeForMessage_Int(message);



NB: 
La g�n�ration du code MAC utilise l'algorithme d'encryption DES, de l� l'inclusion des
fichiers DES.cpp et DES.h (c'�tait painfull � coder).