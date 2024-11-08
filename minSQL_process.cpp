#include "minSQL_struct.hpp"

bool _create_table(Database & base,const string &name,const vector<string>& cname,const vector<Data_type>& ty){
    int top=base.data.size();
    for(int i=0;i<top;i++) for(int e=i+1;e<top;e++) if(base.data[e].Table_name==base.data[i].Table_name) return 0;
    base.data.push_back(Table(name,cname,ty));
    return 1;
}

Data_type what_type(const string & a){
    if(a=="TEXT") return TEXT;
    if(a=="INTEGER") return INTEGER;
    if(a=="FLOAT") return FLOAT;
    return ERROR_TYPE;
}

bool _insert_into(Table& table,const vector<Parameter_content>& para){
    vector<Table_content> con;
    int len=para.size();
    if(para.size()!=table.data_type.size()) return 0;
    for(int i=0;i<len;i++){
        if(para[i].index()!=table.data_type[i]) return 0;
        push_into_vector(con,para[i]);
    }
    table.row.push_back(con);
    return 1;
}

void push_into_vector(vector<Table_content> & table,const Parameter_content& c){
    if(c.index()==TEXT) table.push_back(get<string>(c));
    if(c.index()==INTEGER) table.push_back(get<int>(c));
    if(c.index()==FLOAT) table.push_back(get<float>(c));
}

Table _select_from(const Table& table,const vector<string> &column,const Condition & con){
    Table ans;
    ans.Table_name=table.Table_name;
    int len=column.size();
    vector<int> pos;
    for(int i=0;i<len;i++){
        ans.column_name.push_back(column[i]);
        ans.data_type.push_back(which_column_type(table,column[i]));
        pos.push_back(which_column(table,column[i]));
    }
    int lenr=table.row.size();
    for(int i=0;i<lenr;i++){
        if(!check_condition(table,con,i)) continue;
        vector<Table_content> lin;
        for(int e : pos) lin.push_back(table.row[i][e]);
        ans.row.push_back(lin);
    }
    return ans;
}

Table_content get_cell(const Table& table,const string & column,const int row){
    int len=table.column_name.size(),pos;
    for(int i=0;i<len;i++) if(table.column_name[i]==column) {pos=i;break;}
    return table.row[row][pos];
}

bool check_condition(const Table& table,const Condition & con,const int row){
    int len=con.size();
    bool state=1;
    for(int i=0,now;i<len;i++){
        now=(make_comp(get_cell(table,con[i].column.column_name,row),con[i].content,con[i].sign));
        if(con[i].pre_bool_op==OR) state=state||now;
        else if(con[i].pre_bool_op==AND) state=state&&now;
    }
    return state;
}

bool make_comp(Table_content a,Table_content b,Compare_sign op){
    if(op==EQUAL) return a==b;
    if(op==BIGER) return a>b;
    if(op==SMALLER) return a<b;
    return 1;
}

int which_column(const Table & table,const string & column){
    int len=table.column_name.size();
    for(int i=0;i<len;i++){
        if(table.column_name[i]==column) return i; 
    }
    return -1;
}

Data_type which_column_type(const Table & table,const string & column){
    return table.data_type[which_column(table,column)];
}

bool _delete_from_where(Table& table,const Condition & con){
    int len=table.row.size();
    for(int i=len-1;i>=0;i--){
        if(check_condition(table,con,i)) table.row.erase(table.row.begin()+i);
    }
    return 1;
}