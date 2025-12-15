input = open('input1b.txt','r')
output = open('output1b.txt','w')
frst_line = list(input.readline().split())
number_of_vertices = int(frst_line[0])
number_of_edges = int(frst_line[1])
adj_list = []
new_L = []
SP = []
d = {}
for i in range(number_of_edges):
    line = (input.readline().split('\n'))
    for j in line:
        if j != "":
          adj_list.append(j)
for item in range(number_of_vertices+1):
    new_L.append(item)
for i in  adj_list:
  sp = i.split()
  SP.append(sp)
for num in new_L:
    list_of_tuple = []
    for elem in SP:
      if num == int(elem[0]):
        key = num
        list_of_tuple.append((tuple((elem[1],elem[2]))))
      else:
        key = num
        list_of_tuple.append("")
      st = ' '.join(['({}, {})'.format(int(item[0]), int(item[1])) for item in list_of_tuple if item])
      d[key] = st
for k,v in d.items():
    output.write(f'{k} : {v}\n')
input.close()
output.close()