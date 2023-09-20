/*
OVERVIEW
-idea to come up with your own software that allows users to deposit cash for safe keeping and future planning
-Money withdrawn by customer from ATM
-First-step:enter username and password to login
-option of checking their balance; changing their password; withdraw cash; 
-cash deposits involves administrator stationed near an ATM machine
-Admin logs in using their username and password to deposit on behalf of customer
-option to make deposit for normal user; create an account for a new user; change their own Admin password
-include back option/buttons
-presenttable UI/UX 

an algorithm that will enable the administrator to store user details in a local database file
"usersDB.txt" for users
and "admin.txt"for admin login credntials with default admin password

The bare minimum user details required are their usernames, password and account balance. 
It is this usersDB.txt database file that will be updated and queried whenever either the user or the administrator 
makes changes to the user data. It must always reflect the current state of data. 
You are required to work with at least one user.  

Implement validation checks for usernames and passwords to enhance security and enable user to 
retype username or password if incorrect.
Validation also for account balance such that a user cannot withdraw more than they have in the account 
*/

#include <iostream> //standard input - output operations
#include <fstream> //file input - output operations
#include <string> //strings
#include <algorithm> //transform characters to lowercase
using namespace std;

//Struct for user details
//user-defined to group related data items of different data types together
struct User 
{
    string username;
    string password;
    double accbalance;
};

//Struct for admin details
struct Admin 
{
    string username;
    string password;
};

