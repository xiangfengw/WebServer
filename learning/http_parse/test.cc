#include <string.h>
#include <iostream>

using namespace std;

int main() {
    char buffer[30] = "GET   ./index         HTTP1.0";
    char *szTemp = buffer;
    char *szURL = strpbrk(szTemp, " \t");     // 找请求行中的空格和缩进
    if (!szURL)
    {
        return 0;
    }
    *szURL = '\0';
    szURL++;
    // cout << szURL << endl;
    // cout << szTemp << endl;

    // char a[] = "adacccc";
    // char b[] = "ac";
    // cout << strspn(a, b) << endl;
    
    char a[] = "abc\0defg";
    char b[] = "abc";
    char c[] = "abcdefg";
    cout << (int)strcasecmp(a, b) << endl;
    cout << (int)strncasecmp(c, b, 3) << endl;

    szURL += strspn(szURL, " \t");

    char *szVersion = strpbrk(szURL, " \t");

    return 0;
}