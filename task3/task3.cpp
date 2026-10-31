#include <iostream>
#include <conio.h>
#include <string>
#include <exception>
#include <Windows.h>

using namespace std;

int SumDitgits(int num) {
	int sum = 0;
	while (num > 0) {
		sum += num % 10;
		num /= 10;
	}
	return sum;
}

int main(int argc, char *argv[])
{
	setlocale(NULL, "Russian");

	try {

		Sleep(1000);

		cout << endl << endl << "---// Расчет суммы цифр чисел в диапозоне //---" << endl << endl;

		Sleep(1000);

		if (argc < 3) {
			throw invalid_argument("не переданы параметры диапазона чисел.");
		}

		string str_num1 = argv[1];
		string str_num2 = argv[2];

		int num1, num2;
		try {
			num1 = stoi(str_num1);
			num2 = stoi(str_num2);
		}
		catch (exception) {
			throw invalid_argument("не удалось перевести подструки в параметрах в целые числа.");
		}

		if (num1 < 0 || num2 < 0) {
			throw invalid_argument("параметры должны быть больше нуля.");
		}

		if (num1 > num2) {
			int temp = num1;
			num1 = num2;
			num2 = temp;
		}

		cout << "---// Суммы цифр в диапозоне от " << num1 << " до " << num2 << ":" << endl;

		Sleep(1000);

		int numMax = 0;
		for (int i = num1; i <= num2; i++) {
			int sum = SumDitgits(i);
			cout << "| cумма цифр " << i << " - " << sum << endl;
			if (sum > numMax)
				numMax = sum;

			Sleep(100);
		}
		cout << endl;

		Sleep(1000);

		cout << "Максимальная сумма цифр: " << numMax << endl;

	}
	catch (exception& e) {
		cout << endl << "Ошибка: " << e.what() << endl;
	}

	Sleep(1000);

	cout << endl << "Нажмите на клавишу для завершение процесса..." << endl;
	_getch();

	return 0;
}