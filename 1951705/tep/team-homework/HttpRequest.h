#pragma once
#include <string>
class HttpRequest
{
    std::string message;
    static const std::string HTTP_VERSION;

public:
    void setMethod(const std::string resource = "/", const std::string method = "GET");
    void setHeader(const std::string header = "");
    void setBody(const std::string body = "");
    void clear() { message.clear(); }
    std::string getMessage() { return message; }
    HttpRequest() {}
    HttpRequest(const std::string resource, const std::string method) { setMethod(resource, method); }
    ~HttpRequest() { clear(); }
};
