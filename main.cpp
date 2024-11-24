#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// declared functions
void clear_screen();
void home_options();
void welcome_message();
void login_user();
void signup_user();
void logged_in_user(string email, string name, string user_role, int user_participation);
void logged_in_admin(string email, string name, string user_role, int added_quiz_count);
void take_quiz();

void take_quiz(){
    
}
void logged_in_user(string email, string name, string user_role, int user_participation)
{
    cout << "************************************************" << endl;
    cout << "            Welcome back " << name << endl;
    cout << "            Email: " << email << endl;
    cout << "            Role: " << user_role << endl;
    cout << "            Total Participation: " << user_participation << endl;
    cout << "************************************************" << endl;

    cout << endl;
    cout << "1. Take Quiz" << endl;
    cout << "2. My History" << endl;
    cout << "3. Leaderboard" << endl;
    cout << "4. Sign Out" << endl<<endl;
    int option;
    while (true)
    {
        cout << "Choose option: ";
        cin >> option;
        if (option == 1)
        {
        }
        else if (option == 2)
        {
        }
        else if (option == 3)
        {
        }
        else if (option == 4)
        {
            break;
        }
        else
        {
            cout << "Please choose a valid option...";
        }
    }
}
void logged_in_admin(string email, string name, string user_role, int added_quiz_count)
{
}
void login_user()
{
    string email, password;
    cout << "Enter your email: ";
    cin >> email;
    cout << "Enter your password: ";
    cin >> password;

    ifstream user_file("users.txt");
    string line;
    bool user_found = false;
    string user_role, name, user_participation;

    while (getline(user_file, line))
    {
        // Find positions of commas to extract data
        int pos1 = line.find(',');
        int pos2 = line.find(',', pos1 + 1);
        int pos3 = line.find(',', pos2 + 1);
        int pos4 = line.find(',', pos3 + 1);

        string stored_email = line.substr(0, pos1);
        string stored_password = line.substr(pos1 + 1, pos2 - pos1 - 1);
        name = line.substr(pos2 + 1, pos3 - pos2 - 1);
        user_role = line.substr(pos3 + 1, pos4 - pos3 - 1);
        user_participation = line.substr(pos4 + 1);

        // Check if the entered email and password match
        if (stored_email == email && stored_password == password)
        {
            user_found = true;
            break;
        }
    }

    user_file.close();

    if (user_found)
    {
        cout << "User found" << endl;
        cout << user_role << endl;
        if (user_role == "user")
        {
            clear_screen();
            logged_in_user(email, name, user_role, stoi(user_participation));
        }
        else if (user_role == "admin")
        {
            clear_screen();
            logged_in_admin(email, name, user_role, stoi(user_participation));
        }
    }
    else
    {
        cout << "Invalid email or password. Please try again." << endl;
    }
}
void signup_user()
{
    string email, password, name;

    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter your email: ";
    cin >> email;
    cout << "Enter your password: ";
    cin >> password;

    ofstream user_file("users.txt", ios::app);
    if (user_file.is_open())
    {
        user_file << email << "," << password << "," << name << "," << "user" << endl;
        user_file.close();
        cout << "Signup successful! You can now login." << endl;
    }
    else
    {
        cout << "Unable to open file for writing." << endl;
    }
}
void welcome_message()
{
    cout << "************************************************" << endl;
    cout << "*        Welcome to Terminal Based Quiz        *" << endl;
    cout << "*                Developed by-                 *" << endl;
    cout << "*                   ::Md Al Imran              *" << endl;
    cout << "*                   ::Nafisa Iffat             *" << endl;
    cout << "*                   ::Sabbir Hossain           *" << endl;
    cout << "*                   ::Md. Nabil Jobaead        *" << endl;
    cout << "*                   ::Md Rakibul Islam         *" << endl;
    cout << "************************************************" << endl;
}
void home_options()
{
    int option;
    cout << "1. Login" << endl;
    cout << "2. Sign Up" << endl;
    cout << "3. Leaderboard" << endl;
    cout << "0. Exit" << endl;
    while (true)
    {
        cout << "Choose option: ";
        cin >> option;
        if (option == 1)
        {
            login_user();
            break;
        }
        else if (option == 2)
        {
            signup_user();
            break;
        }
        else if (option == 3)
        {
        }
        else if (option == 0)
        {
            break;
        }
        else
        {
            cout << "Please choose a valid option..." << endl;
        }
    }
}
void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
int main()
{
    clear_screen();
    welcome_message();
    home_options();
    return 0;
}