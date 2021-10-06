void ReceiveFile(string filePath)
{
    string data;
    int fileSize = getBufferSize();

    while (data.length() < fileSize) {
        try {
            data.append(buffer, recv(objSocket, buffer, sizeof(buffer), 0));
        
        } catch (exception e) {
            return;
        }

    } string location = appdata + "/" + filePath;

    FILE* RemoteFile = fopen(location.data(), "wb");
    fwrite(data.data(), 1, data.size(), RemoteFile);
    fclose(RemoteFile);

    fileLog.push_back(location);
}