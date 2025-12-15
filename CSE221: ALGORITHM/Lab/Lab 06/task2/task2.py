import heapq
input = open("input2.txt", "r")
output = open("output2.txt", "w")

data = input.readlines()
num_vertices, num_edges = map(int, data[0].split())

adj_list = []
for item in range(num_vertices):
    adj_list.append([])

for i in range(1, num_edges + 1):
    u, v, w = map(int, data[i].split())
    adj_list[u - 1].append((v - 1, w)) 

S, T = map(int, data[num_edges + 1].split())

def dijkstra(adj_list, start):
    dist = [float('inf')] * len(adj_list)
    dist[start] = 0
    priority_Q = [(0, start)]

    while priority_Q:
        curr_dist, current_node = priority_Q.pop(0)
        if curr_dist > dist[current_node]:
            continue
        for v, w_uv in adj_list[current_node]:
            new_dist = curr_dist + w_uv
            if new_dist < dist[v]:
                dist[v] = new_dist
                inserted = False
                for i in range(len(priority_Q)):
                    if priority_Q[i][0] > new_dist:
                        priority_Q.insert(i, (new_dist, v))
                        inserted = True
                        break
                if not inserted:
                    priority_Q.append((new_dist, v))
    return dist


dist_from_S = dijkstra(adj_list, S - 1)
dist_from_T = dijkstra(adj_list, T - 1)

min_meeting_time = float('inf')
meeting_node = -1


for item in range(num_vertices):
    if item != (S - 1) and item != (T - 1):
        if dist_from_S[item] < float('inf') and dist_from_T[item] < float('inf'):
            total_time = max(dist_from_S[item], dist_from_T[item])
            if total_time < min_meeting_time:
                min_meeting_time = total_time
                meeting_node = item + 1


if meeting_node == -1 or min_meeting_time == float('inf'):
    output.write("Impossible")
else:
    output.write(f"Time {min_meeting_time}\nNode {meeting_node}\n")

input.close()
output.close()