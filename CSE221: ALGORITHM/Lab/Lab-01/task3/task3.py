input = open('input3.txt','r')
output = open('output3.txt','w')
next(input)
frst_line = list(map(int,input.readline().split()))
scnd_line = list(map(int,input.readline().split()))
d = {}
for item in range (len(frst_line)):
        new_key = frst_line[item]             #Make a dictionary with ids are the kay & marks are the value.
        new_val = scnd_line[item]  
        d[new_key] = new_val
#Sorting Algorithm
for i in range(len(scnd_line)):
    idx=i
    for j in range(i+1,len(scnd_line)):        #In this sorting algorithm, find the largest element of the list.
        if scnd_line[j] > scnd_line[idx]:      #Then swap the largest number with the first element of the unsorted
          idx=j                                #list. Continue this process until the entire list is sorted. 
    temp = scnd_line[idx]            
    scnd_line[idx] = scnd_line[i]
    scnd_line[i]=temp
d1 = {}
for item in scnd_line:
  for k,v in d.items():                      #Ceate another dictionary with the sorted marks, where ids are the keys
    if item == v:                            #and values are the marks.
      key = str(k)
      val = str(item)
      d1[key]=val
L = []
dict_2 = {}
for item in range (len(scnd_line)):
    key2 = scnd_line[item]
    value = item
    dict_2[key2] = value                  #create a list with only unique ids.Means there is no repeatation of marks.
for k in dict_2.keys():
    L.append(k)
d_1 = {}
for elem in L:
  l = []
  for m,n in d1.items():
    if (elem) == int(n):              
      l.append(m)
  if len(l) < 1:
    new_key = l[0]                        #This is sorting algoprithm where I sorted the ids with 
    new_val = elem                        #the same marks with ascending order.
    d_1[new_key] = new_val
  else:
    for i in range(len(l)):
      min_idx=i
      for j in range(i+1,len(l)):
        if l[j]<l[min_idx]:
          min_idx=j
      temp = l[min_idx]
      l[min_idx] = l[i]
      l[i]=temp
  for ele in l:
      new_key = ele
      new_val = elem                       #then create a new dictionary to new sorted ids with the sorted marks
      d_1[new_key] = new_val               #and print the output.
for k,v in d_1.items():
    output.write(f'ID: {k} Mark: {v}\n')