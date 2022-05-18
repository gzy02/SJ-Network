#include "HttpArg.h"
#include "HttpRequest.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <unistd.h>
using namespace std;

int http_create_socket(const char *ip, int port);
string http_recv_request(int sockfd);
int http_send_request(int sockfd, string buffer);

string get_cookie(const string resp);
string get_alert(const string resp);
string FileToStr(string filepath);
string MapToString(map<string, string> data);
string UrlEncode(string s);
void search_to_submit(string url, string html, map<string, pair<string, string>> &dstfile_input_name);
void findLeft_to_jump(string html, vector<string> &urls);

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
        login_data["password"] = UrlEncode(password);
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
    {
        sockfd = http_create_socket(ip.c_str(), port);
        resource = "/lib/smain.php"; // refresh������
        myhttp.clear();
        myhttp.setMethod(resource, "GET");
        myhttp.setHeader(host_header);
        myhttp.setHeader(con_header);
        myhttp.setHeader(cookie_header);
        myhttp.setHeader(); //��һ��

        string buffer_get = myhttp.getMessage();
        len = http_send_request(sockfd, buffer_get);
        response = http_recv_request(sockfd);
        string alert = get_alert(response);
        if (alert.length() == 0)
            cout << "��ȡ��ҳ��Ϣ�ɹ�\n";
        else
        {
            cout << alert << endl;
            exit(EXIT_FAILURE);
        }
        close(sockfd);
    }

    //����Ϊ�������в������˳��Ĳ������ҳ��Ѱ�ҿ��ύ���ļ�

    //���ȸ��ݱ��ĵ�refreshѰ�ҵ���һ����תҳ
    //Ȼ��ͨ��get�����ȡ��Ӧ
    //������Ӧƥ�����г��ǳ�����Ĳ����
    //Ȼ�������Щ����Ѱ����ύ���ļ��Լ���Ӧ��url��upfile_name

    vector<string> urls;
    findLeft_to_jump(response, urls);                     //�ų��˵ǳ�url
    map<string, pair<string, string>> dstfile_input_name; // dstfile����վ��input���nameһһ��Ӧ
    for (int i = 0; i < urls.size(); ++i)
    {
        sockfd = http_create_socket(ip.c_str(), port);
        resource = "/lib/smain.php" + urls[i];
        cout << "���ڷ�����ҳ��Դ��" << resource << endl;
        myhttp.clear();
        myhttp.setMethod(resource, "GET");
        myhttp.setHeader(host_header);
        myhttp.setHeader(con_header);
        myhttp.setHeader(cookie_header);
        myhttp.setHeader(); //��һ��

        string buffer_get = myhttp.getMessage();
        len = http_send_request(sockfd, buffer_get);
        response = http_recv_request(sockfd);
        search_to_submit(urls[i], response, dstfile_input_name);
        close(sockfd);
    }

    map<string, pair<string, string>>::iterator iter;
    cout << "�������в������Դ��ɣ��ҵ��ѷ�����ҵ���£�" << endl;
    for (iter = dstfile_input_name.begin(); iter != dstfile_input_name.end(); ++iter)
    {
        cout << iter->first << "\t" << iter->second.second << endl;
    }

    if (dstfile_input_name.count(dstfile) == 0)
    {
        cerr << "dstfile name error!\n";
        exit(EXIT_FAILURE);
    }

    //����Ϊ�ύ
    {
        resource = "/lib/smain.php" + dstfile_input_name[dstfile].first;
        sockfd = http_create_socket(ip.c_str(), port);

        string boundary = "77daaf90ce52cd73f471a7d37953cdff"; //������ɼ���

        string body = "--" + boundary + "\r\n";
        body += "Content-Disposition: form-data; name=\"submit\"\r\n\r\n";
        body += "�ύ\r\n";
        body += "--" + boundary + "\r\n";
        body += "Content-Disposition: form-data; name=\"" + dstfile_input_name[dstfile].second + "\"; filename=\"" + dstfile + "\"\r\n";
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

        cout << get_alert(response) << endl;
        close(sockfd);
    }
    return 0;
}
