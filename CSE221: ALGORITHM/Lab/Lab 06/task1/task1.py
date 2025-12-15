
input = open("input1.txt", "r")
output = open("output1.txt", "w")

data = input.readlines()
num_vertices, num_edges = map(int, data[0].split())

adj_list = []
for item in range(num_vertices):
    adj_list.append([])

for i in range(1, num_edges + 1):
    u, v, w = map(int, data[i].split())
    adj_list[u - 1].append((v - 1, w))  

source = int(data[-1].strip())

def dijkstra(adj_list, num_vertices, source):
    dist = [float('inf')] * num_vertices
    dist[source] = 0
    priority_Q = [(0, source)]

    while priority_Q:
        priority_Q.sort()
        curr_dist, u = priority_Q.pop(0)
        for v, w_uv in adj_list[u]:
            new_dist = curr_dist + w_uv
            if new_dist < dist[v]:
                dist[v]= new_dist
                priority_Q.append((new_dist, v))

    return dist

shortest_dist = dijkstra(adj_list, num_vertices, source - 1)

for d in shortest_dist:
    if d == float('inf'):
        output.write("-1")
    else:
        output.write(f"{d} ")

input.close()
output.close()