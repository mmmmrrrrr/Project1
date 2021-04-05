0 S->programstruct
1 programstruct->program_head punc_semicolon program_body punc_point
2 program_head->program id punc_round_left idlist punc_round_right
3 program_head->program id
4 program_body->const_declarations var_declarations subprogram_declarations compound_statement
5 idlist->idlist punc_comma id
6 idlist->id
7 const_declarations->const const_declaration punc_semicolon
8 const_declarations->null
9 const_declaration->const_declaration punc_semicolon id relop_e const_value
10 const_declaration->id relop_e const_value
11 const_value->addop_add num
12 const_value->addop_sub num
13 const_value->num
14 const_value->letter
15 var_declarations->var var_declaration punc_semicolon
16 var_declarations->null
17 var_declaration->var_declaration punc_semicolon idlist punc_colon type
18 var_declaration->idlist punc_colon type
19 type->basic_type
20 type->array punc_square_left period punc_square_right of basic_type
21 basic_type->integer
22 basic_type->real
23 basic_type->boolean
24 basic_type->char
25 period->period punc_comma digits punc_point punc_point digits
26 period->digits punc_point punc_point digits
27 subprogram_declarations->subprogram_declarations subprogram punc_semicolon
28 subprogram_declarations->null
29 subprogram->subprogram_head punc_semicolon subprogram_body
30 subprogram_head->procedure id formal_parameter
31 subprogram_head->function id formal_parameter punc_colon basic_type
32 formal_parameter->punc_round_left parameter_list punc_round_right
33 formal_parameter->null
34 parameter_list->parameter_list punc_semicolon parameter
35 parameter_list->parameter
36 parameter->var_parameter
37 parameter->value_parameter
38 var_parameter->var value_parameter
39 value_parameter->idlist punc_colon basic_type
40 subprogram_body->const_declarations var_declarations compound_statement
41 compound_statement->begin statement_list end
42 statement_list->statement_list punc_semicolon statement
43 statement_list->statement
44 statement->variable assignop expression
45 statement->procedure_call
46 statement->compound_statement
47 statement->if expression then statement else_part
48 statement->for id assignop expression to expression do statement
49 statement->read punc_round_left variable_list punc_round_right
50 statement->write punc_round_left expression_list punc_round_right
51 statement->null
52 variable_list->variable_list punc_comma variable
53 variable_list->variable
54 variable->id id_varpart
55 id_varpart->punc_square_left expression_list puncsquare_right
56 id_varpart->null
57 procedure_call->id
58 procedure_call->id punc_round_left expression_list punc_round_right
59 else_part->else statement
60 else_part->null
61 expression_list->expression_list punc_comma expression
62 expression_list->expression
63 expression->simple_expression relop simple_expression
64 expression->simple_expression
65 simple_expression->simple_expression addop term
66 simple_expression->term
67 term->term mulop factor
68 term->factor
69 factor->num
70 factor->variable
71 factor->id punc_round_left expression_list punc_round_right
72 factor->punc_round_left expression punc_round_right
73 factor->not factor
74 factor->addop_sub factor
75 relop->relop_e
76 relop->relop_ne
77 relop->relop_l
78 relop->relop_le
79 relop->relop_g
80 relop->relop_ge
81 addop->addop_add
82 addop->addop_sub
83 addop->addop_or
84 num->fnum
85 num->digits
86 mulop->mulop_mul
87 mulop->mulop_divide
88 mulop->mulop_div
89 mulop->mulop_mod
90 mulop->mulop_and