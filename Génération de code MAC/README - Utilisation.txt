Afin de créer un code mac pour un message donnée, il faut:

1.Se créer une instance de MACGen avec une clé de 64 bit:

MACGen macGen = MACGen(clé);



2.Appeler la méthode pour obtenir un code MAC en passant le message en paramètre:

string codeMac = GetCodeForMessage_Str(message);
OU
int codeMac = GetCodeForMessage_Int(message);



NB: 
La génération du code MAC utilise l'algorithme d'encryption DES, de là l'inclusion des
fichiers DES.cpp et DES.h (c'était painfull à coder).