from collections import deque

input = open('input2.txt','r')
output = open('output2.txt','w')
frst_line = list(input.readline().split())
number_of_vertices = int(frst_line[0])
number_of_edges = int(frst_line[1])

adj_dic ={}
list_of_tuple = []

for i in range(number_of_edges):
  line  = (input.readline().split('\n'))
  for j in line:
    if j:
      a, b = map(int, j.split())
      list_of_tuple.append((a, b))
        
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
    self.adj_list = [[] for _ in range(num_vertices)]
    self.vertices = [Vertex(i) for i in range(num_vertices)]

  def BFS(self, start_vertex_idx):
    for vertex in self.vertices:
      vertex.colour = 0
    Q = deque()
    start_vertex = self.vertices[start_vertex_idx]  # Accessing the vertex object using the index
    start_vertex.colour = 1
    Q.append(start_vertex)
    while Q:
      u = Q.popleft()
      output.write(f'{u.idx} {" "}')
      for v_idx in self.adj_dic[u.idx]:
        v = self.vertices[v_idx]
        if v.colour == 0:
          v.colour = 1
          Q.append(v)
        u.colour = 2
          
graph = Graph(len(adj_dic))
graph.adj_dic = adj_dic
graph.BFS(1)

input.close()
output.close()