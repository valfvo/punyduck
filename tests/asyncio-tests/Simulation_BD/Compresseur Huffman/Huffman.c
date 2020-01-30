#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float frequences[256];

// 1) Calculer les frequences

void calculFrequences(char* fichier)
{
	//initialisation du tableau
	for (int i = 0; i < 256; i++) 
	{
		frequences[i] = 0;
	}

	//on ouvre le fichier en lecture
	FILE* fd = fopen(fichier, "r");

	if(fd)
	{
		int c;
		float nb_c = 0;
		//On calcule le nombre d'occurence de chaque caractère du fichier
		while((c = fgetc(fd)) != EOF) 
		{
			++frequences[c];
			++nb_c;	
		}

		for (int i = 0; i < 256; i++)
		{
			if(frequences[i]){
				frequences[i] = frequences[i]/nb_c; // On convertit le nombre d'occurence en fréquence
				printf("%c %3d %f\n", i, i, frequences[i]); //on print les fréquences des characteres qui existent dans le fichier
			}
		}
	}
	else 
	{
		printf("%s\n", "erreur de fichier !");
	}
}

// 2) Creation arbre

struct noeud
{	
	int pere;
	int fd;
	int fg;
	float freq;
};

struct noeud arbre[511];
int dernierN = 255;  // racine

void creationArbre()
{
	for(int i=0 ; i< 511; ++i) //On initialise chaque noeud de l'arbre
	{
		arbre[i].pere = -1; arbre[i].fd = -1; arbre[i].fg = -1; arbre[i].freq = 0;
	}
	for(int i=0 ; i< 256; ++i) //On transpose la fréquence de chaque caractère du fichier au noeud de l'arbre correspondant
	{
		arbre[i].freq = frequences[i];
		if(arbre[i].freq == 1)
		{
			dernierN = i;
		}
	}
	
	int imin1; int imin2; float fmin1; float fmin2; //variables pour trouver les deux plus petites freq et leurs indices pour former les noeuds

	do{ //On construit les relations père-fils entre les noeuds, pour construire l'arbre Huffman de notre fichier
		imin1 = -1; imin2 = -1; fmin1 = 1; fmin2 = 1;

		for(int i = 0; i <= dernierN; i++)
		{
			//On cherche fmin1 et imin1
			if(arbre[i].pere == -1 && arbre[i].freq > 0 && arbre[i].freq <= fmin1)
			{
				imin1 = i;
				fmin1 = arbre[i].freq;
			}	
		}
		for(int i = 0; i <= dernierN; i++)
		{
			//On cherche fmin2 et imin2
			if(arbre[i].pere == -1 && arbre[i].freq > 0 && arbre[i].freq < fmin2 && i!=imin1)
			{
				imin2 = i;
				fmin2 = arbre[i].freq;
			}
		}

		//On met à jour les noeuds de l'arbre
		if(imin1 != -1 && imin2 != -1) //Si on a trouvé deux nouvelles fréquences minimales
		{			
			dernierN++;

			//La racine devient le père des deux fréquences minimales trouvées, avec une fréquences = à la somme de celles de ses 2 fils
			arbre[dernierN].freq = arbre[imin1].freq + arbre[imin2].freq; 
			arbre[dernierN].fg = imin1;
			arbre[dernierN].fd = imin2;

			arbre[imin1].pere = dernierN;
			arbre[imin2].pere = dernierN;
		}
		
		
	}while(imin1 != -1 && imin2 != -1);


	for(int i=0 ; i< 511; ++i)
	{
		if (arbre[i].freq > 0) //On affiche l'arbre
			printf("%i %c : %i %i %i %f\n", i, i, arbre[i].pere, arbre[i].fd, arbre[i].fg, arbre[i].freq);
	}
}

// 3) Génération du code Huffman

char* codes[256]; //Tableau qui stockera le code Huffman de chaque caractère

void parcours(int noeud, char* code)
{
	//Fonction qui parcourt la chaine de caractère de notre fichier et attribue à chaque caractère son code Huffman
	if(arbre[noeud].fd != -1 && arbre[noeud].fg != -1) //Si le noeud n'est pas une feuille
	{
		char* nvCode = (char*) malloc(strlen(code) + 2);
		strcpy(nvCode, code); //On créé un nouveau code qui est le même que celui d'avant, mais avec 2 charactères de plus
		
		//On ajoute zéro à la fin et on continue le parcours de l'arbre en allant à gauche
		nvCode[strlen(code)] = '0';
		nvCode[strlen(code)+1] = '\0';
		parcours(arbre[noeud].fg, nvCode);
		nvCode[strlen(code)] = '1'; //On change le zéro en un, et on va à droite
		parcours(arbre[noeud].fd, nvCode);
		free(nvCode);
	}
	else if(arbre[noeud].fd == -1 && arbre[noeud].fg == -1 && dernierN < 255)
	{
		codes[noeud] = malloc(strlen(code)+1);
		codes[noeud][strlen(code)] = '1';
		codes[noeud][strlen(code)+1] = '\0';	
	}
	else //Si le noeud est une feuille, on ajoute le code du caractère correspondant dans le tableau codes
	{
		codes[noeud] = malloc(strlen(code));
		strcpy(codes[noeud], code);	//On met dans le tableau codes à l'indice des lettres le code généré par récurrence 
	}
}

