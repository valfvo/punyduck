#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned int bitsrestants(char* buffer)
{
    int nbbitsRestants = 0;
    for(int i = 2; i >= 0; i--)
        nbbitsRestants += (buffer[i]-'0') * pow(2, 2-i);
    return nbbitsRestants;
}

struct BitRead
{
    char* buffer;
    int bit;
};

typedef struct BitRead BitRead;

char readBit(BitRead *reader)
{
    char b = (reader->buffer[reader->bit])-'0';
    reader->bit++;

    return b;
}

char readOctet(BitRead *reader)
{
    char lettre = 0;
    int c;
    for(int i = 0; i < 8; i++)
    {
        c = readBit(reader);
        lettre = lettre + (c * pow(2, 7-i));
    }

    return lettre;
}

struct noeud
{	
	char lettre;
    struct noeud *fd;
	struct noeud *fg;
};
typedef struct noeud noeud;

noeud recupArbre(BitRead *reader)
{
    if(readBit(reader))
    {
        noeud leaf = {readOctet(reader), NULL, NULL};
        return leaf;
    }
    else
    {
        noeud *fd = (noeud*) malloc(sizeof(noeud));
        *fd = recupArbre(reader);
        noeud *fg = (noeud*) malloc(sizeof(noeud));
        *fg = recupArbre(reader);
        noeud node = {0, fd, fg};
        return node;
    }  
}

size_t iStr = 0;

void dechiffrage(noeud node, BitRead *reader, char* str)
{
    if(node.fd != NULL && node.fg != NULL) //Si ce n'est pas une feuille
    {
        if(readBit(reader))
            dechiffrage(*(node.fd), reader, str);
        else
            dechiffrage(*(node.fg), reader, str);
    }
    else //Si c'est une feuille
    {
        printf("%c", node.lettre);
        size_t savlen = strlen(str);
        str[iStr] = node.lettre;
        iStr++;
    }
}


void decompresse(char* fichier)
{
	FILE* fd = fopen(fichier, "rb");
    char* newFile = malloc(strlen(fichier)-5);
    memcpy(newFile, fichier, strlen(fichier)-5);
    newFile[strlen(fichier)-5] = '\0';
    FILE* newfd = fopen(newFile, "w");

    char* tmpBuffer;
    char* buffer;
    int c, nboctet, tailleBuffer;
    noeud racine = {0, NULL, NULL};
    char* strfinal = (char*) malloc(1100000);

	if(fd)
	{
		int i = 0;
		fseek(fd, 0, SEEK_END);
        nboctet = ftell(fd);
        tailleBuffer = nboctet*8;
        int nb_val_lues = nboctet;

		rewind(fd);

		tmpBuffer = malloc(nboctet);
        buffer = malloc(tailleBuffer);
        fread(tmpBuffer, 1, nboctet, fd);
    }

    int k = 0;
    for(int i = 0; i < nboctet; i++)
    {
        for(int j = 7; j >= 0; j--)
        {
            buffer[k] = (tmpBuffer[i] & (1u << j) ? '1' : '0');
            k++;
        }
    }

    free(tmpBuffer);

    printf("Buffer : \n");
    for(int i = 0; i < tailleBuffer; i++)
    {
        printf("%i", buffer[i]-'0');
    }
    printf("\n");

    unsigned int nbbitsRestants = bitsrestants(buffer);

    printf("Bits restants : %i\n", nbbitsRestants);

    BitRead reader = {buffer, 3};
    racine = recupArbre(&reader);

    printf("\nCode : \n");
    printf("Taille buffer : %d, reader.bit : %i\n", tailleBuffer, reader.bit);
    
    while(reader.bit < (tailleBuffer-nbbitsRestants))
    {
        // printf("caca\n");
        int savebit = reader.bit;
        // printf("%d\n", reader.bit);
        dechiffrage(racine, &reader, strfinal);
        if(savebit == reader.bit) reader.bit++;
    }
    printf("\n");

    fwrite(strfinal, 1, iStr, newfd);

    fclose(fd);
    fclose(newfd);
}

int main(int argc, char** argv)
{
    decompresse(argv[1]);

    return 0;
}