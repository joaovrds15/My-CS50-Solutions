import re
import math


def passNumberToArray(number):
    numberCasted = int(number)
    cardList = []
    while(numberCasted > 0):
        cardList.append(numberCasted % 10)
        numberCasted = math.floor(numberCasted / 10)

    return cardList


def checkSum(cardList, number):
    temp = 0
    for i in range(len(cardList)):
        if(i % 2 == 0):
            temp += cardList[i]
        else:
            temp += (cardList[i] * 2) % 10
            temp += math.floor((cardList[i] * 2) / 10)

    if(temp % 10 == 0):
        checkBrand(number)
    else:
        print("INVALID")



def checkBrand(number):
    if(number[0] == '3' and (number[1] == '7' or number[1] == '4')):
        print("AMEX")
    elif(number[0] == '4'):
        print("VISA")
    elif(number[0] == '5' and (number[1] == '1' or number[1] == '2' or number[1] == '3' or number[1] == '4' or number[1] == '5')):
        print("MASTERCARD")
    else:
        print("INVALID")


def main():
    number = input("Number:")
    cardInList = passNumberToArray(number)
    if(len(number) < 13):
        print("INVALID")
        return
    checkSum(cardInList, number)


if __name__ == "__main__":
    main()