#pragma once

#include "../../libcpprest/include/cpprest/http_listener.h"
#include "../../libjobject/Reflectable.h"

#include "IRESTController.h"

#include <string>

using namespace web;
using namespace web::http;
using namespace http::experimental::listener;

class RESTController : public Reflectable<RESTController>, public IRESTController {
private:
    std::wstring m_endpoint;
    Object& m_object;
    http_listener m_listener;

public:
    RESTController(std::wstring endpoint, Object& object);
    ~RESTController();
    void handleHttpMessage(http_request message);
    void init();
    void start();
    void shutdown();
};