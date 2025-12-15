from collections import deque

input = open('input3.txt','r')
output = open('output3.txt','w')
frst_line = list(input.readline().split())
number_of_vertices = int(frst_line[0])
number_of_edges = int(frst_line[1])

adj_dic ={}
list_of_tuple = []

for i in range(number_of_edges):
  line  = (input.readline().split('\n'))
  for j in line:
    if j:
      u, v = map(int, j.split())
      list_of_tuple.append((u, v))
        
adj_dic = {i: [] for i in range(number_of_vertices + 1)}
for a, b in list_of_tuple:
    adj_dic[a].append(b)
    adj_dic[b].append(a)

class Vertex:
  def __init__(self, idx):
    self.idx = idx
    self.colour = 0

class Graph:
  def __init__(self, num_vertices):
    self.num_vertices = num_vertices
    self.adj_dic = [[] for _ in range(num_vertices)]
    self.vertices = [Vertex(i) for i in range(num_vertices)]

  def DFS(self, u):
    self.vertices[u].colour = 1
    output.write(f'{u} {" "}')
    for v in self.adj_dic[u]:
      if self.vertices[v].colour == 0:
        self.DFS(v)


graph = Graph(len(adj_dic))
graph.adj_dic = adj_dic
graph.DFS(1)

input.close()
output.close()