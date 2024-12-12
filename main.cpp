#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <iomanip>
#include <unordered_map>
using namespace std;

// declared functions
void clear_screen();
void home_options();
void welcome_message();
void login_user();
void signup_user();
void logged_in_user(string email, string name, string user_role, int user_participation);
void logged_in_admin(string email, string name, string user_role, int added_quiz_count);
void take_quiz(string email, string name, string user_role, int user_participation);
vector<string> split(const string &s, const string &delimiter);
void leaderboard();
void my_history(string email);

// all function definition
struct LeaderboardEntry
{
    string email;
    int totalCorrectAnswers;
    int totalQuestions;
    long long totalDuration; // Total duration in milliseconds
};
void my_history(string email)
{
    ifstream history_file("history.txt");
    if (!history_file.is_open())
    {
        cout << "Unable to open history file." << endl;
        return;
    }

    string line;
    vector<string> history;
    while (getline(history_file, line))
    {
        vector<string> parts = split(line, "|||");
        if (parts.size() == 5)
        {
            string emailInLine = parts[0];
            if (email == emailInLine)
            {
                history.push_back(line);
            }
        }
    }
    cout << "***********************************************************" << endl;
    cout << "                       My history                 " << endl;
    cout << "                   Email: " << email << endl;
    cout << "                   Total Participation: " << history.size() << endl;
    cout << "***********************************************************" << endl;
    cout << "Date         |    Correct   |    Total    |    Duration " << endl;

    for (string line : history)
    {
        vector<string> parts = split(line, "|||");
        if (parts.size() == 5)
        {
            string emailInLine = parts[0];
            long long date = stoll(parts[1]);
            int correctAnswers = stoi(parts[2]);
            int totalQuestions = stoi(parts[3]);
            long long duration = stoll(parts[4]);
            tm *localTime = localtime(&date);
            int day = localTime->tm_mday;
            int month = localTime->tm_mon + 1;
            int year = localTime->tm_year + 1900;
            cout << day << "/" << month << "/" << year << "   |       " << correctAnswers << "      |      " << totalQuestions << "      |     " << (double)duration / 1000 << "s" << endl;
        }
    }
}
void leaderboard()
{
    ifstream history_file("history.txt");
    if (!history_file.is_open())
    {
        cout << "Unable to open history file." << endl;
        return;
    }

    unordered_map<string, LeaderboardEntry> leaderboard_map;
    string line;

    // Read and parse the history file
    while (getline(history_file, line))
    {
        vector<string> parts = split(line, "|||");
        if (parts.size() == 5)
        {
            string email = parts[0];
            int correctAnswers = stoi(parts[2]);
            int totalQuestions = stoi(parts[3]);
            long long duration = stoll(parts[4]);

            // Aggregate results for each participant
            if (leaderboard_map.find(email) == leaderboard_map.end())
            {
                leaderboard_map[email] = {email, correctAnswers, totalQuestions, duration};
            }
            else
            {
                leaderboard_map[email].totalCorrectAnswers += correctAnswers;
                leaderboard_map[email].totalQuestions += totalQuestions;
                leaderboard_map[email].totalDuration += duration;
            }
        }
    }
    history_file.close();

    // Prepare a vector for sorting
    vector<LeaderboardEntry> leaderboard_data;
    for (const auto &entry : leaderboard_map)
    {
        leaderboard_data.push_back(entry.second);
    }

    // Sort the leaderboard data based on average time per question (ascending order)
    sort(leaderboard_data.begin(), leaderboard_data.end(), [](const LeaderboardEntry &a, const LeaderboardEntry &b)
         {
        double avgTimeA = (a.totalQuestions > 0) ? static_cast<double>(a.totalDuration) / a.totalQuestions : numeric_limits<double>::max();
        double avgTimeB = (b.totalQuestions > 0) ? static_cast<double>(b.totalDuration) / b.totalQuestions : numeric_limits<double>::max();
        double priorityA=a.totalCorrectAnswers/a.totalQuestions*avgTimeA;
        double priorityB=b.totalCorrectAnswers/b.totalQuestions*avgTimeB;
        return priorityA < priorityB; });

    // Display the leaderboard
    cout << "************************************************************" << endl;
    cout << "                         Leaderboard                   " << endl;
    cout << "************************************************************" << endl;
    cout << "Email                    | Correct | Total | Avg Time (s)" << endl;
    cout << "------------------------------------------------------------" << endl;

    for (const auto &entry : leaderboard_data)
    {
        double avgTime = (entry.totalQuestions > 0) ? static_cast<double>(entry.totalDuration) / entry.totalQuestions : 0.0;
        cout << left << setw(25) << entry.email                                                                        // Email
             << "| " << setw(8) << entry.totalCorrectAnswers                                                           // Correct answers
             << "| " << setw(6) << entry.totalQuestions                                                                // Total questions
             << "| " << fixed << setprecision(2) << fixed << setprecision(3) << (double)avgTime / 1000 << "s" << endl; // Average time
    }

    cout << "************************************************************" << endl;
}
vector<string> split(const string &s, const string &delimiter)
{
    vector<string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);
    while (end != string::npos)
    {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start));
    return tokens;
}
void take_quiz(string email, string name, string user_role, int user_participation)
{
    ifstream quiz_file("quiz.txt");
    vector<string> questions;
    string line;

    // Read all questions from the file
    while (getline(quiz_file, line))
    {
        questions.push_back(line);
    }
    quiz_file.close();

    // Check if there are enough questions
    if (questions.size() < 2)
    {
        cout << "Not enough questions in the file." << endl;
        return;
    }

    // Seed the random number generator
    mt19937 eng(static_cast<unsigned int>(time(0)));

    // Shuffle the questions
    shuffle(questions.begin(), questions.end(), eng);

    // Select the first 20 questions
    vector<string> selected_questions(questions.begin(), questions.begin() + 2);

    int total_score = 0;
    int total_questions = selected_questions.size();

    // Start timing
    auto start_time = chrono::high_resolution_clock::now();

    for (int i = 0; i < total_questions; i++)
    {
        vector<string> question_parts = split(selected_questions[i], "|||");
        // if(question_parts.size()!=6)
        string email = question_parts[0];
        string question = question_parts[1];
        string option1 = question_parts[2];
        string option2 = question_parts[3];
        string option3 = question_parts[4];
        string option4 = question_parts[5];
        int correct_answer = stoi(question_parts[6]);

        // Display the question and options
        cout << "Question " << (i + 1) << ": " << question << endl;
        cout << "1. " << option1 << endl;
        cout << "2. " << option2 << endl;
        cout << "3. " << option3 << endl;
        cout << "4. " << option4 << endl;

        // Get user answer
        int user_answer;
        while (true)
        {
            cout << "Your answer (1-4): ";
            cin >> user_answer;

            // Check if the answer is correct
            if (user_answer == correct_answer)
            {
                cout << "Correct!" << endl;
                total_score++;
                break;
            }
            else if (user_answer > 4 || user_answer < 1)
            {
                cout << "Please select an option from 1 to 4." << endl;
            }
            else
            {
                cout << "Wrong! The correct answer was option " << correct_answer << "." << endl;
                break;
            }
        }

        cout << endl;
    }

    // End timing
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    // Display total score and time taken
    cout << "You completed the quiz!" << endl;
    cout << "Your score: " << total_score << "/" << total_questions << endl;
    cout << "Total time taken: " << (double)duration / 1000 << " seconds." << endl;

    // Update the user's participation count
    ifstream user_file("users.txt");
    vector<string> users;
    string user_line;
    int participation_count = 0;

    // Read all users and find the current user
    while (getline(user_file, user_line))
    {
        if (user_line.find(email) == 0)
        {
            int last_comma = user_line.rfind(',');
            participation_count = stoi(user_line.substr(last_comma + 1)) + 1;
            user_line = user_line.substr(0, last_comma + 1) + to_string(participation_count);
        }
        users.push_back(user_line);
    }
    user_file.close();

    // Write the updated user data back to the file
    ofstream user_file_out("users.txt");
    for (const auto &u : users)
    {
        user_file_out << u << endl;
    }
    user_file_out.close();

    // Save the quiz result to history.txt
    ofstream history_file("history.txt", ios::app);
    if (history_file.is_open())
    {
        time_t date = time(0);
        history_file << email << "|||" << date << "|||" << total_score << "|||" << total_questions << "|||" << duration << endl;
        history_file.close();
    }
    else
    {
        cout << "Unable to open history file for writing." << endl;
    }
    while (true)
    {
        cout << endl
             << endl
             << "Press 0 for home screen: ";
        int x;
        cin >> x;
        if (x == 0)
        {
            clear_screen();
            logged_in_user(email, name, user_role, user_participation + 1);
            break;
        }
    }
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
    cout << "4. Sign Out" << endl
         << endl;
    int option;
    while (true)
    {
        cout << "Choose option: ";
        cin >> option;
        if (option == 1)
        {
            clear_screen();
            take_quiz(email, name, user_role, user_participation);
            break;
        }
        else if (option == 2)
        {
            clear_screen();
            my_history(email);
            while (true)
            {
                cout << endl
                     << endl
                     << "Press 0 for home screen: ";
                int x;
                cin >> x;
                if (x == 0)
                {
                    clear_screen();
                    logged_in_user(email, name, user_role, user_participation);
                    break;
                }
            }
            break;
        }
        else if (option == 3)
        {
            clear_screen();
            leaderboard();
            while (true)
            {
                cout << endl
                     << endl
                     << "Press 0 for home screen: ";
                int x;
                cin >> x;
                if (x == 0)
                {
                    clear_screen();
                    logged_in_user(email, name, user_role, user_participation);
                    break;
                }
            }
            break;
        }
        else if (option == 4)
        {
            clear_screen();
            welcome_message();
            home_options();
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
        user_file << email << "," << password << "," << name << "," << "user,0" << endl;
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
            clear_screen();
            leaderboard();
            while (true)
            {
                cout << endl
                     << endl
                     << "Press 0 for home screen: ";
                int x;
                cin >> x;
                if (x == 0)
                {
                    clear_screen();
                    welcome_message();
                    home_options();
                    break;
                }
            }
            break;
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
