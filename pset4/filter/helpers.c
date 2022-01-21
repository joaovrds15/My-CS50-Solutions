#include "helpers.h"
#include <math.h>
typedef uint8_t BYTE;
#include <stdio.h>
#define BLUR 3

int ceilTo(float value);
int average(int width,int height,RGBTRIPLE image[height][width],int start, int end, char color);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            float avg = (float) (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed)/3;
            int avgFinal = (int) round(avg);
            image[i][j].rgbtBlue = avgFinal;
            image[i][j].rgbtRed = avgFinal;
            image[i][j].rgbtGreen = avgFinal;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height;j++){
             int redSepia = round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue);
             redSepia = ceilTo((float) redSepia);
             int greenSepia = round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue);
             greenSepia = ceilTo((float) greenSepia);
             int blueSepia = round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue);
             blueSepia = ceilTo((float)blueSepia);
             image[i][j].rgbtBlue = blueSepia;
             image[i][j].rgbtRed = redSepia;
             image[i][j].rgbtGreen = greenSepia;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for(int i = 0; i < width/2;i++){
        for(int j = 0; j < height;j++){
            temp = image[j][i];
            image[j][i] = image[j][(width-1)-i];
            image[j][(width-1)-i] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    double averageRed = 0;
    double averageGreen = 0;
    double averageBlue = 0;
    int counter = 0;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width;j++){
            counter = 0;
            averageRed = 0;
            averageGreen = 0;
            averageBlue = 0;
            for(int row = i - 1; row <= i + 1; row++){
                for(int col = j - 1; col <= j + 1;col++){
                    if((row >= 0 && row < height) && (col >= 0 && col < width)){

                        averageRed += image[row][col].rgbtRed;
                        averageGreen += image[row][col].rgbtGreen;
                        averageBlue += image[row][col].rgbtBlue;
                        counter++;
                    }
                }
            }

            if(counter != 0){
                temp[i][j].rgbtRed = round(averageRed/(double) counter);
                temp[i][j].rgbtGreen = round(averageGreen/(double)counter);
                temp[i][j].rgbtBlue = round(averageBlue/(double)counter);
            }
            else{
                return;
            }

        }
    }
    for(int k = 0; k < height;k++){
        for(int l = 0; l < width;l++){
            image[k][l] = temp[k][l];
        }
    }
    return;


}
int ceilTo(float value){
    if(value > 255){
        return 255;
    }
    return value;
}
