#!/usr/bin/env python3

import socket
import time

HOST = '172.20.10.5'  # The server's hostname or IP address
PORT = 80        # The port used by the server

while True:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(b'Hello, world')
        data = s.recv(1024)
        time.sleep(10)
    

print('Received', repr(data))
