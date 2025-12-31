#include <iostream>
#include <string>

using namespace std;

int main()
{
    string s;
    getline(cin,s);
    bool flag=true;
    int n=s.size()-1;
    for (size_t i = 0; i < n/2; i++)
    {
        if (s[i]!=s[n-1-i])
        {
            flag=false;
        }
    }

    if (flag==true)
    {
        cout<<n;
    }
    else cout<<"no";
    
}