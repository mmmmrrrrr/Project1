#include "grammar_analysis.h"
#include <fstream>
#include <string>

string mark_words[]   //��ź�ö��ֵ��Ӧ�ļǺţ��������
{
	/*�ս��*/
	"EOF", //�ı�����
	"null",//��
	"program",
	"const",
	"var",
	"array",
	"of",
	"integer",
	"real",
	"boolean",
	"char",
	"procedure",
	"function",
	"begin",
	"end",
	"if",
	"then",
	"for",
	"to",
	"do",
	"read",
	"write",
	"else",
	"not",
	"uminus",
	"id",
	"fnum",    //����
	"digits",  //������
	//letter,
	"assignop",
	"punc_question", //?
	"punc_not",      //~
	"punc_comma",    //,
	"punc_semicolon",//;
	"punc_colon",    //:
	"punc_point",    //.
	"punc_round_left",  //(
	"punc_round_right", //)
	"punc_square_left", //[
	"punc_square_right",//]
	"mulop_and",
	"mulop_div",   //����
	"mulop_mod",
	"mulop_mul",
	"mulop_divide",//�������
	"addop_or",
	"addop_add",
	"addop_sub",
	"relop_e",     //=
	"relop_ne",    //<>
	"relop_l",     //<
	"relop_le",    //<=
	"relop_g",     //>
	"relop_ge",    //>=
	"single_quote",//'
	"letter",      //�ַ�����
	"Boundary",    //---�ս������ս���ķֽ���---
	/*���ս��*/
	"S",
	"programstruct",
	"program_head",
	"program_body",
	"idlist",
	"mulop",
	"relop",
	"addop",
	"num",
	"const_declarations",
	"const_declaration",
	"const_value",
	"var_declarations",
	"var_declaration",
	"type",
	"basic_type",
	"period",
	"subprogram_declarations",
	"subprogram",
	"subprogram_head",
	"formal_parameter",
	"parameter_list",
	"parameter",
	"var_parameter",
	"value_parameter",
	"subprogram_body",
	"compound_statement",
	"statement_list",
	"statement",
	"variable_list",
	"variable",
	"id_varpart",
	"procedure_call",
	"else_part",
	"expression_list",
	"expression",
	"simple_expression",
	"term",
	"factor",
	//"��"
	////NOW //DFA�еĵ� .
};

