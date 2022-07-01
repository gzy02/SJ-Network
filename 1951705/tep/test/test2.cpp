#include <iostream>
#include <map>
#include <string>
using namespace std;
int main()
{
    string test_str = "&空你好+";
    string cmd = "echo -n \"" + test_str + "\" |tr -d \'\\n\' |od -An -tx1|tr \' \' %";
    cout << cmd << endl;
    string target;
    FILE *fp = popen(cmd.c_str(), "r");
    if (fp != NULL)
    {
        char buf[1024];
        target = fgets(buf, 1024, fp);
        // cout << target.length() << endl;
        target = target.substr(0, target.length() - 1); //回车不要
        pclose(fp);
        cout << target << endl;
    }
    target = "filename*=UTF-8\'\'%E6%95%B0%E6%8D%AE%E5%BA%93.png";
    string cmd2 = "printf $(echo -n \"" + target + "\" | sed \'s/\\\\/\\\\\\\\/g;s/\\(%\\)\\([0-9a-fA-F][0-9a-fA-F]\\)/\\\\x\\2/g\')";
    cout << cmd2 << endl;
    fp = popen(cmd2.c_str(), "r");
    if (fp != NULL)
    {
        char buf[1024];
        target = fgets(buf, 1024, fp);
        pclose(fp);
        cout << target << endl;
    }

    return 0;
}