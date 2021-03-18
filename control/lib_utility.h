#ifndef _LIB_UTILITY_H
#define _LIB_UTILITY_H

#include <cstring>
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>

#include "lib_vector.h"

using namespace std;

namespace RandomUtility
{
//random shuffle
template <typename T>
inline void randomShuffle(Vector<T> & a, int id_from, int id_to)
{
    //  mt19937 rng(chrono::system_clock::now().time_since_epoch().count());
    srand(time(NULL));
    for (int i = id_to - id_from; i >= 1; --i)
    {
        int x = id_from + i;
        int y = id_from + rand() % i;
        T tmp = a[x];
        a[x] = a[y];
        a[y] = tmp;
    }
}

//random shuffle
template <typename T>
inline void randomShuffle(Vector<T> & a)
{
    randomShuffle(a, 0, a.size() - 1);
}
}

namespace FileUtility
{
inline bool check_existed_file(std::fstream& ff)
{
    return ff.good();
}

inline bool check_empty_file(std::fstream& ff)
{
    return ff.peek() == std::ifstream::traits_type::eof();
}

inline int DeleteDirectory(const std::string &refcstrRootDirectory,
                           bool              bDeleteSubdirectories = true)
{
    bool            bSubdirectory = false;       // Flag, indicating whether
    // subdirectories have been found
    HANDLE          hFile;                       // Handle to directory
    std::string     strFilePath;                 // Filepath
    std::string     strPattern;                  // Pattern
    WIN32_FIND_DATA FileInformation;             // File information


    strPattern = refcstrRootDirectory + "\\*.*";
    hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
    if(hFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if(FileInformation.cFileName[0] != '.')
            {
                strFilePath.erase();
                strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;

                if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    if(bDeleteSubdirectories)
                    {
                        // Delete subdirectory
                        int iRC = DeleteDirectory(strFilePath, bDeleteSubdirectories);
                        if(iRC)
                            return iRC;
                    }
                    else
                        bSubdirectory = true;
                }
                else
                {
                    // Set file attributes
                    if(::SetFileAttributes(strFilePath.c_str(),
                                           FILE_ATTRIBUTE_NORMAL) == FALSE)
                        return ::GetLastError();

                    // Delete file
                    if(::DeleteFile(strFilePath.c_str()) == FALSE)
                        return ::GetLastError();
                }
            }
        }
        while(::FindNextFile(hFile, &FileInformation) == TRUE);

        // Close handle
        ::FindClose(hFile);

        DWORD dwError = ::GetLastError();
        if(dwError != ERROR_NO_MORE_FILES)
            return dwError;
        else
        {
            if(!bSubdirectory)
            {
                // Set directory attributes
                if(::SetFileAttributes(refcstrRootDirectory.c_str(),
                                       FILE_ATTRIBUTE_NORMAL) == FALSE)
                    return ::GetLastError();

                // Delete directory
                if(::RemoveDirectory(refcstrRootDirectory.c_str()) == FALSE)
                    return ::GetLastError();
            }
        }
    }

    return 0;
}
}

//clipboard
namespace ClipboardUtility
{
inline void _GetWindowHandle(HWND& windowHandle)
{
    windowHandle = GetForegroundWindow();
    if (windowHandle == NULL)
    {
        throw runtime_error("Failed to retrieve handle for active window.");
    }
}

inline void set(const std::string& string)
{
    HWND windowHandle;
    _GetWindowHandle(windowHandle);

    if (!OpenClipboard(windowHandle))
        throw runtime_error("Failed to open clipboard.");

    if (EmptyClipboard())
    {
        // The Windows clipboard seems to want to store a terminating char
        // in the last position. So we reserve an additional byte for that.
        std::size_t allocSize = string.size() + 1U;
        HGLOBAL globalAlloc = GlobalAlloc(GMEM_MOVEABLE, allocSize);
        if (!globalAlloc)
        {
            CloseClipboard();
            throw std::runtime_error("Failed to allocate required space.");
        }
        std::memcpy(GlobalLock(globalAlloc), string.c_str(), allocSize);
        GlobalUnlock(globalAlloc);
        if (SetClipboardData(CF_TEXT, globalAlloc) == NULL)
        {
            CloseClipboard();
            GlobalFree(globalAlloc);
            throw std::runtime_error("Failed to set clipboard data.");
        }
        CloseClipboard(); // Not much we can do if this fails...
        GlobalFree(globalAlloc);
    }
    else
    {
        CloseClipboard();
        throw runtime_error("Failed to empty the clipboard.");
    }
}

inline void get(std::string& output)
{
    HWND windowHandle;
    _GetWindowHandle(windowHandle);

    if (!OpenClipboard(windowHandle))
        throw runtime_error("Failed to open clipboard.");

    if (IsClipboardFormatAvailable(CF_TEXT))
    {
        HGLOBAL clipboardData = GetClipboardData(CF_TEXT);
        if (clipboardData != NULL)
        {
            LPVOID clipboardDataPointer = GlobalLock(clipboardData);
            if (clipboardDataPointer != NULL)
            {
                output.clear();
                output.assign(static_cast<char*>(clipboardDataPointer));
            }
            GlobalUnlock(clipboardData);
            CloseClipboard();
        }
    }
    CloseClipboard();
}

//paste string to clipboard
inline void setClipboard(const string& str)
{
    try
    {
        ClipboardUtility::set(str);
    }
    catch (const runtime_error & e)
    {
        cout << e.what() << ::endl;
    }
}

inline string getClipboard()
{
    string str;
    try
    {
        ClipboardUtility::get(str);
    }
    catch (const runtime_error & e)
    {
        cout << e.what() << ::endl;
    }
    return str;
}
}

