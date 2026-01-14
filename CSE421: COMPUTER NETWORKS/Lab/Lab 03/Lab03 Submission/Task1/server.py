import socket

port = 5050
DATA = 16
format = 'utf-8'
device_name = socket.gethostname()
server_ip = socket.gethostbyname(device_name)
server_socket_address = (server_ip, port)  #socket address
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #socket object
server.bind(server_socket_address)
server.listen()
print("Our Server is listening")

while True:
    server_socket, client_add = server.accept()
    print("Connected to ", client_add)
    connected = True

    while connected:
        upcoming_message_length = server_socket.recv(DATA).decode(format)
        if not upcoming_message_length.strip():
            print("Client Disconnected abruptly", client_add)
            break
        print("Upcoming msglength is", upcoming_message_length.strip())

        message_length = int(upcoming_message_length.strip())
        message = server_socket.recv(message_length).decode(format)
       
        if message.lower() == 'disconnect':
            print("Disconnect with", client_add)
            connected = False
           
        print(message)
        server_socket.send("Message Received".encode(format))
   
    server_socket.close()