// 4) Création du fichier compressé

struct BitStream
{
	char* buffer;
	size_t iOctet;
	int offset;
};

typedef struct BitStream BitStream;

void writeBit(BitStream *bitstream, unsigned int bit)
{
	bit = !!bit;  // force bit à etre 0 ou 1
	if(bit)
    {
        bitstream->buffer[bitstream->iOctet] |= 1UL << bitstream->offset;
    }
    --bitstream->offset;
    if(bitstream->offset < 0)
    {
        ++bitstream->iOctet;
        bitstream->offset = 7;
    }
}

void writeCode(BitStream *bitstream, char* code)
{
	for(int i=0; i<strlen(code); i++)
    {
        unsigned int bit = code[i] == '0' ? 0b0 : 0b1;	//0b -> binaire
        writeBit(bitstream, bit);
	}
}

void writeChar(BitStream *bitstream, char c)
{
	for(int i=7; i>=0; --i)
	{
		writeBit(bitstream, (c >> i) & 1U);	//ecrire les 8 bits du charactère en partant de la gauche
	}
}

/*regles :
1- si le noeud est une feuille on ecrit 1 en binaire suivit du code ascii du caractere
   l'index est le code ascii du caractere
2- sinon on ecrit 0 en binaire
*/ 

void encodeTree(size_t index, BitStream *bitstream)
{
    if(arbre[index].fd == -1 && arbre[index].fg == -1) //si c'est une feuille 
    {
        writeBit(bitstream, 1U);
        writeChar(bitstream, index);
    }
    else //Si c'est un noeud
    {
        writeBit(bitstream, 0U);

        encodeTree(arbre[index].fd, bitstream);
        encodeTree(arbre[index].fg, bitstream);
    }
}

int nbChar = 0;
int calculNbChar(size_t node)
{
    if(arbre[node].fd == -1 && arbre[node].fg == -1) //si c'est une feuille 
		nbChar++;
	else
	{
		calculNbChar(arbre[node].fd);
		calculNbChar(arbre[node].fg);
	}
}

void compresse(char* fichier)
{
	//On créé un fichier qui a pour nom celui du fichier de base + ".huff"
	FILE* fd = fopen(fichier, "r");
    char* newFile = malloc(strlen(fichier)+6);
    strcpy(newFile, fichier);
    strcpy(newFile+strlen(fichier), ".huff\0");
	FILE* newFichier = fopen(newFile,"wb");
    free(newFile);

	if(fd)
	{
		int c;
		calculNbChar(dernierN);
		int tailleBuffer = 10*nbChar + 2;	//taille arbre
		while((c = fgetc(fd)) != EOF){
			for(int i=0; i<strlen(codes[c]); i++)
			{
				tailleBuffer++;	//on rajoute taille fichier
			}
			c++;
		}

		char* buffer = malloc(tailleBuffer);
		for(int i=0; i<tailleBuffer; i++)
        {
			buffer[i] = '\0';
		}
		BitStream bitstream = {buffer, 0, 4};	//on met offset a 4 pour laisser les 3 premiers bits libres
		encodeTree(dernierN, &bitstream); //Codage de l'arbre en binaire
		fseek(fd,0, SEEK_SET);	//on revient au debut du fichier
		while((c = fgetc(fd)) != EOF)
        {
			writeCode(&bitstream, codes[c]);
			c+=1;
		}
		unsigned int bitsRestants = bitstream.offset + 1;

		size_t newBufferSize = bitstream.iOctet + (bitstream.offset != 7); //On supprime les octets en trop
		bitstream.iOctet = 0;
		bitstream.offset = 7;

		//On écrit dans les 3 premiers bits du buffer le nombre de bits vides restants à la fin du buffer
		writeBit(&bitstream, (bitsRestants>>2)&1);
		writeBit(&bitstream, (bitsRestants>>1)&1);
		writeBit(&bitstream, bitsRestants&1);
        buffer = realloc(buffer, newBufferSize);
        fwrite(buffer, 1, newBufferSize, newFichier);		
   
		free(buffer);
	}
}

int main(int argc, char** argv){

	// 1- calculer les frequences
	printf("------------------------------------- Etape 1 -------------------------------------\n");
	calculFrequences(argv[1]);

	printf("------------------------------------- Etape 2 -------------------------------------\n");
	// 2 - creation arbre
	creationArbre();

	printf("------------------------------------- Etape 3 -------------------------------------\n");
	// 3- génération du code Huffman
	parcours(dernierN,"");
	

	for(int i=0 ; i< 256; ++i)
	{
		if (arbre[i].freq > 0)
			printf("%i %c : %s\n", i, i, codes[i]);
	}

	printf("------------------------------------- Etape 4 -------------------------------------\n");
	// 4 - creation du fichier compréssé
	compresse(argv[1]);
	printf("Création du fichier compresse\n");

	return 0;
}