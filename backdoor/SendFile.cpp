#include "utilities.h"

void SendFile()
{
    send("success");
	std::string filePath = recv();
    std::ifstream localFile(filePath.data(), std::ios::binary);

    if (!localFile.is_open()) {
        send("invalid");
        return;

    } send("valid");

    std::vector<unsigned char> buf(std::istreambuf_iterator<char>(localFile), {});
    std::string contents(buf.begin(), buf.end());

    localFile.close();
    sendAll(contents);
}