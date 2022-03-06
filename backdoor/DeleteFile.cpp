#include "utilities.h"

void DeleteFile()
{
    send("success");
    std::string filePath = recv();
    std::ifstream localFile(filePath);

    if (!localFile.is_open()) {
        send("invalid");
        return;

    } localFile.close(); send("valid");

    if (remove(filePath.data()) != 0) {
        send("[!] Unable to Delete File");
    }
    else send("[+] File Deleted");
}