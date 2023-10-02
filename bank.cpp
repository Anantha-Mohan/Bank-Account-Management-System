#include<iostream>
#include<fstream>
#include<stdexcept>
#include<vector>

using namespace std;

class Account{
	public:
	Account(int accno,string accholder,double balance):
		accno_(accno),accholder_(accholder),balance_(balance){}
	virtual void withdraw(double amount)
	{
		if(balance_>=amount)
			balance_-=amount;
		else
			throw runtime_error("Insufficient Balance\n");
	}
	virtual void deposit(double amount)
	{
		balance_+=amount;
	}
	virtual void display()
	{
		cout<<"Account Number:"<<accno_<<endl;
		cout<<"Account Holder:"<<accholder_<<endl;
		cout<<"Balance Amount:"<<balance_<<endl<<endl;
	}
	virtual ~Account(){}
	
	virtual char typeGetter()=0;
	
	friend void saveAccountData(const vector<Account*>& accounts);
	friend void loadAccountData(const vector<Account*>& accounts);
	
	int accNoGetter()
	{
		return accno_;
	}
	
	protected:
	int accno_;
	string accholder_;
	double balance_;
	char accType_;
};

class SavingsAccount:public Account{
	public:
	SavingsAccount(int accno,string accholder,double balance,double interestRate):
	Account(accno,accholder,balance),interestRate_(interestRate){}
	void applyInterest()
	{
		balance_=balance_+(balance_*interestRate_/100);
	}
	void display()
	{
		cout<<"******Savings Account******"<<endl;
		Account::display();
	}
	char typeGetter() override
	{
		return 's';
	}
	
	friend void saveAccountData(const vector<Account*>& accounts);
	friend void loadAccountData(const vector<Account*>& accounts);
	
	private:
	double interestRate_;
};

class CheckingAccount:public Account{
	public:
	CheckingAccount(int accno,string accholder,double balance,double overDraft):
	Account(accno,accholder,balance),overDraft_(overDraft){}
	void withdraw(double amount)
	{
		if(amount<=overDraft_+balance_)
		{
			balance_-=amount;
		}
		else
		{
			throw runtime_error("Over Draft Limit Exceeded\n");
		}
	}
	void display()
	{
		cout<<"******Checking Account******"<<endl;
		Account::display();
	}
	char typeGetter() override
	{
		return 'c';
	}
	
	friend void saveAccountData(const vector<Account*>& accounts);
	friend void loadAccountData(const vector<Account*>& accounts);
	
	private:
	double overDraft_;
};

void saveAccountData(const vector<Account*>& accounts)
{
	ofstream ofile("accounts.txt",ios::app);
	if(!ofile)
	{
		cerr<<"File Cannot be Opened"<<endl;
	}
	for(Account* account:accounts)
		{
			if(account->typeGetter()=='s')
			{
				SavingsAccount* save=dynamic_cast<SavingsAccount*>(account);
				ofile
				<<account->accno_<<" "
				<<account->accholder_<<" "
				<<account->balance_<<" "
				<<save->typeGetter()<<" "
				<<save->interestRate_
				<<endl;
			}
			else if(account->typeGetter()=='c')
			{
			CheckingAccount* check=dynamic_cast<CheckingAccount*>(account);
				ofile
				<<account->accno_<<" "
				<<account->accholder_<<" "
				<<account->balance_<<" "
				<<check->typeGetter()<<" "
				<<check->overDraft_
				<<endl;
			}
		}
	ofile.close();
}

void loadAccountData(vector<Account*>& accounts)
{
	ifstream ifile("accounts.txt");
	if(!ifile)
	{
		cerr<<"File Cannot be Opened"<<endl;
	}
	else
	{
		int accno;
		string accholder;
		double balance;
		char type;
		double var;
		while(ifile>>accno>>accholder>>balance>>type>>var)
		{
			if(type=='c')
			{
				accounts.push_back(new CheckingAccount(accno,accholder,balance,var));
			}
			else if(type=='s')
			{
				accounts.push_back(new SavingsAccount(accno,accholder,balance,var));
			}
		}
	}
	ifile.close();
}

