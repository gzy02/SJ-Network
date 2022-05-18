#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <regex>
#include <vector>
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
        cout << "Can't open file!srcfile name error!" << endl;
        exit(EXIT_FAILURE);
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
string UrlEncode(string s)
{
    char need[6];
    string temp;
    string changelist = "+/%#&?= ";
    string::iterator it;
    for (size_t i = 0; i < s.length(); i++)
    {
        for (size_t j = 0; j < changelist.length(); j++)
        {
            if (s[i] == changelist[j])
            {
                sprintf(need, "%%%X", int(s[i]));
                temp = need;
                it = s.begin() + i;
                s.erase(it);
                s.insert(s.begin() + i, temp.begin(), temp.end());
                i += temp.length() - 1;
            }
        }
    }
    return s;
}
void search_to_submit(string url, string html, map<string, pair<string, string>> &dstfile_input_name)
{
    string pattern("<tr>.*?<input type=\"file\".*?name=\"(.*?)\".*?></tr>"), subpattern("<td>(.*?)</td>");
    regex r(pattern), sub(subpattern);
    string temp, name, upfile;
    for (sregex_iterator it(html.begin(), html.end(), r), end_it; it != end_it; ++it)
    {
        int num = 0;
        temp = it->str();
        upfile = it->str(1);
        for (sregex_iterator sub_it(temp.begin(), temp.end(), sub), e_it; sub_it != e_it; ++sub_it)
        {
            num++;
            if (num == 2)
            {
                name = sub_it->str(1);
                dstfile_input_name[name] = make_pair(url, upfile);
            }
        }
    }
}
void findLeft_to_jump(string html, vector<string> &urls)
{
    string pattern("<left>(.*?)</left>"), subpattern("<a href=(.*?)>(.*?)</a>");
    regex r(pattern), sub(subpattern);
    smatch results;
    string temp, url;
    size_t pos;
    string substr = "smain.php";
    for (sregex_iterator it(html.begin(), html.end(), r), end_it; it != end_it; ++it)
    {
        temp = it->str(1);
        for (sregex_iterator sub_it(temp.begin(), temp.end(), sub), e_it; sub_it != e_it; ++sub_it)
        {
            if (sub_it->str(2) != string("退出"))
            {
                url = sub_it->str(1);

                if ((pos = url.find(substr)) != string::npos)
                {
                    pos += substr.length();
                    string res = url.substr(pos); //尾部
                    res.erase(res.end() - 1);
                    urls.push_back(res);
                }
            }
        }
    }
}
