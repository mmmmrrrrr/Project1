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
struct Data_Type{
    Basic_Type basic_type;
    int dimension;//为0代表普通变量，为1表示一维数组，为2表示二维...
    vector<int> upper_bound;//数组上限
    vector<int> lower_bound;//数组下限
};
enum Parameter_Type
{
    _value=1,
    _reference
}; //传值调用，引用调用
struct Parameter{
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
}; //定义标识符所属类型

struct Identifier{
    string name;
    Data_Type data_type;
    Identifer_Type identifer_type;
    int constant_value;
    vector<Parameter> parameter_list;//参数列表
    Data_Type return_value_data_type;//返回值类型
    bool is_error;//是否存在错误
}error_id;
struct Id_Table
{
    Id_Table *father;//指向父符号表的指针
    unordered_map<string, Identifier> name_to_id;//hash表存储名字到标识符的映射
}*now_ID_table;
struct Error_Information
{
    int line_num;
    string error_type;
};//记录错误信息作为返回值

Identifier search_identifier(string name);//根据名字进行查找，返回标识符信息
bool insert_identifier(Identifier id);//插入标识符到当前位置
void location();
void relocation();
void init_Id_Table();