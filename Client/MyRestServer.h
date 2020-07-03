#pragma once

#include "../libjobject/Reflectable.h"

class MyRestServer : public Reflectable<MyRestServer> {
public:
    int GET_method1();
    std::map<std::string, double> GET_method2();
    std::vector<std::string> GET_method3(std::vector<std::string> pathParameters, std::map<std::string, std::string> queryParameters);
};