# Multiple clients connecting to a server
# Learnt from : https://youtu.be/3QiPPX-KeSc (Tech with Tim - Socket)
import socket
import threading  # so that other clients are not left waiting

HEADER = 64  # Bytes
PORT = 5050
SERVER = socket.gethostbyname(socket.gethostname())
ADDR = (SERVER, PORT)
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT"

# IF you are connected over the same network (eg. WIFI), you can exchange info between different computers : the other computer should have the client.py file running
# IF you want to send info over the internet , instead of your private IP, use the public IP which connects to the Internet. THen u can send it to anyone , anywhere on the Internet.

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # create a socket
server.bind(ADDR)  # bind the info to the socket


def handle_client(conn, addr):  # conn --> connection so that we can put it on another thread
    # this runs for each individual client
    print(f"[NEW CONNECTION] {addr} connected.")

    connected = True
    while connected:
        # we don't know the length of the message that client will send. So we create a 64 bytes HEADER which knows how many bytes client is sending :p
        msg_length = conn.recv(HEADER).decode(FORMAT)  # blocks communication till the client sends something over the socket
        # thats why we have threads, so that multiple clients dont get blocked
        if msg_length:
            msg_length = int(msg_length)
            msg = conn.recv(msg_length).decode(FORMAT)
            if (msg == DISCONNECT_MESSAGE):
                connected = False

            print(f"[{addr}] {msg}")
            conn.send("Message Received".encode(FORMAT))
    conn.close()


def start():
    server.listen()  # listen for clients
    print(f"[LISTENING] Server is listening on {SERVER}")
    while True:
        conn, addr = server.accept()  # block clients till they are accepted
        # When a new connection occurs, the connection is passed on to a thread to handle_client
        thread = threading.Thread(target=handle_client, args=(conn, addr)) 
        thread.start()
        print(f"[ACTIVE CONNECTIONS] {threading.activeCount() - 1}")  # doing -1 cos the start () thread is always running since its listening.


print("[STARTING] server is starting...")
start()
