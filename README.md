# Windows Backdoor
A Hidden and Undetectable Remote Access Tool written in C++ and Server in Python3

This program utilizes the standard winsock library for sending and receiving data with the server and can be connected to from anywhere as long as it's running and not already connected. There are constants that can be modifed within the backdoor.cpp file such as 'startup' which allows the program to copy itself to the startup folder, 'SERVER' to allow the client to indicate which IP to connect too and 'TIMEOUT' to allow the client to indicate when to automatically restart the program when no data is being received (default: 5 minutes).

# Client Requirements
-  System: Windows
-  Compiler: C++ MinGW 64-Bit

![](images/connection_commands.png)<br/>
![](images/commands.png)<br/>
![](images/clients.png)<br/>
![](images/console.png)

# Usage
- Run the 'server.py' file on your own machine you would like to control the remote client from.
- Run the compiled 'backdoor.exe' file (backdoor.cpp) on your targets machine with the specified server IP Address to establish a connection on an unoccupied port.

# Compile C++ Program
- g++ backdoor.cpp -mwindows -o backdoor -lws2_32 -lstrmiids -lole32 -loleaut32 -static-libgcc -static-libstdc++ -static

# Contributing
- Feel free to submit any possible bugs as I'll be glad to respond to anyone and help with whatever's needed to help fix and improve this program.

# Disclaimer
- This program is for educational purposes only! Use at your own risk.
