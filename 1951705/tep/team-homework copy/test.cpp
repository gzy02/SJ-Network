#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

int http_create_socket(const char *ip, int port) //这里直接用阻塞形式了
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = inet_addr(ip);
    if (0 != connect(sockfd, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)))
    {
        cerr << "Create socket error." << endl;
        cerr << errno << endl
             << strerror(errno) << endl;

        exit(EXIT_FAILURE);
    }
    return sockfd;
}

string http_recv_request(int sockfd)
{
    const int BUFFER_SIZE = 4096;
    char buffer_tep[BUFFER_SIZE + 1];
    string result;
    while (1)
    {
        memset(buffer_tep, 0, BUFFER_SIZE);
        int len = recv(sockfd, buffer_tep, BUFFER_SIZE, 0); //接受数据
        if (len == -1)
        {
            cerr << "Error: recv\n";
            cerr << errno << endl
                 << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }
        else if (len == 0) // disconnect
        {
            break;
        }
        buffer_tep[len] = '\0'; //加尾零
        result += buffer_tep;   //把buffer_tep数据copy到result中
    }
    return result;
}

int http_send_request(int sockfd, string buffer)
{
    int len = send(sockfd, buffer.c_str(), buffer.length(), 0); // GET 请求 send
    if (len == -1)
    {
        cerr << "Error: send" << endl;
        cerr << errno << endl
             << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    return len;
}

string http_prepare_request_headers(const string ip, int port, const string resource, const string method = "GET",
                                    const string cookie = "", const string msg = "",
                                    const string content_type = "application/x-www-form-urlencoded", const string boundary = "")
{
    const string HTTP_VERSION = "HTTP/1.1";
    const string CONNECTION_TYPE = "keep-alive";

    string buffer;
    string cookie_head = "";
    if (cookie.length() != 0)
        cookie_head = "Cookie:" + cookie + "\r\n";

    if (method == "GET")
    {
        buffer = "GET " + resource + " " + HTTP_VERSION + "\r\n" +
                 cookie_head +
                 "Host: " + ip + ":" + to_string(port) + "\r\n" +
                 "Connection: " + CONNECTION_TYPE + "\r\n" +
                 "\r\n";
    }
    if (method == "POST") //额外处理data
    {
        buffer = "POST " + resource + " " + HTTP_VERSION + "\r\n" +
                 //"Accept: */*" + "\r\n" +
                 //"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.54 Safari/537.36 Edg/101.0.1210.39" + "\r\n" +
                 cookie_head +
                 "Host: " + ip + ":" + to_string(port) + "\r\n" +
                 "Connection: " + CONNECTION_TYPE + "\r\n" +
                 "Content-Type: " + content_type + boundary + "\r\n" +
                 "Content-Length: " + to_string(msg.length()) + "\r\n" +
                 "\r\n";
        buffer += msg;
    }
    return buffer;
}

string get_cookie(const string resp)
{
    string substr = "Set-Cookie: ";
    string cookie;
    size_t pos;
    if ((pos = resp.find(substr)) != string::npos)
    {
        pos += substr.length();
        while (resp[pos] != ';')
        {
            // cout << resp[pos] << endl;
            cookie.push_back(resp[pos]);
            ++pos;
        }
    }
    return cookie;
}
string get_alert(const string resp)
{
    string substr = "alert";
    string res;
    size_t pos;
    if ((pos = resp.find(substr)) != string::npos)
    {
        pos += substr.length() + 2;                      // 2个字符，即 ( 和 '
        while (resp[pos] != '\'' && pos < resp.length()) // alert('xxx')
        {
            if (resp[pos] == '\\' && pos + 1 < resp.length() && resp[pos + 1] == 'n')
            {
                res.push_back('\n'); //尾零
                pos += 2;
            }
            else
            {
                res.push_back(resp[pos]);
                ++pos;
            }
        }
    }
    return res;
}
string FileToStr(string filepath)
{
    fstream f(filepath, ios::in | ios::binary);
    if (f.good())
    {
        f.unsetf(ios::skipws); // 关闭inputFile的忽略空格标志,可以文件中的保留空格
        istream_iterator<char> iter(f);
        string s(iter, istream_iterator<char>());
        return s;
    }
    else
    {
        cout << "Can't open file!" << endl;
        return "";
    }
}
string MapToString(map<string, string> data)
{
    string res = "";
    map<string, string>::iterator iter;
    for (iter = data.begin(); iter != data.end(); ++iter)
    {
        res += iter->first + "=" + iter->second + "&";
    }
    res.erase(res.end() - 1);
    return res;
}

int main() // int argc, char *argv[])
{
    string ip = "10.10.108.117";
    int port = 20013;
    string resource = "/";

    int sockfd = http_create_socket(ip.c_str(), port);
    string buffer_get = http_prepare_request_headers(ip, port, resource, "GET");

    int len = http_send_request(sockfd, buffer_get);
    string response = http_recv_request(sockfd);
    close(sockfd);

    string cookie = get_cookie(response); //获得cookie
    {
        sockfd = http_create_socket(ip.c_str(), port);
        map<string, string> login_data;
        login_data["username"] = "1951705";
        login_data["password"] = "tongjigzy_02";
        login_data["input_ans"] = "42";
        login_data["auth_answer"] = "42";
        login_data["login"] = "登录";
        string msg = "";
        msg = MapToString(login_data);

        string buffer_login = http_prepare_request_headers(ip, port, resource, "POST", cookie, msg);
        len = send(sockfd, buffer_login.c_str(), buffer_login.length(), 0); // GET 请求 send
        if (len == -1)
        {
            cerr << "Error: send" << endl;
            exit(-1);
            cerr << errno << endl
                 << strerror(errno) << endl;
        }
        response = http_recv_request(sockfd);
        // cout << "Login response: \n"
        //      << response << endl;
        close(sockfd);
    }

    {
        resource = "/lib/smain.php?action=第0章"; //%B5%DA0%D5%C2";
        sockfd = http_create_socket(ip.c_str(), port);
        // map<string, string> submit_data;
        // submit_data["submit"] = "提交";

        string content_type = "multipart/form-data; boundary=";
        string boundary = "77daaf90ce52cd73f471a7d37953cdff";

        string body = "--" + boundary + "\r\n";
        body += "Content-Disposition: form-data; name=\"submit\"\r\n\r\n";
        body += "提交\r\n";
        body += "--" + boundary + "\r\n";
        body += "Content-Disposition: form-data; name=\"upfile3\"; filename=\"test3.pdf\"\r\n";
        body += "\r\n";
        string file = FileToStr("./test-submit/test3.pdf");
        if (file == "")
            return -1; //小心文件里面有\0
        body += file;
        body += "\r\n";
        body += "--" + boundary + "--\r\n";

        string buffer_submit = http_prepare_request_headers(ip, port, resource, "POST", cookie, body, content_type, boundary);

        // cout << "buffer_submit:" << endl
        //      << buffer_submit << endl;

        len = send(sockfd, buffer_submit.c_str(), buffer_submit.length(), 0);
        if (len == -1)
        {
            cerr << "Error: send" << endl;
            exit(-1);
            cerr << errno << endl
                 << strerror(errno) << endl;
        }
        response = http_recv_request(sockfd);

        fstream f("response.html", ios::out | ios::binary);
        if (f.good())
        {
            f.unsetf(ios::skipws); // 关闭inputFile的忽略空格标志,可以文件中的保留空格
            f << response;
        }
        else
        {
            cout << "Can't open file!" << endl;
        }
        cout << get_alert(response) << endl;

        // cout << "Final response: \n"
        //      << response << endl;
        close(sockfd);
    }

    return 0;
}
