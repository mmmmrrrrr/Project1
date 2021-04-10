#pragma once
#include <bits/stdc++.h>
using namespace std;

enum Basic_Type:int
{
	_integer = 1,
	_real,
	_boolean,
	_char,
};
struct Data_Type
{
	Basic_Type basic_type;
	int constant_value;
	int dimension;			 //为0代表普通变量，为1表示一维数组，为2表示二维...
	vector<int> upper_bound; //数组上限
	vector<int> lower_bound; //数组下限
	bool operator==(const Data_Type &t) const;
};
enum Parameter_Type:int
{
	_value = 1,
	_reference
}; //传值调用，引用调用
struct Parameter
{
	string name;
	Data_Type data_type;
	Parameter_Type parameter_type;
};
enum Id_Type:int
{
	_constant = 1,
	_variable,
	_procedure,
	_function
}; //定义标识符所属类型

struct Id
{
	string name;
	Data_Type data_type;
	Id_Type id_type;
	vector<Parameter> parameter_list; //参数列表
	Data_Type return_value_data_type; //返回值类型
	bool is_error;					  //是否存在错误
} error_id, temp1;
struct Id_Table
{
	Id_Table *father;					//指向父符号表的指针
	map<string, Id> name_to_id; //hash表存储名字到标识符的映射
} * now_id_table;
struct Error_Information
{
	int line_num;
	string error_type;
}; //记录错误信息作为返回值

vector<string> operator_stack;
vector<Id> id_stack;
vector<Id> idList;
vector<vector<Id>> exprListStack;
map<int, string> numToProduct;

void encouter_an_error(string error_information, int line);
Id search_id(string name); //根据名字进行查找，返回标识符信息
bool insert_id(Id id);	   //插入标识符到当前位置
void location();
void relocation();
void init_id_table();
void semantic_analysis(vector<int> &product_sequence, vector<string> token_sequence);