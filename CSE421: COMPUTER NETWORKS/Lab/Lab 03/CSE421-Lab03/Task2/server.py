import socket

port = 5050
DATA = 16
FORMAT = 'utf-8'
DISCONNECT_MSG = 'disconnect'
device_name = socket.gethostname()
server_ip = socket.gethostbyname(device_name)
server_socket_address = (server_ip, port)  #socket address
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #socket object
server.bind(server_socket_address)
server.listen()
print("Our Server is listening...")

while True:
    server_socket, client_add = server.accept()
    print("Connected to ", client_add)
    connected = True
    while connected:
        upcoming_message_length = server_socket.recv(DATA).decode(FORMAT)
        if not upcoming_message_length.strip():
            print("Client Disconnected abruptly", client_add)
            break
        print("Upcoming msglength is", upcoming_message_length.strip())

        message_length = int(upcoming_message_length.strip())
        message = server_socket.recv(message_length).decode(FORMAT)
       
        if message.lower() == DISCONNECT_MSG:
            server_socket.send("BYE. NICE TO SERVE YOU".encode(FORMAT))
            print("Disconnect with", client_add)
            connected = False
        else:
            vowels = 'aeiouAEIOU'
            count = 0
            for ch in message:
                if ch in vowels:
                    count += 1
   
            if count == 0:
                server_socket.send("Not enough vowels".encode(FORMAT))
            elif count <= 2:
                server_socket.send("Enough vowels I guess".encode(FORMAT))
            else:    
                server_socket.send("Too many vowels".encode(FORMAT))

        print("Received", message)
    server_socket.close()