//convert a string to lowercase for username/password authentication
string toLowerCase(const string& str) //keep the string constant (cant modify)
{
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

//validate a username
bool isValidUsername(const string& username) 
{    
    return username.length() >= 6;//Username must have at least 6 characters
}

//validate a password
bool isValidPassword(const string& password) 
{
    return password.length() >= 8;//Password must have at least 8 characters
}

//validate a withdrawal amount
bool isValidWithdrawal(double withdrawalAmount, double accbalance) 
{
    return withdrawalAmount <= accbalance;//amount must be less than or equal to the account balance
}

//user login
bool userLogin(const string& username, const string& password, User& loggedInUser) 
{
    ifstream file("usersDB.txt");
    if (file.is_open()) 
    {
        User user;
        while (file >> user.username >> user.password >> user.accbalance) 
        {
            if (toLowerCase(user.username) == toLowerCase(username) && user.password == password) 
            {
                loggedInUser = user;
                file.close();
                return true;
            }
        }
        file.close();
    } 
    else 
    {
        cout << "Failed to open user database file." << endl;
    }
    return false;
}

//check user balance
void checkBalance(const User& user) {
    cout << "\nYour account balance is: " << user.accbalance << endl;
}

//change user password
void changePassword(User& user) {
    string newPassword;
    cout << "Enter new password: ";
    cin >> newPassword;
    user.password = newPassword;
    cout << "\nPassword changed successfully." << endl;
}

//user withdrawal
void withdrawCash(User& user) 
{
    double withdrawalAmount;
    cout << "Enter withdrawal amount: ";
    cin >> withdrawalAmount;
    if (isValidWithdrawal(withdrawalAmount, user.accbalance)) 
    {
        user.accbalance -= withdrawalAmount;
        cout << "\nWithdrawal successful. Current balance is: " << user.accbalance << endl;
    } 
    else 
    {
        cout << "Invalid withdrawal amount or insufficient balance." << endl;
    }
}

//admin login
bool adminLogin(const string& username, const string& password) 
{
    ifstream file("admin.txt");
    if (file.is_open()) 
    {
        Admin admin;
        while (file >> admin.username >> admin.password) 
        {
            if (toLowerCase(admin.username) == toLowerCase(username) && admin.password == password) {
                file.close();
                return true;
            }
        }
        file.close();
    } else {
        cout << "Failed to open admin database file." << endl;
    }
    return false;
}

//create a new user
void createUser() {
    User newUser;
    cout << "Enter username: ";
    cin >> newUser.username;
    cout << "Enter password: ";
    cin >> newUser.password;
    cout << "Enter initial account balance: ";
    cin >> newUser.accbalance;

    ofstream file("usersDB.txt", ios::app);
    if (file.is_open()) {
        file << "\n"<<newUser.username << "\n" << newUser.password << "\n" << newUser.accbalance << "\n" << endl;
        cout << "\nUser created successfully." << endl;
        file.close();
    } 
    else 
    {
        cout << "Failed to open user database file." << endl;
    }
}

//change admin password
void changeAdminPassword() 
{
    string newAdminPassword;
    cout << "Enter new admin password: ";
    cin >> newAdminPassword;

    ofstream file("admin.txt");
    if (file.is_open()) 
    {
        file << "admin" << "\n" << newAdminPassword << "\n";
        cout << "\nAdmin password changed successfully." << endl;
        file.close();
    } 
    else 
    {
        cout << "Failed to open admin database file." << endl;
    }
}

//admin to deposit cash for a user
void depositCash() 
{
    string username;
    double depositAmount;
    cout << "Enter the username of the user to deposit cash for: ";
    cin >> username;

    ifstream file("usersDB.txt");
    if (file.is_open()) {
        User user;
        bool userFound = false;
        while (file >> user.username >> user.password >> user.accbalance) 
        {
            if (toLowerCase(user.username) == toLowerCase(username)) 
            {
                userFound = true;
                cout << "Enter deposit amount: ";
                cin >> depositAmount;
                user.accbalance += depositAmount;
                break;
            }
        }
        file.close();
        if (userFound) 
        {
            ofstream outFile("usersDB.txt");
            if (outFile.is_open()) 
            {
                file.open("usersDB.txt");
                User tempUser;
                while (file >> tempUser.username >> tempUser.password >> tempUser.accbalance) 
                {
                    if (toLowerCase(tempUser.username) == toLowerCase(username)) 
                    {
                        outFile << user.username << "\n" << user.password << "\n" << user.accbalance << "\n";
                    } 
                    else 
                    {
                        outFile << tempUser.username << "\n" << tempUser.password << "\n" << tempUser.accbalance << "\n";
                    }
                }
                file.close();
                outFile.close();
                cout << "Deposit successful." << endl;
            } 
            else 
            {
                cout << "Failed to open user database file." << endl;
            }
        } 
        else 
        {
            cout << "User not found." << endl;
        }
    }
    else 
    {
        cout << "Failed to open user database file." << endl;
    }
}

//masked password entry
string getMaskedPassword() 
{
    string password;
    char ch;
    while ((ch = _getwch()) != '\r') 
    {
        if (ch == '\b') 
        {
            if (!password.empty()) 
            {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else 
        {
            password += ch;
            cout << '*';
        }
    }
    cout << endl;
    return password;
}


//main function
int main() 
{
    int choice;
    User loggedInUser;

    cout<<"\n*#---------------------------------------#*"<<endl;
    cout << "****WELCOME TO MAGENDO E-CASH SERVICES****" << endl;
    cout<<"ATM-Automatic Teller Machine\n";

    while (true) 
    {
        cout << "\n1. Normal User Login";
        cout << "\n2. Administrator Login";
        cout << "\n3. Exit\n";
        cout << "\nChoose an option to proceed: ";
        cin >> choice;

        if (choice == 1) 
        {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            password = getMaskedPassword();

            if (userLogin(username, password, loggedInUser)) 
            {
                int userChoice;
                while (true) 
                {
                    cout << "\n1. Withdraw Cash";
                    cout << "\n2. Check your Balance";
                    cout << "\n3. Reset your Password";
                    cout << "\n4. Back to Main Menu...\n";
                    cout << "\nEnter option to proceed: ";
                    cin >> userChoice;

                    switch (userChoice) {
                        case 1:
                            withdrawCash(loggedInUser);
                            break;
                        case 2:
                            checkBalance(loggedInUser);
                            break;
                        case 3:
                            changePassword(loggedInUser);
                            break;
                        case 4:
                            cout << "Logged out." << endl;
                            break;
                        default:
                            cout << "Invalid Option." << endl;
                            break;
                    }

                    if (userChoice == 4) 
                    {
                        break;
                    }
                }
            } 
            else 
            {
                cout << "Invalid username or password." << endl;
            }
        } 
        else if (choice == 2) 
        {
            string username, password;
            cout << "Enter admin username: ";
            cin >> username;
            cout << "Enter admin password: ";
            password = getMaskedPassword();

            if (adminLogin(username, password)) 
            {
                int adminChoice;
                while (true) 
                {
                    cout << "\n1. Register New User";
                    cout << "\n2. Change My Admin Password";
                    cout << "\n3. Deposit Cash for User";
                    cout << "\n4. Back to Main Menu...\n";
                    cout << "\nEnter option to proceed: ";
                    cin >> adminChoice;

                    switch (adminChoice) 
                    {
                        case 1:
                            createUser();
                            break;
                        case 2:
                            changeAdminPassword();
                            break;
                        case 3:
                            depositCash();
                            break;
                        case 4:
                            cout << "Logged out." << endl;
                            break;
                        default:
                            cout << "Invalid Option." << endl;
                            break;
                    }

                    if (adminChoice == 4) 
                    {
                        break;
                    }
                }
            } 
            else 
            {
                cout << "Invalid admin username or password." << endl;
            }
        } else if (choice == 3) 
        {
            break;
        } 
        else 
        {
            cout << "Invalid Option." << endl;
        }
    }

    return 0;
}