Grammar initGrammer()
{
	Grammar grammar;

	grammar.push_back({ S,programstruct });
	grammar.push_back({ programstruct,program_head,punc_semicolon,program_body,punc_point});
	grammar.push_back({ program_head,program_,id,punc_round_left,idlist,punc_round_right });
	grammar.push_back({ program_head,program_,id });
	grammar.push_back({ program_body,const_declarations,var_declarations,subprogram_declarations,compound_statement });
	grammar.push_back({ idlist,idlist,punc_comma,id });
	grammar.push_back({ idlist,id });
	grammar.push_back({ const_declarations,const_,const_declaration,punc_semicolon });
	grammar.push_back({ const_declarations,null_ });
	grammar.push_back({ const_declaration,const_declaration,punc_semicolon,id,relop_e,const_value });
	grammar.push_back({ const_declaration,id,relop_e,const_value });
	grammar.push_back({ const_value,addop_add,num });
	grammar.push_back({ const_value,addop_sub,num });
	grammar.push_back({ const_value,num });
	grammar.push_back({ const_value,letter });
	grammar.push_back({ var_declarations,var_,var_declaration,punc_semicolon });
	grammar.push_back({ var_declarations,null_ });
	grammar.push_back({ var_declaration,var_declaration,punc_semicolon,idlist,punc_colon,type });
	grammar.push_back({ var_declaration,idlist,punc_colon,type });
	grammar.push_back({ type,basic_type });
	grammar.push_back({ type,array_,punc_square_left,period,punc_square_right,of_,basic_type });
	grammar.push_back({ basic_type,integer_ });
	grammar.push_back({ basic_type,real_ });
	grammar.push_back({ basic_type,boolean_ });
	grammar.push_back({ basic_type,char_ });
	grammar.push_back({ period,period,punc_comma,digits,punc_point,punc_point,digits });
	grammar.push_back({ period,digits,punc_point,punc_point,digits });
	grammar.push_back({ subprogram_declarations,subprogram_declarations,subprogram,punc_semicolon });
	grammar.push_back({ subprogram_declarations,null_ });
	grammar.push_back({ subprogram,subprogram_head,punc_semicolon,subprogram_body });
	grammar.push_back({ subprogram_head,procedure_,id,formal_parameter });
	grammar.push_back({ subprogram_head,function_,id,formal_parameter,punc_colon,basic_type });
	grammar.push_back({ formal_parameter,punc_round_left,parameter_list,punc_round_right });
	grammar.push_back({ formal_parameter,null_ });
	grammar.push_back({ parameter_list,parameter_list,punc_semicolon,parameter });
	grammar.push_back({ parameter_list,parameter });
	grammar.push_back({ parameter,var_parameter });
	grammar.push_back({ parameter,value_parameter });
	grammar.push_back({ var_parameter,var_,value_parameter });
	grammar.push_back({ value_parameter,idlist,punc_colon,basic_type });
	grammar.push_back({ subprogram_body,const_declarations,var_declarations,compound_statement });
	grammar.push_back({ compound_statement,begin_,statement_list,end_ });
	grammar.push_back({ statement_list,statement_list,punc_semicolon,statement });
	grammar.push_back({ statement_list,statement });
	grammar.push_back({ statement,variable,assignop,expression });
	grammar.push_back({ statement,procedure_call });
	grammar.push_back({ statement,compound_statement });
	grammar.push_back({ statement,if_,expression,then_,statement,else_part });
	grammar.push_back({ statement,for_,id,assignop,expression,to_,expression,do_,statement });
	grammar.push_back({ statement,read_,punc_round_left,variable_list,punc_round_right });
	grammar.push_back({ statement,write_,punc_round_left,expression_list,punc_round_right });
	grammar.push_back({ statement,null_ });
	grammar.push_back({ variable_list,variable_list,punc_comma,variable });
	grammar.push_back({ variable_list,variable });
	grammar.push_back({ variable,id,id_varpart });
	grammar.push_back({ id_varpart,punc_square_left,expression_list,punc_square_right });
	grammar.push_back({ id_varpart,null_ });
	grammar.push_back({ procedure_call,id });
	grammar.push_back({ procedure_call,id,punc_round_left,expression_list,punc_round_right });
	grammar.push_back({ else_part,else_,statement });
	grammar.push_back({ else_part,null_ });
	grammar.push_back({ expression_list,expression_list,punc_comma,expression });
	grammar.push_back({ expression_list,expression });
	grammar.push_back({ expression,simple_expression,relop,simple_expression });
	grammar.push_back({ expression,simple_expression });
	grammar.push_back({ simple_expression,simple_expression,addop,term });
	grammar.push_back({ simple_expression,term });
	grammar.push_back({ term,term,mulop,factor });
	grammar.push_back({ term,factor });
	grammar.push_back({ factor,num });
	grammar.push_back({ factor,variable });
	grammar.push_back({ factor,id,punc_round_left,expression_list,punc_round_right });
	grammar.push_back({ factor,punc_round_left,expression,punc_round_right });
	grammar.push_back({ factor,not_,factor });
	grammar.push_back({ factor,addop_sub,factor });
	grammar.push_back({ relop,relop_e });
	grammar.push_back({ relop,relop_ne });
	grammar.push_back({ relop,relop_l });
	grammar.push_back({ relop,relop_le });
	grammar.push_back({ relop,relop_g });
	grammar.push_back({ relop,relop_ge });
	grammar.push_back({ addop,addop_add });
	grammar.push_back({ addop,addop_sub });
	grammar.push_back({ addop,addop_or });
	grammar.push_back({ num,fnum });
	grammar.push_back({ num,digits });
	grammar.push_back({ mulop,mulop_mul });
	grammar.push_back({ mulop,mulop_divide });
	grammar.push_back({ mulop,mulop_div });
	grammar.push_back({ mulop,mulop_mod });
	grammar.push_back({ mulop,mulop_and });

	//for (int i = 0; i < grammar.size(); i++)
	//{
	//	cout << i << " " << mark_words[grammar[i][0]] << "->";
	//	for (int j = 1; j < grammar[i].size(); j++)
	//	{
	//		cout << mark_words[grammar[i][j]];
	//		cout << " ";
	//	}
	//	cout << endl;
	//}

	return grammar;
}

