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

typedef vector<Condition_parameter> Condition;
typedef vector<Set_config> Set_configs;
typedef variant<string,int,float> Table_content;
typedef variant<string,int,float,Condition,Column_pos,Set_configs> Parameter_content;
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

class Table{
    public:
    string Table_name;
    vector<string> column_name;
    vector<Data_type> data_type;
    Table_row row;
};

class Database{
    public:
    string data_base_name;
    vector<Table> data;
};

struct Visitor {
    void operator()(int i) { std::cout << "It's an int: " << i << std::endl; }
    void operator()(float f) { std::cout << "It's a float: " << f << std::endl; }
    void operator()(const std::string& s) { std::cout << "It's a string: " << s << std::endl; }
};

Command_line get_command(ifstream & IN);

inline bool _is_empty(const char & a);

inline bool is_special(const char & a);

inline bool is_special(const string & a);

inline void get_quotation_content(string & a,stringstream & input);

ostream & operator<<(ostream& a,const Condition_parameter& b);
ostream & operator<<(ostream& a,const Condition& b);
ostream & operator<<(ostream& a,const Column_pos& b);
ostream & operator<<(ostream& a,const Table_content& b);
ostream & operator<<(ostream& a,const Set_config& b);
ostream & operator<<(ostream& a,const Set_configs& b);
#endif