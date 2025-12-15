from collections import deque

input = open("input2.txt", "r")
output = open("output2.txt", "w")

num_courses, num_preq = map(int, input.readline().split())
preq = []
for line in input.readlines():
    u,v = map(int, line.split())
    preq.append((u,v))
    
def lexicographic_seq(num_courses, preq):
    adj_list = {}
    indegree = {}
    for a, b in preq:
        if a not in adj_list:
            adj_list[a] = []
        if b not in adj_list:
            adj_list[b] = []
        adj_list[a].append(b)
        if b not in indegree:
            indegree[b] = 0
        indegree[b] += 1
    
    Q = deque()
    for i in range(1, num_courses+1):
        if i not in indegree:
            Q.append(i)
    
    order = []
    while Q:

        Q = deque(sorted(Q))
        num = Q.popleft()
        order.append(num)
        
        if num in adj_list:
            for item in adj_list[num]:
                indegree[item] -= 1
                if indegree[item] == 0:
                    Q.append(item)
    
    if len(order) != num_courses:
        return "IMPOSSIBLE"
    else:
        return order

lexo = lexicographic_seq(num_courses, preq)
if lexo == "IMPOSSIBLE":
    output.write(lexo)
else:
    output.write(" ".join(map(str, lexo)))


input.close()
output.close()