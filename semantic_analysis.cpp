#include "semantic_analysis.h"
#pragma execution_character_set("utf-8")
vector<string> opStack;
vector<Id> idStack;
vector<Id> idList, paramList, varList;
vector<vector<Id>> exprListStack;
Id errorId, temp1;
IdTable *nowIdTable;

queue<Id> constRecord;
vector<vector<Id>> rlistRecord;
vector<vector<Id>> wlistRecord;
map<pair<string, string>, Id> nameToArray;

Id getConst()
{
	temp1 = constRecord.front();
	constRecord.pop();
	return temp1;
}

vector<Id> getRlist()
{
	vector<Id> t(rlistRecord.back());
	rlistRecord.pop_back();
	return t;
}
vector<Id> getWlist()
{
	vector<Id> t(wlistRecord.back());
	wlistRecord.pop_back();
	return t;
}
Id getArray(string funName, string arrayName)
{
	if (nameToArray.find(make_pair(funName, arrayName)) == nameToArray.end())
	{
		if (nameToArray.find(make_pair("", arrayName)) == nameToArray.end())
		{
			cerr << "getArray can't find " << arrayName << " in \"" << funName << "\" !!!" << endl;
			exit(0);
		}
		else
			return nameToArray[make_pair("", arrayName)];
	}
	return nameToArray[make_pair(funName, arrayName)];
}

