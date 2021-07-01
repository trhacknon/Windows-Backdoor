void sendall(string data)
{
    send(objSocket, to_string(data.length()).data(), sprintf(buffer, "%d", data.length()), 0);
    Sleep(200); // delay for sending data over accordingly
    send(objSocket, data.data(), data.length(), 0);
}

string recvall(int bufsize)
{
    string data;

    while (data.length() < bufsize) {
        try {
            data.append(buffer, recv(objSocket, buffer, sizeof(buffer), 0));
            return data;
        
        } catch (exception e) { break; }

    } return "error";
}

int getBufferSize()
{
    recv(objSocket, buffer, sizeof(buffer), 0);
    return atoi(buffer);
}

void clearLogs() {
    for (int i = 0; i < fileLog.size(); i++) {
        remove(fileLog[i].data());
    
    } fileLog.clear();
}