#include "minSQL_struct.hpp"

Command_line get_command(ifstream & IN){
    string ans,in;
    IN>>ans;
    while(ans[ans.length()-1]!=';'){
        IN>>in;
        ans=ans+" "+in;
    }
    ans.erase(ans.begin()+ans.length()-1);
    int pos,len=ans.length();
    Command_type type;
    Parameter para;
    cout<<ans<<endl;
    cout<<len<<endl;
    if((pos=ans.find("CREATE DATABASE"))!=ans.npos){
        type=CREATE_DATABASE;
        para.push_back(ans.substr(pos+strlen("CREATE DATABASE")+1));
        // para.insert()
    }
    else if((pos=ans.find("USE DATABASE"))!=ans.npos){
        type=USE_DATABASE;
        para.push_back(ans.substr(pos+strlen("USE DATABASE")+1));
    }
    else if((pos=ans.find("CREATE TABLE"))!=ans.npos){
        type=CREATE_TABLE;
        //get name
        int pos3=ans.find('(')-1;
        if(ans[pos3]==' ') pos3--;
        pos=strlen("CREATE TABLE")+1;
        if(pos>pos3) return Command_line(ERROR_COMMAND,Parameter());
        para.push_back(ans.substr(pos,pos3-pos+1));
        // cout<<"NAME "<<ans.substr(pos,pos3-pos+1)<<endl;
        //get parameter in ()
        pos=ans.find('(')+1;
        int pos1=ans.find(',')-1;
        if(ans.find(')')==ans.npos||pos==ans.npos) return Command_line(ERROR_COMMAND,Parameter());
        while(ans[pos]==' '&&pos<len) pos++;
        while(ans[pos1]==' '&&pos1>0) pos1--;
        if(pos>pos1) return Command_line(ERROR_COMMAND,Parameter());
        while(1){
            // cout<<pos<<" "<<pos1<<endl;
            int pos2=ans.find(' ',pos);
            if(pos1<=pos2) return Command_line(ERROR_COMMAND,Parameter());
            if(ans.find(' ',pos2+1)<=pos1) return Command_line(ERROR_COMMAND,Parameter());
            para.push_back(ans.substr(pos,pos2-pos));
            para.push_back(ans.substr(pos2+1,pos1-pos2));
            pos=pos1+1;
            while((ans[pos]==' '||ans[pos]==',')&&pos<len) pos++;
            pos1=ans.find(',',pos);
            // cout<<pos<<" "<<pos1<<endl;
            if(pos1==ans.npos) break;
            pos1--;
            while(ans[pos1]==' '&&pos1>=0) pos1--;
            if(pos>pos1) Command_line(ERROR_COMMAND,Parameter());
        }
        pos1=ans.find(')',pos)-1;
        while(ans[pos1]==' '&&pos1>=0) pos1--;
        if(pos>pos1) Command_line(ERROR_COMMAND,Parameter());
        int pos2=ans.find(' ',pos);
        if(pos1<=pos2) return Command_line(ERROR_COMMAND,Parameter());
        if(ans.find(' ',pos2+1)<=pos1) return Command_line(ERROR_COMMAND,Parameter());
        para.push_back(ans.substr(pos,pos2-pos));
        para.push_back(ans.substr(pos2+1,pos1-pos2));
    }
    else if((pos=ans.find("DROP TABLE"))!=ans.npos){

    }
    else if((pos=ans.find("INSERT INTO"))!=ans.npos){

    }
    else if((pos=ans.find("SELECT"))!=ans.npos&&ans.find("INNER JOIN")!=ans.npos){

    }
    else if((pos=ans.find("SELECT"))!=ans.npos&&ans.find("INNER JOIN")==ans.npos){

    }
    else if((pos=ans.find("UPDATE"))!=ans.npos){

    }
    else if((pos=ans.find("DELETE FROM"))!=ans.npos){

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

// template<typename T,typename B>
// // requires std::same_as<B,Table_content> || std::same_as<B,Parameter_content>
// T get_variant_content(const B & C){
//     return get<get_Data_type(C)>(C);
// }

// template string get_variant_content<string,Parameter_content>(const Parameter_content& C);
//??????????????????????????
//gunhui .hpp dingyile


// template <typename... T>
// // requires std::same_as<T,Table_content> || std::same_as<T,Parameter_content>
// Data_type get_Data_type(const variant<T...> &C)
// {
//     if(C.index()==TEXT) return TEXT;
//     if(C.index()==INTEGER) return INTEGER;
//     if(C.index()==FLOAT) return FLOAT;
//     if(C.index()==CONDITION) return CONDITION;
//     return ERROR_TYPE;
// }
// template Data_type get_Data_type<string,int,float,condition>(const variant<string,int,float,condition> & C);
// template Data_type get_Data_type<string,int,float>(const variant<string,int,float> & C);
//??????????????????????????
//gunhui .hpp dingyile