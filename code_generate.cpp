#include "code_generate.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#pragma execution_character_set("utf-8")
using namespace std;

int token_seq_pos1;

string arraySub(string funName, string &s, int &pos)
{
	string re = "";
	for (; pos < s.size(); ++pos)
	{
		if (s[pos] == '[')
		{
			string arrayName = "";
			for (int j = pos - 1; j >= 0; --j)
			{
				if (('a' <= s[j] && s[j] <= 'z') || ('A' <= s[j] && s[j] <= 'Z') || ('0' <= s[j] && s[j] <= '9') || s[j] == '_')
					arrayName.push_back(s[j]);
				else
					break;
			}
			reverse(arrayName.begin(), arrayName.end());
			Id arrayId = getArray(funName, arrayName);
			for (auto i : arrayId.dataType.lowerBound)
			{
				++pos;
				re += '[' + arraySub(funName, s, pos) + '-' + to_string(i) + ']';
			}
			--pos;
		}
		else if (s[pos] == ']')
		{
			++pos;
			break;
		}

		else
			re.push_back(s[pos]);
	}
	return re;
}

bool isInt(string str)
{
	bool isNum = false;

	int index = 0;
	for (; str[index] != '\0'; index++)
	{
		switch (str[index])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			isNum = true;
			break;
		case '-':
		case '+':
			if (index != 0)
			{
				return false;
			}
			break;
		default:
			return false;
		}
	}

	if (!isNum)
	{
		return false;
	}

	return true;
}

string id_type(string s)
{
	int i = 0;
	if (s == "true")
		return "bool";
	if (s == "flase")
		return "bool";
	if (s[0] == '\'')
		return "char";
	for (; i < s.length(); i++)
	{
		bool flag;
		flag = isInt(s);
		if (flag == true)
			return "short";
		else
			return "float";
	}
	return "";
}

string idtoType(Id name_to_id) //�ҵ������ڷ��ű��ж�Ӧ������
{
	string id_type;
	if (name_to_id.dataType.basicType == 1)
		id_type = "short";
	else if (name_to_id.dataType.basicType == 2)
		id_type = "float";
	else if (name_to_id.dataType.basicType == 3)
		id_type = "bool";
	else if (name_to_id.dataType.basicType == 4)
		id_type = "char";
	else
		id_type = "none";
	return id_type;
}

string type2Place(string t)
{
	if (t == "short")
		return "%hd";
	else if (t == "float")
		return "%lf";
	else if (t == "char")
		return "%c";
	else
		return "%hd";
}

vector<string> splitString(string s, char c)
{
	vector<string> v;
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + 1;
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
	return v;
}

string replaceString(string s, char c, string r)
{
	string res = "";
	int i = 0;
	for (auto j : splitString(s, c))
	{
		if (i == 0)
			res = j;
		else
			res += r + j;
		i += 1;
	}
	return res;
}

