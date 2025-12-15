input = open('input5.txt','r')
output = open('output5.txt','w')
arr_length = int(input.readline())
line = input.readline()
arr = list(map(int,line.split()))
def quickSort(a,start_idx,end_idx):
  if start_idx < end_idx:  #quickSort function recursively sort the array by using partition function.
    idx = partition(a,start_idx,end_idx)
    quickSort(a,start_idx,idx-1)
    quickSort(a,idx+1,end_idx)
  return a
def partition(arr,starting_index,ending_index):
  pivot_index = arr[ending_index]          #We take last element as pivot.
  i = starting_index-1
  for j in range(starting_index,ending_index): #For each element arr[j] in the subarray, if it's less than or equal to the pivot element, we increament "i" then swaps  with a[i].
                                              #Finally, after the end of the iteration like this, the pivot element swaps with a[i+1].
                                             #It returns the index of the pivot element after partitioning.
    if arr[j] <= pivot_index:                 #Then we call "quickSort" function again, repeat the process and then sort the array in this way.
      i += 1
      arr[i],arr[j] = arr[j],arr[i]
  arr[i+1],arr[ending_index] = arr[ending_index],arr[i+1]
  return i+1
sorted_array = quickSort(arr,0,arr_length-1)
for i in sorted_array:
  output.write(f'{i}{' '}')
input.close()
output.close()