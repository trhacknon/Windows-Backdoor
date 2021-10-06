void ReadFile(string filePath)
{
    string data;
    vector<char> contents;
    ifstream localFile(filePath);

    if (!localFile.is_open()) {
        sendall("invalid");
        return;
    
    } sendall("valid");

    streampos start = localFile.tellg();
    localFile.seekg(0, ios::end);

    streampos end = localFile.tellg();
    localFile.seekg(0, ios::beg);

    contents.resize(static_cast<size_t>(end - start));
    localFile.read(&contents[0], contents.size());

    for (const char& c : contents) {
        data += c;
    
    }; localFile.close(); sendall(data);
}