namespace StringUtility
{
//Having only alpha
inline bool IsAlpha(const string& input)
{
    for (int i = 0; i < (int) input.size(); ++i)
        if (! isalpha(input[i]))
            return false;
    return true;
}

//Having only alpha and space
inline bool IsAlphaSpace(const string& input)
{
    for (int i = 0; i < (int) input.size(); ++i)
        if (! isspace(input[i]) && ! isalpha(input[i]))
            return false;
    return true;
}

//Having only digit
inline bool IsDigit(const string& input)
{
    for (int i = 0; i < (int) input.size(); ++i)
        if (! isdigit(input[i]))
            return false;
    return true;
}

//Having alpha or digit
inline bool IsAlNum(const string& input)
{
    for (int i = 0; i < (int) input.size(); ++i)
        if (! isalnum(input[i]))
            return false;
    return true;
}

//Having at least one punctuation char
inline bool HavingPunctChar(const string& input)
{
    for (int i = 0; i < (int) input.size(); ++i)
        if (ispunct(input[i]))
            return true;
    return false;
}

//Having at least one space
inline bool HavingSpace(const string& input)
{
    for (int i = 0; i < (int) input.size(); ++i)
        if (isspace(input[i]))
            return true;
    return false;
}

//clear space leading, trailing, among words
inline void Trim(string& input)
{
    if (input.empty())
        return;
    int st = 0;
    int en = input.size() - 1;
    string correct = "";
    while (st <= en && input[st] == ' ')
        ++st;
    while (en >= st && input[en] == ' ')
        --en;
    if (st <= en)
    {
        for (int i = st; i <= en; )
        {
            if (input[i] == ' ')
            {
                int cnt = 0;
                while (i + cnt <= en && input[i + cnt] == ' ')
                    ++cnt;
                correct += ' ';
                i += cnt;
            }
            else
            {
                correct += input[i];
                ++i;
            }
        }
    }
    input = correct;
}

inline void UpperCaseFirstLetterPerWord(string& input)
{
    if (input.empty())
        return;
    for (int i = 0; i < (int) input.size(); ++i)
    {
        if (!isalpha(input[i]))
            continue;
//			if ((i == 0 || (i > 0 && input[i - 1] == ' ')) && isalpha(input[i]))
        if (i == 0 || (i > 0 && input[i - 1] == ' '))
        {
            input[i] = toupper(input[i]);
        }
        else
        {
            input[i] = tolower(input[i]);
        }
    }
}

inline void UpperCaseAllLetter(string& input)
{
    if (input.empty())
        return;
    for (int i = 0; i < (int) input.size(); ++i)
    {
        input[i] = toupper(input[i]);
    }
}

inline void LowerCaseAllLetter(string& input)
{
    if (input.empty())
        return;
    for (int i = 0; i < (int) input.size(); ++i)
    {
        input[i] = tolower(input[i]);
    }
}

//////////////////////////////////////////////////////
//string
inline string to_string(const int& x)
{
    char schar[snprintf(NULL, 0,"%d",x)];
    sprintf(schar, "%d", x);
    return string(schar);
}

//convert string to char array not constant
//diff with str.c_str() is constant
inline char *toCharArr(const string& str)
{
    char *ret = new char[str.size()];
    strcpy(ret, str.c_str());
    return ret;
}

inline Vector<string> tokenize(const string& str, const string& delims)
{
    Vector<string> ret;
    char *tmp = toCharArr(str);
    char *p = strtok(tmp, delims.c_str());
    while (p)
    {
        ret.push_back(string(p));
        p = strtok(NULL, delims.c_str());
    }
    delete[] tmp;
    return ret;
}

//	bool check_password (int limSz, string correctPsw) {
//			string input;
//			for (int i = 1 ; i <= 3 ; i++) {
//				cout << "Password: ";
//				input = "";
//				int j = 0;
//				char c;
//				while (j < limSz && (c = getch()) != 13) {
//					 input += c;
//					 cout << '*'; ++j;
//				}
//				if (input == correctPsw) return true;
//			}
//			return false;
//	}

//UTF-8 encoding file stream
/* Example:
	std::ofstream testFile;
	testFile.open("demo.xml", std::ios::out | std::ios::binary);
	std::wstring text =
		L"< ?xml version=\"1.0\" encoding=\"UTF-8\"? >\n"
		L"< root description=\"this is a naïve example\" >\n< /root >";
	std::string outtext = to_utf8(text);
	testFile << outtext;
	testFile.close();
*/

inline Vector<string> splitStringByChar(const string& str, char ch)
{
    Vector<string> res;
    if (str != "")
    {
        stringstream strm(str);
        string ele;
        while(getline(strm, ele, '\n'))
        {
            res.push_back(ele);
        }
    }
    return res;
}

inline void adjustStringToFit(string& str, int len)
{
    if ((int) str.size() <= len)
        return;
    str = str.substr(0, len);
    str[len - 1] = str[len - 2] = str[len - 3] = '.';
}

inline std::string to_utf8(const wchar_t* buffer, int len)
{
    int nChars = ::WideCharToMultiByte(CP_UTF8, 0, buffer, len, NULL, 0, NULL, NULL);
    if (nChars == 0)
        return "";

    string newbuffer;
    newbuffer.resize(nChars) ;
    ::WideCharToMultiByte(CP_UTF8, 0, buffer, len, const_cast< char* >(newbuffer.c_str()), nChars, NULL, NULL);

    return newbuffer;
}

inline std::string to_utf8(const std::wstring& str)
{
    return to_utf8(str.c_str(), (int)str.size());
}
}

#endif
