#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
using namespace std;

#define MIN_BALANCE 500
class InsufficientAmount{};

class Account
{
    private :
        long accountNumber;
        string first_name;
        string last_name;
        double balance;
        static long NextAccountNumber;
    public :
        Account(){}
        Account(double bal, string fname, string lname);
        long getAccNo(){ return accountNumber; } 
        string getfname(){ return first_name; }
        string getlname(){ return last_name; }
        double getbal(){ return balance; }
        void Deposit(double amount);
        void Withdral(double amount);
        static void setLastAccountNumber(long accountNumber);
        static long getLastAccountNumber();

        friend ifstream & operator >> (ifstream &ifs, Account &acc);
        friend ofstream & operator << (ofstream &ofs, Account &acc);
        friend ostream & operator << (ostream &os, Account &acc);
};

long Account :: NextAccountNumber = 0;

class Bank
{
    private :
        map<long, Account> accounts;
    public:
        Bank();
        Account OpenAccount(string fname, string lname, double bal);
        Account BalanceEnquiry(long accountNumber);
        Account Deposit(long accountNumber, double amount);
        Account Withdral(long accountNumber, double amount);
        void CloseAccount(long accountNumber);
        void ShowAllAccount();
        ~ Bank();
};

int main()
{
    Bank b;
    Account acc;
    string fname, lname;
    long accountNumber;
    double balance;
    double amount;

    int choice;
    cout << "// === # Banking System # === //" << '\n';
    do
    {
        cout << "\n\tSelect one option below --- ";
        cout << "\n\t1. Open an Account ";
        cout << "\n\t2. Balance Enquiry ";
        cout << "\n\t3. Deposit Amount ";
        cout << "\n\t4. Withdral Amount ";
        cout << "\n\t5. Close an Account ";
        cout << "\n\t6. Show All Account ";
        cout << "\n\t7. Quit ... ";
        cout << "\nEnter your choice : ";
        cin >> choice;

        switch(choice)
        {
            case 1 :
                cout << "Enter First Name : ";
                cin >> fname;
                cout << "Enter Last Name : ";
                cin >> lname;
                cout << "Enter Initial Balance : ";
                cin >> balance;
                acc = b.OpenAccount(fname, lname, balance);
                cout << "\nCongradulation Account is Created ...!" << '\n';
                cout << acc;
                break;
            case 2 :
                cout << "Enter your Account Number : ";
                cin >> accountNumber;
                acc = b.BalanceEnquiry(accountNumber);
                cout << "\nYour Bank Details -" << '\n'
                     << "----------------------\n";
                cout << acc;
                break;
            case 3 :
                cout << "Enter Account Number : ";
                cin >> accountNumber;
                cout << "Enter Amount to Deposit : ";
                cin >> amount;
                acc = b.Deposit(accountNumber, amount);
                cout << "\nAmount is Deposited." << '\n';
                cout << acc;
                break;
            case 4 :
                cout << "Enter your Account Number : ";
                cin >> accountNumber;
                cout << "Enter Amount to Withdral : ";
                cin >> amount;
                acc = b.Withdral(accountNumber, amount);
                cout << "\nAmount Withdrawn." << '\n';
                cout << acc;
                break;
            case 5 :
                cout << "Enter your Account Number : ";
                cin >> accountNumber;
                b.CloseAccount(accountNumber);
            case 6 :
                b.ShowAllAccount();
                break;
            case 7 :
                break;
            default :
                cout << "\nEnter correct choice ... ";
                exit(0);
        }
    } while (choice != 7);
    
    return 0;
}

Account :: Account(double balance, string fname, string lname)
{
    NextAccountNumber ++;
    accountNumber = NextAccountNumber;
    first_name = fname;
    last_name = lname;
    this -> balance = balance;
}

void Account :: Deposit(double amount)
{
    balance += amount;
}

void Account :: Withdral(double amount)
{
    if(balance - amount < MIN_BALANCE)
        throw InsufficientAmount();
    balance -= amount;
}

void Account :: setLastAccountNumber(long accountNumber)
{
    NextAccountNumber = accountNumber;
}

long Account :: getLastAccountNumber()
{
    return NextAccountNumber;
}

ifstream & operator >> (ifstream &ifs, Account &acc)
{
    ifs >> acc.accountNumber;
    ifs >> acc.first_name;
    ifs >> acc.last_name;
    ifs >> acc.balance;
    return ifs;
}

ofstream & operator << (ofstream &ofs, Account &acc)
{
    ofs << acc.accountNumber << '\n';
    ofs << acc.first_name << '\n';
    ofs << acc.last_name << '\n';
    ofs << acc.balance << '\n';
    return ofs;
}

ostream & operator << (ostream &os, Account & acc)
{
    os << "First Name : " << acc.getfname() << '\n';
    os << "Last Name : " << acc.getlname() << '\n';
    os << "Account Number : " << acc.getAccNo() << '\n';
    os << "Balance : " << acc.getbal() << '\n';
    return os;
}

Bank :: Bank()
{
    Account account;
    ifstream infile;

    infile.open("Bank_Data");
    if(!infile)
    {
        return;
    }
    while(!infile.eof())
    {
        infile >> account;
        accounts.insert(pair<long, Account>(account.getAccNo(), account));
    }
    Account :: setLastAccountNumber(account.getAccNo());

    infile.close();
}

Account Bank :: OpenAccount(string fname, string lname, double balance)
{
    ofstream outfile;
    Account account(balance, fname, lname);
    accounts.insert(pair<long, Account> (account.getAccNo(), account));
    outfile.open("Bank_Data", ios :: trunc);

    map<long, Account> :: iterator itr;
    for(itr = accounts.begin(); itr != accounts.end(); ++itr)
    {
        outfile << itr -> second;
    }

    outfile.close();
    return account;
}

Account Bank :: BalanceEnquiry(long accountNumber)
{
    map<long, Account> :: iterator itr = accounts.find(accountNumber);
    return itr -> second;
}

Account Bank :: Deposit(long accountNumber, double amount)
{
    map<long, Account> :: iterator itr = accounts.find(accountNumber);
    itr -> second.Deposit(amount);
    return itr -> second;
}

Account Bank :: Withdral(long accountNumber, double amount)
{
    map<long, Account> :: iterator itr = accounts.find(accountNumber);
    itr -> second.Withdral(amount);
    return itr -> second;
}

void Bank :: CloseAccount(long accountNumber)
{
    map<long, Account> :: iterator itr = accounts.find(accountNumber);
    cout << "Account Deleted ...!\n" << itr -> second;
    accounts.erase(accountNumber);
    cout << "Other Accounts - \n";
}

void Bank :: ShowAllAccount()
{
    map<long, Account> :: iterator itr;
    for(itr = accounts.begin(); itr != accounts.end(); ++itr)
    {
        cout << "----------------------\n";
        cout << "Accounts - " << itr -> first << '\n' << itr -> second;
        cout << "----------------------\n";
    }
}

Bank :: ~ Bank()
{
    ofstream outfile;
    outfile.open("Bank_Data", ios :: trunc);

    map<long, Account> :: iterator itr;
    for(itr = accounts.begin(); itr != accounts.end(); ++itr)
    {
        outfile << itr -> second;
    }

    outfile.close();
}