from cs50 import get_int

l = 1
height = get_int("Height:")

while(height > 8 or height < 1):
    height = get_int("Height:")

for i in range(height, 0, -1):
    for j in range(i-1):
        print(end=" ")
    for k in range(l):
        print("#", end="")
    print()
    l += 1

