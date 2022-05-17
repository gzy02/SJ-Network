#include "HttpArg.h"
#include "HttpRequest.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h>
using namespace std;

int http_create_socket(const char *ip, int port);
string http_recv_request(int sockfd);
int http_send_request(int sockfd, string buffer);

string get_cookie(const string resp);
string get_alert(const string resp);
string FileToStr(string filepath);
string MapToString(map<string, string> data);

int main(int argc, char *argv[])
{
    HttpArg myarg(argc, argv);

    // string ip = "10.10.108.117";
    // int port = 20013;
    // string resource = "/";
    // string username = "1951705";
    // string password = "tongjigzy_02";
    // string srcfile = "./test-submit/test6-socket-tcp-sync.pdf";
    // string dstfile = "test6-socket-tcp-sync.pdf";

    string ip = myarg.ip;
    int port = myarg.port;
    string username = myarg.user;
    string password = myarg.passwd;
    string srcfile = myarg.srcfile;
    string dstfile = myarg.dstfile;
    string resource = "/";

    map<string, string> dstfile_input_name; // dstfile����վ��input���nameһһ��Ӧ
    dstfile_input_name["test1.tar.bz2"] = "upfile1";
    dstfile_input_name["test2.pdf"] = "upfile2";
    dstfile_input_name["test3.pdf"] = "upfile3";
    dstfile_input_name["test4.cpp"] = "upfile4";
    dstfile_input_name["test5-rpm_service.tar.bz2"] = "upfile5";
    dstfile_input_name["test6-socket-tcp-sync.pdf"] = "upfile6";

    if (dstfile_input_name.count(dstfile) == 0)
    {
        cerr << "dstfile name error!\n";
        exit(EXIT_FAILURE);
    }

    const string CONNECTION_TYPE = "keep-alive";
    string con_header = "Connection: " + CONNECTION_TYPE;
    string host_header = "Host: " + ip + ":" + to_string(port);

    //����Ϊ��ȡcookie
    int sockfd = http_create_socket(ip.c_str(), port);

    HttpRequest myhttp;
    myhttp.setMethod(resource, "GET");
    myhttp.setHeader(host_header);
    myhttp.setHeader(con_header);
    myhttp.setHeader(); //��һ��

    string buffer_get = myhttp.getMessage();

    int len = http_send_request(sockfd, buffer_get);
    string response = http_recv_request(sockfd);
    close(sockfd);

    string cookie = get_cookie(response); //���cookie
    string cookie_header = "Cookie:" + cookie;
    cout << "��ȡcookie�ɹ���cookie��\n"
         << cookie << endl;

    //����Ϊ��¼
    {
        sockfd = http_create_socket(ip.c_str(), port);
        map<string, string> login_data;
        login_data["username"] = username;
        login_data["password"] = password;
        login_data["input_ans"] = "42"; //д������
        login_data["auth_answer"] = "42";
        login_data["login"] = "��¼";
        string body = MapToString(login_data); //���HTTP���ı��ĸ�ʽ

        string content_type_header = "Content-Type: application/x-www-form-urlencoded";
        string content_len_header = "Content-Length: " + to_string(body.length());

        myhttp.clear(); //����
        myhttp.setMethod(resource, "POST");
        myhttp.setHeader(host_header);
        myhttp.setHeader(con_header);
        myhttp.setHeader(cookie_header);
        myhttp.setHeader(content_type_header);
        myhttp.setHeader(content_len_header);
        myhttp.setHeader(); //��һ��
        myhttp.setBody(body);

        string buffer_login = myhttp.getMessage();
        len = http_send_request(sockfd, buffer_login);
        response = http_recv_request(sockfd);
        string login_alert = get_alert(response);
        if (login_alert.length() == 0)
            cout << "��¼�ɹ�\n";
        else
        {
            cout << login_alert << endl;
            exit(EXIT_FAILURE);
        }

        close(sockfd);
    }

    //����Ϊ�ύ
    {
        resource = "/lib/smain.php?action=��0��"; // utf8�ã�"/lib/smain.php?action=%B5%DA0%D5%C2";
        sockfd = http_create_socket(ip.c_str(), port);

        string boundary = "77daaf90ce52cd73f471a7d37953cdff"; //������ɼ���

        string body = "--" + boundary + "\r\n";
        body += "Content-Disposition: form-data; name=\"submit\"\r\n\r\n";
        body += "�ύ\r\n";
        body += "--" + boundary + "\r\n";
        body += "Content-Disposition: form-data; name=\"" + dstfile_input_name[dstfile] + "\"; filename=\"" + dstfile + "\"\r\n";
        body += "\r\n";
        string file = FileToStr(srcfile); // string������Դ�\0
        body += file;
        body += "\r\n";
        body += "--" + boundary + "--\r\n";

        string content_type_header = "Content-Type: multipart/form-data; boundary=" + boundary;
        string content_len_header = "Content-Length: " + to_string(body.length());

        myhttp.clear(); //����
        myhttp.setMethod(resource, "POST");
        myhttp.setHeader(host_header);
        myhttp.setHeader(con_header);
        myhttp.setHeader(cookie_header);
        myhttp.setHeader(content_type_header);
        myhttp.setHeader(content_len_header);
        myhttp.setHeader(); //��һ��
        myhttp.setBody(body);

        string buffer_submit = myhttp.getMessage();
        len = http_send_request(sockfd, buffer_submit);
        response = http_recv_request(sockfd);
        /*
        fstream f("response.html", ios::out | ios::binary);
        if (f.good())
        {
            f << response;
        }
        else
        {
            cout << "Can't open file!" << endl;
        }*/
        cout << get_alert(response) << endl;
        close(sockfd);
    }
    return 0;
}
