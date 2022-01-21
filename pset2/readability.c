#include <stdio.h>
#include <string.h>
#include <math.h>
int calculateLetters(char * text);
int calculateWords(char * text);
int calculateSentences(char * text);


int main(void){
    char text[3000];
    printf("Text: ");
    scanf("%[^\n]",text);
    int letters = calculateLetters(text);
    int words = calculateWords(text);
    int sentences = calculateSentences(text);
    float averageLetters =  (100/(float)words) * letters;
    float averageSentences = (100/(float)words) * sentences;
    float index = 0.0588 * averageLetters - 0.296 * averageSentences - 15.8;
    if(index > 1 && index < 15){
        printf("Grade %d\n",(int)round(index));
    }
    else if(index < 1){
        printf("Before Grade 1\n");
    }
    else{
        printf("Grade 16+\n");
    }


}

int calculateLetters(char * text){
    int tamanho = 0;
    int i = 0;
    while(text[i] != '\0'){
        if((text[i] >= 'A' && text[i] <= 'Z') || (text[i] >= 'a' && text[i] <='z'))
        {
            tamanho++;
        }
        i++;
    }
    return tamanho;
}

int calculateWords(char * text){
    /*For this code words are separete by spaces, so if you calculate the amount of
    spaces plus one we have the amount of words*/

    int spaces = 0;
    int i = 0;
    while(text[i] != '\0'){
        if(text[i] == ' ')
        {
            spaces++;
        }
        i++;
    }
    return spaces + 1;
}

int calculateSentences(char * text){
    int sentences = 0;
    int i = 0;
    while(text[i] != '\0'){
        if(text[i] == '?' || text[i] == '!' || text[i] == '.'){
            sentences++;
        }
        i++;
    }
    return sentences;
}