fset getFIRST(Grammar grammar)
{
	fset first;
	bool change=true;

	//���ķ����ķ��ս��������ĵ�һ��
	mark tem = Boundary;
	for (int i = 0; i < grammar.size(); i++)  
	{
		mark now = grammar[i][0];
		if (now != tem)   //δ����first����
		{
			first.insert({ now,set<mark>() });
			tem = now;
		}
	}

	while (change)
	{
		change = false;
		for (int i = 0; i < grammar.size(); i++)
		{
			mark now = grammar[i][1], left = grammar[i][0]; //left�����ս��
			int before, after;  //��¼insertǰ�󼯺ϳ��ȣ��ж��Ƿ��и���
			if (now > Boundary) //�Ƿ��ս��
			{
				set<mark> nowfirst = first[now]; //�÷��ս����first��
				if (nowfirst.find(null_) != nowfirst.end()) //first�����п�,���ǿշ��ż���first�����У��ٿ���һ�����ս��
				{
					bool flag = true;   //�Ƿ�鵽���һ�����ս��

					before = first[left].size();
					nowfirst.erase(null_);
					first[left].insert(nowfirst.begin(), nowfirst.end());
					after = first[left].size();
					if (before < after)   //first�������仯
					{
						change = true;
					}

					for (int j = 2; j < grammar[i].size(); j++)
					{
						if (grammar.size() == 2)
						{
							break;
						}
						mark nexts = grammar[i][j]; //��һ������
						if (nexts < Boundary) //���ս��
						{
							before = first[left].size();
							first[left].insert(nexts);
							after = first[left].size();
							if (before < after)   //first�������仯
							{
								change = true;
							}
							flag = false;
							break;
						}
						else
						{
							set<mark> nextfirst = first[nexts];
							if (nextfirst.find(null_) != nextfirst.end()) //�п�
							{
								before = first[left].size();
								nextfirst.erase(null_);
								first[left].insert(nextfirst.begin(), nextfirst.end());
								after = first[left].size();
								if (before < after)   //first�������仯
								{
									change = true;
								}
							}
							else
							{
								before = first[left].size();
								first[left].insert(nextfirst.begin(), nextfirst.end());
								after = first[left].size();
								if (before < after)   //first�������仯
								{
									change = true;
								}
								flag = false;
								break;
							}
						}
					}
					if (flag) //���ж��Ƿ��ս�������һ�����ս����first���п�
					{
						set<mark> f = first[grammar[i][grammar[i].size() - 1]];
						if (f.find(null_) != f.end())   //�п�
						{
							before = first[left].size();
							first[left].insert(null_);
							after = first[left].size();
							if (before < after)   //first�������仯
							{
								change = true;
							}
						}
					}
				}
				else    //first����û�п�,�����з��ż��뵽first����
				{
					if (!nowfirst.empty())
					{
						before = first[left].size();
						first[left].insert(nowfirst.begin(), nowfirst.end());
						after = first[left].size();
						if (before < after)   //first�������仯
						{
							change = true;
						}
					}
				}
			}
			else   //���ս����ֱ�Ӽ���first����
			{
				int before, after;   
				before = first[left].size();
				first[left].insert(now);
				after = first[left].size();
				if (before < after)   //first�������仯
				{
					change = true;
				}
			}
		}
	}
	
	//���,����debug
	//cout << "***************** FIRST�� *****************" << endl;
	//for (auto iter = first.begin(); iter != first.end(); ++iter)
	//{
	//	set<mark> value=iter->second;
	//	//vector<mark> value;
	//	cout << mark_words[iter->first] << " : ";
	//	for (auto it = value.begin(); it != value.end(); it++)
	//	{
	//		cout << mark_words[*it] << " ";
	//	}
	//	cout << endl;
	//}
	//cout << endl;

	return first;
}

