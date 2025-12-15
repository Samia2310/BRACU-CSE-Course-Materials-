input = open("input1a.txt", "r")
output = open("output1a.txt", "w")

num, preq = map(int, input.readline().split())

adj = [[] for i in range(num + 1)]  

for i in range(preq):
    u, v = map(int, input.readline().split())
    adj[u].append(v)

def DFS(u, adj, visited, stack, cycle):
    visited[u] = True
    cycle[u] = True
    for v in adj[u]:
        if cycle[v]:  # Cycle detected
            return False
        if not visited[v]:
            if not DFS(v, adj, visited, stack, cycle):
                return False
    cycle[u] = False
    stack.append(u)
    return True

def TopSort(adj, V):
    stack = []
    visited = [False] * (V + 1)
    cycle = [False] * (V + 1)  
    for i in range(1, V + 1):
        if not visited[i]:
            if not DFS(i, adj, visited, stack, cycle):
                print("IMPOSSIBLE")
                return
    while stack:
        output.write(str(stack.pop()) + " ")

if preq == 0: 
    output.write("impossible")  
else:
    TopSort(adj, num)

input.close()
output.close()