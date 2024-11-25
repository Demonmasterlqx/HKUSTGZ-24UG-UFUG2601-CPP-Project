#include<bits/stdc++.h>
using namespace std;

#ifndef __minSQL_struct__
#define __minSQL_struct__

#define mp make_pair

struct Condition_parameter;
enum Data_type:int;
enum Command_type:int;
enum Compare_sign:int;
enum BOOL_OP:int;
enum Compute_op:int;
class Command_line;
class Table;
class Database;
struct Visitor;
struct Column_pos;
struct Compute_para;

//用于where中的条件储存
typedef vector<Condition_parameter> Condition;

/*
表示SET中的每一个单元是什么
例如：GPA=(GPA*0.2)+2
Com_content负责储存 "(" "GPA" "*" 0.2 ")" "+" 2
*/
typedef variant<string,float,Compute_op> Com_content;

/*
储存一整个等号左边的式子
*/
typedef vector<Com_content> Com_contents;

/*
储存了所有SET之后的表达式
*/
typedef vector<Compute_para> Compute_paras;
typedef variant<string,int,float> Table_content;
typedef variant<string,int,float,Condition,Column_pos,Compute_paras> Parameter_content;
typedef vector<vector<Table_content>> Table_row;
typedef vector<Parameter_content> Parameter;
typedef pair<bool,bool> pbb;

enum Data_type:int{
    TEXT,
    INTEGER,
    FLOAT,
    CONDITION,
    COLUMN_POS,
    COMPUTE_PARAS,
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
    NO_SAME,
    ERROR_COMPARE_SIGN
};

enum Compute_op:int{
    ADD,
    SUB,
    DIV,
    MUT,
    GET_NEGATIVE,
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    ERROR_COMPUTE_OP
};

enum BOOL_OP:int{
    AND,
    OR,
    ERROR_BOOL_OP
};

//用于确定当前列在当前数据库中的具体位置
struct Column_pos{
    // Column_pos(const string &v1,const string& v2);
    Column_pos(const string &v1);
    Column_pos()=default;
    string table_name;
    string column_name;
};

//用于where中的条件储存
struct Condition_parameter{
    Condition_parameter(const BOOL_OP &pre,const string &v1,const string &v2,const string & op,const pbb& is_string);
    BOOL_OP pre_bool_op;
    Compare_sign sign;
    Table_content contentl,contentr;
    pbb is_string;
    Data_type result_type=ERROR_TYPE;
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

/* 
用于 SET 中的计算数值部分
储存了一个SET之后的表达式
*/
struct Compute_para{
    string target;
    Com_contents sentence;
    Compute_para(const string& tar,const Com_contents & sentence);
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


Command_line get_command(ifstream & IN);

bool _is_empty(const char & a);
bool is_special(const char & a);
bool is_special(const string & a);
bool is_compute_op(const string & a);

bool get_quotation_content(string & a,stringstream & input);

// ostream & operator<<(ostream& a,const Condition_parameter& b);
// ostream & operator<<(ostream& a,const Condition& b);
ostream & operator<<(ostream& a,const Column_pos& b);
ostream & operator<<(ostream& a,const Table_content& b);

Data_type what_type(const string & a);
Data_type what_type(const int & a);
int which_column(const Table & table,const string & column);
Data_type which_column_type(const Table & table,const string & column);
void push_into_vector(vector<Table_content> & table,const Parameter_content& c);
Compute_op which_compute_op(const string & a);

Table_content get_cell(const Table& table,const string & column,const int row);
bool check_condition(const Table& table,const Condition & con,const int row);
bool make_comp(Table_content a,Table_content b,Compare_sign op);
float get_num(const Table& table,const int & i,const Com_content & target);
Com_content convert_com_contents(const string & con);
void compute_translate_sentence(Table& table,const int & R,Table_content & target,const Data_type _type,Com_contents sentence);
bool get_where(stringstream & input,Condition &condition,string & para_string,string & para_string1,string &para_string2);

bool _create_table(Database & base,const string &name,const vector<string>& cname,const vector<Data_type>& ty);
bool _insert_into(Table& table,const vector<Parameter_content>& para);
Table _select_from(const Table& table,const vector<string> &column,const Condition & con);
bool _delete_from_where(Table& table,const Condition & con);
bool _updata_set_where(Table& table,const Compute_paras & compara,const Condition & con);
bool _select_from_inner_join_on(const Column_pos& pos1,const Column_pos& pos2,const Table& table1,Table& table2,const Column_pos&con1,const Column_pos&con2);
#endif