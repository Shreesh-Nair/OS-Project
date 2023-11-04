import socket

# Server configuration
server_address = ('localhost', 12345)

# Create a socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(server_address)
server_socket.listen(1)

print("Server is listening for incoming connections...")

# Accept a connection from the client
client_socket, client_address = server_socket.accept()

while True:
    request = client_socket.recv(1024).decode('utf-8')
    print(f"Received request: {request}")

    # Get input from the server for permission
    response = input("Grant permission (Y/N): ").strip().upper()

    if response == "Y":
        response = "Access Granted"
    else:
        response = "Access Denied"

    client_socket.send(response.encode('utf-8'))

# Close the sockets
client_socket.close()
server_socket.close()
