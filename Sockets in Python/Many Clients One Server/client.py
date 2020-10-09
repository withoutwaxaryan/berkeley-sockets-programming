import socket

HEADER = 64  # Bytes
PORT = 5050
SERVER = "127.0.1.1"  # as per my server
ADDR = (SERVER, PORT)
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT"

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(ADDR)


def send(msg):
    message = msg.encode(FORMAT)
    msg_length = len(message)
    send_length = str(msg_length).encode(FORMAT)
    send_length = send_length + b' ' * (HEADER - len(send_length))  # TO MAKE HEADER LENGTH 64
    client.send(send_length)
    client.send(message)
    print(client.recv(2048)).decode(FORMAT)


send("Hello World")
send(DISCONNECT_MESSAGE)
