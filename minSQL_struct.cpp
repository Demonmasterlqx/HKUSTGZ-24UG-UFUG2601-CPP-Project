#include "minSQL_struct.hpp"

Command_line get_command(ifstream & IN){
    stringstream input;
    string ans,para_string,para_string1,para_string2;
    char lin=0;bool pre_empty=1;
    Command_type type=ERROR_COMMAND;
    Parameter para;
    Condition condition;
    Set_configs sets;
    BOOL_OP pre_sign=AND;
    bool in_=0;
    auto get_bool_type=[](const string &para_string){
        if(para_string=="OR") return OR;
        else if(para_string=="AND") return AND;
        return ERROR_BOOL_OP;
    };
    while(1){
        lin=IN.get();
        // cout<<(int)lin<<endl;
        if(lin==-30) {IN.get(),IN.get();lin='\'';in_^=1;}
        if(in_||lin=='\''){
            if(pre_empty==1)input<<" ";
            input<<lin;
            pre_empty=0;
        }
        else{
            if(is_special(lin)){
                if(_is_empty(lin)) {if(pre_empty==0) input<<" ",pre_empty=1;}
                else {
                    if(pre_empty==0) input<<' '<<lin<<' ',pre_empty=1;
                    else input<<lin<<' ',pre_empty=1;
                }
            }
            else input<<lin,pre_empty=0;
        }
        if(lin==';') break;
    }
    ans=input.str();
    cout<<ans<<endl;
    if(ans.find("CREATE DATABASE")!=ans.npos){
        type=CREATE_DATABASE;
        input>>para_string>>para_string>>para_string;
        para.push_back(para_string);
        // para.insert()
    }
    else if(ans.find("USE DATABASE")!=ans.npos){
        type=USE_DATABASE;
        input>>para_string>>para_string>>para_string;
        para.push_back(para_string);
    }
    else if(ans.find("CREATE TABLE")!=ans.npos){
        type=CREATE_TABLE;
        input>>para_string>>para_string>>para_string;
        para.push_back(para_string);
        input>>para_string;
        if(para_string!="(") return Command_line(ERROR_COMMAND,Parameter());
        while(1){
            if(input.eof()) return Command_line(ERROR_COMMAND,Parameter());
            input>>para_string>>para_string1;
            if(is_special(para_string)||is_special(para_string1)) return Command_line(ERROR_COMMAND,Parameter());
            para.push_back(para_string);
            para.push_back(para_string1);
            input>>para_string;
            if(para_string==")") break;
            if(para_string!=",") return Command_line(ERROR_COMMAND,Parameter());
        }
    }
    else if(ans.find("DROP TABLE")!=ans.npos){
        type=DROP_TABLE;
        input>>para_string>>para_string>>para_string;
        para.push_back(para_string);
    }
    else if(ans.find("INSERT INTO")!=ans.npos){
        type=INSERT_INTO;
        input>>para_string>>para_string>>para_string;
        para.push_back(para_string);
        input>>para_string;
        if(para_string!="VALUES") return Command_line(ERROR_COMMAND,Parameter());
        input>>para_string;
        if(para_string!="(") return Command_line(ERROR_COMMAND,Parameter());
        while(1){
            input>>para_string;
            // if(is_special(para_string)) return Command_line(ERROR_COMMAND,Parameter());
            get_quotation_content(para_string,input);
            para.push_back(para_string);
            input>>para_string;
            if(para_string==")") break;
            if(para_string!=",") return Command_line(ERROR_COMMAND,Parameter());
        }
    }
    else if(ans.find("SELECT")!=ans.npos&&ans.find("INNER JOIN")!=ans.npos){
        type=SELECT_FROM_INNER_JOIN_ON;
        input>>para_string;//SELECT
        //get column
        input>>para_string>>para_string1>>para_string1;
        para.push_back(Column_pos(para_string));
        para.push_back(Column_pos(para_string1));
        input>>para_string; //FROM
        input>>para_string;
        para.push_back(para_string);
        input>>para_string>>para_string; //INNER JOIN
        input>>para_string;
        para.push_back(para_string);
        input>>para_string; //ON
        input>>para_string>>para_string1>>para_string2;
        para.push_back(Column_pos(para_string));
        para.push_back(Column_pos(para_string2));
    }
    else if(ans.find("SELECT")!=ans.npos&&ans.find("INNER JOIN")==ans.npos){
        type=SELECT_FROM;
        input>>para_string;
        if(!input.eof()) input>>para_string;
        else return Command_line(ERROR_COMMAND,Parameter());
        if(para_string=="*"){
            para.push_back(para_string);
            if(!input.eof()) input>>para_string;
            else return Command_line(ERROR_COMMAND,Parameter());
            if(para_string!="FROM") return Command_line(ERROR_COMMAND,Parameter());
        }
        else{
            para.push_back(para_string);
            while(1){
                if(!input.eof()) input>>para_string;
                else return Command_line(ERROR_COMMAND,Parameter());
                if(para_string=="FROM") break;
                else if(para_string==",") continue;
                para.push_back(para_string);
            }
        }
        input>>para_string;
        para.push_back(para_string);
        //where
        input>>para_string;
        if(para_string=="WHERE"){
            while(1){
                input>>para_string>>para_string1>>para_string2;
                get_quotation_content(para_string2,input);
                condition.push_back(Condition_parameter(pre_sign,para_string,para_string2,para_string1));
                input>>para_string;
                if(para_string==";") break;
                pre_sign=get_bool_type(para_string);
            }
            para.push_back(condition);
        }
        else{
            para.push_back(condition);
        }
    }
    else if(ans.find("UPDATE")!=ans.npos){
        type=UPDATE_SET_WHERE;
        input>>para_string; //UPDATE
        input>>para_string;
        para.push_back(para_string);
        input>>para_string; //SET
        while(1){
            input>>para_string>>para_string1>>para_string2;
            sets.push_back(Set_config(para_string,para_string2));
            input>>para_string;
            if(para_string=="WHERE"||para_string==";") break;
        }// WHERE
        para.push_back(sets);
        if(para_string=="WHERE"){
            while(1){
                input>>para_string>>para_string1>>para_string2;
                get_quotation_content(para_string2,input);
                condition.push_back(Condition_parameter(pre_sign,para_string,para_string2,para_string1));
                input>>para_string;
                if(para_string==";") break;
                pre_sign=get_bool_type(para_string);
            }
            para.push_back(condition);
        }
        else{
            para.push_back(condition);
        }
    }
    else if(ans.find("DELETE FROM")!=ans.npos){
        type=DELETE_FROM_WHERE;
        input>>para_string>>para_string;//DELETE FROM
        input>>para_string;
        para.push_back(para_string);
        //where
        input>>para_string;
        if(para_string=="WHERE"){
            while(1){
                input>>para_string>>para_string1>>para_string2;
                get_quotation_content(para_string2,input);
                condition.push_back(Condition_parameter(pre_sign,para_string,para_string2,para_string1));
                input>>para_string;
                if(para_string==";") break;
                pre_sign=get_bool_type(para_string);
            }
            para.push_back(condition);
        }
        else{
            para.push_back(condition);
        }
    }
    else {return Command_line(ERROR_COMMAND,Parameter());}
    return Command_line(type,para);
}

