#include "../../libcpprest/include/cpprest/http_listener.h"
#include "../../libjobject/Object.h"
#include "../../libjobject/Reflectable.h"
#include "../../libjobject/Metadata.h"

#include "RESTController.h"

#include <Windows.h>

#include <map>

using namespace web;
using namespace web::http;
using namespace http::experimental::listener;

#pragma warning(push)
#pragma warning(disable : 4267) // conversion from 'size_t' to 'int', possible loss of data
std::string
WideToMulti(wchar_t const* value, size_t nchars, unsigned int codePage = CP_UTF8)
{
    std::string result;
    if (value && 0 < nchars) {
        int reserve = WideCharToMultiByte(codePage, 0, value, nchars, NULL, 0, NULL, NULL);
        if (0 < reserve) {
            std::vector<char> buffer(reserve);
            if (WideCharToMultiByte(codePage, 0, value, nchars, &buffer[0], buffer.size(), NULL, NULL)) {
                result.assign(buffer.begin(), buffer.end());
            }
        }
    }
    return (result);
}

std::string
WideToMulti(std::wstring value, unsigned int codePage = CP_UTF8)
{
    return(WideToMulti(value.c_str(), value.length(), codePage));
}

std::wstring
MultiToWide(char const* value, size_t nchars, unsigned int codePage = CP_UTF8)
{
    std::wstring result;
    if (value && 0 < nchars) {
        int reserve = MultiByteToWideChar(codePage, 0, value, nchars, NULL, 0);
        if (0 < reserve) {
            std::vector<wchar_t> buffer(reserve);
            if (MultiByteToWideChar(codePage, 0, value, nchars, &buffer[0], buffer.size())) {
                result.assign(buffer.begin(), buffer.end());
            }
        }
    }
    return (result);
}


std::wstring
MultiToWide(std::string value, unsigned int codePage = CP_UTF8)
{
    return(MultiToWide(value.c_str(), value.length(), codePage));
}


template <>
class Serialization<std::wstring> {
public:
    static Json::Value Serialize(std::wstring value) {
        Json::Value json(WideToMulti(value));
        return json;
    }

    static std::wstring Deserialize(Json::Value json) {
        return MultiToWide(json.asString());
    }
};

RESTController::RESTController(std::wstring endpoint, Object& object) : m_endpoint(endpoint), m_object(object) {}
RESTController::~RESTController() {}

