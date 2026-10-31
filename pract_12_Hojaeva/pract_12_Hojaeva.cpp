#include <iostream>
#include <string>
#include <exception>
#include <Windows.h>
#include <cwchar>

using namespace std;
static int sizeList = 0;

struct ProcessInfo {
	string name;
	HANDLE hProcess;
	DWORD dwProcessId;
};

void AddProcess(ProcessInfo*& processes, ProcessInfo& process) {
	if (processes == nullptr) {
		processes = new ProcessInfo[1];
		processes[0] = process;
		sizeList = 1;
	}
	else
	{
		ProcessInfo* newProcesses = new ProcessInfo[sizeList + 1];
		for (int i = 0; i < sizeList; i++) {
			newProcesses[i] = processes[i];
		}
		newProcesses[sizeList] = process;

		delete[] processes;
		processes = newProcesses;
		newProcesses = nullptr;

		sizeList++;
	}
}

void DeleteProcess(ProcessInfo*& processes, int indexProcess) {
	if (processes == nullptr || indexProcess < 0 || indexProcess >= sizeList)
		return;

	if (sizeList == 1) {
		delete[] processes;
		processes = nullptr;
		sizeList = 0;
		return;
	}

	ProcessInfo* newProcesses = new ProcessInfo[sizeList - 1];
	int newIndex = 0;

	for (int i = 0; i < sizeList; i++) {
		if (i == indexProcess) {
			continue;
		}
		newProcesses[newIndex] = processes[i];
		newIndex++;
	}

	delete[] processes;
	processes = newProcesses;

	sizeList--;
}

void CheckProcesses(ProcessInfo*& processes) {
	if (processes != nullptr) {
		for (int i = 0; i < sizeList; i++) {
			DWORD exitCode;
			GetExitCodeProcess(processes[i].hProcess, &exitCode);
			if (exitCode != STILL_ACTIVE) {
				CloseHandle(processes[i].hProcess);
				DeleteProcess(processes, i);
				i--;
			}
		}
	}
}

void ShowProcesses(ProcessInfo*& processes) {
	CheckProcesses(processes);
	for (int i = 0; i < sizeList; i++) {
		cout << "#" << i + 1 << " | " << processes[i].name << " | PID: " << processes[i].dwProcessId << " | HP: " << processes[i].hProcess << endl;
	}
}

void PrintDisclaimer() {
	cout << endl << "---// Дисклеймер" << endl;
	cout << "При вводе параметров для запуска процесса," << endl;
	cout << "данное приложение не несет ответственности за переданные данные." << endl;
	cout << "Если вы введете неверные данные, данная программа не будет их проверять." << endl;
	cout << "Пожалуйста, вводите данные корректно!" << endl << endl;
}

int main()
{
	setlocale(NULL, "Russian");

	cout << endl << "----// Менеджер процессов //----" << endl << endl;

	ProcessInfo* processes = nullptr;
	bool isWorking = true;

	while (isWorking) {
		try {

			cout << endl << "---// Меню " << endl;
			cout << "1. Создание процесса." << endl;
			cout << "2. Список процессов." << endl;
			cout << "3. Завершение процесса." << endl;
			cout << "4. Количество работающих процессов." << endl;
			cout << "5. Выход. " << endl << endl;
			cout << "Ваш выбор:  ";
			string str_choice;
			cin >> str_choice;

			int choice = 0;
			try {
				choice = stoi(str_choice);
			}
			catch (exception) {
				throw invalid_argument("ожидалось числовое значение.");
			}

			cout << endl;

			switch (choice) {
			case 1: {
				cout << "---// Создание процесса" << endl << endl;
				cout << "1. taskmgr.exe" << endl;
				cout << "2. ping.exe" << endl;
				cout << "3. task3.exe" << endl << endl;
				cout << "Выберите программу: ";
				cin >> str_choice;

				int choiceProg = 0;
				try {
					choiceProg = stoi(str_choice);
				}
				catch (exception) {
					throw invalid_argument("ожидалось числовое значение.");
				}

				ProcessInfo process{};
				string progName = "";
				string args = "";

				cout << endl;
				if (choiceProg == 1) {
					progName = "taskmgr.exe";
					process.name = progName;
				}
				else if (choiceProg == 2) {
					cout << "Для запуска процесса необходимо ввести параметры запуска." << endl;
					cout << "Пример ввода параметров: [IP-adress] / -n [N] [IP-adress] / -t [IP-adress]" << endl;
					PrintDisclaimer();
					cout << "Введите параметры: ";
					cin.ignore();
					getline(cin, args);

					progName = "ping.exe";
					process.name = progName;
				}
				else if (choiceProg == 3) {
					cout << "Для запуска процесса необходимо ввести параметры запуска." << endl;
					cout << "Параметры - это два целых положительных числовых значения для определения диапозона," << endl;
					cout << "где одно значение превышает другое." << endl;
					PrintDisclaimer();
					cout << "Введите параметры через пробел: ";
					cin.ignore();
					getline(cin, args);

					progName = "C:\\Users\\st310-06\\Documents\\hojaeva pr-31\\task3\\x64\\Debug\\task3.exe";
					process.name = "task3.exe";
				}
				else {
					cout << "Такой программы нет. Попробуйте еще раз." << endl;
					break;
				}

				string progLine = progName;
				if (!args.empty()) {
					progLine += " ";
					progLine += args;
				}

				wstring prog(progLine.begin(), progLine.end());
				const wchar_t* cmdLine = prog.c_str();

				STARTUPINFO si;
				PROCESS_INFORMATION piApp;

				ZeroMemory(&si, sizeof(STARTUPINFO));
				si.cb = sizeof(STARTUPINFO);

				if (!CreateProcess(NULL, (LPWSTR)cmdLine, NULL, NULL, FALSE,
					CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp)) {
					throw runtime_error("не удалось создать процесс код - " + to_string(GetLastError()) + '.');
				}

				process.hProcess = piApp.hProcess;
				process.dwProcessId = piApp.dwProcessId;
				AddProcess(processes, process);

				CloseHandle(piApp.hThread);

				break;
			}
			case 2: {
				cout << "---// Список процессов" << endl << endl;
				if (processes == nullptr) {
					cout << "Список пуст." << endl;
				}
				else {
					ShowProcesses(processes);
				}

				break;
			}
			case 3: {
				cout << "---// Завершение процесса" << endl << endl;

				if (processes == nullptr) {
					cout << "Нет запущенных процессов." << endl;
				}
				else {
					ShowProcesses(processes);
					cout << endl << "Введите номер процесса для удаления: ";
					
					cin >> str_choice;
					
					int index;
					try {
						index = stoi(str_choice);
					}
					catch (exception) {
						throw invalid_argument("ожидалось числовое значение.");
					}

					cout << endl;
					if (index > 0 && index <= sizeList) {
						index--;

						TerminateProcess(processes[index].hProcess, 0);

						cout << "Процесс успешно завершен." << endl;
					}
					else {
						cout << "Неверный индекс!" << endl;
					}
				}

				break;
			}
			case 4: {
				cout << "---// Количество работающих процессов" << endl << endl;

				cout << "Количество процессов: " << sizeList << endl;

				break;
			}
			case 5: {
				cout << "---// Выход из программы." << endl;
				isWorking = false;
				break;
			}
			default: {
				cout << "Неверный пункт меню. Попробуйте еще раз." << endl;
				break;
			}
			}
		}
		catch (exception& e) {
			cout << endl << "Ошибка: " << e.what() << endl << endl;
		}

		CheckProcesses(processes);
	}
	cout << endl << "Завершение работы программы." << endl;

	delete[] processes;

	return 0;
}