#include<bits/stdc++.h>
using namespace std;

void welcome_message(){
    cout<<"************************************************"<<endl;
    cout<<"*        Welcome to Terminal Based Quiz        *"<<endl;
    cout<<"*                Developed by-                 *"<<endl;
    cout<<"*                   ::Md Al Imran              *"<<endl;
    cout<<"*                   ::Nafisa Iffat             *"<<endl;
    cout<<"*                   ::Sabbir Hossain           *"<<endl;
    cout<<"*                   ::Md. Nabil Jobaead        *"<<endl;
    cout<<"*                   ::Md Rakibul Islam         *"<<endl;
    cout<<"************************************************"<<endl;
}
void home_options(){
    
}
void clear_screen(){
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
    return 0;
}