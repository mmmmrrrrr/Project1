#pragma once
#include <bits/stdc++.h>
using namespace std;
enum Basic_Type
{
	_integer = 0,
	_real,
	_boolean,
	_char,
	_array
};
struct Data_Type {
	Basic_Type basic_type;
	int constant_value;
	int dimension;//Ϊ0������ͨ������Ϊ1��ʾһά���飬Ϊ2��ʾ��ά...
	vector<int> upper_bound;//��������
	vector<int> lower_bound;//��������
};
enum Parameter_Type
{
	_value = 1,
	_reference
}; //��ֵ���ã����õ���
struct Parameter {
	string name;
	Data_Type data_type;
	Parameter_Type parameter_type;
};
enum Identifer_Type
{
	_constant = 0,
	_variable,
	_procedure,
	_function
}; //�����ʶ����������

struct Identifier {
	string name;
	Data_Type data_type;
	Identifer_Type identifer_type;
	vector<Parameter> parameter_list;//�����б�
	Data_Type return_value_data_type;//����ֵ����
	bool is_error;//�Ƿ���ڴ���
}error_id;
struct Id_Table
{
	Id_Table *father;//ָ�򸸷��ű��ָ��
	unordered_map<string, Identifier> name_to_id;//hash��洢���ֵ���ʶ����ӳ��
}*now_ID_table;
struct Error_Information
{
	int line_num;
	string error_type;
};//��¼������Ϣ��Ϊ����ֵ

vector<string>operator_stack;
vector<Identifier> Id_stack;

Identifier search_identifier(string name);//�������ֽ��в��ң����ر�ʶ����Ϣ
bool insert_identifier(Identifier id);//�����ʶ������ǰλ��
void location();
void relocation();
void init_Id_Table();
void semantic_analysis(vector<int> &product_sequence, vector<string> token_sequence);