fset getFOLLOW(Grammar grammar, fset first)
{
	fset follow;
	bool change = true;

	//���ķ����ķ��ս��������ĵ�һ��
	mark tem = Boundary;
	for (int i = 0; i < grammar.size(); i++)
	{
		mark now = grammar[i][0];
		if (now != tem)   //δ����first����
		{
			follow.insert({ now,set<mark>() });
			tem = now;
		}
	}
	follow[S].insert(EOF_);     //��$���뿪ʼ���ŵ�FOLLOW����

	while (change)
	{
		change = false;
		int before, after; //�����жϼ����Ƿ����仯
		for (int i = 0; i < grammar.size(); i++)
		{
			vector<mark> current_line = grammar[i];  //��ǰ��

			//if (i == 47)
			//{
			//	cout << " aaa" << endl;
			//}

			for (int j = 1; j < current_line.size(); j++)
			{
				mark now = current_line[j];      //��ǰ����
				mark last = current_line[j - 1]; //ǰһ������

				if (j == 1 && current_line.size() != 2)
				{
					continue;
				}
				if (j == current_line.size() - 1)  //����ǰ����Ϊ���һ��
				{
					if (now < Boundary)  //���ս��
					{
						if (last > Boundary && j != 1)
						{
							before = follow[last].size();
							follow[last].insert(now);
							after = follow[last].size();
							if (before != after)
								change = true;
						}
					}
					else
					{
						set<mark> now_first = first[now];
						if (now_first.find(null_) != now_first.end() && j != 1 && last > Boundary) //�п�,���󲿵�follow�ӵ�ǰһ����follow��
						{
							before = follow[last].size();
							follow[last].insert(follow[current_line[0]].begin(), follow[current_line[0]].end());
							after = follow[last].size();
							if (before != after)
								change = true;
						}
						now_first.erase(null_);

						//���󲿵�follow�������һ����follow��
						before = follow[now].size();
						follow[now].insert(follow[current_line[0]].begin(), follow[current_line[0]].end());
						after = follow[now].size();
						if (before != after)
							change = true;

						//��ǰfirst�ӵ�ǰһ��follow��
						if (last > Boundary && j != 1)
						{
							before = follow[last].size();
							follow[last].insert(now_first.begin(), now_first.end());
							after = follow[last].size();
							if (before != after)
								change = true;
						}
					}

					continue;
				}

				if (last < Boundary)     //���ǰһ���������ս����ֱ�ӽ�����һ��ѭ��
					continue;

				if (now < Boundary )       //��ǰ�������ս��
				{
					before = follow[last].size();
					follow[last].insert(now);
					after = follow[last].size();
					if (before != after)
						change = true;
				}
				else     //��ǰ�����Ƿ��ս��
				{
					for (int k = j; k < current_line.size(); k++)
					{
						mark now2 = current_line[k];
						mark last2 = current_line[k - 1];
						set<mark> now2_first = first[now2];
						if (now2 < Boundary) //���ս��
						{
							before = follow[last2].size();
							follow[last2].insert(now2);
							after = follow[last2].size();
							if (before != after)
								change = true;

							before = follow[last].size();
							follow[last].insert(now2);
							after = follow[last].size();
							if (before != after)
								change = true;
							break;
						}
						if (now2_first.find(null_) != now2_first.end()) //now2_first���п�
						{
							now2_first.erase(null_);
							before = follow[last2].size();
							follow[last2].insert(now2_first.begin(), now2_first.end());
							after = follow[last2].size();
							if (before != after)
								change = true;

							//now2��first����last��follow��
							before = follow[last].size();
							follow[last].insert(now2_first.begin(), now2_first.end());
							after = follow[last].size();
							if (before != after)
								change = true;

							if (k == current_line.size() - 1)  //����ǰ����Ϊ���һ��
							{
								//���󲿵�follow����now��follow��
								before = follow[now].size();
								follow[now].insert(follow[current_line[0]].begin(), follow[current_line[0]].end());
								after = follow[now].size();
								if (before != after)
									change = true;
							}
						}
						else   //now2_first���޿գ�firstֱ�Ӽӽ�ȥ
						{
							before = follow[last2].size();
							follow[last2].insert(now2_first.begin(),now2_first.end());
							after = follow[last2].size();
							if (before != after)
								change = true;
							
							before = follow[last].size();
							follow[last].insert(now2_first.begin(), now2_first.end());
							after = follow[last].size();
							if (before != after)
								change = true;

							break;
						}
					}
				}
			}
		}
	}

	//���,����debug
	//cout << "***************** FOLLOW�� *****************" << endl;
	//for (auto iter = follow.begin(); iter != follow.end(); ++iter)
	//{
	//	set<mark> value = iter->second;
	//	cout << mark_words[iter->first] << " : ";
	//	for (auto it = value.begin(); it != value.end(); it++)
	//	{
	//		cout << mark_words[*it] << " ";
	//	}
	//	cout << endl;
	//}
	//cout << endl;

	return follow;
}

