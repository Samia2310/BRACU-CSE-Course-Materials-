input = open("input6.txt", 'r')
output = open("output6.txt", 'w') 

num_rows, num_cols = map(int, input.readline().split())
grid = [input.readline().strip() for row in range(num_rows)]

def dfs(grid, x, y, visited):
    if x < 0 or x >= len(grid) or y < 0 or y >= len(grid[0]) or grid[x][y] == '#' or visited[x][y]:
        return 0
    if grid[x][y] != '.':
        visited[x][y] = True
        diamond_count = 1
    else:
        diamond_count = 0
    visited[x][y] = True
    for dx, dy in [(0, 1), (1, 0), (0, -1), (-1, 0)]:
        diamond_count += dfs(grid, x + dx, y + dy, visited)
 
   return diamond_count


def max_diamonds(grid):
    num_rows, num_cols = len(grid), len(grid[0])
    visited = [[False] * num_cols for row in range(num_rows)]
    max_diamond_count = 0

    for i in range(num_rows):
        for j in range(num_cols):
            if grid[i][j] != '.' and not visited[i][j]:
                diamonds_collected = dfs(grid, i, j, visited)
                max_diamond_count = max(max_diamond_count, diamonds_collected)
    return max_diamond_count

result = max_diamonds(grid)
output.write(f'{result}')

input.close()
output.close()