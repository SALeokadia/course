#include <iostream>
#include "Stack.h"

using namespace std;

int main()
{
	UINT nCount = 5;
	UINT Toplimit = 1;
	while(Toplimit <= nCount)
	{
		CStack istk;
		for(UINT i = 0; i < Toplimit; ++i)
		{
			istk.Push(20 + i * 2);
		}

		istk.Print();
		cout << "Current stack length is " << istk.GetLength() << endl;
		cout << "The stack Top data is " << istk.GetTop()->data << endl;
		istk.Pop();
		cout << "After pop stack, the rest of stack " << endl;
		istk.Print();
		cout << endl;
		++Toplimit;
	}
	return 0;
}