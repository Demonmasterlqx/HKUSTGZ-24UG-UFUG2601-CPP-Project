#include "minSQL_struct.hpp"

bool _create_table(Database & base,const string &name,const vector<string>& cname,const vector<Data_type>& ty){
    int top=base.data.size();
    for(int i=0;i<top;i++) if(name==base.data[i].Table_name) return 0;
    base.data.push_back(Table(cname,ty));
}

Data_type what_type(const string & a){
    if(a=="TEXT") return TEXT;
    if(a=="INTEGER") return INTEGER;
    if(a=="FLOAT") return FLOAT;
    return ERROR_TYPE;
}