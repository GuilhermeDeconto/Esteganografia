#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"

// Um pixel RGB
typedef struct
{
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct
{
    int width, height;
    RGB *img;
} Img;

void cripto(char vector[], int size);

void splitBitsWords(char letra, unsigned char *bit8_7, unsigned char *bit6_5, unsigned char *bit4_3, unsigned char *bit2_1);

void esteganografia(char vector[], int type, float num_bytes, Img pic);

void load(char *name, Img *pic);

// Carrega uma imagem para a struct Img
void load(char *name, Img *pic)
{
    int channel;
    pic->img = (unsigned char *)SOIL_load_image(name, &pic->width, &pic->height, &channel, SOIL_LOAD_RGB);
    if (!pic->img)
    {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, channel);
}

int main(int argc, char **argv)
{
    Img pic;
    if (argc < 1)
    {
        printf("loader [img]\n");
        exit(1);
    }
    load(argv[1], &pic);

    char password[pic.width]; //Tamanho maximo da password corresponde a um linha
    printf("Digite a senha para criptografar a mensagem:");
    scanf(" %[^\n]s", password);

    // Pega o size da password e cria outra variável pegando o valor da password e somando mais 1 por causa do simbolo
    float passwordSize = strlen(password);
    int passwordSizeWithSymbol = passwordSize + 1;

    // Cria o array com o size com o simbolo ja incluido
    char passwordWithSymbol[passwordSizeWithSymbol];
    strcpy(passwordWithSymbol, password);

    //////////////////////////////////////////////////////////////////////////////////

    char message[(pic.width * pic.height) - pic.width]; // tamanho maximo da message é a partir da segunda linha ate o final da imagem.
    printf("Digite a mensagem a ser criptografada:");
    scanf(" %[^\n]s", message); // Pega mesagem completa incluindo espaços

    // pega o size da message cria outro size somando mais 1 por causa do simbolo
    float messageSize = strlen(message);
    int messageSizeWithSymbol = messageSize + 1;

    // Cria um novo vector com o vector antigo mais 1 espaço (estava bugando a imagem se tentava concatenar).
    char messageWithSymbol[messageSizeWithSymbol];
    strcpy(messageWithSymbol, message);

    // Calcula o numero de bytes que o laço vai ter q percorrer
    float num_bytesPassword = ((passwordSize + 1) * 4) / 3;
    float num_bytes = ((messageSize + 1) * 4) / 3;

    // Adiciona o simbolo de parada a password e a message
    char sinal[] = "|"; // ASCIII 124 - simbolo de parada
    strcat(passwordWithSymbol, sinal);

    strcat(messageWithSymbol, sinal);

    // Realiza a criptografia da message
    cripto(messageWithSymbol, messageSizeWithSymbol);

    esteganografia(passwordWithSymbol, 0, ceil(num_bytesPassword), pic); // Arredonda pra cima o size, valor 0 significa que é uma password.
    esteganografia(messageWithSymbol, 1, ceil(num_bytes), pic);          // Arredonda pra cima o size, valor 1 significa que é uma message.

    //Salva a imagem
    SOIL_save_image("saida.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height, 3, pic.img);

    free(password);
    free(message);
    free(passwordWithSymbol);
    free(messageWithSymbol);
    free(pic.img);

    return 1;
}

// Preapara a letra para passar pro RGB.

void splitBitsWords(char letra, unsigned char *bit8_7, unsigned char *bit6_5, unsigned char *bit4_3, unsigned char *bit2_1)
{

    unsigned int mask2 = 0b00000011;
    int aux;
    aux = letra;

    //printf("aux = %c\n", aux);

    unsigned char palavra = aux;
    *bit8_7 = palavra >> 6;
    *bit6_5 = palavra >> 4;
    *bit4_3 = palavra >> 2;
    *bit2_1 = palavra;

    *bit8_7 = *bit8_7 & mask2;
    *bit6_5 = *bit6_5 & mask2;
    *bit4_3 = *bit4_3 & mask2;
    *bit2_1 = *bit2_1 & mask2;
}

// Bota a letra no RGB
// O type pode ser 0 ou 1.
// se for 0 significa q o vector que ele esta passando é uma password entao ele vai botar a password na primeira linha da imagem
// se for 1 significa q o vector é uma message entao ele vai botar a message na segunda linhha da imagem.
void esteganografia(char vector[], int type, float num_bytes, Img pic)
{

    int j = 0;                      //contador da palavra a cada laco 3 letras
    unsigned int mask = 0b11111100; // Mascara para limpar os 2 primeiros bits do rgb
    int pos_bytes;
    int starts;

    // TODO | configurar salto aqui
    if (type >= 1)
    {
        pos_bytes = num_bytes + pic.width;
        starts = pic.width; // Começa segunda linha
    }
    else
    {
        pos_bytes = num_bytes;
        starts = 0; // Começa primeira linha
    }

    for (int i = starts; i < pos_bytes; i = i + 4)
    {

        unsigned char bit8_7;
        unsigned char bit6_5;
        unsigned char bit4_3;
        unsigned char bit2_1;

        int num1 = i + 1;

        if (num1 >= pos_bytes)
        {
            break;
        }

        pic.img[i].r = pic.img[i].r & mask;
        pic.img[i].g = pic.img[i].g & mask;
        pic.img[i].b = pic.img[i].b & mask;
        pic.img[i + 1].r = pic.img[i + 1].r & mask;

        splitBitsWords(vector[j], &bit8_7, &bit6_5, &bit4_3, &bit2_1);

        pic.img[i].r = pic.img[i].r | bit8_7;
        pic.img[i].g = pic.img[i].g | bit6_5;
        pic.img[i].b = pic.img[i].b | bit4_3;
        pic.img[i + 1].r = pic.img[i + 1].r | bit2_1;

        num1 = i + 2;

        if (num1 >= pos_bytes)
        {
            break;
        }

        pic.img[i + 1].g = pic.img[i + 1].g & mask;
        pic.img[i + 1].b = pic.img[i + 1].b & mask;
        pic.img[i + 2].r = pic.img[i + 2].r & mask;
        pic.img[i + 2].g = pic.img[i + 2].g & mask;

        splitBitsWords(vector[j + 1], &bit8_7, &bit6_5, &bit4_3, &bit2_1);

        pic.img[i + 1].g = pic.img[i + 1].g | bit8_7;
        pic.img[i + 1].b = pic.img[i + 1].b | bit6_5;
        pic.img[i + 2].r = pic.img[i + 2].r | bit4_3;
        pic.img[i + 2].g = pic.img[i + 2].g | bit2_1;

        num1 = i + 3;

        if (num1 >= pos_bytes)
        {
            break;
        }

        pic.img[i + 2].b = pic.img[i + 2].b & mask;
        pic.img[i + 3].r = pic.img[i + 3].r & mask;
        pic.img[i + 3].g = pic.img[i + 3].g & mask;
        pic.img[i + 3].b = pic.img[i + 3].b & mask;

        splitBitsWords(vector[j + 2], &bit8_7, &bit6_5, &bit4_3, &bit2_1);

        pic.img[i + 2].b = pic.img[i + 2].b | bit8_7;
        pic.img[i + 3].r = pic.img[i + 3].r | bit6_5;
        pic.img[i + 3].g = pic.img[i + 3].g | bit4_3;
        pic.img[i + 3].b = pic.img[i + 3].b | bit2_1;

        j = j + 3;
    }
}

// A criptografia utilizada é a cifra de cesar, simplesmente trocamos a letra atual pela proxima do alfabeto.
void cripto(char vector[], int size)
{   
   int i;
    for (i = 0; i < size; i++)
    {
        if (vector[i] == 'z')
        {
            vector[i] = 97;
        }
        else if (vector[i] == 'Z')
        {
            vector[i] = 65;
        }
        else if (vector[i] == '|')
        {
            break;
        }
        else if (vector[i] != ' ')
        {
            vector[i] = vector[i] + 1;
        }
    }
}