LR_PredictTable getTable(Grammar grammar)    //����LR������
{
	vector<closure> DFA;  //�����Ŀ���淶��
	closure current_clo;  //��ŵ�ǰ����ıհ�
	vector<Go> go;         //���DFA����Ŀ��ָ��
	LR_PredictTable LRtable;

	//���ķ��Ŀ�ʼ������հ� Ȼ����չ
	current_clo.sentences.push_back(grammar[0]);
	current_clo.point_pos.push_back(1);  //�������Ҳ�������
	extend_closure(grammar, current_clo);
	DFA.push_back(current_clo);

	//cout << "***************** closure 0 *****************" << endl;
	//cout << "S -> ��programstruct" << endl;

	int count = 1;  //��ǰ�հ�����������������һ���ı��
	for (int I = 0; I < DFA.size(); I++)   //I ��ʾ��ǰ�հ���I0,I1,I2....
	{
		closure c = DFA[I];
		vector<bool> flag; //������ʾÿ������Ƿ�δ����
		int c_size = c.sentences.size();

		for (int i = 0; i < c_size; i++)
			flag.push_back(true);

		for (int i = 0; i < c_size; i++)
		{
			//������Ŀ����ÿ����䣬����������Ŀ
			sentence thisline = c.sentences[i];
			mark nextone;

			if (c.point_pos[i] == thisline.size())  //��á���ķ���
				nextone = null_;
			else
				nextone = thisline[c.point_pos[i]];

			if (flag[i] == true && nextone != null_) //�˾�δ���������ҡ��������
			{
				flag[i] = false;
				closure new_closure;

				//����Ƽ����µıհ���
				new_closure.sentences.push_back(thisline);
				new_closure.point_pos.push_back(c.point_pos[i] + 1);

				//���ҡ��������ͬ����䣬���ӵ���Ŀ��������չ�հ�
				for (int j = i + 1; j < c_size; j++)
				{
					sentence thisline_j = c.sentences[j]; 
					mark nextone_j;

					if (c.point_pos[j] == thisline_j.size())  //��á���ķ���
						nextone_j = null_;
					else
						nextone_j = thisline_j[c.point_pos[j]];

					if (flag[j] == true && nextone_j == nextone) 
					{
						flag[j] = false;
						//����Ƽ����µıհ���
						new_closure.sentences.push_back(thisline_j);
						new_closure.point_pos.push_back(c.point_pos[j] + 1);
					}
				}
				extend_closure(grammar, new_closure);

				//���new_closure�Ƿ��Ѵ���
				int isUnique = true;

				for (int j = 0; j < DFA.size(); j++)
				{
					closure exist_closure = DFA[j];

					if (exist_closure.point_pos.size() == new_closure.point_pos.size())
					{
						bool f = true;  //T���ҵ���ͬ��
						
						for (int k = 0; k < exist_closure.point_pos.size(); k++)
						{
							sentence es = exist_closure.sentences[k];
							int ep = exist_closure.point_pos[k];

							if (in_closure(new_closure, es, ep))  
							{
								continue;
							}
							f = false;
							break;
						} 
						if (f)   //�ҵ�����ȫ��ͬ�ıհ�
						{
							isUnique = false;
							Go tem;

							tem.from = I;
							tem.to = j;
							tem.by = nextone;
							go.push_back(tem);  //go(I,nextone)=j

							break;
						}
					}
				}

				//�����ظ�����뵱ǰDFA
				if (isUnique) {
					Go tem;

					tem.from = I;
					tem.to = count;
					tem.by = nextone;

					DFA.push_back(new_closure);
					go.push_back(tem);  //go(I,nextone)=j

					//���,����debug
					//cout << "***************** closure " << count << " *****************" << endl;
					//for (int k = 0; k < new_closure.sentences.size(); k++)
					//{
					//	for (int j = 0; j < new_closure.sentences[k].size(); j++)
					//	{
					//		if (j == 1)
					//			cout << "->";
					//		if (j == new_closure.point_pos[k])
					//			cout << "��";
					//		cout << mark_words[new_closure.sentences[k][j]] << " ";
					//	}
					//	if (new_closure.point_pos[k] == new_closure.sentences[k].size())
					//		cout << "��";
					//	cout << endl;
					//}
					//cout << endl;

					count++;
				}
			}
		}
	}

	//���ķ���FIRST��FOLLOW����Ϊ����һ�����������
	fset first = getFIRST(grammar);
	fset follow = getFOLLOW(grammar, first);
	

	/* SLR(1)�������Ĺ��죺

		����x    �ƽ�Sx
		����x    ��ԼRx
		 0       ����
		-233     ����	*/
	//��ʼ��
	vector<int> tem;
	for (int i = 0; i < factor + 1; i++)
		tem.push_back(0);
	for (int i = 0; i < DFA.size(); i++)
		LRtable.push_back(tem);



	//�����Լ��
	for (int i = 0; i < DFA.size(); i++)
	{
		closure c = DFA[i];
		for (int j = 0; j < c.point_pos.size(); j++)
		{
			if (c.point_pos[j] == c.sentences[j].size())   //�������ĩβ
			{
				sentence reduce = c.sentences[j];
				int k = 0;    //No:�ù�Լ�����grammar�е����

				for (; k < grammar.size(); k++)
				{
					if (reduce == grammar[k])
						break;
				}

				for (auto it = follow[reduce[0]].begin(); it != follow[reduce[0]].end(); it++)
				{
					LRtable[i][*it] = -k;
					if (k == 0)
					{
						LRtable[i][*it] = -233;
					}
				}
			}
		}

	}

	//�����ƽ���
	for (int i = 0; i < go.size(); i++)
	{
		LRtable[go[i].from][go[i].by] = go[i].to;
	}

	//��ӡԤ�������
	//cout << "************************* Ԥ������� *************************" << endl;
	//for (int i = 0; i < DFA.size(); i++) 
	//{
	//	for (int j = 0; j < (factor + 1); j++)
	//	{
	//		cout << LRtable[i][j] << " ";
	//	}
	//	cout << endl;
	//}

	return LRtable;
}

