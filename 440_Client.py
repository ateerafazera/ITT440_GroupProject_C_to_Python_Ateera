import socket

def send_data(client_socket):
    user_input = input("Enter a string: ")
    client_socket.sendall(user_input.encode())
    response = client_socket.recv(1024).decode()
    print("Server response:", response)
    return response

def main():
    server_ip = input("Enter the server IP address: ")
    server_port = int(input("Enter the server port number: "))

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((server_ip, server_port))
    print("Connected to the server.")

    while True:
        response = send_data(client_socket)
        if "Exit" in response:
           # print("Server response:", response)
            break

    print("Server connection closed. Chat ended.")
    client_socket.close()

if __name__ == "__main__":
    main()
