#pragma once
#include <bits/stdc++.h>
#include "grammar_analysis.h"
#include"semantic_analysis.h"
using namespace std;
string  id_type(string s);
string idtoType(Id name_to_id);//�ҵ������ڷ��ű��ж�Ӧ������
string type2Place(string t);
vector<string> splitString(string s, char c);
string replaceString(string s, char c, string r);
string generateCode(vector<int>& product_seq, vector<token>& token_seq,int token_seq_pos);
string generate_Code(vector<int>& product_seq, vector<token>& token_seq,int token_seq_pos);
