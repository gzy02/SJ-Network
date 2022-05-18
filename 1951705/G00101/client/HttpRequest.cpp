#include "HttpRequest.h"
#include <string>
using namespace std;
const std::string HttpRequest::HTTP_VERSION = "HTTP/1.1";

void HttpRequest::setMethod(const std::string resource, const std::string method)
{
    message += method + " " + resource + " " + HTTP_VERSION + "\r\n";
}
void HttpRequest::setHeader(const std::string header)
{
    message += header + "\r\n";
}
void HttpRequest::setBody(const std::string body)
{
    message += body;
}
