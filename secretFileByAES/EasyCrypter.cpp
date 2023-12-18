#include "myencryption.h"

using namespace std;

void welcome()
{
	cout << " ______                 _____                  _               \n"
			"|  ____|               / ____|                | |              \n"
			"| |__   __ _ ___ _   _| |     _ __ _   _ _ __ | |_ ___ _ __    \n"
			"|  __| / _` / __| | | | |    | '__| | | | '_ \\| __/ _ \\ '__| \n"
			"| |___| (_| \\__ \\ |_| | |____| |  | |_| | |_) | ||  __/ |    \n"
			"|______\\__,_|___/\\__, |\\_____|_|   \\__, | .__/ \\__\\___|_|\n"
			"                  __/ |             __/ | |                    \n"
			"                 |___/             |___/|_|                    \n";
	cout << "		 _             ____  _ _            \n"
			"		| |           |___ \\| (_)          \n"
			"		| |__  _   _    __) | |_ ______ _   \n"
			"		| '_ \\| | | |  |__ <| | |_  / _` | \n"
			"		| |_) | |_| |  ___) | | |/ / (_| |  \n"
			"		|_.__/ \\__, | |____/|_|_/___\\__,_|\n"
			"		        __/ |                       \n"
			"		       |___/                        \n";
	return;
}

void fileprocess()
{
	string filename;
	char mode;
	getline(cin, filename);
	cin >> mode;

	if (mode == 'e')
	{
		EN_FILE(filename);
		cout << "ENCRYPTION DONE !" << endl;
	}

	else
	{
		DE_FILE(filename);
		cout << "ENCRYPTION DONE !" << endl;
	}
}

int main()
{
	OpenSSL_add_all_algorithms();// random seed initialization
	getKey();
	welcome();
	fileprocess();

	return 0;
}