_Pragma("once")
#include "grammar_analysis.h"
#include "semantic_analysis.h"
using namespace std;
bool isInt(string str);//判断一个字符串是不是整数
string  id_type(string s);//找到常量对应的类型
string idtoType(Id name_to_id);//找到变量对应的类型
string type2Place(string t);//找到类型对应的输出格式符，如char对应%c
vector<string> splitString(string s, char c);//切分字符串
string replaceString(string s, char c, string r);//连接字符串
int calculate(int num);//计算单词在单词序列中的位置
string generateCode(vector<int>& product_seq, vector<token>& token_seq, int token_seq_pos);//生成C语言句子
string generate_Code(vector<int>& product_seq, vector<token>& token_seq, int token_seq_pos);//生成目标代码
string arraySub(string funName,string &s,int &pos);