void extend_closure(Grammar grammar, closure& c)         //��ɱհ�����չ
{
	for (int i = 0; i < c.sentences.size(); i++) 
	{
		sentence thisline = c.sentences[i];
		mark nextone;
		if (c.point_pos[i] == thisline.size())
			nextone = null_;
		else
			nextone = thisline[c.point_pos[i]];

		if (nextone > Boundary)   //����Ƿ��ս��������Ҫ��չ 
		{
			int k = 0;
			for (; k < grammar.size(); k++)   //�ҵ���nextone��ͷ�����
			{
				if (grammar[k][0] == nextone)
					break;
			}
			
			for (int j=k; j < grammar.size() && grammar[j][0] == nextone; j++)
			{
				sentence line = grammar[j];
				int point_pos;

				if (line[1] == null_) 
				{
					point_pos = 2;  //������null_����
				}
				else 
				{
					point_pos = 1;  //�������Ҳ�������
				}

				if (!in_closure(c, line, point_pos))
				{
					c.sentences.push_back(line);
					c.point_pos.push_back(point_pos);
				}
			}
		}
	}

	return;
}

bool in_closure(closure c, sentence s, int pos) //�ж�һ���հ����Ƿ���������
{
	for (int i = 0; i < c.sentences.size(); i++)
	{
		sentence now=c.sentences[i];
		int now_pos = c.point_pos[i];
		int j=0;

		if (now_pos != pos || now.size() != s.size())
			continue;
		for (; j < now.size(); j++)
		{
			if (now[j] != s[j])
				break;
		}
		if (j == now.size())
			return true;
	}
	return false;
}

