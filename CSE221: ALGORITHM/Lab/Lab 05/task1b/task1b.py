from collections import deque

input = open("input1b.txt", "r")
output = open("output1b.txt", "w")

num_vertices, num_edges = map(int, input.readline().split())

adj = [[] for _ in range(num_vertices)] 
indegree = [0] * num_vertices

for elem in range(num_edges):
    u, v = map(int, input.readline().split())
    adj[u - 1].append(v - 1)  
    indegree[v - 1] += 1  
    
def TopSort(adj, indegree):
    Q = deque()
    for vertex in range(len(indegree)):
        if indegree[vertex] == 0:
            Q.append(vertex)

    line = []

    while Q:
        current = Q.popleft()
        line.append(current)

        for adj_vert in adj[current]:
            indegree[adj_vert] -= 1
            if indegree[adj_vert] == 0:
                Q.append(adj_vert)

    if len(line) != len(adj):
        return []  
    else:
        return line

topologicalsort = TopSort(adj, indegree)

if topologicalsort:
    res = []
    for x in topologicalsort:
      res.append(str(x + 1))
    for V in res:
        output.write(V + " ")
else:
    output.write("IMPOSSIBLE")

input.close()
output.close()