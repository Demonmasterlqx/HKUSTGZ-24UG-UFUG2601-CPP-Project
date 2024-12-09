#include<bits/stdc++.h>
#include "minSQL_struct.hpp"

#ifndef __CALCULATOR__
#define __CALCULATOR__
using namespace std;

typedef variant<float,Compute_op> Exprssion_content;
typedef vector<variant<float,Compute_op>> Exprssion_contents;

class Nifix_expression;
// class Postfix_notation;

int get_compute_op_level(const Compute_op& op);

float calculate_sentence(const Com_contents & sentence);

class Nifix_expression{
    public:
    Nifix_expression()=default;
    Nifix_expression(const Com_contents & sentence);
    float calculate();
    private:
    Exprssion_contents nifix_sentence;
};

#endif