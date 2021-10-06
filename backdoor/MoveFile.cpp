void MoveFile(string directory, string filePath)
{
    ifstream localFile(filePath);
    if (localFile.is_open()) {
        if (GetFileAttributesA(directory.data()) != INVALID_FILE_ATTRIBUTES) {

            sendall("valid");
            localFile.close();

            string newLocation = directory + "/" + filePath.substr(filePath.find_last_of("/\\") + 1);
            MoveFileA(filePath.data(), newLocation.data());

            return;
        
        } else sendall("invalid-dir");
    
    } else sendall("invalid-file");

    localFile.close();
}