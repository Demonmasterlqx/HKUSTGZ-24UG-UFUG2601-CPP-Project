#include<bits/stdc++.h>
#include "minSQL_struct.hpp"
#include "calculator.hpp"

int get_compute_op_level(const Compute_op& op){
    if(op==ADD||op==SUB) return 1;
    if(op==MUT||op==DIV) return 2;
    if(op==GET_NEGATIVE) return 3;
    return 0;
}

//保持sentence的类型只有float和string
Nifix_expression::Nifix_expression(const Com_contents & sentence){
    stack<Compute_op> op;
    for(const auto& i:sentence){
        if(holds_alternative<Compute_op>(i)){
            Compute_op now=get<Compute_op>(i);
            int now_level=get_compute_op_level(now);
            if(now==LEFT_PARENTHESIS){
                op.push(now);
                continue;
            }
            if(now==RIGHT_PARENTHESIS){
                while(op.top()!=LEFT_PARENTHESIS) nifix_sentence.push_back(op.top()),op.pop();
                op.pop();
                continue;
            }
            while(!op.empty()&&get_compute_op_level(op.top())>=now_level){
                nifix_sentence.push_back(op.top()),op.pop();
            }
            op.push(now);
        }
        else nifix_sentence.push_back(get<float>(i));
    }
    while(!op.empty()) nifix_sentence.push_back(op.top()),op.pop();
}

float Nifix_expression::calculate(){
    stack<float> num;
    for(auto i : nifix_sentence){
        if(holds_alternative<Compute_op>(i)){
            auto now=get<Compute_op>(i);
            if(now==GET_NEGATIVE){
                float opnum=num.top();
                num.pop(),num.push(-opnum);
            }
            else {
                float opnuml,opnumr;
                opnumr=num.top(),num.pop();
                opnuml=num.top(),num.pop();
                switch (now){
                case ADD: num.push(opnuml+opnumr);break;
                case SUB: num.push(opnuml-opnumr);break;
                case MUT: num.push(opnuml*opnumr);break;
                case DIV: num.push(opnuml/opnumr);break;
                }
            }
        }
        else num.push(get<float>(i));
    }
    return num.top();
}

float calculate_sentence(const Com_contents & sentence){
    return Nifix_expression(sentence).calculate();
}