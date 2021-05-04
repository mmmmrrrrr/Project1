#include "semantic_analysis.h"
vector<string> opStack;
vector<Id> idStack;
vector<Id> idList,paramList,varList;
vector<vector<Id>> exprListStack;
Id errorId, temp1;
IdTable *nowIdTable;

queue<Id>constRecord;
queue<vector<Id>>rlistRecord;
queue<vector<Id>>wlistRecord;

Id getConst(){
	temp1=constRecord.front();
	constRecord.pop();
	return temp1;
}

vector<Id> getRlist(){
	vector<Id> t(rlistRecord.front());
	rlistRecord.pop();
	return t;
}
vector<Id> getWlist(){
	vector<Id> t(wlistRecord.front());
	wlistRecord.pop();
	return t;
}

void reportError(string errInformation, token t)
{
	cout << "Syntax error:"<<"    <line "<<t.line<<">    \""<<t.content<<"\"    "<<errInformation;
	exit(0);
}
bool DataType::operator==(const DataType &t) const
{
	if(! t.basicType ||!basicType)
		return 0;
	if (t.basicType == basicType)
		return 1;
	else if (max(t.basicType, basicType) < 3)
		return 1;
	else
		return 0;
}
Id searchId(string name)
{
	for (IdTable *t = nowIdTable; t != NULL; t = t->father)
	{
		if (t->nameToId.find(name) != t->nameToId.end())
			return t->nameToId[name];
	}
	return errorId;
} //根据名字进行查找，返回标识符信息
bool insert_id(Id id)
{
	//cout << "id.dim" << id.dataType.dimension << endl;
	//cout << id.isError << endl;
	if (nowIdTable->nameToId.find(id.name) != nowIdTable->nameToId.end())
		return 0;
	nowIdTable->nameToId[id.name] = id;
	return 1;
} //返回1表示正确插入，返回0表示已经有重复的标识符
void location()
{
	auto t = nowIdTable;
	nowIdTable = new IdTable;
	nowIdTable->father = t;
} //定位
void relocation()
{
	auto t = nowIdTable->father;
	delete nowIdTable;
	nowIdTable = t;
} //重定位
void initIdTable()
{
	errorId.isError = 1;
	location();
}
void semantic_analysis(const vector<int> &product_seq, const vector<token> &token_seq, map<int,string> &numToProduct)
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
			idStack[idStack.size() - 1].name = token_seq[token_seq_pos + 1].content;
			if (!insert_id(idStack[idStack.size() - 1]))
			{
				reportError("repeated definition", token_seq[token_seq_pos]);
				return;
			}
			constRecord.push(idStack[idStack.size()-1]);
			idStack.pop_back();
			token_seq_pos += 3;
			break;

		case 10: // const_declaration->id relop_e const_value
			idStack[idStack.size() - 1].name = token_seq[token_seq_pos].content;
			if (!insert_id(idStack[idStack.size() - 1]))
			{
				reportError("repeated definition", token_seq[token_seq_pos]);
				return;
			}
			constRecord.push(idStack[idStack.size()-1]);
			idStack.pop_back();

			token_seq_pos += 2;
			break;

		case 11: // const_value->addop_add num
			idStack[idStack.size() - 1].idType = _constant;
			break;

		case 12: // const_value->addop_sub num
			idStack[idStack.size() - 1].idType = _constant;
			idStack[idStack.size() - 1].dataType.constVal =
				-idStack[idStack.size() - 1].dataType.constVal;
			break;

		case 13: // const_value->num
			idStack[idStack.size() - 1].idType = _constant;
			break;

		case 14: // const_value->letter
			temp.dataType.basicType = _char;
			temp.idType = _constant;
			idStack.push_back(temp);
			token_seq_pos += 1;
			break;

		case 15: // var_declarations->var var_declaration punc_semicolon
			token_seq_pos += 2;
			break;

		case 16: // var_declarations->null
			break;

		case 17: // var_declaration->var_declaration punc_semicolon idlist punc_colon type
			temp = idStack[idStack.size() - 1];
			for (auto &j : idList)
			{
				j.dataType = temp.dataType;
				j.idType = _variable;
				if (!insert_id(j))
				{
					reportError("repeated definition", token_seq[token_seq_pos]);
					return;
				}
			}
			idList.clear();
			idStack.pop_back();
			token_seq_pos += 2;
			break;

		case 18: // var_declaration->idlist punc_colon type
			temp = idStack[idStack.size() - 1];
			while (idList.size())
			{
				idList[idList.size() - 1].dataType = temp.dataType;
				idList[idList.size() - 1].idType = _variable;
				if (!insert_id(idList[idList.size() - 1]))
				{
					reportError("repeated definition", token_seq[token_seq_pos]);
					return;
				}
				idList.pop_back();
			}
			idStack.pop_back();
			token_seq_pos += 1;
			break;

		case 19: // type->basic_type
			break;

		case 20: // type->array punc_square_left period punc_square_right of basic_type
			idStack[idStack.size() - 2].dataType.basicType = idStack[idStack.size() - 1].dataType.basicType;
			idStack.pop_back();
			token_seq_pos += 4;
			break;

		case 21: // basic_type->integer
			temp = temp1;
			temp.dataType.basicType = _integer;
			idStack.push_back(temp);
			token_seq_pos += 1;
			break;

		case 22: // basic_type->real
			temp = temp1;
			temp.dataType.basicType = _real;
			idStack.push_back(temp);
			token_seq_pos += 1;
			break;

		case 23: // basic_type->boolean
			temp = temp1;
			temp.dataType.basicType = _boolean;
			idStack.push_back(temp);
			token_seq_pos += 1;
			break;

		case 24: // basic_type->char
			temp = temp1;
			temp.dataType.basicType = _char;
			idStack.push_back(temp);
			token_seq_pos += 1;
			break;

		case 25: // period->period punc_comma digits punc_point punc_point digits
			mi = atoi(token_seq[token_seq_pos + 1].content.c_str());
			mx = atoi(token_seq[token_seq_pos + 4].content.c_str());
			if (mi > mx)
			{
				reportError("lowerBound greater than upperBound", token_seq[token_seq_pos]);
			}
			idStack[idStack.size() - 1].dataType.dimension += 1;
			idStack[idStack.size() - 1].dataType.lowerBound.push_back(mi);
			idStack[idStack.size() - 1].dataType.upperBound.push_back(mx);
			token_seq_pos += 5;
			break;

		case 26: // period->digits punc_point punc_point digits
			temp = temp1;
			temp.dataType.dimension = 1;
			mi = atoi(token_seq[token_seq_pos].content.c_str());
			mx = atoi(token_seq[token_seq_pos + 3].content.c_str());
			if (mi > mx)
			{
				reportError("lowerBound greater than upperBound", token_seq[token_seq_pos]);
			}
			temp.dataType.lowerBound.push_back(mi);
			temp.dataType.upperBound.push_back(mx);
			idStack.push_back(temp);
			token_seq_pos += 4;
			break;

		case 27: // subprogram_declarations->subprogram_declarations subprogram punc_semicolon
			token_seq_pos += 1;
			break;

		case 28: // subprogram_declarations->null
			break;

		case 29: // subprogram->subprogram_head punc_semicolon subprogram_body
			relocation();
			token_seq_pos += 1;
			break;

		case 30: // subprogram_head->procedure id formal_parameter
			temp = temp1;
			temp.idType = _procedure;
			temp.name = token_seq[token_seq_pos + 1].content;
			for (auto j : paramList)
			{
				//cout << "j.dataType.basicType=" << j.dataType.basicType << endl;
				temp.paramList.push_back({j.name, j.dataType});
			}
			insert_id(temp);
			location();
			for (auto j : paramList)
				insert_id(j);
			//parameter insert id table.
			paramList.clear();
			token_seq_pos += 2;
			break;

		case 31: // subprogram_head->function id formal_parameter punc_colon basic_type
			temp = temp1;
			temp.idType = _function;
			temp.name = token_seq[token_seq_pos + 1].content;
			temp.retDataType = idStack[idStack.size() - 1].dataType;
			for (auto j : paramList)
				temp.paramList.push_back({j.name, j.dataType});
			insert_id(temp);
			location();
			for (auto j : paramList)
				insert_id(j);
			//parameter insert id table.
			temp = temp1;
			temp.idType = _variable;
			temp.name = token_seq[token_seq_pos + 1].content;
			temp.dataType = idStack[idStack.size() - 1].dataType;
			insert_id(temp);
			//return value insert id table.
			paramList.clear();
			idStack.pop_back();
			token_seq_pos += 3;
			break;

		case 32: // formal_parameter->punc_round_left parameter_list punc_round_right
			token_seq_pos += 2;
			break;

		case 33: // formal_parameter->null
			break;

		case 34: // parameter_list->parameter_list punc_semicolon parameter
			for (auto j : idList)
			{
				//cout << "idList.dataType.basicType=" << j.dataType.basicType << endl;
				paramList.push_back(j);
			}
			idList.clear();
			token_seq_pos += 1;
			break;

		case 35: // parameter_list->parameter
			for (auto j : idList)
			{
				//cout << "idList.dataType.basicType=" << j.dataType.basicType << endl;
				paramList.push_back(j);
			}
			idList.clear();
			break;

		case 36: // parameter->var_parameter
			for (auto &j : idList)
				j.dataType.param_type = _value;
			break;

		case 37: // parameter->value_parameter
			for (auto &j : idList)
				j.dataType.param_type = _reference;
			break;

		case 38: // var_parameter->var value_parameter
			token_seq_pos += 1;
			break;

		case 39: // value_parameter->idlist punc_colon basic_type
			//cout << "temp.dataType.basicType=" << temp.dataType.basicType << endl;
			temp = idStack[idStack.size() - 1];
			for (auto &j : idList)
			{
				//cout << "idList.dataType.basicType=" << j.dataType.basicType << endl;
				j.dataType.basicType = temp.dataType.basicType;
			}
			idStack.pop_back();
			token_seq_pos += 1;
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
			if (idStack[idStack.size() - 1].dataType == idStack[idStack.size() - 2].dataType)
			{
				if (idStack[idStack.size() - 1].dataType.basicType > idStack[idStack.size() - 2].dataType.basicType)
					reportError("different assignment types.", token_seq[token_seq_pos]);
			}
			else
				reportError("different assignment types ", token_seq[token_seq_pos]);
			idStack.pop_back();
			idStack.pop_back();
			token_seq_pos += 1;
			break;

		case 45: // statement->procedure_call
			break;

		case 46: // statement->compound_statement
			break;

		case 47: // statement->if expression then statement else_part
			if (idStack[idStack.size() - 1].dataType.basicType != _boolean)
			{
				reportError("not boolean type in if expression ", token_seq[token_seq_pos]);
			}
			idStack.pop_back();
			token_seq_pos += 2;
			break;

		case 48: // statement->for id assignop expression to expression do statement
			temp = searchId(token_seq[token_seq_pos - 1].content);
			if (temp.dataType.basicType == _integer)
			{
				reportError("not integer type in for if id ", token_seq[token_seq_pos]);
			}
			if (idStack[idStack.size() - 1].dataType.basicType != _integer || idStack[idStack.size() - 2].dataType.basicType != _integer)
			{
				//cout << "idStack[idStack.size() - 1].dataType.basicType=" << idStack[idStack.size() - 1].dataType.basicType << endl;
				//cout << "idStack[idStack.size() - 2].dataType.basicType=" << idStack[idStack.size() - 2].dataType.basicType << endl;
				reportError("not integer type in for if expression ", token_seq[token_seq_pos]);
			}
			idStack.pop_back();
			idStack.pop_back();
			token_seq_pos += 5;
			break;

		case 49: // statement->read punc_round_left variable_list punc_round_right
			//cout<<"varList.size()="<<varList.size()<<endl;
			rlistRecord.push(varList);
			varList.clear();
			token_seq_pos += 3;
			break;

		case 50: // statement->write punc_round_left expression_list punc_round_right
			wlistRecord.push(exprListStack[exprListStack.size()-1]);
			exprListStack.pop_back();
			token_seq_pos += 3;
			break;

		case 51: // statement->null
			break;

		case 52: // variable_list->variable_list punc_comma variable
			//cout<<"varList push"<<endl;
			varList.push_back(idStack[idStack.size() - 1]);
			//cout<<"varList.size()="<<varList.size()<<endl;
			idStack.pop_back();
			token_seq_pos += 1;
			break;

		case 53: // variable_list->variable
			//cout<<"varList push"<<endl;
			varList.push_back(idStack[idStack.size() - 1]);
			idStack.pop_back();
			break;

		case 54: // variable->id id_varpart

			idStack[idStack.size() - 1].name = token_seq[token_seq_pos].content;
			//cout << "---" << token_seq[token_seq_pos].content << endl;

			temp = searchId(idStack[idStack.size() - 1].name);
			//cout << "yyy" << temp.dataType.basicType << endl;
			//cout << temp.isError << endl;
			if (temp.isError)
			{
				reportError("use undefined variable.", token_seq[token_seq_pos]);
			}
			/*if (idStack[idStack.size() - 2].idType == _constant)
			{
				reportError("use constant error.", token_seq[token_seq_pos]);
			}*/
			if (temp.dataType.dimension != idStack[idStack.size() - 1].dataType.dimension)
			{
				//cout << temp.dataType.dimension << ' ' << idStack[idStack.size() - 1].dataType.dimension << endl;
				reportError("use array error.", token_seq[token_seq_pos]);
			}
			if(temp.idType==_constant){
				reportError("constant in assignment",token_seq[token_seq_pos]);
			}
			idStack[idStack.size() - 1] = temp;
			token_seq_pos += 1;
			break;

		case 55: // id_varpart->punc_square_left expression_list puncsquare_right
			for (auto j : exprListStack[exprListStack.size() - 1])
			{
				if (j.dataType.basicType != _integer)
					reportError("use array error.", token_seq[token_seq_pos]);
			}
			temp = temp1;
			temp.dataType.dimension = exprListStack[exprListStack.size() - 1].size();
			//cout << "dim:" << temp.dataType.dimension << endl;
			exprListStack.pop_back();
			idStack.push_back(temp);
			token_seq_pos += 2;
			break;

		case 56: // id_varpart->null
			temp = temp1;
			idStack.push_back(temp);
			break;

		case 57: // procedure_call->id
			temp = searchId(token_seq[token_seq_pos].content);
			if (temp.idType != _procedure && temp.idType != _function)
			{
				reportError("A non function(procedure) identifier was incorrectly called", token_seq[token_seq_pos]);
			}
			if (temp.paramList.size() != 0)
			{
				reportError("Wrong number of parameters ", token_seq[token_seq_pos]);
			}
			token_seq_pos += 1;
			break;

		case 58: // procedure_call->id punc_round_left expression_list punc_round_right
			temp = searchId(token_seq[token_seq_pos].content);
			if (temp.idType != _procedure && temp.idType != _function)
			{
				reportError("A non function(procedure) identifier was incorrectly called", token_seq[token_seq_pos]);
			}
			if (temp.paramList.size() != exprListStack[exprListStack.size() - 1].size())
			{
				reportError("Wrong number of parameters ", token_seq[token_seq_pos]);
			}
			for (int j = 0; j < temp.paramList.size(); ++j)
			{
				if (temp.paramList[j].dataType == exprListStack[exprListStack.size() - 1][j].dataType)
				{

					if (temp.paramList[j].dataType.basicType < exprListStack[exprListStack.size() - 1][j].dataType.basicType)
					{
						//cout << "temp.paramList[j].dataType.basicType=" << temp.paramList[j].dataType.basicType << endl;
						//cout << "exprListStack[exprListStack.size() - 1][j].dataType.basicType=" << exprListStack[exprListStack.size() - 1][j].dataType.basicType << endl;
						reportError("Wrong parameter type1 ", token_seq[token_seq_pos]);
					}
				}
				else
					reportError("Wrong parameter type2 ", token_seq[token_seq_pos]);
			}
			exprListStack.pop_back();
			token_seq_pos += 3;
			break;

		case 59: // else_part->else statement
			token_seq_pos += 1;
			break;

		case 60: // else_part->null
			break;

		case 61: // expression_list->expression_list punc_comma expression
			exprListStack[exprListStack.size() - 1].push_back(idStack[idStack.size() - 1]);
			idStack.pop_back();
			token_seq_pos += 1;
			break;

		case 62: // expression_list->expression
			exprListStack.push_back(vector<Id>(1, idStack[idStack.size() - 1]));
			idStack.pop_back();
			break;

		case 63: // expression->simple_expression relop simple_expression
			if (idStack[idStack.size() - 1].dataType == idStack[idStack.size() - 2].dataType)
				;
			else
				reportError("different type in relop", token_seq[token_seq_pos - 1]);
			opStack.pop_back();
			idStack.pop_back();
			idStack.pop_back();
			temp = temp1;
			temp.dataType.basicType = _boolean;
			idStack.push_back(temp);
			break;

		case 64: // expression->simple_expression
			break;

		case 65: // simple_expression->simple_expression addop term
			operator_stack_top = opStack[opStack.size() - 1];
			if (idStack[idStack.size() - 1].dataType == idStack[idStack.size() - 2].dataType)
			{
				temp = temp1;
				temp.dataType.basicType = max(idStack[idStack.size() - 2].dataType.basicType, idStack[idStack.size() - 1].dataType.basicType);
				if (operator_stack_top == "or")
				{
					if (temp.dataType.basicType != _boolean)
						reportError("not boolean type in or.", token_seq[token_seq_pos - 1]);
				}
				else
				{
					if (temp.dataType.basicType > 2)
						reportError("not integer(real) type in add(sub).", token_seq[token_seq_pos - 1]);
				}
				idStack.pop_back();
				idStack.pop_back();
				idStack.push_back(temp);
			}
			else
				reportError("different type in addop", token_seq[token_seq_pos - 1]);
			opStack.pop_back();

			break;

		case 66: // simple_expression->term
			break;

		case 67: // term->term mulop factor
			operator_stack_top = opStack[opStack.size() - 1];
			if (idStack[idStack.size() - 1].dataType == idStack[idStack.size() - 2].dataType)
			{
				temp = temp1;
				temp.dataType.basicType = max(idStack[idStack.size() - 2].dataType.basicType, idStack[idStack.size() - 1].dataType.basicType);
				if (operator_stack_top == "and")
				{
					if (temp.dataType.basicType != _boolean)
						reportError("not boolean type in and.", token_seq[token_seq_pos - 1]);
				}
				else if (operator_stack_top == "div" || operator_stack_top == "mod")
				{
					if (temp.dataType.basicType != _integer)
						reportError("not integer type in div(mod).", token_seq[token_seq_pos - 1]);
				}
				else
				{
					if (temp.dataType.basicType > 2)
						reportError("not integer(real) type in divide(mul).", token_seq[token_seq_pos - 1]);
				}
				idStack.pop_back();
				idStack.pop_back();
				idStack.push_back(temp);
			}
			else
				reportError("different type in mulop", token_seq[token_seq_pos - 1]);
			opStack.pop_back();
			break;

		case 68: // term->factor
			break;

		case 69: // factor->num
			break;

		case 70: // factor->variable
			break;

		case 71: // factor->id punc_round_left expression_list punc_round_right
			temp = searchId(token_seq[token_seq_pos].content);
			if (temp.idType != _function)
			{
				reportError("A non function identifier was incorrectly called", token_seq[token_seq_pos]);
			}
			if (temp.paramList.size() != exprListStack[exprListStack.size() - 1].size())
			{
				reportError("Wrong number of parameters ", token_seq[token_seq_pos]);
			}
			for (int j = 0; j < temp.paramList.size(); ++j)
			{
				if (temp.paramList[j].dataType == exprListStack[exprListStack.size() - 1][j].dataType)
				{

					if (temp.paramList[j].dataType.basicType < exprListStack[exprListStack.size() - 1][j].dataType.basicType)
					{
						reportError("Wrong parameter type", token_seq[token_seq_pos]);
					}
				}
				else
					reportError("Wrong parameter type", token_seq[token_seq_pos]);
			}
			exprListStack.pop_back();
			temp = temp1;
			temp.dataType = searchId(token_seq[token_seq_pos].content).retDataType;
			idStack.push_back(temp);
			token_seq_pos += 3;
			break;

		case 72: // factor->punc_round_left expression punc_round_right
			token_seq_pos += 2;
			break;

		case 73: // factor->not factor
			if (idStack[idStack.size() - 1].dataType.basicType != _boolean)
			{
				reportError("not boolean type in not.", token_seq[token_seq_pos - 1]);
			}
			token_seq_pos += 1;
			break;

		case 74: // factor->addop_sub factor
			if (idStack[idStack.size() - 1].dataType.basicType > 2)
			{
				reportError("not integer(real) type in minus.", token_seq[token_seq_pos - 1]);
			}
			token_seq_pos += 1;
			break;

		case 75: // relop->relop_e
			opStack.push_back("e");
			token_seq_pos += 1;
			break;

		case 76: // relop->relop_ne
			opStack.push_back("ne");
			token_seq_pos += 1;
			break;

		case 77: // relop->relop_l
			opStack.push_back("l");
			token_seq_pos += 1;
			break;

		case 78: // relop->relop_le
			opStack.push_back("le");
			token_seq_pos += 1;
			break;

		case 79: // relop->relop_g
			opStack.push_back("g");
			token_seq_pos += 1;
			break;

		case 80: // relop->relop_ge
			opStack.push_back("ge");
			token_seq_pos += 1;
			break;

		case 81: // addop->addop_add
			opStack.push_back("add");
			token_seq_pos += 1;
			break;

		case 82: // addop->addop_sub
			opStack.push_back("sub");
			token_seq_pos += 1;
			break;

		case 83: // addop->addop_or
			opStack.push_back("or");
			token_seq_pos += 1;
			break;

		case 84: // num->fnum
			temp = temp1;
			temp.dataType.basicType = _real;
			idStack.push_back(temp);
			token_seq_pos += 1;
			break;

		case 85: // num->digits
			temp = temp1;
			temp.dataType.basicType = _integer;
			temp.dataType.constVal = atoi(token_seq[token_seq_pos].content.c_str());
			idStack.push_back(temp);
			token_seq_pos += 1;
			break;

		case 86: // mulop->mulop_mul
			opStack.push_back("mul");
			token_seq_pos += 1;
			break;

		case 87: // mulop->mulop_divide
			opStack.push_back("divide");
			token_seq_pos += 1;
			break;

		case 88: // mulop->mulop_div
			opStack.push_back("div");
			token_seq_pos += 1;
			break;

		case 89: // mulop->mulop_mod
			opStack.push_back("mod");
			token_seq_pos += 1;
			break;

		case 90: // mulop->mulop_and
			opStack.push_back("and");
			token_seq_pos += 1;
			break;
		}

		if (idStack.size())
			cout << idStack.size() << ":" << (int)idStack[idStack.size() - 1].dataType.basicType << "xxx" << endl;
		else
			cout << idStack.size() << ":"
				 << "xxx" << endl;
		cout << "exprListStackSize" << exprListStack.size() << endl;
		if (exprListStack.size())
			cout << "exprList.size()=" << exprListStack[exprListStack.size() - 1].size() << endl;
	}
	//cout<<"RList.size()="<<rlistRecord.size()<<endl;
}
/*
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

	initIdTable();

	for (auto i : product_seq)
		cout << i << endl;
	for (auto i : token_seq)
		cout << i.content << endl;

	semantic_analysis(product_seq, token_seq);
	cout << "Success!!" << endl;
	return 0;
}
*/
