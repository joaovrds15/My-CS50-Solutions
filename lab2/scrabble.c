#include <stdio.h>
#include <cs50.h>
#include <string.h>

int POINTS[] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10};
int computate_score(string word);

int main(void){
   string word1 = get_string("Player 1:");
   string word2 = get_string("Player 2:");

   int score1 = computate_score(word1);
   int score2 = computate_score(word2);

   //Winner logic
   if(score1 > score2){
       printf("Player 1 wins!\n");
   }
   else if(score1 < score2){
       printf("Player 2 wins!\n");
   }
   else{
       printf("Tie!\n");
   }
}

int computate_score(string word){
    char lower = 'a';
    char upper = 'A';
    int position = 0;
    int score = 0;
    for(int i = 0, n = strlen(word); i < n; i++){
        lower = 'a';
        upper = 'A';
        while(word[i] != lower && word[i] != upper && position < 26){
            position++;
            lower++;
            upper++;
        }
        if(position < 26){
           score += POINTS[position];
        }
        position = 0;
   }
   return score;
}