#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

const string HTTP_VERSION = "HTTP/1.1";
const string CONNECTION_TYPE = "keep-alive";
#define BUFFER_SIZE 4096

int http_create_socket(const char *ip, int port) //这里直接用阻塞形式了
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = inet_addr(ip);
    if (0 != connect(sockfd, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)))
        return -1;
    return sockfd;
}

string http_recv_request(int sockfd)
{
    char buffer_tep[BUFFER_SIZE + 1];
    string result;

    while (1)
    {
        memset(buffer_tep, 0, BUFFER_SIZE);
        int len = recv(sockfd, buffer_tep, BUFFER_SIZE, 0); //接受数据
        printf("Recv len: %d\n", len);
        if (len == 0 || len == -1) // disconnect or error
        {
            if (len == -1)
                printf("Error: recv\n");
            break;
        }
        buffer_tep[len] = '\0'; //加尾零
        result += buffer_tep;   //把buffer_tep数据copy到result中
    }
    return result;
}
string http_prepare_request_headers(const string ip, int port, const string resource, const string method, const map<string, string> data)
{
    string buffer;

    if (method == "GET")
        buffer = "GET " + resource + " " + HTTP_VERSION + "\r\n" +
                 "Host:" + ip + ":" + to_string(port) + "\r\n" +
                 "Connection:" + CONNECTION_TYPE + "\r\n" +
                 "\r\n";
    if (method == "POST") //处理data
        buffer = "POST " + resource + " " + HTTP_VERSION + "\r\n" +
                 "Host:" + ip + ":" + to_string(port) + "\r\n" +
                 "Connection:" + CONNECTION_TYPE + "\r\n" +
                 "\r\n";
    /*
    sprintf(buffer,
            "POST / %s \r\n"
            "Host:%s:%d\r\n"
            "username=%s&password=%s&input_ans=1&auth_answer=1&login=登录\r\n"
            "%s\r\n"
            "\r\n",
            HTTP_VERSION,
            ip, port, username, password,
            CONNECTION_TYPE);*/
    return buffer;
}
int main() // int argc, char *argv[])
{
    string ip = "10.10.108.117";
    int port = 20013;
    int sockfd = http_create_socket(ip.c_str(), port);

    const char *username = "1951705";
    const char *password = "tongjigzy_02";
    map<string, string> data;
    data["username"] = "1951705";
    data["password"] = "tongjigzy_02";

    const char *resource = "/";
    string buffer = http_prepare_request_headers(ip, port, resource, "GET", data);
    int len = send(sockfd, buffer.c_str(), buffer.length(), 0); // send
    if (len == -1)
    {
        cerr << "Error: send" << endl;
        exit(-1);
    }
    string response = http_recv_request(sockfd);
    cout << "response: " << response << endl;
    return 0;
}