int main()
{
	vector<Account*> accounts;
	loadAccountData(accounts);
	
	while(true)
	{
		cout<<"\n\n************ Banking System Menu ************"<<endl;
		cout << "1. Create Savings Account" << endl;
		cout << "2. Create Checking Account" << endl;
		cout << "3. Deposit" << endl;
		cout << "4. Withdraw" << endl;
		cout << "5. Display Account Details" << endl;
		cout << "6. Apply Interest (Savings Accounts)" << endl;
		cout << "7. Save Account Data" << endl;
		cout << "8. Exit" << endl;
		cout << "Enter your choice: ";
		
		int choice;
		cin >> choice;
		int accountNumber;
		double balance;
		string accountholder;
		double interest;
		double overdraft;
		int amount;
		switch(choice)
		{
			case 1:
			{
				cout<<"\n\n*****Savings Accounts Creation Menu*****\n\n";
				cout << "Enter Account Number: ";
				cin >> accountNumber;
				cout << "Enter Account Holder's Name: ";
				cin.ignore();
				getline(cin, accountholder);
				cout << "Enter Initial Balance: ";
				cin >> balance;
				cout << "Enter Interest Rate (for Savings Accounts): ";
		        cin >> interest;
		        accounts.push_back(new SavingsAccount(accountNumber,accountholder,balance,interest));
		        cout<<"        ***Savings Account Created***"<<endl;
				break;
			}
			case 2:
			{
				cout<<"\n\n*****Checking Accounts Creation Menu*****\n\n";
					cout << "Enter Account Number: ";
					cin >> accountNumber;
					cout << "Enter Account Holder's Name: ";
					cin.ignore();
					getline(cin, accountholder);
					cout << "Enter Initial Balance: ";
					cin >> balance;
					cout << "Enter OverDraft Limit: ";
				cin >>overdraft ;
				accounts.push_back(new CheckingAccount(accountNumber,accountholder,balance,overdraft));
				cout<<"        ***Checking Account Created***"<<endl;
				break;
			}
			case 3:
			{
				cout<<"\n\n*****Desposit Menu*****\n\n";
				cout << "Enter Account Number: ";
				cin >> accountNumber;
				cout << "Enter Deposit Amount: ";
				cin >> amount;
				
				for(Account* account:accounts)
				{
					if(account->accNoGetter()==accountNumber)
					{
						account->deposit(amount);
						cout<<"***Deposit Completed***"<<endl;
						break;
					}
				}
				break;
			}
			case 4:
			{
				cout<<"\n\n*****Withdrawal Menu*****\n\n";
				cout << "Enter Account Number: ";
				cin >> accountNumber;
				cout << "Enter Withdrawal Amount: ";
				cin >> amount;
				
				for(Account* account:accounts)
				{
					if(account->accNoGetter()==accountNumber)
					{
						try
						{
							account->withdraw(amount);
							cout<<"***Withdrawal Completed***"<<endl;
						}
						catch(runtime_error& e)
						{
							cerr<<"Error :"<<e.what()<<endl;
						}
						break;
					}
				}
				break;
			}
			case 5:
			{
				cout << "\n\nEnter Account Number: ";
				cin >> accountNumber;
				
				for(Account* account:accounts)
				{
					if(account->accNoGetter()==accountNumber)
					{
						account->display();
						break;
					}
				}
				break;
			}
			case 6:
			{
				cout << "Enter Account Number for Interest Application (Savings Account): ";
                		cin >> accountNumber;
                		for(Account* account:accounts)
				{
					if(account->accNoGetter()==accountNumber&&account->typeGetter()=='s')
					{
						SavingsAccount* save = dynamic_cast<SavingsAccount*>(account);
						save->applyInterest();
						cout<<"Interest Applied on Savings Account\n"<<endl;
					}
				}
			break;
			}
			case 7:
			{
				saveAccountData(accounts);
				cout<<endl<<"Account Data Saved to File\n"<<endl;
				break;
			}
			case 8:
			{
				for(Account* account:accounts)
				{
					delete account;
				}
				return 0;
			}
			default:
				cout<<endl<<"Invalid Choice"<<endl;
				break;
		}
	}
	return 0;
}

