#include "minSQL_struct.hpp"
#include "calculator.hpp"

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
    return holds_alternative<int>(table.row[row][pos]) ? (float)get<int>(table.row[row][pos]) : table.row[row][pos];
}

bool check_condition(const Table& table,const Condition & con,const int row){
    int len=con.size();
    bool state=1;
    for(int i=0,now;i<len;i++){
        Table_content l,r;
        if(holds_alternative<string>(con[i].contentl)&&!con[i].is_string.first) l=get_cell(table,get<string>(con[i].contentl),row);
        else l=con[i].contentl;
        if(holds_alternative<string>(con[i].contentr)&&!con[i].is_string.second) r=get_cell(table,get<string>(con[i].contentr),row);
        else r=con[i].contentr;
        now=(make_comp(l,r,con[i].sign));
        if(con[i].pre_bool_op==OR) state=state||now;
        else if(con[i].pre_bool_op==AND) state=state&&now;
    }
    return state;
}

bool make_comp(Table_content a,Table_content b,Compare_sign op){
    if(op==EQUAL) return a==b;
    if(op==BIGER) return a>b;
    if(op==SMALLER) return a<b;
    if(op==NO_SAME) return a!=b;
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

bool _updata_set_where(Table& table,const Compute_paras & compara,const Condition & con){
    vector<int> comparapos;
    int lenr=table.row.size(),lenco=compara.size();
    for(int i=0;i<lenco;i++){
        comparapos.push_back(which_column(table,compara[i].target));
        // compute_translate_sentence(table,which_column_type(table,compara[i].target),compara[i].sentence);
    }
    for(int i=0;i<lenr;i++){
        if(!check_condition(table,con,i)) continue;
        for(int e=0,pos,tartype;e<lenco;e++){
            pos=comparapos[e];
            compute_translate_sentence(table,i,table.row[i][pos],table.data_type[pos],compara[e].sentence);
            // updata_compute(table,i,table.row[i][comparapos[e]],compara[e],table.data_type[comparapos[e]]);
        }
    }
    return 1;
}

float get_num(const Table& table,const int & i,const Com_content & target){
    if(target.index()!=TEXT) return get<float>(target);
    int e=which_column(table,get<string>(target));
    if(table.data_type[e]==INTEGER) return get<int>(table.row[i][e]);
    if(table.data_type[e]==FLOAT) return get<float>(table.row[i][e]);
    return 0;
}

Table& _select_from_inner_join_on(const Column_pos& pos1,const Column_pos& pos2,const Table& table1,Table& table2,const Column_pos&con1,const Column_pos&con2){
    Table ans;
    ans.Table_name=table2.Table_name;
    ans.column_name.push_back(pos1.column_name);
    ans.column_name.push_back(pos2.column_name);
    ans.data_type.push_back(which_column_type(table1,pos1.column_name));
    ans.data_type.push_back(which_column_type(table2,pos2.column_name));

    int cpos1=which_column(table1,con1.column_name),cpos2=which_column(table2,con2.column_name);
    int lr1=table1.row.size(),lr2=table2.row.size();
    int tag1=which_column(table1,pos1.column_name),tag2=which_column(table2,pos2.column_name);
    for(int i=0;i<lr1;i++){
        for(int e=0;e<lr2;e++){
            if(table1.row[i][cpos1]!=table2.row[e][cpos2]) continue;
            vector<Table_content> LL;
            LL.push_back(table1.row[i][tag1]);
            LL.push_back(table2.row[e][tag2]);
            ans.row.push_back(LL);
        }
    }
    table2=ans;
    return table2;
}

void compute_translate_sentence(Table& table,const int & R,Table_content & target,const Data_type _type, Com_contents sentence){
    if(_type==TEXT){
        target=get<string>(sentence[0]);
        return;
    }
    for(int i=sentence.size()-1;i>=0;i--){
        if(sentence[i].index()!=TEXT) continue;
        int colum=which_column(table,get<string>(sentence[i]));
        if(table.row[R][colum].index()==INTEGER) sentence[i]=(float)get<int>(table.row[R][colum]);
        if(table.row[R][colum].index()==FLOAT) sentence[i]=get<float>(table.row[R][colum]);
    }
    if(_type==INTEGER) target=(int)calculate_sentence(sentence);
    if(_type==FLOAT) target=(float)calculate_sentence(sentence);
}
