import math


def main():
    text = input("Text: ")
    averageLetter = (100/calculateWords(text)) * calculateLetters(text)
    averageSentences = (100/calculateWords(text)) * calculateSentences(text)
    result = round(0.0588 * averageLetter - 0.296 * averageSentences - 15.8)
    if(result < 1):
        print("Before Grade 1")
    elif(result > 15):
        print("Grade 16+")
    else:
        print("Grade " + str(result))


def calculateLetters(text):
    letters = 0
    for i in range(len(text)):
        if((text[i] >= 'A' and text[i] <= 'Z') or (text[i] >= 'a' and text[i] <= 'z')):
            letters += 1
    return letters


def calculateWords(text):
    spaces = 0
    for i in range(len(text)):
        if(text[i] == ' '):
            spaces += 1
    spaces += 1
    return spaces


def calculateSentences(text):
    sentences = 0
    for i in range(len(text)):
        if(text[i] == '.' or text[i] == '?' or text[i] == '!'):
            sentences += 1

    return sentences


if __name__ == "__main__":
    main()