#include <windows.h>
#include <iostream>
#include <shlobj.h>
#include "qstring.h"

#pragma comment(lib, "shell32.lib")
#ifndef UNICODE  
	typedef std::string String;
#else
	typedef std::wstring String;
#endif

/*QString myDocumentsFolderAddress() {
	CHAR my_documents[MAX_PATH];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

	if (result != S_OK)
		//std::cout << "Error: " << result << "\n";
		return QString();
	else
		//std::cout << "Path: " << my_documents << "\n";
	return my_documents;
}*/

QString localAppFolderAddress() {
	TCHAR szPath[MAX_PATH];

	if (SUCCEEDED(SHGetFolderPath(NULL,
		CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE,
		NULL,
		0,
		szPath)))
	{
		return QString("%1/").arg(QString::fromStdWString(szPath));
		//PathAppend(szPath, TEXT("New Doc.txt"));
		//HANDLE hFile = CreateFile(szPath, ...);
	}
}