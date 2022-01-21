#include <stdio.h>
#include <cs50.h>

int main(void){
    int height = 0;
    do{
        height = get_int("Height:");
    }while(height < 1 || height > 8);
    int i = 1;
    int j = 0;

    while(i <= height){
        for(j = i; j < height;j++){
            printf(" ");
        }
        for(int k = 0; k < i;k++){
            printf("#");
        }
        printf("\n");
        i++;
    }

}