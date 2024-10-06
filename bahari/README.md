<h2> a terminal chat tool</h4>

why you ask? ...why not?


probably going to fork some already existing repo

This is a simple chat cli implemented in C, allowing multiple clients to connect to a server and communicate with each other.

<h4> Project structure</h4>

```chat_app/
├── include/
│   └── chat_utils.h
├── src/
│   ├── chat_utils.c
│   ├── client.c
│   └── server.c
├── Makefile
└── README.md
```

### Building

run:
```
make
```

This will create a bin directory containing the server and client executables.

### running the app

1. Start the server:
```
./bin/server
```
listening on port 8080

2. Run clients:
```
./bin/client <server_ip>
```

3. Cleaning up:
```
make clean
```

### Usage

Once connected, clients can type messages and press Enter to send them.
All connected clients will receive messages from other clients.
To exit the client application, type 'exit' and press Enter.

### Notes

 - The server needs to be run on a machine with a public IP address, or behind a properly configured router with port forwarding set up for port 8080.
 - Firewall rules on the server machine may need to be adjusted to allow incoming connections on port 8080.
 - This is a basic implementation and doesn't include security features like encryption.
