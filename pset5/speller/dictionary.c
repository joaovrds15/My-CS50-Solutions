// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

void toLowerCase(char * word);
// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;
// Number of buckets in hash table
const unsigned int N = 26;
int sizeDict = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{   //Check if the argument passed is the same found in the table
    char copy[50];
    strcpy(copy,word);
    toLowerCase(copy);
    int index = hash(copy);
    node * temp = table[index];
    while(temp != NULL){
        if(strcmp(copy,temp->word) == 0){
            return true;
        }
        temp = temp->next;
    }

    return false;

}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int length = strlen(word);
    long hash_value = 0;
    for(int i = 0; i < length;i++){
        hash_value += word[i];
        hash_value = (hash_value * word[i]) % N;
    }
    return hash_value;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE * dict = fopen(dictionary,"r");
    if(dict == NULL){
        return false;
    }
    char buffer[LENGTH + 1];
    int index = 0;
    while(fscanf(dict, "%s",buffer) != EOF){
        node * n = malloc(sizeof(node));
        strcpy(n->word,buffer);
        index = hash(n->word);
        n->next = table[index];
        table[index] = n;
        sizeDict++;
    }

    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return sizeDict;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node * temp;
    node * crawler;
    for(int i = 0; i < N;i++){
        if(table[i] != NULL){
            crawler = table[i];
            while(crawler != NULL){
                temp = crawler->next;
                free(crawler);
                crawler = temp;
            }
        }
        if(crawler == NULL && i == N - 1){
            return true;
        }
    }
    return false;
}

void toLowerCase(char * word){
    for(int i = 0; word[i] != '\0';i++){
        if(word[i] >= 65 && word[i] <= 90){
            word[i] += 32;
        }
    }
}