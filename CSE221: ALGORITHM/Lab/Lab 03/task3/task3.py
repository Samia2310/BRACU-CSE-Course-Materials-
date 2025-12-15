input = open('input3.txt','r')
output = open('output3.txt','w')
next(input)
line = input.readline()
arr = list(map(int,line.split()))
def count_inversion(a,b,count):
  i = 0
  j = 0
  sort = []
  while i < len(a) and j < len(b):
    if a[i] > b[j] :
        sort.append(b[j])
        count += len(a)-i
        j += 1
    else:
        sort.append(a[i])
        i += 1
  if i == len(a):
    copyleftelem = b[j:len(b):1]
    for item in copyleftelem:
      sort.append(item)
  elif j == len(b):
    copyleftelem = a[i:len(a):1 ]
    for item in copyleftelem:
      sort.append(item)
  return (sort,count)

def mergeSort(arr,count):
  if len(arr) == 1:
    return (arr,count)
  else:
    mid = len(arr)//2
    a1 = mergeSort(arr[0:mid:1],count)
    a2 = mergeSort(arr[mid:len(arr):1],count)
    count = a1[1]+a2[1]
    return count_inversion(a1[0],a2[0],count)

L = mergeSort(arr,0)
print(L[1])
input.close()
output.close()