int calculate(int num)
{
	switch (num)
	{
	case 1:
		token_seq_pos1 -= 2;
		return token_seq_pos1;
	case 2:
		token_seq_pos1 -= 4;
		return token_seq_pos1;
	case 3:
		token_seq_pos1 -= 2;
		return token_seq_pos1;
	case 5:
		token_seq_pos1 -= 2;
		return token_seq_pos1;
	case 7:
		token_seq_pos1 -= 2;
		return token_seq_pos1;
	case 9:
		token_seq_pos1 -= 3;
		return token_seq_pos1;
	case 10:
		token_seq_pos1 -= 2;
		return token_seq_pos1;
	case 15:
		token_seq_pos1 -= 2;
		return token_seq_pos1;
	case 17:
		token_seq_pos1 -= 2;
		return token_seq_pos1;
	case 20:
		token_seq_pos1 -= 4;
		return token_seq_pos1;
	case 25:
		token_seq_pos1 -= 5;
		return token_seq_pos1;
	case 26:
		token_seq_pos1 -= 4;
		return token_seq_pos1;
	case 27:
		token_seq_pos1 -= 1;
		return token_seq_pos1;
	case 29:
		token_seq_pos1 -= 1;
		return token_seq_pos1;
	case 30:
		token_seq_pos1 -= 2;
		return token_seq_pos1;
	case 31:
		token_seq_pos1 -= 3;
		return token_seq_pos1;
	case 32:
		token_seq_pos1 -= 2;
		return token_seq_pos1;
	case 34:
		token_seq_pos1 -= 1;
		return token_seq_pos1;
	case 38:
		token_seq_pos1 -= 1;
		return token_seq_pos1;
	case 39:
		token_seq_pos1 -= 1;
		return token_seq_pos1;
	case 41:
		token_seq_pos1 -= 2;
		return token_seq_pos1;
	case 47:
		token_seq_pos1 -= 2;
		return token_seq_pos1;
	case 48:
		token_seq_pos1 -= 5;
		return token_seq_pos1;
	case 49:
		token_seq_pos1 -= 3;
		return token_seq_pos1;
	case 50:
		token_seq_pos1 -= 3;
		return token_seq_pos1;
	case 55:
		token_seq_pos1 -= 2;
		return token_seq_pos1;
	case 58:
		token_seq_pos1 -= 3;
		return token_seq_pos1;
	case 71:
		token_seq_pos1 -= 3;
		return token_seq_pos1;
	case 72:
		token_seq_pos1 -= 2;
		return token_seq_pos1;
	case 6:
	case 11:
	case 12:
	case 14:
	case 18:
	case 21:
	case 22:
	case 23:
	case 24:
	case 42:
	case 44:
	case 52:
	case 54:
	case 57:
	case 59:
	case 61:
	case 73:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	case 80:
	case 81:
	case 82:
	case 83:
	case 84:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 90:
		token_seq_pos1 -= 1;
		return token_seq_pos1;
	case 4:
	case 8:
	case 13:
	case 16:
	case 19:
	case 28:
	case 33:
	case 35:
	case 36:
	case 37:
	case 40:
	case 43:
	case 45:
	case 46:
	case 51:
	case 53:
	case 56:
	case 60:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
	case 70:
		return token_seq_pos1;
	defalut:
		return token_seq_pos1;
	}
	return token_seq_pos1;
}

