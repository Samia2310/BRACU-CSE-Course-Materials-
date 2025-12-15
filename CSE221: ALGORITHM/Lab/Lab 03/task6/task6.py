input = open("input6.txt","r")
output = open("output6.txt","w")
arr_length = int(input.readline())
arr = list(map(int,input.readline().split(" ")))
indexes = int(input.readline())
def partition(arr,starting_index,ending_index):
  pivot_index = arr[ending_index]
  i = starting_index-1
  for j in range(starting_index,ending_index): #This function partitions the pivot element and returns its index. 
    if arr[j] <= pivot_index:                  
      i += 1                              # After partitioning, all elements smaller than the pivot are placed to its left and all elements greater than the pivot are placed to its right.
      arr[i],arr[j] = arr[j],arr[i]          
  arr[i+1],arr[ending_index] = arr[ending_index],arr[i+1]    
  return i+1
def quick_selectAlgo(arr,start_idx,end_idx,search_idx):
             
  if start_idx == end_idx: #If start_idx equals end_idx, it means the subarray contains only one element, it's the kth smallest element so return the element.
    return arr[start_idx]
  elif start_idx < end_idx: #Otherwise, we call the partition function.If the element we are searching is less than pivot index, it means the kth element is in the left subarray, so quick_select is called recursively on the left subarray.
                            #if element we are searching is greater than pivot index, it means the kth element is in the right subarray, so quick_select is called recursively on the right subarray.
    q=partition(arr,start_idx,end_idx)
    if q == search_idx:
      return arr[q]                    
    elif search_idx < q:
      return quick_selectAlgo(arr,start_idx,q-1,search_idx)
    else:
      return quick_selectAlgo(arr,q+1,end_idx,search_idx)
for i in range(indexes):
  search_idx = int(input.readline())
  output.write(f'{str(quick_selectAlgo(arr,0,arr_length-1,search_idx-1))}\n')   #Lastly, print the elements of tge required index in this process.
input.close()
output.close()