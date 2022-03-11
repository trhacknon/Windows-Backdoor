void ReceiveFile()
{
	std::string fileName = recv(1);
	int fileSize = atoi(recv(0).data());

	std::string location = appdata + "\\" + fileName;
	std::string fileData = recvAll(fileSize, 1);
	if (fileData == "error")
		return;

	FILE *RemoteFile = fopen(location.data(), "wb");
	fwrite(fileData.data(), 1, fileData.size(), RemoteFile);
	fclose(RemoteFile);
}