#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <cs50.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("Usage: ./recover image\n");
        return 1;
    }
    FILE * card = fopen(argv[1],"r");
    if (card == NULL){
        printf("Invalid Card");
        return 1;
    }
    char filename[10];
    FILE * img;
    BYTE bytes[64];
    int count = 0;
    while(fread(&bytes,sizeof(BYTE),64,card)){
        if(bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] & 0xf0) == 0xe0){
            sprintf(filename,"%03i.jpg",count);
            if(count == 0){
                img = fopen(filename,"w");
                fwrite(bytes,sizeof(BYTE),64,img);
                count++;
            }
            else{
                fclose(img);
                img = fopen(filename,"w");
                fwrite(bytes,sizeof(BYTE),64,img);
                count++;
            }


        }
        else{
            if(count != 0){
                fwrite(bytes,sizeof(BYTE),64,img);
            }
        }
    }
    fclose(img);
    fclose(card);

}