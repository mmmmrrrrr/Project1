#include <bits/stdc++.h>
using namespace std;
struct X{
    int a,b;
};
vector<X>a;
int main()
{
    ios::sync_with_stdio(false);
    a.push_back({0,0});
    a.back().a=1;
    cout<<a.back().a<<endl;
    return 0;
}
/*
			//------
			res=codeStack.back();
			codeStack.pop_back();
			res=codeStack.back()+res;
			codeStack.pop_back();
			codeStack.push_back(res);
			//------
*/
