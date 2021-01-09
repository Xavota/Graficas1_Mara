#include "Arreglo.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
	Arreglo arr(sizeof(int), 5, 5);

	arr.setVal((void***)new int** [5]{  new int* [5]{new int(1), new int(2), new int(3), new int(4), new int(5)},
										new int* [5]{new int(6), new int(7), new int(8), new int(9), new int(10)},
										new int* [5]{new int(11),new int(12),new int(13),new int(14),new int(15)},
										new int* [5]{new int(16),new int(17),new int(18),new int(19),new int(20)},
										new int* [5]{new int(21),new int(22),new int(23),new int(24),new int(25)} });

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cout << *(int*)arr.getVal(i, j) << " ";
		}
		cout << endl;
	}

	system("pause");

	return 0;
}