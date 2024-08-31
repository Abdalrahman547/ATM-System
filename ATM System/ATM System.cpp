#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

const string ClientsFileName = "Clients.txt";

void ShowMainMenueScreen();
void ShowQuickWithdrawScreen();
void ShowNormalWithdrawScreen();
void ShowDepositScreen();
void Login();

struct stClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
};

enum enMainMenueOptions
{
	eQuickWithdraw = 1, eNormalWithdraw = 2,
	eDeposit = 3, eCheckBalance = 4, eExit = 5
};

stClient CurrentClient;

short ReadMainMenueOptions()
{
	short Op;
	do
	{
		cout << "\nChoose what do you want to do? [1 to 5]? ";
		cin >> Op;
	} while (Op < 1 || Op > 5);

	return Op;
}

vector<string> SplitString(string S1, string delim = "#//#")
{
	short pos = 0;
	string word;

	vector<string> vString;

	while ((pos = S1.find(delim)) != string::npos)
	{
		word = S1.substr(0, pos);

		if (word != "")
			vString.push_back(word);

		S1.erase(0, pos + delim.length());
	}

	if (S1 != "")
		vString.push_back(S1);

	return vString;
}

stClient ConvertLineToRecord(string DataLine)
{
	vector<string> vClientDate = SplitString(DataLine);

	stClient Client;

	Client.AccountNumber = vClientDate[0];
	Client.PinCode = vClientDate[1];
	Client.Name = vClientDate[2];
	Client.Phone = vClientDate[3];
	Client.AccountBalance = stod(vClientDate[4]);

	return Client;

}

string ConvertRecordToLine(stClient Client)
{
	string delim = "#//#";

	string Line = "";

	Line += Client.AccountNumber + delim;
	Line += Client.PinCode + delim;
	Line += Client.Name + delim;
	Line += Client.Phone + delim;
	Line += to_string(Client.AccountBalance);

	return Line;
}

vector<stClient> LoadClientsDataFromFile(string FileName)
{
	vector<stClient> vClients;

	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);

			vClients.push_back(Client);
		}

		MyFile.close();
	}

	return vClients;
}

vector<stClient> SaveClientsDataToFile(string FileName, vector<stClient> vClients)
{
	fstream MyFile;
	string Line;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		for (auto C : vClients)
		{
			Line = ConvertRecordToLine(C);

			MyFile << Line << endl;
		}

		MyFile.close();
	}

	return vClients;
}

bool FindCLientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient & Client)
{
	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	for (auto C : vClients)
	{
		if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
		{
			Client = C;
			return true;
		}

	}
	return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
	if (FindCLientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
		return true;
	else
		return false;
}

int getQuickWithdrawAmmount(short QuickWithdrawOption)
{
	switch (QuickWithdrawOption)
	{
	case 1:
		return 20;
	case 2:
		return 50;
	case 3:
		return 100;
	case 4:
		return 200;
	case 5:
		return 400;
	case 6:
		return 600;
	case 7:
		return 800;
	case 8:
			return 1000;
	default:
		return 0;
	}
}

short ReadQuickWithdrawOption()
{
	short choise;

	do
	{
		cout << "\nChoose what do you want to do? [1 to 9]? ";
		cin >> choise;
	} while (choise < 1 && choise > 9);

	return choise;
}

int ReadWithdrawAmount()
{
	int Amount;

	do
	{
		cout << "\nEnter an amount multiple of 5's ? ";

		cin >> Amount;

	} while (Amount % 5 != 0);

	return Amount;

}

int ReadDepositAmount()
{
	int Amount;

	do
	{
		cout << "\nEnter a positive amount? ";

		cin >> Amount;

	} while (Amount <= 0);

	return Amount;

}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<stClient> vClients)
{
	char Answer = 'N';

	cout << "\nAre you sure you want preform this transaction? Y/N ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{
		for (auto& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientsDataToFile(ClientsFileName, vClients);
				cout << "\nDone successfully, new balance = " << C.AccountBalance;
				return true;
			}
		}

	}

	return false;
}

