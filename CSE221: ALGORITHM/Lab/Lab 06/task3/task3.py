
input = open("input3.txt", 'r')
output = open("output3.txt", 'w')

data = input.readline().split()

N = int(data[0])
M = int(data[1])

edges = []
index = 2

while True:
    line = input.readline().strip() 
    if not line:
        break 
    u, v, w = map(int, line.split())
    edges.append((u, v, w))


graph = []
for item in range(N+1):
    graph.append([])

for u, v, w in edges:
    graph[u].append((v, w))

def danger_path(N, graph):
    priority_Q = [(0, 1)]  
    min_danger = [float('inf')] * (N + 1)
    min_danger[1] = 0

    while priority_Q:
        min_index = -1
        min_value = float('inf')
        for i in range(len(priority_Q)):
            if priority_Q[i][0] < min_value:
                min_value = priority_Q[i][0]
                min_index = i

        curr_danger, u = priority_Q.pop(min_index)

        if curr_danger > min_danger[u]:
            continue

        for v, w in graph[u]:
            max_danger = max(curr_danger, w)
            if max_danger < min_danger[v]:
                min_danger[v] = max_danger
                priority_Q.append((max_danger, v))
    return min_danger[N]

result = danger_path(N, graph)

if result == float('inf'):
    output.write("Impossible")
else:
    output.write(f"{result}")


input.close()
output.close()