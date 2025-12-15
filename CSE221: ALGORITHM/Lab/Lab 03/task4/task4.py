input = open('input4.txt','r')  
output = open('output4.txt','w') 
next(input)
line = input.readline()
arr = list(map(int,line.split()))
def formula_input(a,b):
    arr2_new = []
    left_max = max(a)
    right_max = max(abs(i) for i in b)
    return int(left_max +(right_max**2))

def mergeSort(arr):
  if(len(arr) == 2):
        return arr[0] +(arr[-1]**2)
  elif(len(arr) < 2):    #merge sort algorithm recursively to divide the array into smaller subarrays until the length of the array is one .
        return 0    #When the array has two elements, it returns the result of applying the given formula to the first and last elements of the array.
  mid = len(arr)//2     #When the array has fewer than two elements, it returns 0.
  a1 = mergeSort(arr[0:mid:1])
  a2 = mergeSort(arr[mid:len(arr):1])
  max_sum = formula_input(arr[:mid], arr[mid:])
  return max(a1,a2,max_sum)
a = mergeSort(arr)
output.write(f'{a}')
input.close()
output.close()