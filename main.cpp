#include<bits/stdc++.h>
#include "minSQL_struct.hpp"
using namespace std;
// struct VISTOR;
// vector<Database> database;

// struct VISTOR{
//     void operator()(const Condition_parameter & a){cout<<"Condition_parameter "<<a.pre_bool_op<<endl;}
//     void operator()(const string & a){cout<<a<<endl;}
//     void operator()(const condition& a){}
//     // void operator()(const condition& a){for(auto i : a) visit([](const Condition_parameter &a){cout<<"Condition_parameter "<<a.pre_bool_op<<endl;},i);}
//     void operator()(const int & a){cout<<a<<endl;}
//     void operator()(const float & a){cout<<a<<endl;}
//     void operator()(const Column_pos & a){cout<<a.column_name<<" "<<a.table_name<<endl;}
// };

// auto visitor=[](auto a){
//     cout<<a<<endl;
// };

int main (int num,char *file_name[]){
    ifstream in;ofstream out;
    in.open(file_name[1]);
    out.open(file_name[2]);
    // get_command(in);
    while(!in.eof()){
        cout<<"COMMAND:\n";
        auto a=get_command(in);
        cout<<a.get_command_type()<<endl;
        auto g=a.get_parameter();
        cout<<g.size()<<endl;
        for(int i=0;i<g.size();i++){
            visit([](auto a){
                cout<<a<<endl;
            },g[i]);
        }
    }
}
