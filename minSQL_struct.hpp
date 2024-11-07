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

typedef vector<Condition_parameter> condition;
typedef variant<string,int,float> Table_content;
typedef variant<string,int,float,condition> Parameter_content;
typedef vector<vector<Table_content>> Table_row;
typedef vector<Parameter_content> Parameter;

enum Data_type:int{
    TEXT,
    INTEGER,
    FLOAT,
    CONDITION,
    ERROR_TYPE
};

enum Command_type:int{
    CREATE_DATABASE,
    USE_DATABASE,
    CREATE_TABLE,
    DROP_TABLE,
    INSERT_INTO,
    SELECT_FROM,
    SELECT_FROM_WHERE,
    SELECT_FROM_INNER_JOIN_ON,
    UPDATE_SET_WHERE,
    DELETE_FROM_WHERE,
    ERROR_COMMAND
};

enum Compare_sign:int{
    EQUAL,
    BIGER,
    SMALLER
};

enum BOOL_OP:int{
    AND,
    OR
};

struct Condition_parameter{
    BOOL_OP pre_bool_op;
    string column;
    Compare_sign sign;
    Table_content content;
};


class Command_line{
    public:
    Command_line(Command_type G,Parameter F);
    Command_type get_command_type();
    Parameter get_parameter();
    private:
    Command_type command_type;
    Parameter parameter;
};

class Table{
    private:
    string Table_name;
    vector<string> column_name;
    vector<Data_type> data_type;
    Table_row row;
};

class Database{
    private:
    string data_base_name;
    vector<Table> data;
};

struct Visitor {
    void operator()(int i) { std::cout << "It's an int: " << i << std::endl; }
    void operator()(float f) { std::cout << "It's a float: " << f << std::endl; }
    void operator()(const std::string& s) { std::cout << "It's a string: " << s << std::endl; }
};

Command_line get_command(ifstream & IN);

// template <typename... T>
// requires std::same_as<T,Table_content> || std::same_as<T,Parameter_content>
void get_Data_type(const Parameter_content &C){
    if(C.index()==TEXT) 
    if(C.index()==INTEGER) return INTEGER;
    if(C.index()==FLOAT) return FLOAT;
    if(C.index()==CONDITION) return CONDITION;
    return ERROR_TYPE;
}

// template Data_type get_Data_type<string,int,float,condition>(const variant<string,int,float,condition> &C);

#endif