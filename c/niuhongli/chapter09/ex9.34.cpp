#include<iostream>
#include<string>
#include<cctype>
using namespace std;

int main()
{
	string s;
	cout << "Please enter string: " << endl;
	getline(cin,s);
	for(string::iterator iter=s.begin();iter!=s.end();++iter)
		*iter=toupper(*iter);
	cout << "The changed string: " << endl;
	for(string::iterator iter=s.begin();iter!=s.end();++iter)
		cout << *iter;
	return 0;
}
    	
