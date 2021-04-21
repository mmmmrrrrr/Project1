#ifndef SEMANTIC
#define SEMANTIC
#include "grammar_analysis.h"
#include <bits/stdc++.h>
using namespace std;

enum BasicType : int
{
	_integer = 1,
	_real,
	_boolean,
	_char,
};	
enum ParamType : int
{
	_reference = 1,
	_value
}; //传值调用，引用调用
struct DataType
{
	BasicType basicType;
	int constVal;
	int dimension;			//为0代表普通变量，为1表示一维数组，为2表示二维...
	vector<int> upperBound; //数组上限
	vector<int> lowerBound; //数组下限
	ParamType param_type;
	bool operator==(const DataType &t) const;
};
enum IdType : int
{
	_constant = 1,
	_variable,
	_procedure,
	_function
}; //定义标识符所属类型

struct Param
{
	string name;
	DataType dataType;
};

struct Id
{
	string name;
	DataType dataType;
	IdType idType;
	vector<Param> paramList; //参数列表
	DataType retDataType;	 //返回值类型
	bool isError;			 //是否存在错误
};
struct IdTable
{
	IdTable *father;		  //指向父符号表的指针
	map<string, Id> nameToId; //hash表存储名字到标识符的映射
} ;
struct Error_Information
{
	int line_num;
	string error_type;
}; //记录错误信息作为返回值

void reportError(string errInformation, token t);
Id searchId(string name); //根据名字进行查找，返回标识符信息
bool insertId(Id id);	  //插入标识符到当前位置
void location();
void relocation();
void initIdTable();
void semantic_analysis(const vector<int> &product_sequence, const vector<token> &token_sequence, map<int,string> &numToProduct);
#endif