#include<bits/stdc++.h>
#include "minSQL_struct.hpp"
using namespace std;

vector<Database> database;

int main (int num,char *file_name[]){
    ifstream in;ofstream out;
    in.open(file_name[1]);
    out.open(file_name[2]);
    cout<<000<<endl;
    auto a=get_command(in);
    cout<<a.get_command_type()<<endl;
    auto g=a.get_parameter();
    auto h=g[0];
    cout<<g.size()<<endl;
    cout<<get<get_Data_type(h)>(h)<<endl;
}