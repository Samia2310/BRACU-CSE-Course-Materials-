input = open('input2.txt','r')
output = open('output2.txt','w')
next(input)
line = input.readline()
listyy = list(map(int,line.split()))
#sorting algorithm
for i in range (len(listyy)):
    for j in range(len(listyy)-i-1):
      if listyy[j]>listyy[j+1]:
        temp = listyy[j]
        listyy[j]=listyy[j+1]
        listyy[j+1]=temp
for elem in listyy:
    output.write(f'{elem}')
input.close()
output.close()

#In the sorting algorithm, we take an indicator named 'i', which starts the index from 
#0, then another indicator named 'j'. Firstly, it compare the first two elements of the list
#If the first element is greater than the second element, swap them. Then it moves to the
#next pair which is second & third elements.Continue this process until you reach the end of
# the list. After the first pass through the list, the largest element will be at the end.