#include "minSQL_struct.hpp"

#include<bits/stdc++.h>
#include <filesystem>
using namespace std;
namespace fs=std::filesystem;

const string STORTAGE_TAG="HKUSTGZUCUGCPPPROJECTSTORTAGE";
std::filesystem::path current_PATH=std::filesystem::current_path();
std::filesystem::path database_path=current_PATH/"database/";

vector<Database> database;
ifstream in;ofstream out;
int Database_index=-1;

Command_line get_convert_command(ifstream & IN);
Table& get_table(const string& name);
Data_type get_type(const Table & table,const string& name);
void load_in();
void write_in(int);
void write_in(ostream & out,const Table& c);
void print_out(ofstream & out,const Table_content & con);

bool create_database(const string& database_name);
bool create_table(const Command_line& line);
bool use_database(const string& database_name);
bool drop_table(const Command_line& line);
bool inset_into(const Command_line& line);
bool select_from(const Command_line& line);
bool delete_from_where(const Command_line& line);
bool updata_set_where(const Command_line& line);
bool select_from_inner_join_on(const Command_line& line);

int main (int num,char *file_name[]){
    cout<<database_path<<endl;
    // set input and output
    in.open(file_name[1]);
    out.open(file_name[2]);
    //lead in database
    load_in();

    // read_process_command
    while(!in.eof()){
        Command_line a=get_convert_command(in);
        if(a.command_type==CREATE_DATABASE) create_database(get<string>(a.parameter[0]));
        else if(a.command_type==USE_DATABASE) use_database(get<string>(a.parameter[0]));
        else if(a.command_type==CREATE_TABLE) create_table(a);
        else if(a.command_type==DROP_TABLE) drop_table(a);
        else if(a.command_type==INSERT_INTO) inset_into(a);
        else if(a.command_type==SELECT_FROM) select_from(a);
        else if(a.command_type==DELETE_FROM_WHERE) delete_from_where(a);
        else if(a.command_type==UPDATE_SET_WHERE) updata_set_where(a);
        else if(a.command_type==SELECT_FROM_INNER_JOIN_ON) select_from_inner_join_on(a);
        write_in(Database_index);
    }
    in.close();
    out.close();
}

void convert_data(const Data_type & Type,Parameter_content &command){
    stringstream GG;
    int INT;float FLO;
    if(Type==INTEGER){
        GG<<get<string>(command);
        GG>>INT;
        command=INT;
    }
    else if(Type==FLOAT){
        GG<<get<string>(command);
        GG>>FLO;
        command=FLO;
    }
}

void convert_data(const Data_type & Type,Table_content &command){
    stringstream GG;
    int INT;float FLO;
    if(Type==INTEGER){
        GG<<get<string>(command);
        GG>>INT;
        command=INT;
    }
    else if(Type==FLOAT){
        GG<<get<string>(command);
        GG>>FLO;
        command=FLO;
    }
}

Command_line get_convert_command(ifstream & IN){
    Command_line command=get_command(IN);
    stringstream LIN;int INT;float FLO;
    if(command.command_type==INSERT_INTO){
        Table& table=get_table(get<string>(command.parameter[0]));
        for(int i=command.parameter.size()-1,j=table.data_type.size()-1;i;j--,i--){
            convert_data(table.data_type[j],command.parameter[i]);
        }
    }
    else if(command.command_type==SELECT_FROM){
        if(command.parameter[command.parameter.size()-1].index()==CONDITION){
            auto table=get_table(get<string>(command.parameter[command.parameter.size()-2]));
            auto condition=get<Condition>(command.parameter[command.parameter.size()-1]);
            for(int i=condition.size()-1;i>=0;i--){
                convert_data(get_type(table,condition[i].column.column_name),condition[i].content);
            }
            command.parameter[command.parameter.size()-1]=condition;
        }
    }
    else if(command.command_type==UPDATE_SET_WHERE){
        Table& table=get_table(get<string>(command.parameter[0]));
        if(command.parameter[command.parameter.size()-1].index()==CONDITION){
            auto condition=get<Condition>(command.parameter[command.parameter.size()-1]);
            for(int i=condition.size()-1;i>=0;i--){
                convert_data(get_type(table,condition[i].column.column_name),condition[i].content);
            }
            command.parameter[command.parameter.size()-1]=condition;
        }
        auto sets=get<Set_configs>(command.parameter[1]);
        for(int i=sets.size()-1;i>=0;i--){
            convert_data(get_type(table,sets[i].column),sets[i].content);
        }
        command.parameter[1]=sets;
    }
    else if(command.command_type==DELETE_FROM_WHERE){
        Table& table=get_table(get<string>(command.parameter[0]));
        if(command.parameter[command.parameter.size()-1].index()==CONDITION){
            auto condition=get<Condition>(command.parameter[command.parameter.size()-1]);
            for(int i=condition.size()-1;i>=0;i--){
                convert_data(get_type(table,condition[i].column.column_name),condition[i].content);
            }
            command.parameter[command.parameter.size()-1]=condition;
        }
    }
    while(!in.eof()&&_is_empty((char)in.get())) cout<<0; if(!in.eof()) in.unget();
    return command;
}

