from collections import deque

input = open('input5.txt','r')
output = open('output5.txt','w')

def shortest_path(graph, start, Destination):
  queue = deque([(start, [start])])
  visited = [False] * (len(graph) + 1)
  visited[start] = True

  while queue:
    current, path = queue.popleft()
    if current == Destination:
      return path
    for adjacent_node in graph[current]:
      if not visited[adjacent_node]:
        visited[adjacent_node] = True
        queue.append((adjacent_node, path + [adjacent_node]))

def inputs(input):
    a = input.readlines()
    cities, roads, destination = map(int, a[0].split())
    graph = {}
    for item in a[1:]:
        if item.strip():  # Check if the line is not empty
            u, v = map(int, item.split())
            if u not in graph:
                graph[u] = []
            if v not in graph:
                graph[v] = []
            graph[u].append(v)
            graph[v].append(u)
    return graph, 1, destination

graph, start_node, dest_node = inputs(input)
Path = shortest_path(graph, start_node, dest_node)

if Path:
  output.write(f'Time: {len(Path) - 1}\n')
  output.write(f"Shortest Path: {" ".join(map(str, Path))}")