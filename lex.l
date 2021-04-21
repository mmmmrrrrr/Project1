%{
#include<stdio.h>
#include<iostream>
#include<fstream>
using namespace std;
extern "C"//为了能够在C++程序里面调用C函数，必须把每一个需要使用的C函数，其声明都包括在extern "C"{}块里面，这样C++链接时才能成功链接它们。extern "C"用来在C++环境下设置C链接类型。
{	//yacc.y中也有类似的这段extern "C"，可以把它们合并成一段，放到共同的头文件main.h中
	int yywrap(void);
	int yylex(void);//这个是lex生成的词法分析函数，yacc的yyparse()里会调用它，如果这里不声明，生成的yacc.tab.c在编译时会找不到该函数
	
}

void print_f(int code,char* str, int yyleng ,ostream& yyout);
int col=1, line=1;
char ch;
enum Code
{
	/*终结符*/
	error_note = 0,
	white_=1,//换行空白
	note_,//注释
	error_,//错误
	EOF_, //文本结束
	null_,//空
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
	fnum,    //整数
	digits,  //浮点数
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
	mulop_div,   //整除
	mulop_mod,
	mulop_mul,
	mulop_divide,//浮点除法
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
	letter,      //字符常量  如‘a’‘1’
	Boundary,    //---终结符与非终结符的分界线---
	/*非终结符*/
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
	factor
};
%}

%option   caseless 
%option c++ 
%option noyywrap

