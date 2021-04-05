#ifndef GRAMMAR
#define GRAMMAR

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

enum mark
{
	
	EOF_, 
	null_,
	program_,
	const_,
	var_,
	array_,
	of_,
	integer_,
	real_,
	boolean_,
	char_,
	procedure_,
	function_,
	begin_,
	end_,
	if_,
	then_,
	for_,
	to_,
	do_,
	read_,
	write_,
	else_,
	not_,
	uminus_,
	id,
	fnum,    
	digits,  
	
	assignop,
	punc_question, 
	punc_not,      
	punc_comma,    
	punc_semicolon,
	punc_colon,    
	punc_point,    
	punc_round_left,  
	punc_round_right, 
	punc_square_left, 
	punc_square_right,
	mulop_and,
	mulop_div,   
	mulop_mod,
	mulop_mul,
	mulop_divide,
	addop_or,
	addop_add,
	addop_sub,
	relop_e,     
	relop_ne,    
	relop_l,     
	relop_le,    
	relop_g,     
	relop_ge,    
	single_quote,
	letter,      
	Boundary,    
	S,
	programstruct,
	program_head,
	program_body,
	idlist,
	mulop,
	relop,
	addop,
	num,
	const_declarations,
	const_declaration,
	const_value,
	var_declarations,
	var_declaration,
	type,
	basic_type,
	period,
	subprogram_declarations,
	subprogram,
	subprogram_head,
	formal_parameter,
	parameter_list,
	parameter,
	var_parameter,
	value_parameter,
	subprogram_body,
	compound_statement,
	statement_list,
	statement,
	variable_list,
	variable,
	id_varpart,
	procedure_call,
	else_part,
	expression_list,
	expression,
	simple_expression,
	term,
	factor,

	
};

typedef vector<mark> sentence;    
typedef vector<sentence> Grammar; 
typedef map<mark, set<mark>> fset;
typedef vector<vector<int>> LR_PredictTable;    
typedef struct {
	vector<sentence> sentences;   
	vector<int> point_pos;        
}closure;
typedef struct{
	int from;  
	mark by;   
	int to;
}Go;                              
typedef struct {
	int mark; 
	int line; 
	string content; 
}token;

vector<token> getToken();
Grammar initGrammer();                           
fset getFIRST(Grammar grammar);                  
fset getFOLLOW(Grammar grammar, fset first);     
LR_PredictTable getTable(Grammar grammar);       
vector<int> control_program(LR_PredictTable LRtable, Grammar grammar,vector<token>tokens); 

void extend_closure(Grammar grammer, closure &c);
bool in_closure(closure c, sentence s, int pos); 
#endif