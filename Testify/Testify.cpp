#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(void)
{
	int* p = new int(100);
	cout << p << "�ϰ� " << *p << endl;
	long c = reinterpret_cast<long> (p);
	cout << c << "�ϰ�" << endl;


	return 0;
}