input = open('input2.txt','r')
output = open('output2.txt','w')
next(input)
line = input.readline()
arr = list(map(int,line.split()))
def mergesort(arr):
  if len(arr) <= 1:
    return arr
  else:
    mid = len(arr)//2
    a1 = mergesort(arr[0:mid:1])
    a2 = mergesort(arr[mid:])
    return find_maximum(a1,a2)
def find_maximum(a,b):
 if  a >= b:
  return a
 else:
  return b
max_num = mergesort(arr)
output.write(f'{max_num[0]}')
input.close()
output.close()