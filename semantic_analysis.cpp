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
	else if (max(t.basic_type, basic_type) < 3)
		return 1;
	else
		return 0;
}
Id search_id(string name)
{
	for (Id_Table *t = now_id_table; t != NULL; t = t->father)
	{
		if (t->name_to_id.find(name) != t->name_to_id.end())
			return t->name_to_id[name];
	}
	return error_id;
} //根据名字进行查找，返回标识符信息
bool insert_id(Id id)
{
	cout << "id.dim"<<id.data_type.dimension << endl;
	cout << id.is_error << endl;
	if (now_id_table->name_to_id.find(id.name) != now_id_table->name_to_id.end())
		return 0;
	now_id_table->name_to_id[id.name] = id;
	return 1;
} //返回1表示正确插入，返回0表示已经有重复的标识符
void location()
{
	auto t = now_id_table;
	now_id_table = new Id_Table;
	now_id_table->father = t;
} //定位
void relocation()
{
	auto t = now_id_table->father;
	delete now_id_table;
	now_id_table = t;
} //重定位
void init_ID_Table()
{
	error_id.is_error = 1;
	location();
}
void semantic_analysis(vector<int> &product_seq, vector<token> token_seq)
{
	Id temp;
	int token_seq_pos = 0, mi, mx;
	string operator_stack_top;
	for (auto i : product_seq)
	{
		cout << i << ": " << numToProduct[i] << endl;
		if (i)
			cout << token_seq[token_seq_pos].content << endl;

		switch (i)
		{
		case 0: // S->programstruct
			relocation();
			cout << token_seq_pos << ' ' << token_seq.size() << endl;
			if (token_seq_pos != token_seq.size())
				cout << "Token not use out." << endl;
			else
				cout << "Token use out." << endl;
			return;
			break;

		case 1: // programstruct->program_head punc_semicolon program_body punc_point
			token_seq_pos += 2;
			break;

		case 2: // program_head->program id punc_round_left idlist punc_round_right
			idList.clear();
			token_seq_pos += 4;
			break;

		case 3: // program_head->program id
			token_seq_pos += 2;
			break;

		case 4: // program_body->const_declarations var_declarations subprogram_declarations compound_statement
			break;

		case 5: // idlist->idlist punc_comma id
			temp = temp1;
			temp.name = token_seq[token_seq_pos + 1].content;
			idList.push_back(temp);
			token_seq_pos += 2;
			break;

		case 6: // idlist->id
			temp = temp1;
			temp.name = token_seq[token_seq_pos].content;
			idList.push_back(temp);
			token_seq_pos += 1;
			break;

		case 7: // const_declarations->const const_declaration punc_semicolon
			token_seq_pos += 2;
			break;

		case 8: // const_declarations->null
			break;

		case 9: // const_declaration->const_declaration punc_semicolon id relop_e const_value
			id_stack[id_stack.size() - 1].name = token_seq[token_seq_pos + 1].content;
			if (!insert_id(id_stack[id_stack.size() - 1]))
			{
				encouter_an_error("repeated definition", token_seq[token_seq_pos].line);
				return;
			}
			id_stack.pop_back();
			token_seq_pos += 3;
			break;

		case 10: // const_declaration->id relop_e const_value
			id_stack[id_stack.size() - 1].name = token_seq[token_seq_pos].content;
			if (!insert_id(id_stack[id_stack.size() - 1]))
			{
				encouter_an_error("repeated definition", token_seq[token_seq_pos].line);
				return;
			}
			id_stack.pop_back();

			token_seq_pos += 2;
			break;

		case 11: // const_value->addop_add num
			id_stack[id_stack.size() - 1].id_type = _constant;
			break;

		case 12: // const_value->addop_sub num
			id_stack[id_stack.size() - 1].id_type = _constant;
			id_stack[id_stack.size() - 1].data_type.constant_value =
				-id_stack[id_stack.size() - 1].data_type.constant_value;
			break;

		case 13: // const_value->num
			id_stack[id_stack.size() - 1].id_type = _constant;
			break;

		case 14: // const_value->letter
			temp.data_type.basic_type = _char;
			temp.id_type = _constant;
			id_stack.push_back(temp);
			token_seq_pos += 1;
			break;

		case 15: // var_declarations->var var_declaration punc_semicolon
			token_seq_pos += 2;
			break;

		case 16: // var_declarations->null
			break;

		case 17: // var_declaration->var_declaration punc_semicolon idlist punc_colon type
			temp = id_stack[id_stack.size() - 1];
			for(auto j:idList)
			{
				j.data_type = temp.data_type;
				j.id_type = _variable;
				if (!insert_id(j))
				{
					encouter_an_error("repeated definition", token_seq[token_seq_pos].line);
					return;
				}
			}
			idList.clear();
			id_stack.pop_back();
			token_seq_pos += 2;
			break;

		case 18: // var_declaration->idlist punc_colon type
			temp = id_stack[id_stack.size() - 1];
			while (idList.size())
			{
				idList[idList.size() - 1].data_type = temp.data_type;
				idList[idList.size() - 1].id_type = _variable;
				if (!insert_id(idList[idList.size() - 1]))
				{
					encouter_an_error("repeated definition", token_seq[token_seq_pos].line);
					return;
				}
				idList.pop_back();
			}
			id_stack.pop_back();
			token_seq_pos += 1;
			break;

		case 19: // type->basic_type
			break;

		case 20: // type->array punc_square_left period punc_square_right of basic_type
			id_stack[id_stack.size() - 2].data_type.basic_type = id_stack[id_stack.size() - 1].data_type.basic_type;
			id_stack.pop_back();
			token_seq_pos += 4;
			break;

		case 21: // basic_type->integer
			temp = temp1;
			temp.data_type.basic_type = _integer;
			id_stack.push_back(temp);
			token_seq_pos += 1;
			break;

		case 22: // basic_type->real
			temp = temp1;
			temp.data_type.basic_type = _real;
			id_stack.push_back(temp);
			token_seq_pos += 1;
			break;

		case 23: // basic_type->boolean
			temp = temp1;
			temp.data_type.basic_type = _boolean;
			id_stack.push_back(temp);
			token_seq_pos += 1;
			break;

		case 24: // basic_type->char
			temp = temp1;
			temp.data_type.basic_type = _char;
			id_stack.push_back(temp);
			token_seq_pos += 1;
			break;

		case 25: // period->period punc_comma digits punc_point punc_point digits
			mi = atoi(token_seq[token_seq_pos + 1].content.c_str());
			mx = atoi(token_seq[token_seq_pos + 4].content.c_str());
			if (mi > mx)
			{
				encouter_an_error("lower_bound greater than upper_bound", token_seq[token_seq_pos].line);
			}
			id_stack[id_stack.size() - 1].data_type.dimension += 1;
			id_stack[id_stack.size() - 1].data_type.lower_bound.push_back(mi);
			id_stack[id_stack.size() - 1].data_type.upper_bound.push_back(mx);
			token_seq_pos += 5;
			break;

		case 26: // period->digits punc_point punc_point digits
			temp = temp1;
			temp.data_type.dimension = 1;
			mi = atoi(token_seq[token_seq_pos].content.c_str());
			mx = atoi(token_seq[token_seq_pos + 3].content.c_str());
			if (mi > mx)
			{
				encouter_an_error("lower_bound greater than upper_bound", token_seq[token_seq_pos].line);
			}
			temp.data_type.lower_bound.push_back(mi);
			temp.data_type.upper_bound.push_back(mx);
			id_stack.push_back(temp);
			token_seq_pos += 4;
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
			token_seq_pos += 2;
			break;

		case 42: // statement_list->statement_list punc_semicolon statement
			token_seq_pos += 1;
			break;

		case 43: // statement_list->statement
			break;

		case 44: // statement->variable assignop expression
			if (id_stack[id_stack.size() - 1].data_type == id_stack[id_stack.size() - 2].data_type)
			{
				if (id_stack[id_stack.size() - 1].data_type.basic_type == _real & id_stack[id_stack.size() - 2].data_type.basic_type == _integer)
					encouter_an_error("different assignment types.", token_seq[token_seq_pos].line);
			}
			else
				encouter_an_error("different assignment types ", token_seq[token_seq_pos].line);
			id_stack.pop_back();
			id_stack.pop_back();
			token_seq_pos += 1;
			break;

		case 45: // statement->procedure_call
			break;

		case 46: // statement->compound_statement
			break;

		case 47: // statement->if expression then statement else_part
			if (id_stack[id_stack.size() - 1].data_type.basic_type != _boolean)
			{
				encouter_an_error("not boolean type in if expression ", token_seq[token_seq_pos].line);
			}
			token_seq_pos += 2;
			break;

		case 48: // statement->for id assignop expression to expression do statement
			temp = search_id(token_seq[token_seq_pos - 1].content);
			if (temp.data_type.basic_type == _integer)
			{
				encouter_an_error("not integer type in for if id ", token_seq[token_seq_pos].line);
			}
			if (id_stack[id_stack.size() - 1].data_type.basic_type != _integer || id_stack[id_stack.size() - 2].data_type.basic_type != _integer)
			{
				encouter_an_error("not integer type in for if expression ", token_seq[token_seq_pos].line);
			}
			id_stack.pop_back();
			id_stack.pop_back();
			token_seq_pos += 5;
			break;

		case 49: // statement->read punc_round_left variable_list punc_round_right
			token_seq_pos += 3;
			break;

		case 50: // statement->write punc_round_left expression_list punc_round_right
			token_seq_pos += 3;
			break;

		case 51: // statement->null
			break;

		case 52: // variable_list->variable_list punc_comma variable
			break;

		case 53: // variable_list->variable
			break;

		case 54: // variable->id id_varpart

			id_stack[id_stack.size() - 1].name = token_seq[token_seq_pos].content;
			cout << "---" << token_seq[token_seq_pos].content << endl;

			temp = search_id(id_stack[id_stack.size() - 1].name);
			cout << "yyy" << temp.data_type.basic_type << endl;
			cout << temp.is_error << endl;
			if (temp.is_error)
			{
				encouter_an_error("use undefined variable.", token_seq[token_seq_pos].line);
			}
			/*if (id_stack[id_stack.size() - 2].id_type == _constant)
			{
				encouter_an_error("use constant error.", token_seq[token_seq_pos].line);
			}*/
			if (temp.data_type.dimension != id_stack[id_stack.size() - 1].data_type.dimension)
			{
				cout << temp.data_type.dimension << ' ' << id_stack[id_stack.size() - 1].data_type.dimension << endl;
				encouter_an_error("use array error.", token_seq[token_seq_pos].line);
			}
			id_stack[id_stack.size() - 1] = temp;
			token_seq_pos += 1;
			break;

		case 55: // id_varpart->punc_square_left expression_list puncsquare_right
			for (auto j : exprListStack[exprListStack.size() - 1])
			{
				if (j.data_type.basic_type != _integer)
					encouter_an_error("use array error.", token_seq[token_seq_pos].line);
			}
			temp = temp1;
			temp.data_type.dimension = exprListStack[exprListStack.size() - 1].size();
			cout << "dim:"<<temp.data_type.dimension << endl;
			exprListStack.pop_back();
			id_stack.push_back(temp);
			token_seq_pos += 2;
			break;

		case 56: // id_varpart->null
			temp = temp1;
			id_stack.push_back(temp);
			break;

		case 57: // procedure_call->id
			break;

		case 58: // procedure_call->id punc_round_left expression_list punc_round_right
			break;

		case 59: // else_part->else statement
			token_seq_pos += 1;
			break;

		case 60: // else_part->null
			break;

		case 61: // expression_list->expression_list punc_comma expression
			exprListStack[exprListStack.size() - 1].push_back(id_stack[id_stack.size() - 1]);
			id_stack.pop_back();
			token_seq_pos += 1;
			break;

		case 62: // expression_list->expression
			exprListStack.push_back(vector<Id>(1, id_stack[id_stack.size() - 1]));
			id_stack.pop_back();
			break;

		case 63: // expression->simple_expression relop simple_expression
			if (id_stack[id_stack.size() - 1].data_type == id_stack[id_stack.size() - 2].data_type)
				;
			else
				encouter_an_error("different type in relop", token_seq[token_seq_pos - 1].line);
			operator_stack.pop_back();
			id_stack.pop_back();
			id_stack.pop_back();
			temp = temp1;
			temp.data_type.basic_type = _boolean;
			id_stack.push_back(temp);
			break;

		case 64: // expression->simple_expression
			break;

		case 65: // simple_expression->simple_expression addop term
			operator_stack_top = operator_stack[operator_stack.size() - 1];
			if (id_stack[id_stack.size() - 1].data_type == id_stack[id_stack.size() - 2].data_type)
			{
				temp = temp1;
				temp.data_type.basic_type = max(id_stack[id_stack.size() - 2].data_type.basic_type, id_stack[id_stack.size() - 1].data_type.basic_type);
				if (operator_stack_top == "or")
				{
					if (temp.data_type.basic_type != _boolean)
						encouter_an_error("not boolean type in or.", token_seq[token_seq_pos - 1].line);
				}
				else
				{
					if (temp.data_type.basic_type > 2)
						encouter_an_error("not integer(real) type in add(sub).", token_seq[token_seq_pos - 1].line);
				}
				id_stack.pop_back();
				id_stack.pop_back();
				id_stack.push_back(temp);
			}
			else
				encouter_an_error("different type in addop", token_seq[token_seq_pos - 1].line);
			operator_stack.pop_back();

			break;

		case 66: // simple_expression->term
			break;

		case 67: // term->term mulop factor
			operator_stack_top = operator_stack[operator_stack.size() - 1];
			if (id_stack[id_stack.size() - 1].data_type == id_stack[id_stack.size() - 2].data_type)
			{
				temp = temp1;
				temp.data_type.basic_type = max(id_stack[id_stack.size() - 2].data_type.basic_type, id_stack[id_stack.size() - 1].data_type.basic_type);
				if (operator_stack_top == "and")
				{
					if (temp.data_type.basic_type != _boolean)
						encouter_an_error("not boolean type in and.", token_seq[token_seq_pos - 1].line);
				}
				else if (operator_stack_top == "div" || operator_stack_top == "mod")
				{
					if (temp.data_type.basic_type != _integer)
						encouter_an_error("not integer type in div(mod).", token_seq[token_seq_pos - 1].line);
				}
				else
				{
					if (temp.data_type.basic_type > 2)
						encouter_an_error("not integer(real) type in divide(mul).", token_seq[token_seq_pos - 1].line);
				}
				id_stack.pop_back();
				id_stack.pop_back();
				id_stack.push_back(temp);
			}
			else
				encouter_an_error("different type in mulop", token_seq[token_seq_pos - 1].line);
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
			token_seq_pos += 2;
			break;

		case 73: // factor->not factor
			if (id_stack[id_stack.size() - 1].data_type.basic_type != _boolean)
			{
				encouter_an_error("not boolean type in not.", token_seq[token_seq_pos - 1].line);
			}
			token_seq_pos += 1;
			break;

		case 74: // factor->addop_sub factor
			if (id_stack[id_stack.size() - 1].data_type.basic_type > 2)
			{
				encouter_an_error("not integer(real) type in minus.", token_seq[token_seq_pos - 1].line);
			}
			token_seq_pos += 1;
			break;

		case 75: // relop->relop_e
			operator_stack.push_back("e");
			token_seq_pos += 1;
			break;

		case 76: // relop->relop_ne
			operator_stack.push_back("ne");
			token_seq_pos += 1;
			break;

		case 77: // relop->relop_l
			operator_stack.push_back("l");
			token_seq_pos += 1;
			break;

		case 78: // relop->relop_le
			operator_stack.push_back("le");
			token_seq_pos += 1;
			break;

		case 79: // relop->relop_g
			operator_stack.push_back("g");
			token_seq_pos += 1;
			break;

		case 80: // relop->relop_ge
			operator_stack.push_back("ge");
			token_seq_pos += 1;
			break;

		case 81: // addop->addop_add
			operator_stack.push_back("add");
			token_seq_pos += 1;
			break;

		case 82: // addop->addop_sub
			operator_stack.push_back("sub");
			token_seq_pos += 1;
			break;

		case 83: // addop->addop_or
			operator_stack.push_back("or");
			token_seq_pos += 1;
			break;

		case 84: // num->fnum
			temp = temp1;
			temp.data_type.basic_type = _real;
			id_stack.push_back(temp);
			token_seq_pos += 1;
			break;

		case 85: // num->digits
			temp = temp1;
			temp.data_type.basic_type = _integer;
			temp.data_type.constant_value = atoi(token_seq[token_seq_pos].content.c_str());
			id_stack.push_back(temp);
			token_seq_pos += 1;
			break;

		case 86: // mulop->mulop_mul
			operator_stack.push_back("mul");
			token_seq_pos += 1;
			break;

		case 87: // mulop->mulop_divide
			operator_stack.push_back("divide");
			token_seq_pos += 1;
			break;

		case 88: // mulop->mulop_div
			operator_stack.push_back("div");
			token_seq_pos += 1;
			break;

		case 89: // mulop->mulop_mod
			operator_stack.push_back("mod");
			token_seq_pos += 1;
			break;

		case 90: // mulop->mulop_and
			operator_stack.push_back("and");
			token_seq_pos += 1;
			break;
		}

		if (id_stack.size())
			cout << id_stack.size() << ":" << (int)id_stack[id_stack.size() - 1].data_type.basic_type << "xxx" << endl;
		else
			cout << id_stack.size() << ":"  << "xxx" << endl;
		cout << "exprListStackSize"<<exprListStack.size() << endl;
		if(exprListStack.size())
			cout << 'z'<<exprListStack[exprListStack.size()-1].size() << endl;
	}
}

int main()
{
	ifstream productIn;
	productIn.open("C:\\acm\\coding\\Project1\\production_sequence.pas");
	for (int i = 1; i <= 90; ++i)
	{
		int x;
		string y;
		productIn >> x;
		getline(productIn, y, '\n');
		numToProduct[x] = y;
	}

	vector<token> token_seq;
	token_seq.clear();

	Grammar grammar = initGrammer();

	LR_PredictTable LRtable = getTable(grammar);

	vector<int> product_seq(control_program(LRtable, grammar, token_seq));

	init_ID_Table();

	for (auto i : product_seq)
		cout << i << endl;
	for (auto i : token_seq)
		cout << i.content << endl;

	semantic_analysis(product_seq, token_seq);
	cout << "Success!!" << endl;
	return 0;
}