Command_line::Command_line(Command_type G,Parameter F=Parameter()){
    command_type=G;
    parameter=F;
}

Command_type Command_line::get_command_type(){
    return command_type;
}

Parameter Command_line::get_parameter(){
    return parameter;
}

bool is_special(const char & a){
    return a==' '||a=='\n'||a==')'||a=='('||a==','||a=='\''||a==';'||a=='<'||a=='='||a=='>';
}

bool _is_empty(const char & a){
    return a==' '||a=='\n';
}

bool is_special(const string & a){
    if(a.length()>=2) return 0;
    return !(a[0]=='_'||('a'<=a[0]&&a[0]<'z')||('A'<=a[0]&&a[0]<'Z')||('1'<=a[0]&&a[0]<'9'));
}

Condition_parameter::Condition_parameter(const BOOL_OP &pre,const string &v1,const string &v2,const string & op){
    pre_bool_op=pre;
    column=Column_pos(v1);
    auto get_type=[&op](){
        if(op=="<") return SMALLER;
        if(op=="=") return EQUAL;
        if(op==">") return BIGER;
        return ERROR_COMPARE_SIGN;
    };
    sign=get_type();
    content=v2;
}

// Column_pos::Column_pos(const string &v1,const string& v2){
//     table_name=v1;
//     column_name=v2;
// }

ostream & operator<<(ostream& a,const Condition_parameter& b){
    a<<b.column.table_name<<"."<<b.column.column_name<<" "<<b.sign<<" "<<b.content;
    return a;
}

ostream & operator<<(ostream& a,const Condition& b){
    a<<"THERE ARE "<<b.size()<<" CONDITIONS"<<endl;
    for(auto i : b){
        a<<i;
    }
    return a;
}

ostream & operator<<(ostream& a,const Column_pos& b){
    a<<b.table_name<<"."<<b.column_name;
    return a;
}

ostream & operator<<(ostream& a,const Table_content& b){
    if(b.index()==TEXT) a<<get<TEXT>(b);
    if(b.index()==INTEGER) a<<get<INTEGER>(b);
    if(b.index()==FLOAT) a<<get<FLOAT>(b);
    return a;
}

ostream & operator<<(ostream& a,const Set_config& b){
    a<<b.column<<"="<<b.content;
    return a;
}

ostream & operator<<(ostream& a,const Set_configs& b){
    a<<"THERE ARE "<<b.size()<<" SETS\n";
    for(auto i : b){
        cout<<i<<endl;
    }
    return a;
}

Column_pos::Column_pos(const string& v1){
    int pos=v1.find('.');
    if(pos==v1.npos) table_name="",column_name=v1;
    else table_name=v1.substr(0,pos),column_name=v1.substr(pos+1);;
}

Set_config::Set_config(const string & v1,const string & v2){
    column=v1;
    content=v2;
}

void get_quotation_content(string & a,stringstream & input){
    if(a[0]!='\'') return;
    for(int i=a.length();i;i--) input.unget();
    input.get();
    char lin=0;
    stringstream G;
    while(1){
        lin=input.get();
        if(lin=='\'') break;
        G<<lin;
    }
    a=G.str();
}

// Set_config::Set_config(const string & v1){
//     int pos=v1.find(".");
    
// }