#include <stdio.h>
#include <string.h>

int repeatedElements(char string[]);
int numbersInCipher(char string[]);
char* encipher(char plaintext[],char cipher[]);
int calculateCase(char c);

int main(int argc, char * argv[]){
    //Validating key
    if(argv[1] == NULL || ((int) argv[1][0] > 47 && (int) argv[1][0] < 58) || (int) argv[1][0] == 45){
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if(strlen(argv[1]) < 26){
        printf("Key must contain 26 characters\n");
        return 1;
    }
    else if(repeatedElements(argv[1]) == 1){
        printf("Key must not contain repeated characters\n");
        return 1;
    }
    else if(numbersInCipher(argv[1]) == 1){
        printf("Key must only contain alphabetic characters\n");
        return 1;
    }
    //
    else{
        char plaintext[200];
        printf("plaintext:");
        fgets(plaintext,200,stdin);
        char * cipherText = encipher(plaintext,argv[1]);
        printf("ciphertext:%s\n",cipherText);
    }

}

int repeatedElements(char string[]){
    for(int i = 0, n = strlen(string); i < n;i++){
        for(int j = i+1; j < n;j++){
            if(string[i] == string[j]){
                return 1;
            }
        }
    }
    return 0;
}

int numbersInCipher(char string[]){
    for(int i = 0, n = strlen(string); i < n; i++){
        if(string[i] >= 48 && string[i] <= 57){
            return 1;
        }
    }
    return 0;
}
char* encipher(char plaintext[],char cipher[]){
    char lower = 'a';
    char upper = 'A';
    int i;
    int position = 0;
    int n = strlen(plaintext);
    for( i = 0; i < n; i++){
        lower = 'a';
        upper = 'A';
    while(plaintext[i] != lower && plaintext[i] != upper && position < 26){
        position++;
        lower++;
        upper++;
    }
    if(position < 26){
        if(calculateCase(plaintext[i]) == calculateCase(cipher[position])){
            plaintext[i] = cipher[position];
        }
        else{
            char temp = cipher[position];
            if(calculateCase(plaintext[i]) == 1 && calculateCase(temp) == 0){
                temp -= 32;
                plaintext[i] = temp;
            }
            else{
                temp += 32;
                plaintext[i] = temp;
            }
        }

    }

    position = 0;
   }
   return plaintext;
}

int calculateCase(char c){
    if(c >= 'A' && c <= 'Z'){
        return 1;
    }
    else{
        return 0;
    }
}