digit [0-9]
letter_ [a-zA-Z]
id {letter_}({letter_}|{digit})*
digits {digit}{digit}*
fnum -?{digit}+(\.{digit}+)([Ee][-+][0-9]+)?
whitespace ([ \t\r]+)
newline [\n]
letter (\'[^\n\r\t']*\')
note (\{[^'\t\n\}]*\})
errorNote (\{[^'\t\n\}]*\n)
illegal ([^a-zA-Z0-9\t\n>=<:\'\(\)\;\[\]\.\!\+\-\{\/\*\"\,]+)|([+-]?[[:digit:]]+[[:alpha:]])
%%

EOF {enum Code code = EOF_; print_f(code,yytext,yyleng,yyout);}
null {enum Code code = null_; print_f(code,yytext,yyleng,yyout);}
program {enum Code code = program_; print_f(code,yytext,yyleng,yyout);}
const {enum Code code = const_; print_f(code,yytext,yyleng,yyout);}
var {enum Code code = var_; print_f(code,yytext,yyleng,yyout);}
array {enum Code code = array_; print_f(code,yytext,yyleng,yyout);}
of {enum Code code = of_; print_f(code,yytext,yyleng,yyout);}
integer {enum Code code = integer_; print_f(code,yytext,yyleng,yyout);}
real {enum Code code = real_; print_f(code,yytext,yyleng,yyout);}
boolean {enum Code code = boolean_; print_f(code,yytext,yyleng,yyout);}
char {enum Code code = char_; print_f(code,yytext,yyleng,yyout);}
procedure {enum Code code = procedure_; print_f(code,yytext,yyleng,yyout);}
function {enum Code code = function_; print_f(code,yytext,yyleng,yyout);}
begin {enum Code code =  begin_; print_f(code,yytext,yyleng,yyout);}
end {enum Code code = end_; print_f(code,yytext,yyleng,yyout);}
if {enum Code code = if_; print_f(code,yytext,yyleng,yyout);}
then {enum Code code = then_; print_f(code,yytext,yyleng,yyout);}
for {enum Code code = for_; print_f(code,yytext,yyleng,yyout);}
to {enum Code code = to_; print_f(code,yytext,yyleng,yyout);}
do {enum Code code = do_; print_f(code,yytext,yyleng,yyout);}
read {enum Code code = read_; print_f(code,yytext,yyleng,yyout);}
write {enum Code code = write_; print_f(code,yytext,yyleng,yyout);}
else {enum Code code = else_; print_f(code,yytext,yyleng,yyout);}
not {enum Code code = not_; print_f(code,yytext,yyleng,yyout);}
uminus {enum Code code = uminus_; print_f(code,yytext,yyleng,yyout);}

or {enum Code code = addop_or; print_f(code,yytext,yyleng,yyout);}
mod  {enum Code code = mulop_mod; print_f(code,yytext,yyleng,yyout);}
and  {enum Code code = mulop_and; print_f(code,yytext,yyleng,yyout);}
div  {enum Code code = mulop_div; print_f(code,yytext,yyleng,yyout);}
{errorNote} {enum Code code = error_note; print_f(code,yytext,yyleng,yyout);}
{whitespace} { col+=1;
}
{newline} {line+=1; col = 1;}

{digits} {
	if(yyleng > 32)
	{
		printf("Error! The length of this number is too long!\n");
		enum Code code = error_;print_f(code,yytext,yyleng,yyout);
		exit(0);
	} 
	enum Code code = digits; print_f(code,yytext,yyleng,yyout);
}
{fnum} {
	if(yyleng > 32)
	{
		printf("Error! The length of this number is too long!\n");
		enum Code code = error_;print_f(code,yytext,yyleng,yyout);
		exit(0);
	} 
	enum Code code = fnum; print_f(code,yytext,yyleng,yyout);
}
{letter} {
	if(yyleng > 3){
		/*char类型  长度为3*/
		printf("The length of char must be 1.");
		enum Code code = error_; print_f(code,yytext,yyleng,yyout);
	}
	enum Code code = letter; print_f(code,yytext,yyleng,yyout);
}
{note} {
	
	/*不传递给词法*/
}
{id} {
	/*检查长度是否超出范围*/
	if(yyleng > 32){
		printf("Error! The length of this idetifier is too long!\n");
		enum Code code = error_;print_f(code,yytext,yyleng,yyout);
		exit(0);
	}
	enum Code code = id; print_f(code,yytext,yyleng,yyout);
}

"+" {enum Code code = addop_add; print_f(code,yytext,yyleng,yyout);}
"-" {enum Code code = addop_sub; print_f(code,yytext,yyleng,yyout);}
"*" {enum Code code = mulop_mul; print_f(code,yytext,yyleng,yyout);}
"/" {enum Code code = mulop_divide; print_f(code,yytext,yyleng,yyout);}

":=" {enum Code code = assignop; print_f(code,yytext,yyleng,yyout);}
"?" {enum Code code = punc_question; print_f(code,yytext,yyleng,yyout);}
"~" {enum Code code = punc_not; print_f(code,yytext,yyleng,yyout);}
"," {enum Code code = punc_comma; print_f(code,yytext,yyleng,yyout);}
";" {enum Code code = punc_semicolon; print_f(code,yytext,yyleng,yyout);}
":" {enum Code code = punc_colon; print_f(code,yytext,yyleng,yyout);}
"." {enum Code code = punc_point; print_f(code,yytext,yyleng,yyout);}
"(" {enum Code code = punc_round_left; print_f(code,yytext,yyleng,yyout);}
")" {enum Code code = punc_round_right; print_f(code,yytext,yyleng,yyout);}
"\[" {enum Code code = punc_square_left; print_f(code,yytext,yyleng,yyout);}
"\]" {enum Code code = punc_square_right; print_f(code,yytext,yyleng,yyout);}
"=" {enum Code code = relop_e; print_f(code,yytext,yyleng,yyout);}
"<>" {enum Code code = relop_ne; print_f(code,yytext,yyleng,yyout);}
"<" {enum Code code = relop_l; print_f(code,yytext,yyleng,yyout);}
"<=" {enum Code code = relop_le; print_f(code,yytext,yyleng,yyout);}
">" {enum Code code = relop_g; print_f(code,yytext,yyleng,yyout);}
">=" {enum Code code = relop_ge; print_f(code,yytext,yyleng,yyout);}

"\'" {enum Code code = single_quote; print_f(code,yytext,yyleng,yyout);}
	
{illegal} {
	/*含有非法字符的字符串*/
	/*数字开头的id*/
	/*real里是错误类型*/
	enum Code code = error_; print_f(code,yytext,yyleng,yyout);
	
}
%%


void print_f(int code,char* str,int yyleng,ostream& yyout)
{
	if(code==letter)
		yyout<<code-4<<" "<<line<<" "<<str<<endl;
		
	else if(code == error_note)
{

str[yyleng-1]='\0';
yyout<<code-4<<" "<<line<<" "<<str<<endl;
line+=1;
}
	else
{
 int i;
 for(i=0;i<yyleng;i++)
 {
   if(str[i]>='A'&& str[i]<='Z')
        str[i] = str[i]+'a'-'A'; 
 }
yyout<<code-4<<" "<<line<<" "<<str<<endl;
}
}
int main()
{
	ifstream ifile("test.pas");
	ofstream ofile("lexOut.txt");
 	yyFlexLexer yy;
	yy.switch_streams(ifile, ofile);
        printf("[I]>>>Read data from file:test.pas\n");
  
  yy.yylex();
  return 0;
}

