input = open('input4.txt','r')
output = open('output4.txt','w')
frst_line = list(input.readline().split())
number_of_vertices = int(frst_line[0])
number_of_edges = int(frst_line[1])


list_of_tuple = []
nodes = []

for i in range(number_of_edges):
  line  = (input.readline().split('\n'))
  for j in line:
      if j:
          u, v = map(int, j.split())
          list_of_tuple.append((u, v))
          if u not in nodes:
            nodes.append(u)
          if v not in nodes:
            nodes.append(v)

def cycle_graph(list_of_tuple):
    adj_list = {}
    for a, b in list_of_tuple:
        if a not in adj_list:
            adj_list[a] = []
        adj_list[a].append(b)
    def cycle_finding(node, visited, recursion_path):
        visited[node] = True
        recursion_path[node] = True
        for adjacent_node in adj_list.get(node, []):
            if not visited[adjacent_node]:
                if cycle_finding(adjacent_node, visited, recursion_path):
                    return True
            elif recursion_path[adjacent_node]:
                return True
        recursion_path[node] = False
        return False
    visited = {}
    for elem in nodes:
        visited[elem] = False
    recursion_path = {}
    for ele in nodes:
        recursion_path[ele] = False  # Marking all nodes as not in recursion stack

    for elem in nodes:
        if not visited[elem]:
            if cycle_finding(elem, visited, recursion_path):
                return True
    return False
cycle = cycle_graph(list_of_tuple)
if cycle  :
    output.write("YES")
else:
    output.write("NO")

input.close()
output.close()