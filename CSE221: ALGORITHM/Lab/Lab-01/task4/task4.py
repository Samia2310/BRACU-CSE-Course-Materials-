input = open('input4.txt','r')
output = open('output4.txt','w')
iterate = int(input.readline())
arr1 = []
arr = []
d = {}
ARR = []
ARR1 = []
ARR2 = []
new_arr = []
dic = []
new_arr1 = []
new_dic = []
for i in range(iterate):
    line = (input.readline()).split()
    arr1.append([line[0],line[4],line[6]])    #iterate with the given number then make a list
    arr.append(line[0])                       #of those elements which i need to work.
for i in range(len(arr1)):
    key = arr1[i][0]                     #Create a dictionary where railway stations are the keys and departure 
    val = arr[i]                          #times are the valus
    d[key] = val
for i in arr:
  if len(i) < 4:                        #create multiple list with different length to proceed the code to get the desired output
    ARR.append(i)
  elif len(i) == 4:
    ARR1.append(i)
  else:
    ARR2.append(i)
for i in range(len(ARR2)):
    min_idx=i
    for j in range(i+1,len(ARR2)):
        if ord(ARR2[j][0]) < ord(ARR2[min_idx][0]):      #Sorted the train name using ASCii values.
          min_idx=j
        elif ord(ARR2[j][0]) == ord(ARR2[min_idx][0]):
            if ord(ARR2[j][1]) < ord(ARR2[min_idx][1]):
                min_idx=j
    temp = ARR2[min_idx]
    ARR2[min_idx] = ARR2[i]
    ARR2[i]=temp
new_arr =  ARR+ARR1+ARR2
D = {}
Arr = []
for item in new_arr:
    for elem in arr1:
        if elem[0] == item:       #Create a dictionay where keys are the sorted train name and values
            k = item              #are the departure time.
            v = elem[2]
            D[k]=[v]
for k,v in D.items():
    Arr.append(k)
for i in Arr:
    for j in arr1:
        if j[0] == i:                  #create a list with sorted train name, departure place and departure time.
            dic.append([j[0],j[1],j[2]])
for i in new_arr:
    if i not in new_arr1:         #Create a list with the unique train name. Means there in no repeating train name.
        new_arr1.append(i)   

for i in new_arr1:
    l = []
    for j in dic:
        if j[0] == i:
            l.append(j[2])
    if len(l) < 1:
        new_dic.append(l[0])               #This is a sorting algorithm where I create a list with unique train name. Means
    else:                                  #with every different train name, the list get empty and append from the 0 index.
        for i in range(len(l)):            #With every same train name, I sorted the departure time as  latest departure time 
            min_idx = i                    #will get prioritized. I used selection sort. In this sorting algorithm, find the 
            for j  in range(i+1,len(l)):   #largest element of the list. hen swap the largest number with the first element of 
                if l[j][0]!=l[min_idx][0]: #the unsorted list. Continue this process until the entire list is sorted. 
                    l[j][1] > l[min_idx][1]
                    min_idx = i
                elif l[j][1] > l[min_idx][1]:
                    min_idx = j
                temp = l[min_idx]
                l[min_idx] = l[i]
                l[i] = str(temp)
    for i in l:
        new_dic.append(i)
for i in range (len(dic)):           #Then print the output accordingly, with text file.
    print(f' {dic[i][0]} will departure for {dic[i][1]} at {new_dic[i]} ')