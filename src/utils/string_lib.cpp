#include <vector>
#include <string>
#include <iostream>
#include <utils/string_lib.h>
using namespace std;
//using namespace wh::utils;  //这里还是要指定命名空间
vector<string> wh::utils::split(const string &s, const string &seperator)
{
    vector<string> result;
    typedef string::size_type stringSize;
    stringSize i = 0;

    while (i != s.size())
    {
        //找到字符串中首个不等于分隔符的字母；
        int flag = 0;
        while (i != s.size() && flag == 0)
        {
            flag = 1;
            for (stringSize x = 0; x < seperator.size(); ++x)
                if (s[i] == seperator[x])
                {
                    ++i;
                    flag = 0;
                    break;
                }
        }

        //找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        stringSize j = i;
        while (j != s.size() && flag == 0)
        {
            for (stringSize x = 0; x < seperator.size(); ++x)
                if (s[j] == seperator[x])
                {
                    flag = 1;
                    break;
                }
            if (flag == 0)
                ++j;
        }
        if (i != j)
        {
            result.push_back(s.substr(i, j - i));
            i = j;
        }
    }
    return result;
}
