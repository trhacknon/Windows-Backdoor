void DeleteFile(string filePath)
{
    ifstream localFile(filePath);
    if (!localFile.is_open()) {
        sendall("invalid");
        return;
    
    } localFile.close(); sendall("valid");

    if (remove(filePath.data()) != 0) {
        sendall("[!] Unable to Delete File");
    
    } else sendall("[+] File Deleted");
}