string generateCode(vector<int> &product_seq, vector<token> &token_seq, int token_seq_pos)
{
	int s = -1;
	string res = "";
	vector<Id> id;
	vector<Id> id2;
	vector<string> str;
	Id id1;
	Id id3;
	int a, b, flag, i, j, strLen;
	int m;
	string sa, sb, sc, sd;
	string idtype;
	string arrayName;
	string funName, fun_name;
	string str1, str2;
	vector<string> str4;
	int size, size1;
	while (s < 0 && !product_seq.empty())
	{
		s = product_seq.back();
		//cerr << "s=" << s << endl;
		product_seq.pop_back();
	}
	token_seq_pos = calculate(s);
	switch (s)
	{
	case 0: //S->programstruct
		return generateCode(product_seq, token_seq, token_seq_pos);
	case 1:
		//1:programStruct->program_head ; program_body .
		//cerr << "token_seq_pos = " << token_seq_pos << endl;
		res = "#include <stdio.h>\n\n" +
			  generateCode(product_seq, token_seq, token_seq_pos) + "" +
			  generateCode(product_seq, token_seq, token_seq_pos);
		return res;

	case 2: //program_head->program id (idlist)
		token_seq[token_seq_pos + 2].content;
		//cerr << "token_seq_pos = " << token_seq_pos << endl;
		//cerr << token_seq[token_seq_pos + 2].content << endl;
		generateCode(product_seq, token_seq, token_seq_pos);
		return res;
	case 3: // program_head->program id
		token_seq[token_seq_pos + 2].content;
		return res;

	case 4: //program_body->const_declarations var_declarations subprogram_declarations compound_statement
		sa = generateCode(product_seq, token_seq, token_seq_pos);
		strLen = 0;
		sa = arraySub("", sa, strLen);
		res = generateCode(product_seq, token_seq, token_seq_pos) +
			  generateCode(product_seq, token_seq, token_seq_pos) +
			  generateCode(product_seq, token_seq, token_seq_pos) + "int main()\n{\n" +
			  sa + "return 0;\n}";
		return res;

	case 5: //idlist->idlist , id
		res = token_seq[token_seq_pos + 2].content;
		res = generateCode(product_seq, token_seq, token_seq_pos) + "," + res;
		return res;

	case 6: //idlist->id
		res = token_seq[token_seq_pos + 1].content;
		return res;

	case 7: //const_declarations->const const_declaration ;
		res = generateCode(product_seq, token_seq, token_seq_pos);
		return res;

	case 8: //const_declarations->null
		return "";

	case 9: //26:const_declaration->const_declaration ; id = const_value
		res = token_seq[token_seq_pos + 2].content;
		//res = "const " + idtoType(res, res_id_table,name_to_id) + " " + res;
		//res += "=" + generateCode(product_seq, token_seq, token_seq_pos,get_rlist,get_wlist, res_id_table, name_to_id)+";\n";
		//res = generateCode(product_seq, token_seq, token_seq_pos,get_rlist,get_wlist, res_id_table, name_to_id) + res;
		sa = generateCode(product_seq, token_seq, token_seq_pos);
		sb = generateCode(product_seq, token_seq, token_seq_pos);
		idtype = id_type(sa);
		res = sb + "const " + idtype + " " + res + "=" + sa + ";\n";
		return res;

	case 10: //25:const_declaration->id = const_value
		res = token_seq[token_seq_pos + 1].content;
		sb = generateCode(product_seq, token_seq, token_seq_pos);
		idtype = id_type(sb);
		res = "const " + idtype + " " + res + "=" + sb + ";\n";
		return res;

	case 11: //const_value->addop_add num
		return "+" + generateCode(product_seq, token_seq, token_seq_pos);

	case 12: //const_value->addop_sub num
		return "-" + generateCode(product_seq, token_seq, token_seq_pos);

	case 13: //const_value->num
		return generateCode(product_seq, token_seq, token_seq_pos);

	case 14: //const_value->letter
		res = token_seq[token_seq_pos + 1].content;
		return res;

	case 15: //var_declarations->var var_declaration punc_semicolon
		res = generateCode(product_seq, token_seq, token_seq_pos);
		return res;

	case 16: //var_declarations->null
		return "";

	case 17:													  //var_declaration->var_declaration punc_semicolon idlist punc_colon type
		sb = generateCode(product_seq, token_seq, token_seq_pos); //type
		sa = generateCode(product_seq, token_seq, token_seq_pos);
		//cerr << "sa=" << sa << endl;
		//cerr << "sb=" << sb << endl;
		if (sb.find('[') == string::npos)
		{
			res = sb + " " + sa + ";\n";
		}
		else
		{
			for (auto s : splitString(sa, ','))
			{
				res += replaceString(sb, ' ', " " + s) + ";\n";
			}
		}
		res = generateCode(product_seq, token_seq, token_seq_pos) + res; //var+declaration
		return res;

	case 18:													  //var_declaration->idlist punc_colon type
		sb = generateCode(product_seq, token_seq, token_seq_pos); //type
		sa = generateCode(product_seq, token_seq, token_seq_pos);
		//cerr << "sa=" << sa << endl;
		//cerr << "sb=" << sb << endl;
		if (sb.find('[') == string::npos)
		{
			res = sb + " " + sa + ";\n";
		}
		else
		{
			for (auto s : splitString(sa, ','))
			{
				res += replaceString(sb, ' ', " " + s) + ";\n";
			}
		}
		return res;

	case 19: //type->basic_type
		return generateCode(product_seq, token_seq, token_seq_pos);

	case 20: //type->array punc_square_left period punc_square_right of basic_type
		res = generateCode(product_seq, token_seq, token_seq_pos);
		res += " [" + generateCode(product_seq, token_seq, token_seq_pos) + "]";
		return res;

	case 21: //basic_type->integer
		return "short";

	case 22: //basic_type->real
		return "float";

	case 23: //basic_type->boolean
		return "bool";

	case 24: //basic_type->char
		return "char";

	case 25: //period->period punc_comma digits punc_point punc_point digits
		a = atoi(token_seq[token_seq_pos + 2].content.c_str());
		b = atoi(token_seq[token_seq_pos + 5].content.c_str());
		res = generateCode(product_seq, token_seq, token_seq_pos) + "][" + std::to_string(b - a + 1);
		return res;

	case 26: //period->digits punc_point punc_point digits
		a = atoi(token_seq[token_seq_pos + 1].content.c_str());
		b = atoi(token_seq[token_seq_pos + 4].content.c_str());
		return std::to_string(b - a + 1);

	case 27: //subprogram_declarations->subprogram_declarations subprogram punc_semicolon
		res = generateCode(product_seq, token_seq, token_seq_pos) +
			  generateCode(product_seq, token_seq, token_seq_pos);
		return res;

	case 28: //subprogram_declarations->null
		return "";

	case 29: //subprogram->subprogram_head punc_semicolon subprogram_body
		res = generateCode(product_seq, token_seq, token_seq_pos) + generateCode(product_seq, token_seq, token_seq_pos);
		sb = splitString(splitString(res, ' ')[1], '(')[0];
		sa = splitString(res, '\a')[1];

		strLen = 0;
		sa = arraySub(sb, sa, strLen);
		res = splitString(res, '\a')[0] + sa;
		if (res.substr(0, 4) != "void")
		{
			sa = splitString(res, ' ')[0];
			//cerr << "sa=" << sa << endl;
			size = splitString(res, ' ')[0].size();
			//cerr << sa[size - 1] << endl;
			sb = splitString(splitString(res, ' ')[1], '(')[0] + "_" + "returnVal";
			if (sa == "short")
				sa = "{\nshort " + sb + ";";
			if (sa == "char")
				sa = "{\nchar " + sb + ";";
			if (sa == "bool")
				sa = "{\nbool " + sb + ";";
			if (sa == "float")
				sa = "{\nfloat " + sb + ";";
			flag = 0;
			sc = "{";
			while (res.find(sc) != string::npos)
			{
				if (flag == 0)
				{
					res = res.replace(res.find(sc), sc.size(), sa);
					flag++;
					//cerr << "flag=" << flag << endl;
				}
				else
					break;
			}
			flag = 0;
			sb = splitString(splitString(res, ' ')[1], '(')[0] + "_" + "returnVal=";
			sc = splitString(splitString(res, ' ')[1], '(')[0] + "=";
			while (res.find(sc) != string::npos)
			{
				res = res.replace(res.find(sc), sc.size(), sb);
			}
			sc = splitString(splitString(res, ' ')[1], '(')[0] + ";";
			sb = splitString(splitString(res, ' ')[1], '(')[0] + "_" + "returnVal;";
			while (res.find(sc) != string::npos)
			{
				res = res.replace(res.find(sc), sc.size(), sb);
			}
			sc = "}";
			sb = "return " + sb + "\n}";
			while (res.find(sc) != string::npos)
			{
				strLen = 0;
				if (flag == 0)
				{
					res = res.replace(res.find(sc), sc.size(), sb);
					flag++;
				}
				else
					break;
			}
		}
		return res;

	case 30: //subprogram_head->procedure id formal_parameter
		res = "void " + token_seq[token_seq_pos + 2].content;
		//cerr << "token_seq_pos = " << token_seq_pos << endl;
		//cerr << "id=" << token_seq[token_seq_pos + 2].content << endl;
		res += generateCode(product_seq, token_seq, token_seq_pos);
		return res;

	case 31: // subprogram_head->function id formal_parameter punc_colon basic_type
		res = token_seq[token_seq_pos + 2].content;
		res = generateCode(product_seq, token_seq, token_seq_pos) + " " + res;
		res += generateCode(product_seq, token_seq, token_seq_pos);
		return res;

	case 32: //formal_parameter->punc_round_left parameter_list punc_round_right
		res = "(" + generateCode(product_seq, token_seq, token_seq_pos) + ")";
		return res;

	case 33: //formal_parameter->null
		return "";

	case 34: //parameter_list->parameter_list punc_semicolon parameter
		res = generateCode(product_seq, token_seq, token_seq_pos) + "," +
			  generateCode(product_seq, token_seq, token_seq_pos);
		return res;

	case 35: //parameter_list->parameter
	case 36: //parameter->var_parameter
	case 37: //parameter->value_parameter
		return generateCode(product_seq, token_seq, token_seq_pos);

	case 38: //var_parameter->var value_parameter
		res = replaceString(generateCode(product_seq, token_seq, token_seq_pos), ' ', " &");
		return res;

	case 39: //value_parameter->idlist punc_colon basic_type
		sb = generateCode(product_seq, token_seq, token_seq_pos);
		res = replaceString("," + generateCode(product_seq, token_seq, token_seq_pos), ',', "," + sb + " ");
		res = res.substr(1);
		return res;

	case 40: //subprogram_body->const_declarations var_declarations compound_statement
		return "\n{\n" + generateCode(product_seq, token_seq, token_seq_pos) +
			   generateCode(product_seq, token_seq, token_seq_pos) + "\a" + generateCode(product_seq, token_seq, token_seq_pos) + "}\n";
		return res;

	case 41: //compound_statement->begin statement_list end
		res = "" + generateCode(product_seq, token_seq, token_seq_pos) + "";
		//cerr << "res=" << res << endl;
		return res;

	case 42: //statement_list->statement_list punc_semicolon statement
		res = generateCode(product_seq, token_seq, token_seq_pos) + "" +
			  generateCode(product_seq, token_seq, token_seq_pos) + "";
		return res;

	case 43: // statement_list->statement
		return generateCode(product_seq, token_seq, token_seq_pos) + "";

	case 44: //statement->variable assignop expression
		res = generateCode(product_seq, token_seq, token_seq_pos) +
			  "=" + generateCode(product_seq, token_seq, token_seq_pos) + ";\n";
		return res;

	case 45: // statement->procedure_call
		return generateCode(product_seq, token_seq, token_seq_pos) + ";\n";

	case 46: // statement->compound_statement
		res = generateCode(product_seq, token_seq, token_seq_pos);
		if (count(res.begin(), res.end(), ';') > 1)
			res = "{\n" + res + "}\n";
		return res;

	case 47: // statement->if expression then statement else_part
		res = "if(" + generateCode(product_seq, token_seq, token_seq_pos) + ") " +
			  generateCode(product_seq, token_seq, token_seq_pos) + "" +
			  generateCode(product_seq, token_seq, token_seq_pos);
		return res;

	case 48: // statement->for id assignop expression to expression do statement
		sa = token_seq[token_seq_pos + 2].content;
		//cerr << "sa=" << sa << endl;
		res = "for(" + sa + "=" +
			  generateCode(product_seq, token_seq, token_seq_pos) + ";" +
			  sa + "<=" + generateCode(product_seq, token_seq, token_seq_pos) + ";" + sa + "++) " + generateCode(product_seq, token_seq, token_seq_pos);
		return res;

	case 49: // statement->read punc_round_left variable_list punc_round_right
		sb = generateCode(product_seq, token_seq, token_seq_pos);
		//for (auto s : splitString(sb, '|'))
		//{
		//	sa = splitString(s, '[')[0];
		//id1 = id.front();
		//cerr << "sb=" << sb << endl;
		id = getRlist();
		reverse(id.begin(), id.end());
		while (!id.empty())
		{
			id1 = id.back();
			id.pop_back();
			res += " " + type2Place(idtoType(id1));
		}
		res = "\"" + res.substr(1) + "\"" + replaceString("|" + sb, '|', ",&");
		return "scanf(" + res + ");\n";

	case 50: // statement->write punc_round_left expression_list punc_round_right
		sb = generateCode(product_seq, token_seq, token_seq_pos);
		/*for (auto s : splitString(sb, ','))
		{
			sa = splitString(s, '[')[0];
			cerr << "sa1= "<< sa << " ";
			sa = splitString(sa, '(')[0];
			id = getWlist()[i];
			res += " " + type2Place(idtoType(id));
			i++;
			//cerr << "idtype(sa)= " << id_type(sa) << " ";
		}*/
		id2 = getWlist();
		reverse(id2.begin(), id2.end());
		while (!id2.empty())
		{
			id1 = id2.back();
			id2.pop_back();
			res += " " + type2Place(idtoType(id1));
		}
		//cerr << res << endl;
		res = "\"" + res.substr(1) + "\"" + replaceString("|" + sb, '|', ",");
		return "printf(" + res + ");\n";

	case 51: // statement->null
		return "";

	case 52: // variable_list->variable_list punc_comma variable
		res = generateCode(product_seq, token_seq, token_seq_pos) +
			  "|" + generateCode(product_seq, token_seq, token_seq_pos);
		return res;

	case 53: // variable_list->variable
		return generateCode(product_seq, token_seq, token_seq_pos);

	case 54: // variable->id id_varpart
		res = token_seq[token_seq_pos + 1].content;
		res += "" + generateCode(product_seq, token_seq, token_seq_pos);
		return res;

	case 55: // id_varpart->punc_square_left expression_list puncsquare_right
		res = "[" + replaceString(generateCode(product_seq, token_seq, token_seq_pos), ',', "][") + "]";
		return res;

	case 56: // id_varpart->null
		return "";

	case 57: // procedure_call->id
		res = token_seq[token_seq_pos + 1].content + "()";
		return res;

	case 58: // procedure_call->id punc_round_left expression_list punc_round_right
		res = token_seq[token_seq_pos + 1].content;
		res += "(" + generateCode(product_seq, token_seq, token_seq_pos) + ")";
		return res;

	case 59: // else_part->else statement
		res = "else " + generateCode(product_seq, token_seq, token_seq_pos) + "";
		return res;

	case 60: // else_part->null
		return "";

	case 61: // expression_list->expression_list punc_comma expression
		res = generateCode(product_seq, token_seq, token_seq_pos) + "," +
			  generateCode(product_seq, token_seq, token_seq_pos);
		return res;

	case 62: // expression_list->expression
	case 64: // expression->simple_expression
	case 66: // simple_expression->term
		return generateCode(product_seq, token_seq, token_seq_pos);

	case 63: // expression->simple_expression relop simple_expression
	case 65: // simple_expression->simple_expression addop term
	case 67: // term->term mulop factor
		return generateCode(product_seq, token_seq, token_seq_pos) + "" +
			   generateCode(product_seq, token_seq, token_seq_pos) + "" +
			   generateCode(product_seq, token_seq, token_seq_pos);

	case 68: // term->factor
	case 69: // factor->num
	case 70: // factor->variable
		return generateCode(product_seq, token_seq, token_seq_pos);

	case 71: // factor->id punc_round_left expression_list punc_round_right
		res = token_seq[token_seq_pos + 1].content;
		res += "(" + generateCode(product_seq, token_seq, token_seq_pos) + ")";
		return res;

	case 72: // factor->punc_round_left expression punc_round_right
		res = "(" + generateCode(product_seq, token_seq, token_seq_pos) + ")";
		return res;

	case 73: // factor->not factor
		res = "!(" +
			  generateCode(product_seq, token_seq, token_seq_pos) + ")";
		return res;

	case 74: // factor->addop_sub factor
		res = "-" + generateCode(product_seq, token_seq, token_seq_pos);
		return res;

	case 75: // relop->relop_e
		return "==";

	case 76: // relop->relop_ne
		return "!=";

	case 77: // relop->relop_l
		return "<";

	case 78: // relop->relop_le
		return "<=";

	case 79: // relop->relop_g
		return ">";

	case 80: // relop->relop_ge
		return ">=";

	case 81: // addop->addop_add
		return "+";

	case 82: // addop->addop_sub
		return "-";

	case 83: // addop->addop_or
		return "||";

	case 84: // num->fnum
		res = token_seq[token_seq_pos + 1].content;
		return res;

	case 85: // num->digits
		res = token_seq[token_seq_pos + 1].content;
		return res;

	case 86: // mulop->mulop_mul
		return "*";

	case 87: // mulop->mulop_divide
		return "/(float)";

	case 88: // mulop->mulop_div
		return "/";

	case 89: // mulop->mulop_mod
		return "%";

	case 90: // mulop->mulop_and
		return "&&";
	}
	return "";
}
string generate_Code(vector<int> &product_seq, vector<token> &token_seq, int token_seq_pos)
{
	token_seq_pos1 = token_seq_pos;
	return generateCode(product_seq, token_seq, token_seq_pos);
}
