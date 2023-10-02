#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

// Define the base Account class
class Account {
public:
    Account(int accountNumber, string accountHolder, double balance)
        : accountNumber_(accountNumber), accountHolder_(accountHolder), balance_(balance) {}

    virtual void deposit(double amount) {
        balance_ += amount;
    }

    virtual void withdraw(double amount) {
        if (amount <= balance_) {
            balance_ -= amount;
        } else {
            throw std::runtime_error("Insufficient funds.");
        }
    }

    virtual void display() {
        cout << "Account Number: " << accountNumber_ << endl;
        cout << "Account Holder: " << accountHolder_ << endl;
        cout << "Balance: " << balance_ << endl;
    }

    virtual ~Account() {}

    // Getter methods for protected members
    int getAccountNumber() const {
        return accountNumber_;
    }

    string getAccountHolder() const {
        return accountHolder_;
    }

    double getBalance() const {
        return balance_;
    }

protected:
    int accountNumber_;
    string accountHolder_;
    double balance_;
};

// Define derived classes for specific account types
class SavingsAccount : public Account {
public:
    SavingsAccount(int accountNumber, string accountHolder, double balance, double interestRate)
        : Account(accountNumber, accountHolder, balance), interestRate_(interestRate) {}

    void applyInterest() {
        double currentBalance = getBalance();
        currentBalance += currentBalance * interestRate_;
        balance_ = currentBalance; // Update the balance directly
    }
c

private:
    double interestRate_;
};

class CheckingAccount : public Account {
public:
    CheckingAccount(int accountNumber, string accountHolder, double balance, double overdraftLimit)
        : Account(accountNumber, accountHolder, balance), overdraftLimit_(overdraftLimit) {}

    void withdraw(double amount) {
        double currentBalance = getBalance();
        if (amount <= currentBalance + overdraftLimit_) {
            currentBalance -= amount;
            balance_ = currentBalance; // Update the balance directly
        } else {
            throw std::runtime_error("Overdraft limit exceeded.");
        }
    }

    void display() {
        cout << "Checking Account" << endl;
        Account::display();
    }

private:
    double overdraftLimit_;
};

// Function to save account data to a file
void saveAccountData(const vector<Account*>& accounts) {
    ofstream outputFile("accounts.txt");
    if (!outputFile) {
        cerr << "Error: Cannot open the data file for writing." << endl;
        return;
    }

    for (const Account* account : accounts) {
        outputFile << account->getAccountNumber() << " "
                   << account->getAccountHolder() << " "
                   << account->getBalance() << endl;
    }

    outputFile.close();
}

// Function to load account data from a file
void loadAccountData(vector<Account*>& accounts) {
    ifstream inputFile("accounts.txt");
    if (!inputFile) {
        cerr << "Error: Cannot open the data file for reading." << endl;
        return;
    }

    int accountNumber;
    string accountHolder;
    double balance;

    while (inputFile >> accountNumber >> accountHolder >> balance) {
        if (balance < 0) {
            accounts.push_back(new CheckingAccount(accountNumber, accountHolder, balance, 500.0));
        } else {
            accounts.push_back(new SavingsAccount(accountNumber, accountHolder, balance, 0.05));
        }
    }

    inputFile.close();
}

int main() {
    vector<Account*> accounts;

    // Load account data from a file (if available)
    loadAccountData(accounts);

    while (true) {
        cout << "Banking System Menu:" << endl;
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

        switch (choice) {
            case 1: {
                int accountNumber;
                string accountHolder;
                double initialBalance;
                double interestRate;

                cout << "Enter Account Number: ";
                cin >> accountNumber;
                cout << "Enter Account Holder's Name: ";
                cin.ignore();
                getline(cin, accountHolder);
                cout << "Enter Initial Balance: ";
                cin >> initialBalance;
                cout << "Enter Interest Rate (for Savings Accounts): ";
                cin >> interestRate;

                accounts.push_back(new SavingsAccount(accountNumber, accountHolder, initialBalance, interestRate));
                break;
            }
            case 2: {
                int accountNumber;
                string accountHolder;
                double initialBalance;
                double overdraftLimit;

                cout << "Enter Account Number: ";
                cin >> accountNumber;
                cout << "Enter Account Holder's Name: ";
                cin.ignore();
                getline(cin, accountHolder);
                cout << "Enter Initial Balance: ";
                cin >> initialBalance;
                cout << "Enter Overdraft Limit (for Checking Accounts): ";
                cin >> overdraftLimit;

                accounts.push_back(new CheckingAccount(accountNumber, accountHolder, initialBalance, overdraftLimit));
                break;
            }
            case 3: {
                int accountNumber;
                double amount;

                cout << "Enter Account Number: ";
                cin >> accountNumber;
                cout << "Enter Deposit Amount: ";
                cin >> amount;

                for (Account* account : accounts) {
                    if (account->getAccountNumber() == accountNumber) {
                        account->deposit(amount);
                        cout << "Deposit completed." << endl;
                        break;
                    }
                }
                break;
            }
            case 4: {
                int accountNumber;
                double amount;

                cout << "Enter Account Number: ";
                cin >> accountNumber;
                cout << "Enter Withdrawal Amount: ";
                cin >> amount;

                for (Account* account : accounts) {
                    if (account->getAccountNumber() == accountNumber) {
                        try {
                            account->withdraw(amount);
                            cout << "Withdrawal completed." << endl;
                        } catch (const runtime_error& e) {
                            cerr << "Error: " << e.what() << endl;
                        }
                        break;
                    }
                }
                break;
            }
            case 5: {
                int accountNumber;
                cout << "Enter Account Number: ";
                cin >> accountNumber;

                for (Account* account : accounts) {
                    if (account->getAccountNumber() == accountNumber) {
                        account->display();
                        break;
                    }
                }
                break;
            }
            case 6: {
                int accountNumber;
                cout << "Enter Account Number for Interest Application (Savings Account): ";
                cin >> accountNumber;

                for (Account* account : accounts) {
                    if (account->getAccountNumber() == accountNumber) {
                        SavingsAccount* savingsAccount = dynamic_cast<SavingsAccount*>(account);
                        if (savingsAccount) {
                            savingsAccount->applyInterest();
                            cout << "Interest applied to Savings Account." << endl;
                        }
                    }
                }
                break;
            }
            case 7:
                saveAccountData(accounts);
                cout << "Account data saved to file." << endl;
                break;
            case 8:
                // Clean up allocated memory
                for (Account* account : accounts) {
                    delete account;
                }
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}

