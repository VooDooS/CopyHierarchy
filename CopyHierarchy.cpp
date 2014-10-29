// CopyHierarchy.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <windows.h>

#include <direct.h>
#define GetCurrentDir _getcwd

using namespace std;

/*//Explode a wstring
vector<wstring> explode(const wstring &s, char delim)
{
    vector<wstring> res;
    wifstream wiss();
    wiss << ;
    for (wstring token; getline(wiss, token, delim); )
    {
        res.push_back(move(token));
    }
}*/

//Converting strings to wstrings
wstring s2ws(const string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

//Getting the execution path
wstring getexepath()
{
    char cCurrentPath[FILENAME_MAX];

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
        return wstring();
    }

    cCurrentPath[sizeof(cCurrentPath)-1] = '\0'; /* not really required */

    return wstring(s2ws(string(cCurrentPath)));
}

//Recursive copy of directories
vector<wstring> get_all_files_names_within_folder(wstring folder, wstring root, wstring newPath = wstring())
{
    const wstring path = folder + s2ws(string("\\*"));
    vector<wstring> names;

    wcout << path << endl;

    //Creating the directory
    CreateDirectory((root + newPath).c_str(), NULL);

    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile(path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&  wstring(fd.cFileName) != wstring(s2ws(".")) && wstring(fd.cFileName) != wstring(s2ws("..")))
            {
                //Appel récursif:
                get_all_files_names_within_folder(folder + s2ws("\\") + fd.cFileName, root, newPath + s2ws("\\") + fd.cFileName);
            }
        } while (FindNextFile(hFind, &fd));

        FindClose(hFind);
    }
    else {
        cout << "Invalid path." << endl;
    }
    return names;
}


int main(int argc, const char* argv[])
{
    wstring rep;
    wstring root;

    wcout << "Vous etes ici: " << getexepath() << endl;
    cout << "Chemin vers le repertoire a copier: ";
    wcin >> rep;

    root = s2ws("hierarchy"); //rep + s2ws("_hierarchy");

    //CreateDirectory(root.c_str(), NULL);
    get_all_files_names_within_folder(rep, root);

    system("PAUSE");

	return 0;
}

