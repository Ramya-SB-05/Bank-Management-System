#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

class Account {
    int accountNumber;
    char name[50];
    double balance;

public:
    void createAccount();
    void showAccount() const;
    int getAccountNumber() const { return accountNumber; }
    void deposit(double);
    bool withdraw(double);
    void writeToFile();
    static void displayAll();
    static void searchAccount(int);
    static void deleteAccount(int);
};

void Account::createAccount() {
    cout << "\nEnter Account Number: ";
    cin >> accountNumber;
    cout << "Enter Account Holder Name: ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "Enter Initial Balance: ";
    cin >> balance;
    cout << "\nAccount Created Successfully!\n";
}

void Account::showAccount() const {
    cout << left << setw(15) << accountNumber
         << setw(25) << name
         << setw(15) << fixed << setprecision(2) << balance << endl;
}

void Account::deposit(double amount) {
    balance += amount;
}

bool Account::withdraw(double amount) {
    if (amount > balance) {
        cout << "Insufficient balance.\n";
        return false;
    }
    balance -= amount;
    return true;
}

void Account::writeToFile() {
    ofstream outFile("accounts.dat", ios::binary | ios::app);
    outFile.write(reinterpret_cast<char*>(this), sizeof(Account));
    outFile.close();
}

void Account::displayAll() {
    ifstream inFile("accounts.dat", ios::binary);
    if (!inFile) {
        cout << "No accounts found.\n";
        return;
    }
    Account acc;
    cout << left << setw(15) << "Account No"
         << setw(25) << "Name"
         << setw(15) << "Balance" << endl;
    cout << "--------------------------------------------------------\n";

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        acc.showAccount();
    }
    inFile.close();
}

void Account::searchAccount(int accNo) {
    ifstream inFile("accounts.dat", ios::binary);
    if (!inFile) {
        cout << "No accounts found.\n";
        return;
    }
    Account acc;
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccountNumber() == accNo) {
            cout << "\nAccount Found:\n";
            cout << left << setw(15) << "Account No"
                 << setw(25) << "Name"
                 << setw(15) << "Balance" << endl;
            cout << "-------------------------------------------------\n";
            acc.showAccount();
            found = true;
            break;
        }
    }
    inFile.close();
    if (!found) {
        cout << "Account number " << accNo << " not found.\n";
    }
}

void Account::deleteAccount(int accNo) {
    ifstream inFile("accounts.dat", ios::binary);
    if (!inFile) {
        cout << "No accounts found.\n";
        return;
    }
    ofstream outFile("temp.dat", ios::binary);
    Account acc;
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccountNumber() == accNo) {
            found = true;
            continue; // skip writing this account to delete
        }
        outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
    }

    inFile.close();
    outFile.close();

    if (found) {
        remove("accounts.dat");
        rename("temp.dat", "accounts.dat");
        cout << "Account number " << accNo << " deleted successfully.\n";
    } else {
        remove("temp.dat");
        cout << "Account number " << accNo << " not found.\n";
    }
}

void depositMoney(int accNo, double amount) {
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "No accounts found.\n";
        return;
    }
    Account acc;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccountNumber() == accNo) {
            acc.deposit(amount);
            int pos = (int)file.tellg() - sizeof(Account);
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&acc), sizeof(Account));
            cout << "Amount deposited successfully.\n";
            found = true;
            break;
        }
    }
    file.close();
    if (!found) {
        cout << "Account number " << accNo << " not found.\n";
    }
}

void withdrawMoney(int accNo, double amount) {
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "No accounts found.\n";
        return;
    }
    Account acc;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccountNumber() == accNo) {
            if (acc.withdraw(amount)) {
                int pos = (int)file.tellg() - sizeof(Account);
                file.seekp(pos);
                file.write(reinterpret_cast<char*>(&acc), sizeof(Account));
                cout << "Amount withdrawn successfully.\n";
            }
            found = true;
            break;
        }
    }
    file.close();
    if (!found) {
        cout << "Account number " << accNo << " not found.\n";
    }
}

int main() {
    int choice;
    do {
        cout << "\n====== Bank Management System ======\n";
        cout << "1. Create New Account\n";
        cout << "2. Display All Accounts\n";
        cout << "3. Search Account\n";
        cout << "4. Deposit Money\n";
        cout << "5. Withdraw Money\n";
        cout << "6. Delete Account\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            Account acc;
            acc.createAccount();
            acc.writeToFile();
            break;
        }
        case 2:
            Account::displayAll();
            break;
        case 3: {
            int accNo;
            cout << "Enter Account Number to search: ";
            cin >> accNo;
            Account::searchAccount(accNo);
            break;
        }
        case 4: {
            int accNo;
            double amount;
            cout << "Enter Account Number: ";
            cin >> accNo;
            cout << "Enter Amount to Deposit: ";
            cin >> amount;
            depositMoney(accNo, amount);
            break;
        }
        case 5: {
            int accNo;
            double amount;
            cout << "Enter Account Number: ";
            cin >> accNo;
            cout << "Enter Amount to Withdraw: ";
            cin >> amount;
            withdrawMoney(accNo, amount);
            break;
        }
        case 6: {
            int accNo;
            cout << "Enter Account Number to delete: ";
            cin >> accNo;
            Account::deleteAccount(accNo);
            break;
        }
        case 7:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}
