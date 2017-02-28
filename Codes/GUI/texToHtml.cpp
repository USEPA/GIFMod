#include <string>
#include <fstream>
#include "texToHtml.h"

using namespace std;
/*int tex2html(int argc, char* argv[]);
*/

extern "C" char * main2(int argc, char *argv[]);
//extern "C" char* htmlOut(char* part, int onlyOutput);
extern "C" char transfer(char* text);

QString tex2Html(string &tex)
{
	char html[100000];
	*html = '\0';
	char texChar[100000];
	strcpy(texChar, tex.c_str());

	transfer(texChar);

	char* pointer[1];
/*	char* pointer[2];
	pointer[0] = new char[7];
	pointer[1] = new char[2];
	pointer[1][0] = '-';
	pointer[1][1] = 'L';
	pointer[0] = "tth.exe";

	strcpy(html, main2(2, pointer));
*/
	strcpy(html, main2(1, pointer));
//	delete pointer[1];
//	delete pointer[0];
//	delete pointer;

	QString HTML;
	for (int i = 0; i < sizeof(html); i++)
	{
		HTML += html[i];
		if (HTML.right(7).toLower() == "</html>")
			break;
	}

	return HTML;
}

string texFile2Html(string &texFileName)
{
	char html[100000];
	
	ifstream file(texFileName);
	string fileContents;
	while (!file.eof())
	{
		string X;
		getline(file, X);
		fileContents += X + "\n";
	}

	char texChar[100000];
	strcpy(texChar, fileContents.c_str());

	transfer(texChar);

	char* pointer[2];
	pointer[0] = new char[2];
	pointer[1] = new char[2];
	pointer[1][0] = '-';
	pointer[1][1] = 'L';

	strcpy_s(html, main2(2, pointer));
	delete pointer[1];
	delete pointer[0];
//	delete pointer;

	string HTML;
	for (int i = 0; i < sizeof(html); i++)
		HTML += html[i];

	return HTML;
}
/*void main(int argc, char *argv[])
{
	char html[100000];

	ifstream file("tex_b.tex");
	string S;
	while (!file.eof())
	{
		string X; 
		getline(file,X);
		S += X + "\n"; 
	}
	

	char SS[100000];
	strcpy(SS, S.c_str());
	
	transfer(SS);
	
	char* pointer[2];
	pointer[0] = new char[2];
	pointer[1] = new char[2];
	pointer[1][0] = '-'; // use
	pointer[1][1] = 'L';
		
	strcpy_s(html, main2(2, pointer));
	delete pointer[1];
	delete pointer[0];
	delete pointer;
	string HTML;
	for (int i = 0; i < sizeof(html); i++)
		HTML += html[i];

	int i = 0;
}*/



/*void html(char *part)
{
	std::string a;

	static char* html;
	static int length = 0;

	int newLength = length + sizeof(part);
	char* newHtml;
	newHtml = (char*) malloc(newLength);
	for (int i = 0; i < length; i++)
		newHtml[i] = html[i];
	for (int i = 0; i < sizeof(part); i++)
		newHtml[i + length] = part[i];
	free(html);
	html = newHtml;
}*/


