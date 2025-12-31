#include <iostream>
#include <string>
using namespace std;

int main()
{
    string s1,s2;
    getline(cin,s1);
    getline(cin,s2);
    string s="";
    for(int i=0;i<s2.size()-1;i++)
    {
        s+=s2[i];
    }
    for(int i=0;i<s1.size()-1;i++)
    {
        s+=s1[i];
    }
    cout<<s;
}