#include "1.h"
Identifier search_identifier(string name) {
	for (Id_Table *t = now_ID_table; t != NULL; t = t->father)
	{
		if (t->name_to_id.find(name) != t->name_to_id.end())
			return t->name_to_id[name];
	}
	return error_id;
}//根据名字进行查找，返回标识符信息
bool insert_identifier(Identifier id) {
	if (now_ID_table->name_to_id.find(id.name) != now_ID_table->name_to_id.end())
		return 0;
	now_ID_table->name_to_id[id.name] = id;
	return 1;
}//返回1表示正确插入，返回0表示已经有重复的标识符
void location() {
	auto t = now_ID_table;
	now_ID_table = new Id_Table;
	now_ID_table->father = t;
}//定位
void relocation() {
	auto t = now_ID_table->father;
	delete now_ID_table;
	now_ID_table = t;
}//重定位
void init_ID_Table() {
	error_id.is_error = 1;
	location();
}
void semantic_analysis(vector<int> &product_sequence, vector<string> token_sequence) {
	Identifier temp, temp1;
	for (auto i : product_sequence) {
		switch (i) {
		case 11://const_value->addop_add num
			Id_stack[Id_stack.size() - 1].identifer_type = _constant;
			break;
		case 12://const_value->addop_sub num
			Id_stack[Id_stack.size() - 1].identifer_type = _constant;
			Id_stack[Id_stack.size() - 1].data_type.constant_value = -Id_stack[Id_stack.size() - 1].data_type.constant_value;
			break;
		case 13://const_value->num  //?
			Id_stack[Id_stack.size() - 1].identifer_type = _constant;
			insert_identifier(temp);
			break;
		case 14://const_value->letter
			temp = temp1;
			temp.data_type.basic_type = _char;
			temp.identifer_type = _constant;
			Id_stack.push_back(temp);
		///////////////
		case 21://basic_type->integer
			temp = temp1;
			temp.data_type.basic_type = _integer;
			Id_stack.push_back(temp);
			break;
		case 22://basic_type->real
			temp = temp1;
			temp.data_type.basic_type = _real;
			Id_stack.push_back(temp);
			break;
		case 23://basic_type->boolean
			temp = temp1;
			temp.data_type.basic_type = _boolean;
			Id_stack.push_back(temp);
			break;
		case 24://basic_type->char
			temp = temp1;
			temp.data_type.basic_type = _char;
			Id_stack.push_back(temp);
			break;
		/////////////////////////////////////////////
		case 74://factor->addop_sub factor
			Id_stack[Id_stack.size() - 1].identifer_type = _constant;
			Id_stack[Id_stack.size() - 1].data_type.constant_value = -Id_stack[Id_stack.size() - 1].data_type.constant_value;
			break;
		case 75://relop->relop_e
			operator_stack.push_back("e");
			break;
		case 76://relop->relop_ne
			operator_stack.push_back("ne");
			break;
		case 77://relop->relop_l
			operator_stack.push_back("l");
			break;
		case 78://relop->relop_le
			operator_stack.push_back("le");
			break;
		case 79://relop->relop_g
			operator_stack.push_back("g");
			break;
		case 80://relop->relop_ge
			operator_stack.push_back("ge");
			break;
		case 81://addop->addop_add
			operator_stack.push_back("add");
			break;
		case 82://addop->addop_sub
			operator_stack.push_back("sub");
			break;
		case 83://addop->addop_or
			operator_stack.push_back("or");
			break;
		case 84: //num->fnum
			temp = temp1;
			temp.data_type.basic_type = _real;
			Id_stack.push_back(temp);
			break;
		case 85://num->digits
			temp = temp1;
			temp.data_type.basic_type = _integer;

			//还没有写赋值

			Id_stack.push_back(temp);
			break;
		case 86://mulop->mulop_mul
			operator_stack.push_back("mul");
			break;
		case 87://mulop->mulop_divide
			operator_stack.push_back("divide");
			break;
		case 88://mulop->mulop_div
			operator_stack.push_back("div");
			break;
		case 89://mulop->mulop_mod
			operator_stack.push_back("mod");
			break;
		case 90://mulop->mulop_and
			operator_stack.push_back("and");
			break;
	
		}

	}
}
int main()
{
	freopen("a.in", "r", stdin);
	vector<int> product_sequence;
	vector<string> token_sequence;
	int num;
	cin >> num;
	for (int i = 1; i <= num; ++i) {
		int val;
		cin >> val;
		product_sequence.push_back(val);
	}
	cin >> num;
	for (int i = 1; i <= num; ++i) {
		string val;
		cin >> val;
		token_sequence.push_back(val);
	}
	init_ID_Table();
	semantic_analysis(product_sequence, token_sequence);
	return 0;
}