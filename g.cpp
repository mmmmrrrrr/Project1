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
			//------
			//------