void reportError(string errInformation, token t)
{
	cerr << "Semantic error:"
		 << "    <line " << t.line << ">    \"" << t.content << "\"    " << errInformation;
	exit(0);
}
bool DataType::operator==(const DataType &t) const
{
	if (!t.basicType || !basicType)
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
Id searchRootId(string name)
{
	IdTable *t = nowIdTable;
	for (; t->father != NULL; t = t->father)
		;
	if (t->nameToId.find(name) != t->nameToId.end())
		return t->nameToId[name];
	return errorId;
}
bool insert_id(Id id)
{
	//cerr << "id.dim" << id.dataType.dimension << endl;
	//cerr << id.isError << endl;
	if (nowIdTable->nameToId.find(id.name) != nowIdTable->nameToId.end())
		return 0;
	nowIdTable->nameToId[id.name] = id;
	if (id.idType == _variable && id.dataType.dimension)
	{
		nameToArray[make_pair(nowIdTable->funName, id.name)] = id;
		//cerr << "add map " << nowIdTable->funName << " " << id.name << endl;
	}
	return 1;
} //返回1表示正确插入，返回0表示已经有重复的标识符
void location(string funName)
{
	auto t = nowIdTable;
	nowIdTable = new IdTable;
	nowIdTable->father = t;
	nowIdTable->funName = funName;
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
	location("");
}
void semantic_analysis(const vector<int> &productSeq, const vector<token> &tokenSeq, map<int, string> &numToProduct)
{
	Id temp;
	int tokenSeqPos = 0, mi, mx;
	string operator_stack_top;
	for (auto i : productSeq)
	{
		/*
		cerr << i << ": " << numToProduct[i] << endl;
		if (i)
			cerr << tokenSeq[tokenSeqPos].content << endl;
		*/

		switch (i)
		{
		case 0: // S->programstruct
			relocation();
			//cerr << tokenSeqPos << ' ' << tokenSeq.size() << endl;
			if (tokenSeqPos != tokenSeq.size())
				cerr << "Token not use out." << endl;
			/*
			else
				cerr << "Token use out." << endl;
			*/
			return;
			break;

		case 1: // programstruct->program_head punc_semicolon program_body punc_point
			tokenSeqPos += 2;
			break;

		case 2: // program_head->program id punc_round_left idlist punc_round_right
			idList.clear();
			tokenSeqPos += 4;
			break;

		case 3: // program_head->program id
			tokenSeqPos += 2;
			break;

		case 4: // program_body->const_declarations var_declarations subprogram_declarations compound_statement
			break;

		case 5: // idlist->idlist punc_comma id
			temp = temp1;
			temp.name = tokenSeq[tokenSeqPos + 1].content;
			idList.push_back(temp);
			tokenSeqPos += 2;
			break;

		case 6: // idlist->id
			temp = temp1;
			temp.name = tokenSeq[tokenSeqPos].content;
			idList.push_back(temp);
			tokenSeqPos += 1;
			break;

		case 7: // const_declarations->const const_declaration punc_semicolon
			tokenSeqPos += 2;
			break;

		case 8: // const_declarations->null
			break;

		case 9: // const_declaration->const_declaration punc_semicolon id relop_e const_value
			idStack[idStack.size() - 1].name = tokenSeq[tokenSeqPos + 1].content;
			if (!insert_id(idStack[idStack.size() - 1]))
			{
				reportError("repeated definition", tokenSeq[tokenSeqPos]);
				return;
			}
			constRecord.push(idStack[idStack.size() - 1]);
			idStack.pop_back();
			tokenSeqPos += 3;
			break;

		case 10: // const_declaration->id relop_e const_value
			idStack[idStack.size() - 1].name = tokenSeq[tokenSeqPos].content;
			if (!insert_id(idStack[idStack.size() - 1]))
			{
				reportError("repeated definition", tokenSeq[tokenSeqPos]);
				return;
			}
			constRecord.push(idStack[idStack.size() - 1]);
			idStack.pop_back();

			tokenSeqPos += 2;
			break;

		case 11: // const_value->addop_add num
			idStack[idStack.size() - 1].idType = _constant;
			tokenSeqPos += 1;
			break;

		case 12: // const_value->addop_sub num
			idStack[idStack.size() - 1].idType = _constant;
			idStack[idStack.size() - 1].dataType.constVal =
				-idStack[idStack.size() - 1].dataType.constVal;
			tokenSeqPos += 1;
			break;

		case 13: // const_value->num
			idStack[idStack.size() - 1].idType = _constant;
			break;

		case 14: // const_value->letter
			temp.dataType.basicType = _char;
			temp.idType = _constant;
			idStack.push_back(temp);
			tokenSeqPos += 1;
			break;

		case 15: // var_declarations->var var_declaration punc_semicolon
			tokenSeqPos += 2;
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
					reportError("repeated definition", tokenSeq[tokenSeqPos]);
					return;
				}
			}
			idList.clear();
			idStack.pop_back();
			tokenSeqPos += 2;
			break;

		case 18: // var_declaration->idlist punc_colon type
			temp = idStack[idStack.size() - 1];
			for (auto &j : idList)
			{
				j.dataType = temp.dataType;
				j.idType = _variable;
				if (!insert_id(j))
				{
					reportError("repeated definition", tokenSeq[tokenSeqPos]);
					return;
				}
			}

			idList.clear();
			idStack.pop_back();
			tokenSeqPos += 1;
			break;

		case 19: // type->basic_type
			break;

		case 20: // type->array punc_square_left period punc_square_right of basic_type
			idStack[idStack.size() - 2].dataType.basicType = idStack[idStack.size() - 1].dataType.basicType;
			idStack.pop_back();
			tokenSeqPos += 4;
			break;

		case 21: // basic_type->integer
			temp = temp1;
			temp.dataType.basicType = _integer;
			idStack.push_back(temp);
			tokenSeqPos += 1;
			break;

		case 22: // basic_type->real
			temp = temp1;
			temp.dataType.basicType = _real;
			idStack.push_back(temp);
			tokenSeqPos += 1;
			break;

		case 23: // basic_type->boolean
			temp = temp1;
			temp.dataType.basicType = _boolean;
			idStack.push_back(temp);
			tokenSeqPos += 1;
			break;

		case 24: // basic_type->char
			temp = temp1;
			temp.dataType.basicType = _char;
			idStack.push_back(temp);
			tokenSeqPos += 1;
			break;

		case 25: // period->period punc_comma digits punc_point punc_point digits
			mi = atoi(tokenSeq[tokenSeqPos + 1].content.c_str());
			mx = atoi(tokenSeq[tokenSeqPos + 4].content.c_str());
			if (mi > mx)
			{
				reportError("lowerBound greater than upperBound", tokenSeq[tokenSeqPos]);
			}
			idStack[idStack.size() - 1].dataType.dimension += 1;
			idStack[idStack.size() - 1].dataType.lowerBound.push_back(mi);
			idStack[idStack.size() - 1].dataType.upperBound.push_back(mx);
			tokenSeqPos += 5;
			break;

		case 26: // period->digits punc_point punc_point digits
			temp = temp1;
			temp.dataType.dimension = 1;
			mi = atoi(tokenSeq[tokenSeqPos].content.c_str());
			mx = atoi(tokenSeq[tokenSeqPos + 3].content.c_str());
			if (mi > mx)
			{
				reportError("lowerBound greater than upperBound", tokenSeq[tokenSeqPos]);
			}
			temp.dataType.lowerBound.push_back(mi);
			temp.dataType.upperBound.push_back(mx);
			idStack.push_back(temp);
			tokenSeqPos += 4;
			break;

		case 27: // subprogram_declarations->subprogram_declarations subprogram punc_semicolon
			tokenSeqPos += 1;
			break;

		case 28: // subprogram_declarations->null
			break;

		case 29: // subprogram->subprogram_head punc_semicolon subprogram_body
			relocation();
			tokenSeqPos += 1;
			break;

		case 30: // subprogram_head->procedure id formal_parameter
			temp = temp1;
			temp.idType = _procedure;
			temp.name = tokenSeq[tokenSeqPos + 1].content;
			for (auto j : paramList)
			{
				//cerr << "j.dataType.basicType=" << j.dataType.basicType << endl;
				temp.paramList.push_back({j.name, j.dataType});
			}
			if (!insert_id(temp))
			{
				reportError("repeated definition", tokenSeq[tokenSeqPos]);
				return;
			}
			location(temp.name);
			for (auto j : paramList)
			{
				/*
				if (j.idType != _variable)
				{
					cerr << j.name << "not a variable." << endl;
					exit(0);
				}
				*/
				if (!insert_id(j))
				{
					reportError("repeated definition", tokenSeq[tokenSeqPos]);
					return;
				}
			}
			//parameter insert id table.
			paramList.clear();
			tokenSeqPos += 2;
			break;

		case 31: // subprogram_head->function id formal_parameter punc_colon basic_type
			temp = temp1;
			temp.idType = _function;
			temp.name = tokenSeq[tokenSeqPos + 1].content;
			temp.retDataType = idStack[idStack.size() - 1].dataType;
			for (auto j : paramList)
				temp.paramList.push_back({j.name, j.dataType});
			if (!insert_id(temp))
			{
				reportError("repeated definition", tokenSeq[tokenSeqPos]);
				return;
			}
			location(temp.name);
			for (auto j : paramList)
				if (!insert_id(j))
				{
					reportError("repeated definition", tokenSeq[tokenSeqPos]);
					return;
				}
			//parameter insert id table.
			temp = temp1;
			temp.idType = _variable;
			temp.name = tokenSeq[tokenSeqPos + 1].content;
			temp.dataType = idStack[idStack.size() - 1].dataType;
			if (!insert_id(temp))
			{
				reportError("repeated definition", tokenSeq[tokenSeqPos]);
				return;
			}
			//return value insert id table.
			paramList.clear();
			idStack.pop_back();
			tokenSeqPos += 3;
			break;

		case 32: // formal_parameter->punc_round_left parameter_list punc_round_right
			tokenSeqPos += 2;
			break;

		case 33: // formal_parameter->null
			break;

		case 34: // parameter_list->parameter_list punc_semicolon parameter
			for (auto j : idList)
			{
				//cerr << "idList.dataType.basicType=" << j.dataType.basicType << endl;
				j.idType = _variable;
				paramList.push_back(j);
			}
			idList.clear();
			tokenSeqPos += 1;
			break;

		case 35: // parameter_list->parameter
			for (auto j : idList)
			{
				//cerr << "idList.dataType.basicType=" << j.dataType.basicType << endl;
				j.idType = _variable;
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
			tokenSeqPos += 1;
			break;

		case 39: // value_parameter->idlist punc_colon basic_type
			//cerr << "temp.dataType.basicType=" << temp.dataType.basicType << endl;
			temp = idStack[idStack.size() - 1];
			for (auto &j : idList)
			{
				//cerr << "idList.dataType.basicType=" << j.dataType.basicType << endl;
				j.dataType.basicType = temp.dataType.basicType;
			}
			idStack.pop_back();
			tokenSeqPos += 1;
			break;

		case 40: // subprogram_body->const_declarations var_declarations compound_statement
			break;

		case 41: // compound_statement->begin statement_list end
			tokenSeqPos += 2;
			break;

		case 42: // statement_list->statement_list punc_semicolon statement
			tokenSeqPos += 1;
			break;

		case 43: // statement_list->statement
			break;

		case 44: // statement->variable assignop expression
			if (idStack[idStack.size() - 1].dataType == idStack[idStack.size() - 2].dataType)
			{
				if (idStack[idStack.size() - 1].dataType.basicType > idStack[idStack.size() - 2].dataType.basicType)
					reportError("different assignment types.", tokenSeq[tokenSeqPos]);
			}
			else
				reportError("different assignment types ", tokenSeq[tokenSeqPos]);
			idStack.pop_back();
			idStack.pop_back();
			tokenSeqPos += 1;
			break;

		case 45: // statement->procedure_call
			break;

		case 46: // statement->compound_statement
			break;

		case 47: // statement->if expression then statement else_part
			if (idStack[idStack.size() - 1].dataType.basicType != _boolean)
			{
				reportError("not boolean type in if expression ", tokenSeq[tokenSeqPos]);
			}
			idStack.pop_back();
			tokenSeqPos += 2;
			break;

		case 48: // statement->for id assignop expression to expression do statement
			temp = searchId(tokenSeq[tokenSeqPos + 1].content);
			if (temp.isError)
			{
				reportError("use undefined variable.", tokenSeq[tokenSeqPos+1]);
			}
			if (temp.dataType.basicType != _integer)
			{
				reportError("not integer type in for if id ", tokenSeq[tokenSeqPos+1]);
			}
			if (idStack.back().dataType.basicType != _integer || idStack[idStack.size() - 2].dataType.basicType != _integer)
			{
				//cerr << "idStack[idStack.size() - 1].dataType.basicType=" << idStack[idStack.size() - 1].dataType.basicType << endl;
				//cerr << "idStack[idStack.size() - 2].dataType.basicType=" << idStack[idStack.size() - 2].dataType.basicType << endl;
				reportError("not integer type in for if expression ", tokenSeq[tokenSeqPos]);
			}
			idStack.pop_back();
			idStack.pop_back();
			tokenSeqPos += 5;
			break;

		case 49: // statement->read punc_round_left variable_list punc_round_right
			//cerr<<"varList.size()="<<varList.size()<<endl;
			rlistRecord.push_back(varList);
			varList.clear();
			tokenSeqPos += 3;
			break;

		case 50: // statement->write punc_round_left expression_list punc_round_right
			wlistRecord.push_back(exprListStack[exprListStack.size() - 1]);
			exprListStack.pop_back();
			tokenSeqPos += 3;
			break;

		case 51: // statement->null
			break;

		case 52: // variable_list->variable_list punc_comma variable
			//cerr<<"varList push"<<endl;
			varList.push_back(idStack[idStack.size() - 1]);
			//cerr<<"varList.size()="<<varList.size()<<endl;
			idStack.pop_back();
			tokenSeqPos += 1;
			break;

		case 53: // variable_list->variable
			//cerr<<"varList push"<<endl;
			varList.push_back(idStack[idStack.size() - 1]);
			idStack.pop_back();
			break;

		case 54: // variable->id id_varpart

			idStack[idStack.size() - 1].name = tokenSeq[tokenSeqPos].content;
			//cerr << "---" << tokenSeq[tokenSeqPos].content << endl;

			temp = searchId(idStack[idStack.size() - 1].name);
			//cerr << "yyy" << temp.dataType.basicType << endl;
			//cerr << temp.isError << endl;
			if (temp.isError)
			{
				reportError("use undefined variable.", tokenSeq[tokenSeqPos]);
			}
			if (temp.idType != _variable)
			{
				reportError("Incorrect use of non variables.", tokenSeq[tokenSeqPos]);
			}
			if (temp.dataType.dimension != idStack[idStack.size() - 1].dataType.dimension)
			{
				//cerr << temp.dataType.dimension << ' ' << idStack[idStack.size() - 1].dataType.dimension << endl;
				reportError("use array error.", tokenSeq[tokenSeqPos]);
			}
			idStack[idStack.size() - 1] = temp;
			tokenSeqPos += 1;
			break;

		case 55: // id_varpart->punc_square_left expression_list puncsquare_right
			for (auto j : exprListStack[exprListStack.size() - 1])
			{
				if (j.dataType.basicType != _integer)
					reportError("use array error.", tokenSeq[tokenSeqPos]);
			}
			temp = temp1;
			temp.dataType.dimension = exprListStack[exprListStack.size() - 1].size();
			//cerr << "dim:" << temp.dataType.dimension << endl;
			exprListStack.pop_back();
			idStack.push_back(temp);
			tokenSeqPos += 2;
			break;

		case 56: // id_varpart->null
			temp = temp1;
			idStack.push_back(temp);
			break;

		case 57: // procedure_call->id
			temp = searchId(tokenSeq[tokenSeqPos].content);
			if (temp.isError)
			{
				reportError("use undefined variable.", tokenSeq[tokenSeqPos]);
			}
			if (temp.idType != _procedure && temp.idType != _function)
			{
				reportError("A non function(procedure) identifier was incorrectly called", tokenSeq[tokenSeqPos]);
			}
			if (temp.paramList.size() != 0)
			{
				reportError("Wrong number of parameters ", tokenSeq[tokenSeqPos]);
			}
			tokenSeqPos += 1;
			break;

		case 58: // procedure_call->id punc_round_left expression_list punc_round_right
			temp = searchRootId(tokenSeq[tokenSeqPos].content);
			if (temp.idType != _procedure && temp.idType != _function)
			{
				reportError("A non function(procedure) identifier was incorrectly called", tokenSeq[tokenSeqPos]);
			}
			if (temp.paramList.size() != exprListStack[exprListStack.size() - 1].size())
			{
				reportError("Wrong number of parameters ", tokenSeq[tokenSeqPos]);
			}
			for (int j = 0; j < temp.paramList.size(); ++j)
			{
				if (temp.paramList[j].dataType == exprListStack[exprListStack.size() - 1][j].dataType)
				{

					if (temp.paramList[j].dataType.basicType < exprListStack[exprListStack.size() - 1][j].dataType.basicType)
					{
						//cerr << "temp.paramList[j].dataType.basicType=" << temp.paramList[j].dataType.basicType << endl;
						//cerr << "exprListStack[exprListStack.size() - 1][j].dataType.basicType=" << exprListStack[exprListStack.size() - 1][j].dataType.basicType << endl;
						reportError("Wrong parameter type ", tokenSeq[tokenSeqPos]);
					}
				}
				else
					reportError("Wrong parameter type ", tokenSeq[tokenSeqPos]);
				if (temp.paramList[j].dataType.param_type == _value && exprListStack.back()[j].idType != _variable)
				{
					reportError("use not variable to value parameter.", tokenSeq[tokenSeqPos]);
				}
			}
			exprListStack.pop_back();
			tokenSeqPos += 3;
			break;

		case 59: // else_part->else statement
			tokenSeqPos += 1;
			break;

		case 60: // else_part->null
			break;

		case 61: // expression_list->expression_list punc_comma expression
			exprListStack[exprListStack.size() - 1].push_back(idStack[idStack.size() - 1]);
			idStack.pop_back();
			tokenSeqPos += 1;
			break;

		case 62: // expression_list->expression
			exprListStack.push_back(vector<Id>(1, idStack[idStack.size() - 1]));
			idStack.pop_back();
			break;

		case 63: // expression->simple_expression relop simple_expression
			if (idStack[idStack.size() - 1].dataType == idStack[idStack.size() - 2].dataType)
				;
			else
				reportError("different type in relop", tokenSeq[tokenSeqPos - 1]);
			opStack.pop_back();
			idStack.pop_back();
			idStack.pop_back();
			temp = temp1;
			temp.idType = _none;
			temp.dataType.basicType = _boolean;
			idStack.push_back(temp);
			break;

		case 64: // expression->simple_expression
			break;

		case 65: // simple_expression->simple_expression addop term
			operator_stack_top = opStack[opStack.size() - 1];
			if (!(idStack[idStack.size() - 1].dataType == idStack[idStack.size() - 2].dataType))
			{
				reportError("different type in addop", tokenSeq[tokenSeqPos - 1]);
				return;
			}
			temp = temp1;
			temp.dataType.basicType = max(idStack[idStack.size() - 2].dataType.basicType, idStack[idStack.size() - 1].dataType.basicType);
			if (operator_stack_top == "or")
			{
				if (temp.dataType.basicType != _boolean)
					reportError("not boolean type in or.", tokenSeq[tokenSeqPos - 1]);
			}
			else
			{
				if (temp.dataType.basicType > 2)
					reportError("not integer(real) type in add(sub).", tokenSeq[tokenSeqPos - 1]);
			}

			idStack.pop_back();
			idStack.pop_back();
			temp.idType = _none;
			idStack.push_back(temp);
			opStack.pop_back();
			break;

		case 66: // simple_expression->term
			break;

		case 67: // term->term mulop factor
			operator_stack_top = opStack[opStack.size() - 1];
			if (!(idStack[idStack.size() - 1].dataType == idStack[idStack.size() - 2].dataType))
			{
				reportError("different type in mulop", tokenSeq[tokenSeqPos - 1]);
				return;
			}
			temp = temp1;
			temp.dataType.basicType = max(idStack[idStack.size() - 2].dataType.basicType, idStack[idStack.size() - 1].dataType.basicType);
			if (operator_stack_top == "and")
			{
				if (temp.dataType.basicType != _boolean)
					reportError("not boolean type in and.", tokenSeq[tokenSeqPos - 1]);
			}
			else if (operator_stack_top == "div" || operator_stack_top == "mod")
			{
				if (temp.dataType.basicType != _integer)
					reportError("not integer type in div(mod).", tokenSeq[tokenSeqPos - 1]);
			}
			else
			{
				if (temp.dataType.basicType > 2)
					reportError("not integer(real) type in divide(mul).", tokenSeq[tokenSeqPos - 1]);
				if (operator_stack_top == "divide")
				{
					temp.dataType.basicType = _real;
				}
			}
			idStack.pop_back();
			idStack.pop_back();
			temp.idType = _none;
			idStack.push_back(temp);
			opStack.pop_back();
			break;

		case 68: // term->factor
			break;

		case 69: // factor->num
			break;

		case 70: // factor->variable
			break;

		case 71: // factor->id punc_round_left expression_list punc_round_right
			temp = searchRootId(tokenSeq[tokenSeqPos].content);
			if (temp.idType != _function)
			{
				reportError("A non function identifier was incorrectly called", tokenSeq[tokenSeqPos]);
			}
			if (temp.paramList.size() != exprListStack[exprListStack.size() - 1].size())
			{
				reportError("Wrong number of parameters ", tokenSeq[tokenSeqPos]);
			}
			for (int j = 0; j < temp.paramList.size(); ++j)
			{
				if (temp.paramList[j].dataType == exprListStack.back()[j].dataType)
				{

					if (temp.paramList[j].dataType.basicType < exprListStack.back()[j].dataType.basicType)
					{
						reportError("Wrong parameter type", tokenSeq[tokenSeqPos]);
					}
				}
				else
					reportError("Wrong parameter type", tokenSeq[tokenSeqPos]);
				if (temp.paramList[j].dataType.param_type == _value && exprListStack.back()[j].idType != _variable)
				{
					reportError("use not variable to reference parameter.", tokenSeq[tokenSeqPos]);
				}
			}
			exprListStack.pop_back();
			temp = temp1;
			temp.dataType = searchRootId(tokenSeq[tokenSeqPos].content).retDataType;
			temp.idType = _none;
			idStack.push_back(temp);
			tokenSeqPos += 3;
			break;

		case 72: // factor->punc_round_left expression punc_round_right
			tokenSeqPos += 2;
			break;

		case 73: // factor->not factor
			if (idStack[idStack.size() - 1].dataType.basicType != _boolean)
			{
				reportError("not boolean type in not.", tokenSeq[tokenSeqPos - 1]);
			}
			idStack.back().idType = _none;
			tokenSeqPos += 1;
			break;

		case 74: // factor->addop_sub factor
			if (idStack[idStack.size() - 1].dataType.basicType > 2)
			{
				reportError("not integer(real) type in minus.", tokenSeq[tokenSeqPos - 1]);
			}
			idStack.back().idType = _none;
			tokenSeqPos += 1;
			break;

		case 75: // relop->relop_e
			opStack.push_back("e");
			tokenSeqPos += 1;
			break;

		case 76: // relop->relop_ne
			opStack.push_back("ne");
			tokenSeqPos += 1;
			break;

		case 77: // relop->relop_l
			opStack.push_back("l");
			tokenSeqPos += 1;
			break;

		case 78: // relop->relop_le
			opStack.push_back("le");
			tokenSeqPos += 1;
			break;

		case 79: // relop->relop_g
			opStack.push_back("g");
			tokenSeqPos += 1;
			break;

		case 80: // relop->relop_ge
			opStack.push_back("ge");
			tokenSeqPos += 1;
			break;

		case 81: // addop->addop_add
			opStack.push_back("add");
			tokenSeqPos += 1;
			break;

		case 82: // addop->addop_sub
			opStack.push_back("sub");
			tokenSeqPos += 1;
			break;

		case 83: // addop->addop_or
			opStack.push_back("or");
			tokenSeqPos += 1;
			break;

		case 84: // num->fnum
			temp = temp1;
			temp.idType = _constant;
			temp.dataType.basicType = _real;
			idStack.push_back(temp);
			tokenSeqPos += 1;
			break;

		case 85: // num->digits
			temp = temp1;
			temp.idType = _constant;
			temp.dataType.basicType = _integer;
			temp.dataType.constVal = atoi(tokenSeq[tokenSeqPos].content.c_str());
			idStack.push_back(temp);
			tokenSeqPos += 1;
			break;

		case 86: // mulop->mulop_mul
			opStack.push_back("mul");
			tokenSeqPos += 1;
			break;

		case 87: // mulop->mulop_divide
			opStack.push_back("divide");
			tokenSeqPos += 1;
			break;

		case 88: // mulop->mulop_div
			opStack.push_back("div");
			tokenSeqPos += 1;
			break;

		case 89: // mulop->mulop_mod
			opStack.push_back("mod");
			tokenSeqPos += 1;
			break;

		case 90: // mulop->mulop_and
			opStack.push_back("and");
			tokenSeqPos += 1;
			break;
		}

		/*
		if (idStack.size())
			cerr << idStack.size() << ":" << (int)idStack[idStack.size() - 1].dataType.basicType << "xxx" << endl;
		else
			cerr << idStack.size() << ":"
				 << "xxx" << endl;
		cerr << "exprListStackSize" << exprListStack.size() << endl;
		if (exprListStack.size())
			cerr << "exprList.size()=" << exprListStack[exprListStack.size() - 1].size() << endl;
		*/
	}
	//cerr<<"RList.size()="<<rlistRecord.size()<<endl;
}