void PerformQuickWithdrawOption(short QuickWithdrawOption)
{
	if (QuickWithdrawOption == 9) // Exit
		return;

	short WithdrawBalance = getQuickWithdrawAmmount(QuickWithdrawOption);

	if (WithdrawBalance > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeded your balance, make another choise.\n";
		cout << "\nPress any key to continue...";
		system("cls");
		ShowQuickWithdrawScreen();
	}

	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawBalance * -1, vClients);

	CurrentClient.AccountBalance -= WithdrawBalance;

}

void PerformNormalWithdrawOption()
{
	int WithdrawBalance = ReadWithdrawAmount();

	if (WithdrawBalance > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeded your balance, make another choise.\n";
		cout << "\nPress any key to continue...";
		system("cls");
		ShowQuickWithdrawScreen();
	}

	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawBalance * -1, vClients);

	CurrentClient.AccountBalance -= WithdrawBalance;

}

void PerformDepositOption()
{
	int DepositBalance = ReadDepositAmount();

	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositBalance, vClients);

	CurrentClient.AccountBalance += DepositBalance;

}

void ShowQuickWithdrawScreen()
{
	system("cls");

	cout << "\n=====================================================\n";
	cout << "\n\t\tQuick Withdraw Screen\n";
	cout << "\n=====================================================\n";
	cout << "\t[1] 20\t\t[2] 50 \n";
	cout << "\t[3] 100\t\t[4] 200 \n";
	cout << "\t[5] 400\t\t[6] 600 \n";
	cout << "\t[7] 800\t\t[8] 1000 \n";
	cout << "\t[9] Exit\n";
	cout << "\n=====================================================\n";
	cout << "\nYour Balance is " << CurrentClient.AccountBalance << "\n";


	PerformQuickWithdrawOption(ReadQuickWithdrawOption());
}

void ShowNormalWithdrawScreen()
{
	cout << "\n=====================================================\n";
	cout << "\n\t\tNormal Withdraw Screen\n";
	cout << "\n=====================================================\n";

	PerformNormalWithdrawOption();
}

void ShowDepositScreen()
{
	cout << "\n=====================================================\n";
	cout << "\n\t\tNormal Withdraw Screen\n";
	cout << "\n=====================================================\n";
	PerformDepositOption();
}

void ShowCheckBalanceScreen()
{
	system("cls");

	cout << "\n=====================================================\n";
	cout << "\n\t\tCheck Balance Screen\n";
	cout << "\n=====================================================\n";

	cout << "\nYour Balance is " << CurrentClient.AccountBalance << "\n";
}

void GoBackToMainMenue()
{
	cout << "\nPress any key to go to back to Main Menue...";
	system("pause>0");

	ShowMainMenueScreen();
}

void PerformMainMenueOption(enMainMenueOptions Option)
{
	switch (Option)
	{
	case eQuickWithdraw:
		system("cls");
		ShowQuickWithdrawScreen();
		GoBackToMainMenue();
		break;

	case eNormalWithdraw:
		system("cls");
		ShowNormalWithdrawScreen();
		GoBackToMainMenue();
		break;

	case eDeposit:
		system("cls");
		ShowDepositScreen();
		GoBackToMainMenue();
		break;

	case eCheckBalance:
		system("cls");
		ShowCheckBalanceScreen();
		GoBackToMainMenue();
		break;

	case eExit:
		system("cls");
		Login();
		break;
	}
}

void ShowMainMenueScreen()
{
	system("cls");

	cout << "\n=====================================================\n";
	cout << "\n\t\tATM Main Menue Screen\n";
	cout << "\n=====================================================\n";
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit.\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Logout.";
	cout << "\n=====================================================\n";

	PerformMainMenueOption((enMainMenueOptions)ReadMainMenueOptions());
}

void Login()
{
	bool LoginFaild = false;
	string AccountNumber, PindCode;

	do
	{
		system("cls");

		cout << "\n________________________________\n";
		cout << "\tLogin Screen";
		cout << "\n________________________________\n";

		if (LoginFaild)
			cout << "Invalid Account Number/PinCode!";

		cout << "\nEnter Account Number? ";
		cin >> AccountNumber;

		cout << "\nEnter PinCode? ";
		cin >> PindCode;

		LoginFaild = !LoadClientInfo(AccountNumber, PindCode);

	} while (LoginFaild);

	ShowMainMenueScreen();
}

int main()
{
	Login();

	system("pause>0");
	return 0;
}