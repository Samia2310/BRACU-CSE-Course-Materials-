import socket
import threading

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

def client_handle(server_socket, client_add):
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
        else:
            vowels = 'aeiouAEIOU'
            count = 0
            for ch in message:
                if ch in vowels:
                    count += 1
       
            if count == 0:
                server_socket.send("Not enough vowels".encode(format))
            elif count <= 2:
                server_socket.send("Enough vowels I guess".encode(format))
            else:    
                server_socket.send("Too many vowels".encode(format))
           
        print("Received", message)
   
    server_socket.close()

while True:
    server_socket, client_add = server.accept()
    thread = threading.Thread(target=client_handle, args = (server_socket, client_add))
    thread.start()
