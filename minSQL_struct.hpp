#include<bits/stdc++.h>
using namespace std;

#ifndef __minSQL_struct__
#define __minSQL_struct__

struct Condition_parameter;
enum Data_type:int;
enum Command_type:int;
enum Compare_sign:int;
enum BOOL_OP:int;
class Command_line;
class Table;
class Database;
struct Visitor;
struct Column_pos;
struct Set_config;
struct Compute_para;

typedef vector<Condition_parameter> Condition;
typedef variant<string,float> Com_content;
typedef vector<Set_config> Set_configs;
typedef vector<Compute_para> Compute_paras;
typedef variant<string,int,float> Table_content;
typedef variant<string,int,float,Condition,Column_pos,Set_configs,Compute_paras> Parameter_content;
typedef vector<vector<Table_content>> Table_row;
typedef vector<Parameter_content> Parameter;

enum Data_type:int{
    TEXT,
    INTEGER,
    FLOAT,
    CONDITION,
    COLUMN_POS,
    SET_CONFIGS,
    ERROR_TYPE
};

enum Command_type:int{
    CREATE_DATABASE,
    USE_DATABASE,
    CREATE_TABLE,
    DROP_TABLE,
    INSERT_INTO,
    SELECT_FROM,
    SELECT_FROM_INNER_JOIN_ON,
    UPDATE_SET_WHERE,
    DELETE_FROM_WHERE,
    ERROR_COMMAND
};

enum Compare_sign:int{
    EQUAL,
    BIGER,
    SMALLER,
    ERROR_COMPARE_SIGN
};

enum Compute_op{
    ADD,
    SUB,
    DIV,
    MUT,
    ERROR_COMPUTE_OP
};

enum BOOL_OP:int{
    AND,
    OR,
    ERROR_BOOL_OP
};

struct Column_pos{
    // Column_pos(const string &v1,const string& v2);
    Column_pos(const string &v1);
    Column_pos()=default;
    string table_name;
    string column_name;
};

struct Condition_parameter{
    Condition_parameter(const BOOL_OP &pre,const string &v1,const string &v2,const string & op);
    BOOL_OP pre_bool_op;
    Column_pos column;
    Compare_sign sign;
    Table_content content;
};

struct Set_config{
    Set_config()=default;
    Set_config(const string & v1,const string & v2);
    // Set_config(const string & v1);
    string column;
    Table_content content;
};

class Command_line{
    public:
    Command_line(Command_type G,Parameter F);
    Command_type get_command_type();
    Parameter get_parameter();
    public:
    Command_type command_type;
    Parameter parameter;
};

struct Compute_para{
    Com_content para[3];
    Compute_op op;
    Compute_para(const string &para1,const string &para2,const string &para3,const string &op);
};

class Table{
    public:
    Table()=default;
    Table(const string & name,const vector<string>& a,const vector<Data_type>& b):Table_name(name),column_name(a),data_type(b){}
    string Table_name;
    vector<string> column_name;
    vector<Data_type> data_type;
    Table_row row;
};

class Database{
    public:
    Database()=default;
    Database(const string & name):data_base_name(name){}
    string data_base_name;
    vector<Table> data;
};

struct Visitor {
    void operator()(int i) { std::cout << "It's an int: " << i << std::endl; }
    void operator()(float f) { std::cout << "It's a float: " << f << std::endl; }
    void operator()(const std::string& s) { std::cout << "It's a string: " << s << std::endl; }
};

Command_line get_command(ifstream & IN);

bool _is_empty(const char & a);
bool is_special(const char & a);
bool is_special(const string & a);
bool is_compute_op(const string & a);

void get_quotation_content(string & a,stringstream & input);

ostream & operator<<(ostream& a,const Condition_parameter& b);
ostream & operator<<(ostream& a,const Condition& b);
ostream & operator<<(ostream& a,const Column_pos& b);
ostream & operator<<(ostream& a,const Table_content& b);
ostream & operator<<(ostream& a,const Set_config& b);
ostream & operator<<(ostream& a,const Set_configs& b);

Data_type what_type(const string & a);
Data_type what_type(const int & a);
int which_column(const Table & table,const string & column);
Data_type which_column_type(const Table & table,const string & column);
void push_into_vector(vector<Table_content> & table,const Parameter_content& c);
Compute_op which_compute_op(const string & a);

Table_content get_cell(const Table& table,const string & column,const int row);
bool check_condition(const Table& table,const Condition & con,const int row);
bool make_comp(Table_content a,Table_content b,Compare_sign op);
void updata_compute(Table& table,const int & i,Table_content & target,const Compute_para & com,Data_type _type);
float get_num(const Table& table,const int & i,const Com_content & target);
float compute(int num1,int num2,Compute_op op);

bool _create_table(Database & base,const string &name,const vector<string>& cname,const vector<Data_type>& ty);
bool _insert_into(Table& table,const vector<Parameter_content>& para);
Table _select_from(const Table& table,const vector<string> &column,const Condition & con);
bool _delete_from_where(Table& table,const Condition & con);
bool _updata_set_where(Table& table,const Set_configs & set,const Compute_paras & compara,const Condition & con);
bool _select_from_inner_join_on(const Column_pos& pos1,const Column_pos& pos2,const Table& table1,Table& table2,const Column_pos&con1,const Column_pos&con2);
#endif