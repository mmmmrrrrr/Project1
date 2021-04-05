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
	/*�ս��*/
	EOF_, //�ı�����
	null_,//��
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
	fnum,    //����
	digits,  //������
	//letter,
	assignop,
	punc_question, //?
	punc_not,      //~
	punc_comma,    //,
	punc_semicolon,//;
	punc_colon,    //:
	punc_point,    //.
	punc_round_left,  //(
	punc_round_right, //)
	punc_square_left, //[
	punc_square_right,//]
	mulop_and,
	mulop_div,   //����
	mulop_mod,
	mulop_mul,
	mulop_divide,//�������
	addop_or,
	addop_add,
	addop_sub,
	relop_e,     //=
	relop_ne,    //<>
	relop_l,     //<
	relop_le,    //<=
	relop_g,     //>
	relop_ge,    //>=
	single_quote,//'
	letter,      //�ַ�����  �确a����1��
	Boundary,    //---�ս������ս���ķֽ���---
	/*���ս��*/
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

	//NOW //DFA�еĵ� .
};

typedef vector<mark> sentence;    //ÿһ���﷨����
typedef vector<sentence> Grammar; //�����﷨
typedef map<mark, set<mark>> fset;//��first����follow��
typedef vector<vector<int>> LR_PredictTable;    //LRԤ�������      
typedef struct {
	vector<sentence> sentences;   //������
	vector<int> point_pos;        //���λ��,�����һ��������point_pos���ķ���
}closure;
typedef struct{
	int from;  
	mark by;   
	int to;
}Go;                              //go(from,by)=to
typedef struct {
	int mark; //��Ӧ�Ǻŵ�C++ö��ֵ
	int line; //�к�
	string content; //�Ǻ�����
}token;

vector<token> getToken();
Grammar initGrammer();                           //�����﷨����
fset getFIRST(Grammar grammar);                  //����FIRST��
fset getFOLLOW(Grammar grammar, fset first);     //����FOLLOW��
LR_PredictTable getTable(Grammar grammar);       //����LR������
vector<int> control_program(LR_PredictTable LRtable, Grammar grammar,vector<token>tokens); //���з�������

void extend_closure(Grammar grammer, closure &c);//��ɱհ�����չ
bool in_closure(closure c, sentence s, int pos); //�ж�һ���հ����Ƿ���������

#endif