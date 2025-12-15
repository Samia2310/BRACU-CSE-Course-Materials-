with open('input1a.txt','r') as file0 :
    with open('output1a.txt','w') as file1 :
        next(file0)
        for item in file0:
            num = item.strip()
            if int(num) % 2 == 0:
               file1.write(f'{num} is an Even number.\n')
            else:
               file1.write(f'{num} is an Odd number.\n')

file0.close()
file1.close()

#Took the given input as text file which consists some numbers. Showed that the 
# numbers are even or odd by examine whether its remainder is 0 or not by dividing
# with 2. Used module operator (%) which return the remainder of the operation.
# If the remainder is 0 ,the number is even. Otherwise the number is odd.
