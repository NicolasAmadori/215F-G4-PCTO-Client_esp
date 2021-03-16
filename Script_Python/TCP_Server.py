import socket
 
s = socket.socket()         
 
hotspot = True
if(hotspot):
    s.bind(('172.20.10.4', 60101))
else:
    s.bind(('192.168.0.2', 60101))

s.listen(0)                 
 
while True:
    print("Waiting for connection...")
    client, addr = s.accept()
 
    while True:
        content = client.recv(32)
 
        if len(content) ==0:
           break
        else:
            print(content)
 
    print("Closing connection \n")
    client.close()
