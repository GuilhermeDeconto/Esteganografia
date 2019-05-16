#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "SOIL.h"

// Um pixel RGB
typedef struct {
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct {
    int width, height;
    RGB* img;
} Img;


void decrypt(char vector[]);

char splitBitWords(unsigned char* bit8_7, unsigned char* bit6_5, unsigned char* bit4_3, unsigned char* bit2_1);

void decodePass(Img pic);
char decodeMessage(Img pic);
void decodeEsteganography(char vector[], bool isMessage, Img pic);

int calcSize(bool isMessage, Img pic);

void concatLetter(char word, char vector[], int size);

void load(char* name, Img* pic);


// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{
    int chan;
    pic->img = (unsigned char*) SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    
    if(!pic->img)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, chan);
}

int main(int argc, char** argv)
{

    Img pic;
    if(argc < 1) {
        printf("loader [img]\n");
        exit(1);
    }
    load("saida.bmp", &pic);

    decodePass(pic);

    char decodedMessage = decodeMessage(pic);

    decrypt(decodedMessage);

    printf("\nMENSAGEM: %s\n", decodedMessage);

    free(pic.img);
}

char splitBitWords(unsigned char* bit8_7, unsigned char* bit6_5, unsigned char* bit4_3, unsigned char* bit2_1){

    unsigned char word = 0b00000000;
    *bit8_7 = *bit8_7<<6;
    *bit6_5 = *bit6_5<<4;
    *bit4_3 = *bit4_3<<2;
    *bit2_1 = *bit2_1;

    word = *bit8_7 | word;
    word = *bit6_5 | word;
    word = *bit4_3 | word;
    word = *bit2_1 | word;

    return word;
}
void decodePass(Img pic){
    
    int passwordSize = calcSize(0, pic);
    char decryptedPassword[passwordSize];
    decodeEsteganography(decryptedPassword, false, pic);

    char password[passwordSize];

    bool isVerified = false;
    while (isVerified == false){
        printf("Digite a password:");
        scanf(" %[^\n]s", password);

        if(strcmp(password, decryptedPassword) == 0){
            isVerified = true;
        }else{
            printf("\nSenha incorreta! Digite novamente\n\n");
        }
    };
}

char decodeMessage(Img pic){
    int messageSize = calcSize(1, pic);
    char decryptedMessage[messageSize];
    decodeEsteganography(decryptedMessage, true, pic);
    return decryptedMessage;
}

void decodeEsteganography(char vector[], bool isMessage, Img pic){

    unsigned int mask = 0b00000011;

    char word;
    int size = 0;


    int starts;
    int num_bytes = pic.width * pic.height;

    if(isMessage == true){
        starts = pic.width;
    }else{
        starts = 0;
    }

    for(int i=starts; i<num_bytes; i= i+4){

        unsigned char bit8_7;
        unsigned char bit6_5;
        unsigned char bit4_3;
        unsigned char bit2_1;

        bit8_7 = pic.img[i].r & mask;
        bit6_5 = pic.img[i].g & mask;
        bit4_3 = pic.img[i].b & mask;
        bit2_1 = pic.img[i+1].r & mask;

        word = splitBitWords(&bit8_7, &bit6_5, &bit4_3, &bit2_1);

        if(word == '|'){
            vector[size] = '\0';
            break;
        }

        vector[size] = word;
        size = size +1;

        bit8_7 = pic.img[i+1].g & mask;
        bit6_5 = pic.img[i+1].b & mask;
        bit4_3 = pic.img[i+2].r & mask;
        bit2_1 = pic.img[i+2].g & mask;

        word = splitBitWords(&bit8_7, &bit6_5, &bit4_3, &bit2_1);

        if(word == '|'){
            vector[size] = '\0';
            break;
        }

        vector[size] = word;
        size = size +1;

        bit8_7 = pic.img[i+2].b & mask;
        bit6_5 = pic.img[i+3].r & mask;
        bit4_3 = pic.img[i+3].g & mask;
        bit2_1 = pic.img[i+3].b & mask;

        word = splitBitWords(&bit8_7, &bit6_5, &bit4_3, &bit2_1);

         if(word == '|'){
            vector[size] = '\0';
            break;
        }

        vector[size] = word;
        size = size +1;
    }
}


int calcSize(bool isMessage, Img pic){

    unsigned int mask = 0b00000011;

    char word;
    int size = 0;

    int starts;
    int num_bytes = pic.width * pic.height;

    if(isMessage == true){
        starts = pic.width;
    }else{
        starts = 0;
    }

    for(int i=starts; i<num_bytes; i= i+4){

        unsigned char bit8_7;
        unsigned char bit6_5;
        unsigned char bit4_3;
        unsigned char bit2_1;

        bit8_7 = pic.img[i].r & mask;
        bit6_5 = pic.img[i].g & mask;
        bit4_3 = pic.img[i].b & mask;
        bit2_1 = pic.img[i+1].r & mask;

        word = splitBitWords(&bit8_7, &bit6_5, &bit4_3, &bit2_1);

        if(word == '|'){
            return size;
            break;
        }

        size = size +1;

        bit8_7 = pic.img[i+1].g & mask;
        bit6_5 = pic.img[i+1].b & mask;
        bit4_3 = pic.img[i+2].r & mask;
        bit2_1 = pic.img[i+2].g & mask;

        word = splitBitWords(&bit8_7, &bit6_5, &bit4_3, &bit2_1);

        if(word == '|'){
            return size;
            break;
        }

        size = size +1;

        bit8_7 = pic.img[i+2].b & mask;
        bit6_5 = pic.img[i+3].r & mask;
        bit4_3 = pic.img[i+3].g & mask;
        bit2_1 = pic.img[i+3].b & mask;

        word = splitBitWords(&bit8_7, &bit6_5, &bit4_3, &bit2_1);

         if(word == '|'){
            return size;
            break;
        }

        size = size +1;
    }
    return 0;
}

void decrypt(char vector[]) {

    int size = strlen(vector);
    int i;

    for(i= 0; i<size; i++){
        if(vector[i] == 'a'){
            vector[i] = 122;
        }else if(vector[i] == 'A'){
            vector[i] = 90;
        }else if(vector[i] == '|'){
            break;
        }else if(vector[i] != ' '){
            vector[i] = vector[i] - 1;
        }
    }
}
