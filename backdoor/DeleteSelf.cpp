#include "utilities.h"
extern std::string appdata;

void DeleteSelf()
{
	std::string location = appdata + "\\del.vbs";
	command = "start " + appdata + "\\del.vbs";

	std::ofstream vbsFile(location);
	std::ifstream check(startupPath + fileName);

	if (check) fileName = startupPath + fileName;
	else fileName = firstPath;

	vbsFile << "WScript.Sleep(1000)\n"
		"Set f1 = CreateObject(\"Scripting.FileSystemObject\"):f1.DeleteFile \"" + fileName + "\", True\n"
		"Set f2 = CreateObject(\"Scripting.FileSystemObject\"):f2.DeleteFile \"" + location + "\", True";

	vbsFile.close();
	system(command.data());
	exit(0);
}