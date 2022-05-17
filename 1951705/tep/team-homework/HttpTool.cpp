#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
using namespace std;

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
