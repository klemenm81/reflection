#include "MyRestServer.h"

int MyRestServer::GET_method1() {
    printf("Inside GET_method1\n");
    return 0;
}

std::map<std::string, double> MyRestServer::GET_method2() {
    printf("Inside GET_method2\n");
    std::map<std::string, double> myMap;
    myMap["PI"] = 3.14159;
    myMap["radius"] = 4.0;
    return myMap;
}

std::vector<std::string> MyRestServer::GET_method3(
    std::vector<std::string> pathParameters, 
    std::map<std::string, std::string> queryParameters) {
    return pathParameters;
}
