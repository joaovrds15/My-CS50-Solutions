#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void checksum(int *array, int numberLength);
void checkBrand(int *array,int numberLength);

int main(void){
    long int number;
    long int divisor = 1;
    int numberLength = 0;

    printf("Number :");
    scanf("%ld",&number);


    while(number % divisor != number){
        numberLength++;
        divisor *= 10;
    }
    if(numberLength < 13){
        printf("INVALID\n");
    }
    else{
        int *cardNumber = (int *) malloc(numberLength * sizeof(int));
        for(int i = 0; i < numberLength; i++){
            cardNumber[i] = number % 10;
            number = number/10;
        }
        checksum(cardNumber,numberLength);
    }
}
void checksum(int *array, int numberLength)
{
    int temp = 0;
    for(int i = 0; i < numberLength;i++){
        if(i % 2 == 0){
            temp += array[i];
        }
        else{
            temp += (array[i] * 2) % 10;
            temp += (array[i] * 2) /10;
        }
    }
    if(temp % 10 == 0){
        checkBrand(array,numberLength);
    }
    else{
        printf("INVALID\n");
    }
}

void checkBrand(int *array,int numberLength){
    if(array[numberLength-1] == 5){
        if(array[numberLength - 2] == 1 || array[numberLength - 2] == 2 || array[numberLength - 2] == 3 ||  array[numberLength - 2] == 4 ||  array[numberLength - 2] == 5){
            printf("MASTERCARD\n");
        }
        else{
            printf("INVALID\n");
        }
    }
    else if(array[numberLength-1] == 4){
        printf("VISA\n");
    }
    else if(array[numberLength-1] == 3){
        if(array[numberLength-2] == 4 || array[numberLength-2] == 7){
            printf("AMEX\n");
        }
        else{
            printf("INVALID\n");
        }
    }
}