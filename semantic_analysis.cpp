#include "semantic_analysis.h"
#include "grammar_analysis.h"
void encouter_an_error(string error_information, int line)
{
	cout << error_information << "in line " << line << "." << endl;
	exit(0);
}
bool Data_Type::operator==(const Data_Type &t) const
{
	if (t.basic_type == basic_type)
		return 1;
	else if (max(t.basic_type, basic_type) < 2)
		return 1;
	else
		return 0;
}
Identifier search_identifier(string name)
{
	for (Id_Table *t = now_ID_table; t != NULL; t = t->father)
	{
		if (t->name_to_id.find(name) != t->name_to_id.end())
			return t->name_to_id[name];
	}
	return error_id;
} //根据名字进行查找，返回标识符信息
bool insert_identifier(Identifier id)
{
	if (now_ID_table->name_to_id.find(id.name) != now_ID_table->name_to_id.end())
		return 0;
	now_ID_table->name_to_id[id.name] = id;
	return 1;
} //返回1表示正确插入，返回0表示已经有重复的标识符
void location()
{
	auto t = now_ID_table;
	now_ID_table = new Id_Table;
	now_ID_table->father = t;
} //定位
void relocation()
{
	auto t = now_ID_table->father;
	delete now_ID_table;
	now_ID_table = t;
} //重定位
void init_ID_Table()
{
	error_id.is_error = 1;
	location();
}
void semantic_analysis(vector<int> &product_sequence, vector<token> token_sequence)
{
	Identifier temp, temp1;
	int token_sequence_position = 0;
	string operator_stack_top;
	for (auto i : product_sequence)
	{
		cout << i << ' ';
		if (i)
			cout << token_sequence[token_sequence_position].content << endl;
		switch (i)
		{
		case 0: // S->programstruct
			relocation();
			cout << token_sequence_position << ' ' << token_sequence.size() << endl;
			if (token_sequence_position != token_sequence.size())
				cout << "not use out" << endl;
			else
				cout << "use out" << endl;
			return;
			break;

		case 1: // programstruct->program_head punc_semicolon program_body punc_point
			token_sequence_position += 2;
			break;

		case 2: // program_head->program id punc_round_left idlist punc_round_right
			Id_stack.clear();
			token_sequence_position += 4;
			break;

		case 3: // program_head->program id
			token_sequence_position += 2;
			break;

		case 4: // program_body->const_declarations var_declarations subprogram_declarations compound_statement
			break;

		case 5: // idlist->idlist punc_comma id
			temp = temp1;
			temp.name = token_sequence[token_sequence_position + 1].content;
			Id_stack.push_back(temp);
			token_sequence_position += 2;
			break;

		case 6: // idlist->id
			temp = temp1;
			temp.name = token_sequence[token_sequence_position].content;
			Id_stack.push_back(temp);
			token_sequence_position += 1;
			break;

		case 7: // const_declarations->const const_declaration punc_semicolon
			token_sequence_position += 2;
			break;

		case 8: // const_declarations->null
			break;

		case 9: // const_declaration->const_declaration punc_semicolon id relop_e const_value
			Id_stack[Id_stack.size() - 1].name = token_sequence[token_sequence_position + 1].content;
			insert_identifier(Id_stack[Id_stack.size() - 1]);
			Id_stack.pop_back();
			token_sequence_position += 3;
			break;

		case 10: // const_declaration->id relop_e const_value
			Id_stack[Id_stack.size() - 1].name = token_sequence[token_sequence_position].content;
			insert_identifier(Id_stack[Id_stack.size() - 1]);
			Id_stack.pop_back();
			token_sequence_position += 2;
			break;

		case 11: // const_value->addop_add num
			Id_stack[Id_stack.size() - 1].identifer_type = _constant;
			break;

		case 12: // const_value->addop_sub num
			Id_stack[Id_stack.size() - 1].identifer_type = _constant;
			Id_stack[Id_stack.size() - 1].data_type.constant_value =
				-Id_stack[Id_stack.size() - 1].data_type.constant_value;
			break;

		case 13: // const_value->num
			Id_stack[Id_stack.size() - 1].identifer_type = _constant;
			break;

		case 14: // const_value->letter
			temp.data_type.basic_type = _char;
			temp.identifer_type = _constant;
			Id_stack.push_back(temp);
			token_sequence_position += 1;
			break;

		case 15: // var_declarations->var var_declaration punc_semicolon
			token_sequence_position += 2;
			break;

		case 16: // var_declarations->null
			break;

		case 17: // var_declaration->var_declaration punc_semicolon idlist punc_colon type
			temp = Id_stack[Id_stack.size() - 1];
			Id_stack.pop_back();
			while (Id_stack.size())
			{
				Id_stack[Id_stack.size() - 1].data_type = temp.data_type;
				Id_stack[Id_stack.size() - 1].identifer_type = _variable;
				if (!insert_identifier(Id_stack[Id_stack.size() - 1]))
				{
					encouter_an_error("repeated definition", token_sequence[token_sequence_position].line);
					return;
				}
				Id_stack.pop_back();
			}
			token_sequence_position += 2;
			break;

		case 18: // var_declaration->idlist punc_colon type
			temp = Id_stack[Id_stack.size() - 1];
			Id_stack.pop_back();
			while (Id_stack.size())
			{
				Id_stack[Id_stack.size() - 1].data_type = temp.data_type;
				Id_stack[Id_stack.size() - 1].identifer_type = _variable;
				if (!insert_identifier(Id_stack[Id_stack.size() - 1]))
				{
					encouter_an_error("repeated definition", token_sequence[token_sequence_position].line);
					return;
				}
				Id_stack.pop_back();
			}
			token_sequence_position += 1;
			break;

		case 19: // type->basic_type
			break;

		case 20: // type->array punc_square_left period punc_square_right of basic_type

			token_sequence_position += 4;
			break;

		case 21: // basic_type->integer
			temp = temp1;
			temp.data_type.basic_type = _integer;
			Id_stack.push_back(temp);
			token_sequence_position += 1;
			break;

		case 22: // basic_type->real
			temp = temp1;
			temp.data_type.basic_type = _real;
			Id_stack.push_back(temp);
			token_sequence_position += 1;
			break;

		case 23: // basic_type->boolean
			temp = temp1;
			temp.data_type.basic_type = _boolean;
			Id_stack.push_back(temp);
			token_sequence_position += 1;
			break;

		case 24: // basic_type->char
			temp = temp1;
			temp.data_type.basic_type = _char;
			Id_stack.push_back(temp);
			token_sequence_position += 1;
			break;

		case 25: // period->period punc_comma digits punc_point punc_point digits
			token_sequence_position += 5;
			break;

		case 26: // period->digits punc_point punc_point digits
			token_sequence_position += 4;
			break;

		case 27: // subprogram_declarations->subprogram_declarations subprogram punc_semicolon
			break;

		case 28: // subprogram_declarations->null
			break;

		case 29: // subprogram->subprogram_head punc_semicolon subprogram_body
			break;

		case 30: // subprogram_head->procedure id formal_parameter
			break;

		case 31: // subprogram_head->function id formal_parameter punc_colon basic_type
			break;

		case 32: // formal_parameter->punc_round_left parameter_list punc_round_right
			break;

		case 33: // formal_parameter->null
			break;

		case 34: // parameter_list->parameter_list punc_semicolon parameter
			break;

		case 35: // parameter_list->parameter
			break;

		case 36: // parameter->var_parameter
			break;

		case 37: // parameter->value_parameter
			break;

		case 38: // var_parameter->var value_parameter
			break;

		case 39: // value_parameter->idlist punc_colon basic_type
			break;

		case 40: // subprogram_body->const_declarations var_declarations compound_statement
			break;

		case 41: // compound_statement->begin statement_list end
			token_sequence_position += 2;
			break;

		case 42: // statement_list->statement_list punc_semicolon statement
			token_sequence_position += 1;
			break;

		case 43: // statement_list->statement
			break;

		case 44: // statement->variable assignop expression
			if (Id_stack[Id_stack.size() - 1].data_type == Id_stack[Id_stack.size() - 2].data_type)
			{
				if (Id_stack[Id_stack.size() - 2].data_type.basic_type == _real &&
					Id_stack[Id_stack.size() - 1].data_type.basic_type == _integer)
					encouter_an_error("different assignment types.", token_sequence[token_sequence_position].line);
			}
			else
				encouter_an_error("different assignment types.", token_sequence[token_sequence_position].line);
			Id_stack.pop_back();
			Id_stack.pop_back();
			++token_sequence_position;
			break;

		case 45: // statement->procedure_call
			break;

		case 46: // statement->compound_statement
			break;

		case 47: // statement->if expression then statement else_part
			break;

		case 48: // statement->for id assignop expression to expression do statement
			break;

		case 49: // statement->read punc_round_left variable_list punc_round_right
			break;

		case 50: // statement->write punc_round_left expression_list punc_round_right
			break;

		case 51: // statement->null
			break;

		case 52: // variable_list->variable_list punc_comma variable
			break;

		case 53: // variable_list->variable
			break;

		case 54: // variable->id id_varpart

			Id_stack[Id_stack.size() - 1].name = token_sequence[token_sequence_position].content;
			cout << "---" << token_sequence[token_sequence_position].content << endl;

			temp = search_identifier(Id_stack[Id_stack.size() - 1].name);
			if (temp.is_error)
			{
				encouter_an_error("use undefined variable.", token_sequence[token_sequence_position].line);
			}
			if (Id_stack[Id_stack.size() - 1].identifer_type = _constant)
			{
				encouter_an_error("use constant error.", token_sequence[token_sequence_position].line);
			}
			if (temp.data_type.dimension != Id_stack[Id_stack.size() - 1].data_type.dimension)
			{
				encouter_an_error("use array error.", token_sequence[token_sequence_position].line);
			}
			Id_stack[Id_stack.size() - 1] = temp;
			token_sequence_position+=1;
			break;

		case 55: // id_varpart->punc_square_left expression_list puncsquare_right
			break;

		case 56: // id_varpart->null
			temp = temp1;
			Id_stack.push_back(temp);
			break;

		case 57: // procedure_call->id
			break;

		case 58: // procedure_call->id punc_round_left expression_list punc_round_right
			break;

		case 59: // else_part->else statement
			break;

		case 60: // else_part->null
			break;

		case 61: // expression_list->expression_list punc_comma expression
			break;

		case 62: // expression_list->expression
			break;

		case 63: // expression->simple_expression relop simple_expression
			if (Id_stack[Id_stack.size() - 1].data_type == Id_stack[Id_stack.size() - 2].data_type)
				;
			else
				encouter_an_error("different type in relop", token_sequence[token_sequence_position - 1].line);
			operator_stack.pop_back();
			Id_stack.pop_back();
			Id_stack.pop_back();
			temp = temp1;
			temp.data_type.basic_type = _boolean;
			Id_stack.push_back(temp);
			break;

		case 64: // expression->simple_expression
			break;

		case 65: // simple_expression->simple_expression addop term
			operator_stack_top = operator_stack[operator_stack.size() - 1];
			if (Id_stack[Id_stack.size() - 1].data_type == Id_stack[Id_stack.size() - 2].data_type)
			{
				temp = temp1;
				temp.data_type.basic_type =
					max(Id_stack[Id_stack.size() - 2].data_type.basic_type,
						Id_stack[Id_stack.size() - 1].data_type.basic_type);
				if (operator_stack_top == "or")
				{
					if (temp.data_type.basic_type != _boolean)
						encouter_an_error("not boolean type in or.", token_sequence[token_sequence_position - 1].line);
				}
				else
				{
					if (temp.data_type.basic_type > 1)
						encouter_an_error("not integer(real) type in add(sub).", token_sequence[token_sequence_position - 1].line);
				}
				Id_stack.pop_back();
				Id_stack.pop_back();
				Id_stack.push_back(temp);
			}
			else
				encouter_an_error("different type in addop", token_sequence[token_sequence_position - 1].line);
			operator_stack.pop_back();

			break;

		case 66: // simple_expression->term
			break;

		case 67: // term->term mulop factor
			operator_stack_top = operator_stack[operator_stack.size() - 1];
			if (Id_stack[Id_stack.size() - 1].data_type == Id_stack[Id_stack.size() - 2].data_type)
			{
				temp = temp1;
				temp.data_type.basic_type =
					max(Id_stack[Id_stack.size() - 2].data_type.basic_type,
						Id_stack[Id_stack.size() - 1].data_type.basic_type);
				if (operator_stack_top == "and")
				{
					if (temp.data_type.basic_type != _boolean)
						encouter_an_error("not boolean type in and.", token_sequence[token_sequence_position - 1].line);
				}
				else if (operator_stack_top == "div" || operator_stack_top == "mod")
				{
					if (temp.data_type.basic_type != _integer)
						encouter_an_error("not integer type in div(mod).", token_sequence[token_sequence_position - 1].line);
				}
				else
				{
					if (temp.data_type.basic_type >1)
						encouter_an_error("not integer(real) type in divide(mul).", token_sequence[token_sequence_position - 1].line);
				}
				Id_stack.pop_back();
				Id_stack.pop_back();
				Id_stack.push_back(temp);
			}
			else
				encouter_an_error("different type in mulop", token_sequence[token_sequence_position - 1].line);
			operator_stack.pop_back();
			break;

		case 68: // term->factor
			break;

		case 69: // factor->num
			break;

		case 70: // factor->variable
			break;

		case 71: // factor->id punc_round_left expression_list punc_round_right
			break;

		case 72: // factor->punc_round_left expression punc_round_right
			token_sequence_position+=2;
			break;

		case 73: // factor->not factor
			if(Id_stack[Id_stack.size()-1].data_type.basic_type!=_boolean){
				encouter_an_error("not boolean type in not.",token_sequence[ token_sequence_position-1].line);
			}
			token_sequence_position+=1;
			break;

		case 74: // factor->addop_sub factor
			if(Id_stack[Id_stack.size()-1].data_type.basic_type>1){
				encouter_an_error("not integer(real) type in minus.",token_sequence[ token_sequence_position-1].line);
			}
			token_sequence_position+=1;
			break;

		case 75: // relop->relop_e
			operator_stack.push_back("e");
			token_sequence_position += 1;
			break;

		case 76: // relop->relop_ne
			operator_stack.push_back("ne");
			token_sequence_position += 1;
			break;

		case 77: // relop->relop_l
			operator_stack.push_back("l");
			token_sequence_position += 1;
			break;

		case 78: // relop->relop_le
			operator_stack.push_back("le");
			token_sequence_position += 1;
			break;

		case 79: // relop->relop_g
			operator_stack.push_back("g");
			token_sequence_position += 1;
			break;

		case 80: // relop->relop_ge
			operator_stack.push_back("ge");
			token_sequence_position += 1;
			break;

		case 81: // addop->addop_add
			operator_stack.push_back("add");
			token_sequence_position += 1;
			break;

		case 82: // addop->addop_sub
			operator_stack.push_back("sub");
			token_sequence_position += 1;
			break;

		case 83: // addop->addop_or
			operator_stack.push_back("or");
			token_sequence_position += 1;
			break;

		case 84: // num->fnum
			temp = temp1;
			temp.data_type.basic_type = _real;
			Id_stack.push_back(temp);
			token_sequence_position += 1;
			break;

		case 85: // num->digits
			temp = temp1;
			temp.data_type.basic_type = _integer;
			temp.data_type.constant_value = atoi(token_sequence[token_sequence_position].content.c_str());
			Id_stack.push_back(temp);
			token_sequence_position += 1;
			break;

		case 86: // mulop->mulop_mul
			operator_stack.push_back("mul");
			token_sequence_position += 1;
			break;

		case 87: // mulop->mulop_divide
			operator_stack.push_back("divide");
			token_sequence_position += 1;
			break;

		case 88: // mulop->mulop_div
			operator_stack.push_back("div");
			token_sequence_position += 1;
			break;

		case 89: // mulop->mulop_mod
			operator_stack.push_back("mod");
			token_sequence_position += 1;
			break;

		case 90: // mulop->mulop_and
			operator_stack.push_back("and");
			token_sequence_position += 1;
			break;
		}
	}
}

int main()
{
	vector<token> token_sequence;
	token_sequence.clear();

	Grammar grammar = initGrammer();

	LR_PredictTable LRtable = getTable(grammar);

	vector<int> product_sequence(control_program(LRtable, grammar, token_sequence));

	init_ID_Table();

	for (auto i : product_sequence)
		cout << i << endl;
	for (auto i : token_sequence)
		cout << i.content << endl;

	semantic_analysis(product_sequence, token_sequence);
	cout << "Success!!" << endl;
	return 0;
}
