#include "Arreglo.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
	Arreglo arr(sizeof(int), 5, 5);

	arr.setVal((void***)new char** [5]{  new char* [5]{new char('a'),new char('b'),new char('c'),new char('d'),new char('e')},
										 new char* [5]{new char('f'),new char('g'),new char('h'),new char('i'),new char('j')},
										 new char* [5]{new char('k'),new char('l'),new char('m'),new char('n'),new char('o')},
										 new char* [5]{new char('p'),new char('q'),new char('r'),new char('s'),new char('t')},
										 new char* [5]{new char('u'),new char('v'),new char('w'),new char('x'),new char('y')} });

	arr.setVal((void*)new char('º'), 0, 0);
	arr.setVal((void*)new int('+'), 1, 0);
	arr.setVal((void*)new int('-'), 4, 3);

	for (int i = 0; i < arr.getY(); i++)
	{
		for (int j = 0; j < arr.getX(); j++)
		{
			cout << *(char*)arr.getVal(i, j) << " ";
		}
		cout << endl;
	}

	arr.setX(10);
	arr.setY(20);
	arr.setValueSize(sizeof(int));

	int a = 0;

	for (int i = 0; i < arr.getY(); i++)
	{
		for (int j = 0; j < arr.getX(); j++)
		{
			arr.setVal(new int(a++), i, j);
		}
	}

	for (int i = 0; i < arr.getY(); i++)
	{
		for (int j = 0; j < arr.getX(); j++)
		{
			cout << *(int*)arr.getVal(i, j) << " ";
		}
		cout << endl;
	}

	struct temp 
	{
		int i;
		char c;
		float f;
	};

	arr.setX(2);
	arr.setY(2);
	arr.setValueSize(sizeof(temp));

	arr.setVal((void***)new temp** [2]{ new temp * [2]{new temp{10, 'j', 1.5f}, new temp{25, '*', 2.26f}},
										new temp * [2]{new temp{57, '?', 4.117f}, new temp{1192, '=', 0.01f}} });

	for (int i = 0; i < arr.getY(); i++)
	{
		for (int j = 0; j < arr.getX(); j++)
		{
			cout << (*(temp*)arr.getVal(i, j)).i << " " << (*(temp*)arr.getVal(i, j)).c << " " << (*(temp*)arr.getVal(i, j)).f << endl;
		}
		cout << endl;
	}

	system("pause");

	return 0;
}