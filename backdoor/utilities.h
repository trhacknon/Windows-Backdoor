#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define NO_BYTES_IN_BUFFER 0

extern char buffer[8192];
extern SOCKET objSocket;

void send(std::string data)
{
    send(objSocket, data.data(), data.size(), 0);
}

std::string recv()
{
	memset(buffer, 0, sizeof(buffer));
    recv(objSocket, buffer, sizeof(buffer), 0);
	return buffer;
}

void sendAll(std::string data)
{
	std::string size = std::to_string(data.length());

    send(objSocket, size.data(), size.length(), 0);
	if (recv() == "success") {
        send(objSocket, data.data(), data.length(), 0);   
    }
}