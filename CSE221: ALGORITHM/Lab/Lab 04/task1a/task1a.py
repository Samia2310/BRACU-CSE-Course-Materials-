input = open('input1a.txt','r')
output = open('output1a.txt','w')
frst_line = list(input.readline().split())
number_of_vertices = int(frst_line[0])
number_of_edges = int(frst_line[1])
adj_list = []
for i in range(number_of_edges):
    line = (input.readline().split('\n'))
    for j in line:
        adj_list.append(j)
adj_mat = [(number_of_vertices+1)*[0] for i in range(number_of_vertices+1)]
for i in  adj_list:
  sp = i.split()
  if len(sp) == 0:
      pass
  else:
     adj_mat[(int(sp[0]))][(int(sp[1]))] = int(sp[2])
n = 0
for i in adj_mat:
  for j in i:
    output.write(f'{j} {" "}')
    n += 1
    if n%(number_of_vertices+1) == 0:
      output.write('\n')
input.close()
output.close()