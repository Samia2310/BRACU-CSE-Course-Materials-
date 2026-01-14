import socket

port = 5050
data = 16
format = 'utf-8'
device_name = socket.gethostname()
client_ip = socket.gethostbyname(device_name)
client_socket_address = (client_ip, port)  #socket address
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #socket object
client.connect(client_socket_address)

def sending_message(msg):
    message = msg.encode(format)
    msg_length = len(message)
    msg_length_str = str(msg_length).encode(format)
    msg_length_str +=b" "*(data-len(msg_length_str))

    client.send(msg_length_str)
    client.send(message)

    print(client.recv(128).decode(format))

while True:
    msg = input("Enter your message: ")
    sending_message(msg)
    if msg == 'disconnect':
        break
   
