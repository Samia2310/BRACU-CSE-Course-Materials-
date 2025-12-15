input = open("input3.txt", "r")
output = open("output3.txt", "w")

num_vertices, num_edges = map(int, input.readline().split())
graph = {}

for idx in range(num_edges):
    u, v = map(int, input.readline().split())
    if u not in graph:
        graph[u] = []
    graph[u].append(v)

def dfs(graph, vertex, visited, stack):
    visited[vertex] = True
    for neighbor in graph.get(vertex, []):
        if not visited[neighbor]:
            dfs(graph, neighbor, visited, stack)
    stack.append(vertex)

def transpose(graph):
    transposed_graph = {}
    for u in graph:
        for v in graph[u]:
            if v not in transposed_graph:
                transposed_graph[v] = []
            transposed_graph[v].append(u)
    return transposed_graph

def dfs_scc(graph, vertex, visited, scc):
    visited[vertex] = True
    scc.append(vertex)
    for neighbor in graph.get(vertex, []):
        if not visited[neighbor]:
            dfs_scc(graph, neighbor, visited, scc)

def find_scc(graph, num_vertices):
    visited = [False] * (num_vertices + 1)
    stack = []
    for vertex in range(1, num_vertices + 1):
        if not visited[vertex]:
            dfs(graph, vertex, visited, stack)
    transposed_graph = transpose(graph)
    visited = [False] * (num_vertices + 1)
    strongly_connected_components = []

    while stack:
        vertex = stack.pop()
        if not visited[vertex]:
            scc = []
            dfs_scc(transposed_graph, vertex, visited, scc)
            strongly_connected_components.append(scc)
    return strongly_connected_components
sccs = find_scc(graph, num_vertices)
for item in sccs:
    item.sort()
sccs.sort()

for elements in sccs:
    output.write(" ".join(map(str, elements)) + "\n")
    
input.close()
output.close()