Table& get_table(const string& name){
    if(Database_index==-1) exit(0);
    for(int i=database[Database_index].data.size()-1;i>=0;i--){
        if(name==database[Database_index].data[i].Table_name) return database[Database_index].data[i];
    }
    return *(new Table());
    //****************************************************************************************************************************
}

Data_type get_type(const Table & table,const string& name){
    for(int i=table.column_name.size()-1;i>=0;i--){
        if(table.column_name[i]==name) return table.data_type[i];
    }
    return ERROR_TYPE;
}

bool create_database(const string& database_name){
    for(int i=database.size()-1;i>=0;i--){
        if(database[i].data_base_name==database_name) return 0;
    }
    database.push_back(Database(database_name));
    write_in(database.size()-1);
    return 1;
}

bool use_database(const string& database_name){
    for(int i=database.size()-1;i>=0;i--){
        if(database[i].data_base_name==database_name){
            Database_index=i;
            return 1;
        }
    }
    return 0;
}

bool create_table(const Command_line& line){
    vector<string> name;
    vector<Data_type> ty;
    for(int j=line.parameter.size()-1,i=1;i<=j;i+=2){
        name.push_back(get<string>(line.parameter[i]));
        ty.push_back(what_type(get<string>(line.parameter[i+1])));
    }
    return _create_table(database[Database_index],get<string>(line.parameter[0]),name,ty);
}

bool drop_table(const Command_line& line){
    string name=get<string>(line.parameter[0]);
    if(Database_index==-1) return -1;
    int len=database[Database_index].data.size();
    auto &data=database[Database_index].data;
    for(int i=0;i<len;i++){
        if(data[i].Table_name==name) {data.erase(data.begin()+i);return 1;}
    }
    return 0;
}

bool inset_into(const Command_line& line){
    vector<Parameter_content> para;
    int len=line.parameter.size();
    for(int i=1;i<len;i++) para.push_back(line.parameter[i]);
    return _insert_into(get_table(get<string>(line.parameter[0])),para);
}

bool select_from(const Command_line& line){
    Condition con;
    Table ans;
    vector<string> column;
    string table_name;
    int len=line.parameter.size(),top;
    if(line.parameter[len-1].index()==CONDITION){
        top=len-2;
        con=get<CONDITION>(line.parameter[len-1]);
        table_name=get<string>(line.parameter[len-2]);
    }
    else{
        top=len-1;
        table_name=get<string>(line.parameter[len-1]);
    }
    Table &select_table=get_table(table_name);
    if(get<string>(line.parameter[0])=="*"){
        column=select_table.column_name;
    }
    else{
        for(int i=0;i<top;i++){
            column.push_back(get<string>(line.parameter[i]));
        }
    }
    ans=_select_from(select_table,column,con);
    write_in(out,ans);
    return 1;
}

