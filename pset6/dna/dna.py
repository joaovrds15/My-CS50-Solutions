import csv
import sys
import re

def main():
    if len(sys.argv) != 3:
        print("Usage: python dna.py file.csv sequences.txt")
        return

    dataBase = open(sys.argv[1],"r")
    reader = csv.reader(dataBase)
    allelesSequence = {}
    personList = []

    for row in reader:
        personList.append(row)

    for c in range(len(personList[0]) - 1):
        allelesSequence[personList[0][c+1]] = 0

    fileSequence = open(sys.argv[2],"r")
    sequence = fileSequence.read()

    for allele in allelesSequence:
        allelesSequence[allele] = calculateAlleles(sequence,allele)

    result = calculatePerson(personList,allelesSequence)
    if(result != None):
        print(result)
    else:
        print("No match")


#Probably we are facing an error in this function cause its returning differents than it should
def calculateAlleles(sequence,allele):
    alleleLenght = len(allele)
    count = 0
    maxCount = 0
    i = 0
    while i < len(sequence):
        stringSlice = sequence[i : (i + alleleLenght)]
        initialValue = i

        if(stringSlice == allele):
           i += alleleLenght

        finalValue = i
        if(finalValue - initialValue == alleleLenght):
            count+=1
        else:
            if(maxCount < count):
                maxCount = count
            count = 0
            i+=1
    return maxCount



def calculatePerson(personList, allelesSequence):
    count = 0
    numberAlleles = len(personList[0]) - 1
    for i in range(1,len(personList)):
        for j in range(1,len(personList[0])):
            alleleName = personList[0][j]
            if(int(personList[i][j]) == allelesSequence[alleleName]):
                count+=1
        if(count == numberAlleles):
            return(personList[i][0])
        count = 0

if __name__ == "__main__":
    main()




