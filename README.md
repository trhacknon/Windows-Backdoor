# Spyware-RAT
A Hidden and Undetectable Remote Access Tool written in C++ and Server in Python3

This program utilizes the winsock library for sending and receiving data with the server and can be connected to from anywhere as long as it's running and not already connected. There are constants that can be modifed within the client.cpp file such as 'startup' which allows the program to copy itself to the startup (can be detected by anti-virus), 'SERVER' to allow the client to know what IP and to connect wherever the listening socket is and 'TIMEOUT' to allow the client to indicate when to automatically restart the program when no data is being received (default: 2.5 minutes)

Compile Command: g++ client.cpp -o client -lws2_32

[!] Platform: Windows

[!] Compiler: MinGW-64Bit

[!] Port: 5005 (modifiable)

![](images/commands.png)
Remote Tools coming soon:

[+] Capture Screenshot
[+] Capture Webcam