void write_in(ostream & out,const Table& c){
    int lenc=c.column_name.size();
    int lenr=c.row.size();
    for(int i=0;i<lenc;i++){
        out<<c.column_name[i];
        if(i!=lenc-1) out<<',';
    }
    out<<endl;
    for(int i=0;i<lenr;i++){
        for(int e=0;e<lenc;e++){
            if(c.data_type[e]==TEXT) out<<'\"'<<c.row[i][e]<<'\"';
            else out<<c.row[i][e];
            if(e!=lenc-1) out<<',';
        }
        out<<endl;
    }
}

bool delete_from_where(const Command_line& line){
    return _delete_from_where(get_table(get<string>(line.parameter[0])),get<CONDITION>(line.parameter[1]));
}

bool updata_set_where(const Command_line& line){
    return _updata_set_where(get_table(get<string>(line.parameter[0])),get<Set_configs>(line.parameter[1]),get<Compute_paras>(line.parameter[2]),get<Condition>(line.parameter[3]));
}

bool select_from_inner_join_on(const Command_line& line){
    return _select_from_inner_join_on(get<Column_pos>(line.parameter[0]),get<Column_pos>(line.parameter[1]),get_table(get<string>(line.parameter[2])),get_table(get<string>(line.parameter[3])),get<Column_pos>(line.parameter[4]),get<Column_pos>(line.parameter[5]));
}

void write_in(int Database_index){
    if(!(fs::exists(database_path)&&fs::is_directory(database_path))) fs::create_directories(database_path);
    if(!(fs::exists(database_path)&&fs::is_directory(database_path))){
        cerr<<"CRAET DIRECTORY FAIL, CAN'T STORE THE DATA\n";
        return;
    }
    if(Database_index==-1) return;
    const Database & now=database[Database_index];
    fs::path now_da=database_path/now.data_base_name;
    ofstream storeout;storeout.open(now_da);
    cout<<now_da<<endl;
    storeout<<STORTAGE_TAG<<endl;
    storeout<<now.data_base_name<<endl;
    storeout<<now.data.size()<<endl;
    for(int i=0,top=now.data.size();i<top;i++){
        const Table &now_table=now.data[i];
        int lenc=now_table.column_name.size();
        int lenr=now_table.row.size();
        storeout<<now_table.Table_name<<endl;
        storeout<<lenc<<" "<<lenr<<endl;
        for(int e=0;e<lenc;e++){
            storeout<<now_table.column_name[e]<<" "<<now_table.data_type[e]<<endl;
        }
        for(int e=0;e<lenr;e++){
            for(int j=0;j<lenc;j++){
                print_out(storeout,now_table.row[e][j]);
                storeout<<" ";
            }
            storeout<<endl;
        }
    }
    storeout.close();
}

void print_out(ofstream & out,const Table_content & con){
    if(con.index()==INTEGER) out<<get<int>(con);
    else if(con.index()==FLOAT) out<<get<float>(con);
    else if(con.index()==TEXT) out<<get<string>(con);
}

void load_in(){
    if(!(fs::exists(database_path)&&fs::is_directory(database_path))) fs::create_directories(database_path);
    for(const auto& entry : fs::directory_iterator(database_path)) {
        if (!fs::is_regular_file(entry.status())) continue;
        ifstream in;in.open(entry.path());
        string ID;
        in>>ID;
        if(ID!=STORTAGE_TAG) continue;
        Database curdata;
        in>>curdata.data_base_name;
        int num_t=0;
        in>>num_t;
        for(int i=0;i<num_t;i++){
            Table curtabel;
            in>>curtabel.Table_name;
            int num_c=0,num_r=0;
            in>>num_c>>num_r;
            string name_column;int _type=-1;
            for(int e=0;e<num_c;e++){
                in>>name_column>>_type;
                curtabel.column_name.push_back(name_column);
                curtabel.data_type.push_back(what_type(_type));
            }
            for(int e=0;e<num_r;e++){
                vector<Table_content> now_con;
                string aaa;
                for(int j=0;j<num_c;j++){
                    in>>aaa;
                    now_con.push_back(aaa);
                    convert_data(curtabel.data_type[j],now_con[j]);
                }
                curtabel.row.push_back(now_con);
            }
            curdata.data.push_back(curtabel);
        }
        database.push_back(curdata);
    }
}