#include "MyRestServer.h"
#include <Windows.h>

MyRestServer::MyRestServer() {
    char currentDir[MAX_PATH + 1] = { 0 };
    GetCurrentDirectoryA(MAX_PATH + 1, currentDir);
    currentDirectory = currentDir;
}

void MyRestServer::POST_changeDir(std::string newDirectory) {
    SetCurrentDirectoryA(newDirectory.c_str());
    char currentDir[MAX_PATH + 1] = { 0 };
    GetCurrentDirectoryA(MAX_PATH + 1, currentDir);
    currentDirectory = currentDir;
}

std::vector<FilesystemItem> MyRestServer::GET_listDir() const {
    std::string searchQuery = currentDirectory + "\\*";
    WIN32_FIND_DATAA findData = { 0 };
    std::vector<FilesystemItem> ret;
    HANDLE findHandle = FindFirstFileA(searchQuery.c_str(), &findData);
    if (findHandle != INVALID_HANDLE_VALUE) {
        do {
            FilesystemItem item;
            item.fileName = findData.cFileName;
            item.size =
                (((long long)findData.nFileSizeHigh) << 32) |
                findData.nFileSizeLow;
            item.type =
                findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ?
                "DIRECTORY" :
                "FILE";
            ret.push_back(item);
        } while (FindNextFileA(findHandle, &findData));
        FindClose(findHandle);
        return ret;
    }
    throw;
}