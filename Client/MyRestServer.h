#pragma once

#include "../libjobject/Reflectable.h"

struct FilesystemItem : public Reflectable<FilesystemItem> {
    std::string fileName;
    std::string type;
    long long size;
};

class MyRestServer : public Reflectable<MyRestServer> {
private:
    std::string currentDirectory;

public:
    MyRestServer();
    void POST_changeDir(std::string newDirectory);
    std::vector<FilesystemItem> GET_listDir() const;
};
