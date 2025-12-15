input = open('input1b.txt','r')
output = open('output1b.txt','w')
iterate = int(input.readline())
for i in range(iterate):
    words = input.readline().split()
    if words[2] == '+':
        output.write(f'The result of {words[1]} {words[2]} {words[3]} is {int(words[1])+int(words[3])}\n')
    elif words[2] == '-':
        output.write(f'The result of {words[1]} {words[2]} {words[3]} is {int(words[1])-int(words[3])}\n')
    elif words[2] == '*':
        output.write(f'The result of {words[1]} {words[2]} {words[3]} is {int(words[1])*int(words[3])}\n')
    elif words[2] == '/':
        output.write(f'The result of {words[1]} {words[2]} {words[3]} is {int(words[1])/int(words[3])}\n')
input.close()
output.close()

#Took the given input as text file. Task is to check the mathemetical operation. I wrote the conditon of the operators.
# If the operator matches with the given input operator then it will perform as it is. Otherwise, it will check the other conditions.