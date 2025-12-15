input = open('input1.txt','r')
output = open('output1.txt','w')
next(input)
line = input.readline()
arr = list(map(int,line.split()))
def merge(a,b):
  i = 0
  j = 0
  merged_arr = []
  while i < len(a) and j < len(b):
    if a[i] > b[j] :
      merged_arr.append(b[j])
      j += 1
    else:
      merged_arr.append(a[i])
      i += 1
  if i == len(a):
    copyleftelem = b[j:len(b):1]
    for item in copyleftelem:
      merged_arr.append(item)
  elif j == len(b):
    copyleftelem = a[i:len(a):1 ]
    for item in copyleftelem:
      merged_arr.append(item)
  return merged_arr
def mergeSort(arr):
  if len(arr) <= 1:
    return arr
  else:
    mid = len(arr)//2
    a1 = mergeSort(arr[0:mid:1])
    a2 = mergeSort(arr[mid:len(arr):1])
    return merge(a1,a2)
L = mergeSort(arr)
L = mergeSort(arr)
for i in L:
  output.write(f'{i} {' '}')
input.close()
output.close()