vector<int> control_program(LR_PredictTable LRtable, Grammar grammar, vector<token>& words) //���з�������
{
	ifstream token_file;
	string buffer;
	vector<token> tokens; //�ʷ����������ļǺ�������

	/*----------------------Ҫ�ĳɴʷ����������ļ���λ��---------------------*/
	ifstream infile;
	infile.open("lexOut.txt");
	
	token tem;

	char buf[100];
	while (infile.getline(buf, sizeof(buf)) && buf[0]) //buf[0]Ϊ�������һ������
	{
		int p = 0;
		string t = "";
		int n = 0;  //ʶ�𵽼����ո�

		while (buf[p] != '\0')
		{
			if (buf[p] == ' ' && n < 2)
			{
				if (n == 0)
				{
					tem.mark = stoi(t);
				}
				else if (n == 1)
				{
					tem.line = stoi(t);
				}
				t = "";
				n++;
			}
			else
			{
				t += buf[p];
			}
			p++;
		}
		tem.content = t;
		tokens.push_back(tem);
	}
	tem.mark = 0;
	tem.content = "$";
	tokens.push_back(tem);

	vector<int> reduces;  //����ʽ����
	vector<int> state;    //״̬ջ
	vector<int> symbol;   //����ջ
	vector<int> token_num;//��ջ���ս����Ӧ��token���
	int action;           //��ǰ����Ķ���
	bool no_error = true; //ǰ���Ƿ���ִ���

	//��ʼ��ջ
	state.push_back(0);
	symbol.push_back(EOF_);

	for (int i = 0; i < tokens.size(); i++)
	{
		int now_mark = tokens[i].mark;
		int now_line = tokens[i].line;
		string now_content = tokens[i].content;

		if (now_line == 6)
			int aaa=0;

		if (now_mark == -1)  //�ʷ�����ʶ�𵽵Ĵ���
		{
			cout << "Lexical error:\t";
			cout << "<line " << now_line << ">\t";
			cout << "\"" << now_content << "\"\t";
			cout << "This is an illegal mark." << endl;
			continue;
		}

		//����ջ�����ݣ��Ҷ�Ӧ����Ķ���
		action = LRtable[state.back()][now_mark];
		
		if (action > 0)   //�ƽ�shift
		{
			state.push_back(action);
			symbol.push_back(now_mark);
			if (now_mark < Boundary)
			{
				token_num.push_back(i);
			}
		}
		else if (action < 0) //��Լreduce
		{
			if (action == -233) //ACC���ܶ���
			{
				if (no_error)
				{
					cout << "Syntax analysis Success��" << endl;
				}
				else
				{
					cout << "Syntax analysis Failed! " << endl;
				}
				reduces.push_back(0);
				break;
			}

			sentence reduce = grammar[-action]; //��Լ�����

			reduces.push_back(-action);

			vector<int> tem; //��ų�ջ�ķ��ս��token���
			int non_terminal_num=0;

			//����ջ����len������
			int len = reduce.size() - 1;
			if (reduce[1] == null_)
				len = 0;

			while (len--)
			{
				if (symbol.back() < Boundary)
				{
					tem.push_back(token_num.back());
					token_num.pop_back();
					non_terminal_num++;
				}
				state.pop_back();
				symbol.pop_back();
			}
			//�����ս���������ջ�෴��˳�����words��
			if (reduce[1] != null_)
			{
				for (int j = tem.size()-1; non_terminal_num && j >= 0; j--, non_terminal_num--)
				{
					words.push_back(tokens[tem[j]]);
				}
			}

			state.push_back(LRtable[state.back()][reduce[0]]);
			symbol.push_back(reduce[0]);
			i--; //����ֱ�Ӷ�ȡ��һ������
		}
		else  //����
		{
			no_error = false; //��������

			if (now_mark == EOF_)
			{
				cout << "Syntax analysis Failed! " << endl;
				break;
			}
			
			cout << "Syntax error:\t";
			cout << "<line " << now_line << ">\t";
			cout << "\"" << now_content << "\"\t";
			if (now_mark == id)
				cout << "This id is redundant or a symbol is missing nearby." << endl;
			else if (now_mark == mulop_div || now_mark == mulop_mod || now_mark == punc_semicolon || now_mark == assignop ||
				now_mark == mulop_mul || now_mark == mulop_divide || now_mark == addop_add || now_mark == addop_sub)
				cout << "This symbol is redundant or an operand is missing nearby." << endl;
			else if (now_mark == punc_square_left || now_mark == punc_square_right || now_mark == punc_round_left || now_mark == punc_round_right)
				cout << "Parenthesis mismatch." << endl;
			else
				cout << "This mark is redundant" << endl;
		}

	}

	return reduces;
}