void RESTController::handleHttpMessage(http_request message) {
    auto response = json::value::object();
    try {
        Json::StreamWriterBuilder wbuilder;
        wbuilder["indentation"] = "\t";

        //auto extractJsonTask = message.extract_json();
        //extractJsonTask.wait();
        //std::wstring inputArguments = extractJsonTask.get().serialize();
        auto extractStringTask = message.extract_string();
        extractStringTask.wait();
        std::wstring inputArguments = extractStringTask.get();
        std::string inputArgumentsNarrow = WideToMulti(inputArguments);

        Json::Value inputArgumentsJson;
        std::string errs;
        std::stringstream inputArgumentsNarrowStream(inputArgumentsNarrow);

        Json::CharReaderBuilder rbuilder;
        rbuilder["collectComments"] = false;

        if (!inputArguments.empty()) {
            if (!Json::parseFromStream(rbuilder, inputArgumentsNarrowStream, &inputArgumentsJson, &errs)) {
                std::string inputArgumentsNarrowAsString = "\"" + inputArgumentsNarrow + "\"";
                std::stringstream inputArgumentsNarrowStream(inputArgumentsNarrowAsString);
                if (!Json::parseFromStream(rbuilder, inputArgumentsNarrowStream, &inputArgumentsJson, &errs)) {
                    std::string inputArgumentsNarrowAsArray = "[" + inputArgumentsNarrow + "]";
                    std::stringstream inputArgumentsNarrowStream(inputArgumentsNarrowAsArray);
                    Json::parseFromStream(rbuilder, inputArgumentsNarrowStream, &inputArgumentsJson, &errs);
                }
            }
        }

        std::vector<std::wstring> pathParameters = message.relative_uri().split_path(message.relative_uri().path());
        std::map<std::wstring, std::wstring> queryParameters = message.request_uri().split_query(message.request_uri().query());

        Json::Value pathParametersSerialized = Serialization<std::vector<std::wstring>>::Serialize(pathParameters);
        Json::Value queryParametersSerialized = Serialization<std::map<std::wstring, std::wstring>>::Serialize(queryParameters);

        std::vector<std::string> inputArgumentsVector;
        std::vector<std::string> inputArgumentsVectorWithPathParameters;
        std::vector<std::string> inputArgumentsVectorWithPathAndQueryParameters;

        inputArgumentsVectorWithPathParameters.push_back(Json::writeString(wbuilder, pathParametersSerialized));
        inputArgumentsVectorWithPathAndQueryParameters.push_back(Json::writeString(wbuilder, pathParametersSerialized));
        inputArgumentsVectorWithPathAndQueryParameters.push_back(Json::writeString(wbuilder, queryParametersSerialized));

        if (!inputArgumentsJson.empty()) {
            if (inputArgumentsJson.isArray()) {
                for (Json::Value inputArgumentJson : inputArgumentsJson) {
                    inputArgumentsVector.push_back(Json::writeString(wbuilder, inputArgumentJson));
                    inputArgumentsVectorWithPathParameters.push_back(Json::writeString(wbuilder, inputArgumentJson));
                    inputArgumentsVectorWithPathAndQueryParameters.push_back(Json::writeString(wbuilder, inputArgumentJson));
                }
            }
            else {
                inputArgumentsVector.push_back(Json::writeString(wbuilder, inputArgumentsJson));
                inputArgumentsVectorWithPathParameters.push_back(Json::writeString(wbuilder, inputArgumentsJson));
                inputArgumentsVectorWithPathAndQueryParameters.push_back(Json::writeString(wbuilder, inputArgumentsJson));
            }
        }

        wprintf(L"Message type: %s\n", message.method().c_str());
        wprintf(L"Full URL: %s\n", message.absolute_uri().to_string().c_str());
        wprintf(L"Relative URL: %s\n", message.relative_uri().to_string().c_str());
        wprintf(L"Request URL: %s\n", message.request_uri().to_string().c_str());
        wprintf(L"Query parameters: %s\n", message.relative_uri().query().c_str());
        wprintf(L"Received json: %s\n", inputArguments.c_str());

        std::wstring methodName = message.method();
        for (auto& c : methodName) {
            c = toupper(c);
        }
        methodName += L"_";
        methodName += pathParameters[0];

        std::string methodNameNarrow = WideToMulti(methodName);
        Method method = m_object.getClass().getMethod(methodNameNarrow.c_str());

        std::error_code errCode;
        std::string returnValue;

        try {
            returnValue = method.invokeSerialized(m_object, inputArgumentsVectorWithPathAndQueryParameters);
        }
        catch (const Exception & e)
        {
            try {
                returnValue = method.invokeSerialized(m_object, inputArgumentsVectorWithPathParameters);
            }
            catch (const Exception & e) {
                returnValue = method.invokeSerialized(m_object, inputArgumentsVector);
            }
        }

        if (
            returnValue.find("{") == std::string::npos && 
            returnValue.find("[") == std::string::npos &&
            returnValue.find("\"") == std::string::npos
        ) {
            returnValue = std::string("\"") + returnValue + std::string("\"");
        }

        response = json::value::parse(returnValue, errCode);
        message.reply(status_codes::OK, response);
    }
    catch (const Exception & e) {
        std::error_code errCode;
        std::string errMessage = std::string("\"") + e.Message() + std::string("\"");

        response = json::value::parse(errMessage, errCode);
        message.reply(status_codes::InternalError, response);
    }
    catch (const std::exception & e) {
        message.reply(status_codes::InternalError);
    }
}

void RESTController::init() {
    uri endpointURI(m_endpoint);
    uri_builder endpointBuilder;

    endpointBuilder.set_scheme(endpointURI.scheme());
    endpointBuilder.set_host(L"localhost");
    endpointBuilder.set_port(endpointURI.port());
    endpointBuilder.set_path(endpointURI.path());

    m_listener = http_listener(endpointBuilder.to_uri());
    m_listener.support(methods::GET, std::bind(&RESTController::handleHttpMessage, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&RESTController::handleHttpMessage, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&RESTController::handleHttpMessage, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&RESTController::handleHttpMessage, this, std::placeholders::_1));
    m_listener.support(methods::PATCH, std::bind(&RESTController::handleHttpMessage, this, std::placeholders::_1));
}

void RESTController::start() {
    init();
    m_listener.open().wait();
}

void RESTController::shutdown() {
    m_listener.close().wait();
}