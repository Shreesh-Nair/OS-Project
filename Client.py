import socket

# Server configuration
server_address = ('localhost', 12345)

# Create a socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the server
client_socket.connect(server_address)

# Send a request to the server
request = "Can I proceed?"
client_socket.send(request.encode('utf-8'))

# Receive the response from the server
response = client_socket.recv(1024).decode('utf-8')

print(f"Client: {response}")

# Close the socket
client_socket.close()
