#include "grammar_analysis.h"
#include <fstream>
#include <string>
#pragma execution_character_set("utf-8")

string mark_words[] //存放和枚举值对应的记号，方便输出
	{
		/*终结符*/
		"EOF",	//文本结束
		"null", //空
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
		"fnum",	  //整数
		"digits", //浮点数
		//letter,
		"assignop",
		"punc_question",	 //?
		"punc_not",			 //~
		"punc_comma",		 //,
		"punc_semicolon",	 //;
		"punc_colon",		 //:
		"punc_point",		 //.
		"punc_round_left",	 //(
		"punc_round_right",	 //)
		"punc_square_left",	 //[
		"punc_square_right", //]
		"mulop_and",
		"mulop_div", //整除
		"mulop_mod",
		"mulop_mul",
		"mulop_divide", //浮点除法
		"addop_or",
		"addop_add",
		"addop_sub",
		"relop_e",		//=
		"relop_ne",		//<>
		"relop_l",		//<
		"relop_le",		//<=
		"relop_g",		//>
		"relop_ge",		//>=
		"single_quote", //'
		"letter",		//字符常量
		"Boundary",		//---终结符与非终结符的分界线---
		/*非终结符*/
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
		//"·"
		////NOW //DFA中的点 .
	};

Grammar initGrammer()
{
	Grammar grammar;

	grammar.push_back({S, programstruct});
	grammar.push_back({programstruct, program_head, punc_semicolon, program_body, punc_point});
	grammar.push_back({program_head, program_, id, punc_round_left, idlist, punc_round_right});
	grammar.push_back({program_head, program_, id});
	grammar.push_back({program_body, const_declarations, var_declarations, subprogram_declarations, compound_statement});
	grammar.push_back({idlist, idlist, punc_comma, id});
	grammar.push_back({idlist, id});
	grammar.push_back({const_declarations, const_, const_declaration, punc_semicolon});
	grammar.push_back({const_declarations, null_});
	grammar.push_back({const_declaration, const_declaration, punc_semicolon, id, relop_e, const_value});
	grammar.push_back({const_declaration, id, relop_e, const_value});
	grammar.push_back({const_value, addop_add, num});
	grammar.push_back({const_value, addop_sub, num});
	grammar.push_back({const_value, num});
	grammar.push_back({const_value, letter});
	grammar.push_back({var_declarations, var_, var_declaration, punc_semicolon});
	grammar.push_back({var_declarations, null_});
	grammar.push_back({var_declaration, var_declaration, punc_semicolon, idlist, punc_colon, type});
	grammar.push_back({var_declaration, idlist, punc_colon, type});
	grammar.push_back({type, basic_type});
	grammar.push_back({type, array_, punc_square_left, period, punc_square_right, of_, basic_type});
	grammar.push_back({basic_type, integer_});
	grammar.push_back({basic_type, real_});
	grammar.push_back({basic_type, boolean_});
	grammar.push_back({basic_type, char_});
	grammar.push_back({period, period, punc_comma, digits, punc_point, punc_point, digits});
	grammar.push_back({period, digits, punc_point, punc_point, digits});
	grammar.push_back({subprogram_declarations, subprogram_declarations, subprogram, punc_semicolon});
	grammar.push_back({subprogram_declarations, null_});
	grammar.push_back({subprogram, subprogram_head, punc_semicolon, subprogram_body});
	grammar.push_back({subprogram_head, procedure_, id, formal_parameter});
	grammar.push_back({subprogram_head, function_, id, formal_parameter, punc_colon, basic_type});
	grammar.push_back({formal_parameter, punc_round_left, parameter_list, punc_round_right});
	grammar.push_back({formal_parameter, null_});
	grammar.push_back({parameter_list, parameter_list, punc_semicolon, parameter});
	grammar.push_back({parameter_list, parameter});
	grammar.push_back({parameter, var_parameter});
	grammar.push_back({parameter, value_parameter});
	grammar.push_back({var_parameter, var_, value_parameter});
	grammar.push_back({value_parameter, idlist, punc_colon, basic_type});
	grammar.push_back({subprogram_body, const_declarations, var_declarations, compound_statement});
	grammar.push_back({compound_statement, begin_, statement_list, end_});
	grammar.push_back({statement_list, statement_list, punc_semicolon, statement});
	grammar.push_back({statement_list, statement});
	grammar.push_back({statement, variable, assignop, expression});
	grammar.push_back({statement, procedure_call});
	grammar.push_back({statement, compound_statement});
	grammar.push_back({statement, if_, expression, then_, statement, else_part});
	grammar.push_back({statement, for_, id, assignop, expression, to_, expression, do_, statement});
	grammar.push_back({statement, read_, punc_round_left, variable_list, punc_round_right});
	grammar.push_back({statement, write_, punc_round_left, expression_list, punc_round_right});
	grammar.push_back({statement, null_});
	grammar.push_back({variable_list, variable_list, punc_comma, variable});
	grammar.push_back({variable_list, variable});
	grammar.push_back({variable, id, id_varpart});
	grammar.push_back({id_varpart, punc_square_left, expression_list, punc_square_right});
	grammar.push_back({id_varpart, null_});
	grammar.push_back({procedure_call, id});
	grammar.push_back({procedure_call, id, punc_round_left, expression_list, punc_round_right});
	grammar.push_back({else_part, else_, statement});
	grammar.push_back({else_part, null_});
	grammar.push_back({expression_list, expression_list, punc_comma, expression});
	grammar.push_back({expression_list, expression});
	grammar.push_back({expression, simple_expression, relop, simple_expression});
	grammar.push_back({expression, simple_expression});
	grammar.push_back({simple_expression, simple_expression, addop, term});
	grammar.push_back({simple_expression, term});
	grammar.push_back({term, term, mulop, factor});
	grammar.push_back({term, factor});
	grammar.push_back({factor, num});
	grammar.push_back({factor, variable});
	grammar.push_back({factor, id, punc_round_left, expression_list, punc_round_right});
	grammar.push_back({factor, punc_round_left, expression, punc_round_right});
	grammar.push_back({factor, not_, factor});
	grammar.push_back({factor, addop_sub, factor});
	grammar.push_back({relop, relop_e});
	grammar.push_back({relop, relop_ne});
	grammar.push_back({relop, relop_l});
	grammar.push_back({relop, relop_le});
	grammar.push_back({relop, relop_g});
	grammar.push_back({relop, relop_ge});
	grammar.push_back({addop, addop_add});
	grammar.push_back({addop, addop_sub});
	grammar.push_back({addop, addop_or});
	grammar.push_back({num, fnum});
	grammar.push_back({num, digits});
	grammar.push_back({mulop, mulop_mul});
	grammar.push_back({mulop, mulop_divide});
	grammar.push_back({mulop, mulop_div});
	grammar.push_back({mulop, mulop_mod});
	grammar.push_back({mulop, mulop_and});

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
	bool change = true;

	//把文法左侧的非终结符加入表的第一列
	mark tem = Boundary;
	for (int i = 0; i < grammar.size(); i++)
	{
		mark now = grammar[i][0];
		if (now != tem) //未加入first集中
		{
			first.insert({now, set<mark>()});
			tem = now;
		}
	}

	while (change)
	{
		change = false;
		for (int i = 0; i < grammar.size(); i++)
		{
			mark now = grammar[i][1], left = grammar[i][0]; //left左侧非终结符
			int before, after;								//记录insert前后集合长度，判断是否有更新
			if (now > Boundary)								//是非终结符
			{
				set<mark> nowfirst = first[now];			//该非终结符的first集
				if (nowfirst.find(null_) != nowfirst.end()) //first集中有空,将非空符号加入first集合中，再看下一个非终结符
				{
					bool flag = true; //是否查到最后一个非终结符

					before = first[left].size();
					nowfirst.erase(null_);
					first[left].insert(nowfirst.begin(), nowfirst.end());
					after = first[left].size();
					if (before < after) //first集发生变化
					{
						change = true;
					}

					for (int j = 2; j < grammar[i].size(); j++)
					{
						if (grammar.size() == 2)
						{
							break;
						}
						mark nexts = grammar[i][j]; //下一个符号
						if (nexts < Boundary)		//是终结符
						{
							before = first[left].size();
							first[left].insert(nexts);
							after = first[left].size();
							if (before < after) //first集发生变化
							{
								change = true;
							}
							flag = false;
							break;
						}
						else
						{
							set<mark> nextfirst = first[nexts];
							if (nextfirst.find(null_) != nextfirst.end()) //有空
							{
								before = first[left].size();
								nextfirst.erase(null_);
								first[left].insert(nextfirst.begin(), nextfirst.end());
								after = first[left].size();
								if (before < after) //first集发生变化
								{
									change = true;
								}
							}
							else
							{
								before = first[left].size();
								first[left].insert(nextfirst.begin(), nextfirst.end());
								after = first[left].size();
								if (before < after) //first集发生变化
								{
									change = true;
								}
								flag = false;
								break;
							}
						}
					}
					if (flag) //所有都是非终结符且最后一个非终结符的first集有空
					{
						set<mark> f = first[grammar[i][grammar[i].size() - 1]];
						if (f.find(null_) != f.end()) //有空
						{
							before = first[left].size();
							first[left].insert(null_);
							after = first[left].size();
							if (before < after) //first集发生变化
							{
								change = true;
							}
						}
					}
				}
				else //first集中没有空,将所有符号加入到first集中
				{
					if (!nowfirst.empty())
					{
						before = first[left].size();
						first[left].insert(nowfirst.begin(), nowfirst.end());
						after = first[left].size();
						if (before < after) //first集发生变化
						{
							change = true;
						}
					}
				}
			}
			else //是终结符，直接加入first集中
			{
				int before, after;
				before = first[left].size();
				first[left].insert(now);
				after = first[left].size();
				if (before < after) //first集发生变化
				{
					change = true;
				}
			}
		}
	}

	//输出,方便debug
	//cout << "***************** FIRST集 *****************" << endl;
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

	//把文法左侧的非终结符加入表的第一列
	mark tem = Boundary;
	for (int i = 0; i < grammar.size(); i++)
	{
		mark now = grammar[i][0];
		if (now != tem) //未加入first集中
		{
			follow.insert({now, set<mark>()});
			tem = now;
		}
	}
	follow[S].insert(EOF_); //把$加入开始符号的FOLLOW集里

	while (change)
	{
		change = false;
		int before, after; //用来判断集合是否发生变化
		for (int i = 0; i < grammar.size(); i++)
		{
			vector<mark> current_line = grammar[i]; //当前行

			//if (i == 47)
			//{
			//	cout << " aaa" << endl;
			//}

			for (int j = 1; j < current_line.size(); j++)
			{
				mark now = current_line[j];		 //当前符号
				mark last = current_line[j - 1]; //前一个符号

				if (j == 1 && current_line.size() != 2)
				{
					continue;
				}
				if (j == current_line.size() - 1) //若当前符号为最后一个
				{
					if (now < Boundary) //是终结符
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
						if (now_first.find(null_) != now_first.end() && j != 1 && last > Boundary) //有空,把左部的follow加到前一个的follow中
						{
							before = follow[last].size();
							follow[last].insert(follow[current_line[0]].begin(), follow[current_line[0]].end());
							after = follow[last].size();
							if (before != after)
								change = true;
						}
						now_first.erase(null_);

						//把左部的follow加入最后一个的follow中
						before = follow[now].size();
						follow[now].insert(follow[current_line[0]].begin(), follow[current_line[0]].end());
						after = follow[now].size();
						if (before != after)
							change = true;

						//当前first加到前一个follow中
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

				if (last < Boundary) //如果前一个符号是终结符，直接进入下一次循环
					continue;

				if (now < Boundary) //当前符号是终结符
				{
					before = follow[last].size();
					follow[last].insert(now);
					after = follow[last].size();
					if (before != after)
						change = true;
				}
				else //当前符号是非终结符
				{
					for (int k = j; k < current_line.size(); k++)
					{
						mark now2 = current_line[k];
						mark last2 = current_line[k - 1];
						set<mark> now2_first = first[now2];
						if (now2 < Boundary) //是终结符
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
						if (now2_first.find(null_) != now2_first.end()) //now2_first中有空
						{
							now2_first.erase(null_);
							before = follow[last2].size();
							follow[last2].insert(now2_first.begin(), now2_first.end());
							after = follow[last2].size();
							if (before != after)
								change = true;

							//now2的first加入last的follow中
							before = follow[last].size();
							follow[last].insert(now2_first.begin(), now2_first.end());
							after = follow[last].size();
							if (before != after)
								change = true;

							if (k == current_line.size() - 1) //若当前符号为最后一个
							{
								//把左部的follow加入now的follow中
								before = follow[now].size();
								follow[now].insert(follow[current_line[0]].begin(), follow[current_line[0]].end());
								after = follow[now].size();
								if (before != after)
									change = true;
							}
						}
						else //now2_first中无空，first直接加进去
						{
							before = follow[last2].size();
							follow[last2].insert(now2_first.begin(), now2_first.end());
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

	//输出,方便debug
	//cout << "***************** FOLLOW集 *****************" << endl;
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

LR_PredictTable getTable(Grammar grammar) //计算LR分析表
{
	vector<closure> DFA; //存放项目集规范组
	closure current_clo; //存放当前计算的闭包
	vector<Go> go;		 //存放DFA各项目的指向
	LR_PredictTable LRtable;

	//将文法的开始语句加入闭包 然后扩展
	current_clo.sentences.push_back(grammar[0]);
	current_clo.point_pos.push_back(1); //·放在右侧的最左边
	extend_closure(grammar, current_clo);
	DFA.push_back(current_clo);

	//cout << "***************** closure 0 *****************" << endl;
	//cout << "S -> ·programstruct" << endl;

	int count = 1;						 //当前闭包个数，用来决定下一个的编号
	for (int I = 0; I < DFA.size(); I++) //I 表示当前闭包是I0,I1,I2....
	{
		closure c = DFA[I];
		vector<bool> flag; //用来表示每个语句是否未处理
		int c_size = c.sentences.size();

		for (int i = 0; i < c_size; i++)
			flag.push_back(true);

		for (int i = 0; i < c_size; i++)
		{
			//遍历项目集的每条语句，并构造新项目
			sentence thisline = c.sentences[i];
			mark nextone;

			if (c.point_pos[i] == thisline.size()) //获得·后的符号
				nextone = null_;
			else
				nextone = thisline[c.point_pos[i]];

			if (flag[i] == true && nextone != null_) //此句未处理过，且·不在最后
			{
				flag[i] = false;
				closure new_closure;

				//点后移加入新的闭包中
				new_closure.sentences.push_back(thisline);
				new_closure.point_pos.push_back(c.point_pos[i] + 1);

				//查找·后符号相同的语句，添加到项目集，并扩展闭包
				for (int j = i + 1; j < c_size; j++)
				{
					sentence thisline_j = c.sentences[j];
					mark nextone_j;

					if (c.point_pos[j] == thisline_j.size()) //获得·后的符号
						nextone_j = null_;
					else
						nextone_j = thisline_j[c.point_pos[j]];

					if (flag[j] == true && nextone_j == nextone)
					{
						flag[j] = false;
						//点后移加入新的闭包中
						new_closure.sentences.push_back(thisline_j);
						new_closure.point_pos.push_back(c.point_pos[j] + 1);
					}
				}
				extend_closure(grammar, new_closure);

				//检查new_closure是否已存在
				int isUnique = true;

				for (int j = 0; j < DFA.size(); j++)
				{
					closure exist_closure = DFA[j];

					if (exist_closure.point_pos.size() == new_closure.point_pos.size())
					{
						bool f = true; //T能找到相同的

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
						if (f) //找到了完全相同的闭包
						{
							isUnique = false;
							Go tem;

							tem.from = I;
							tem.to = j;
							tem.by = nextone;
							go.push_back(tem); //go(I,nextone)=j

							break;
						}
					}
				}

				//若无重复则加入当前DFA
				if (isUnique)
				{
					Go tem;

					tem.from = I;
					tem.to = count;
					tem.by = nextone;

					DFA.push_back(new_closure);
					go.push_back(tem); //go(I,nextone)=j

					//输出,方便debug
					//cout << "***************** closure " << count << " *****************" << endl;
					//for (int k = 0; k < new_closure.sentences.size(); k++)
					//{
					//	for (int j = 0; j < new_closure.sentences[k].size(); j++)
					//	{
					//		if (j == 1)
					//			cout << "->";
					//		if (j == new_closure.point_pos[k])
					//			cout << "·";
					//		cout << mark_words[new_closure.sentences[k][j]] << " ";
					//	}
					//	if (new_closure.point_pos[k] == new_closure.sentences[k].size())
					//		cout << "·";
					//	cout << endl;
					//}
					//cout << endl;

					count++;
				}
			}
		}
	}

	//求文法的FIRST和FOLLOW集，为了下一步构造分析表
	fset first = getFIRST(grammar);
	fset follow = getFOLLOW(grammar, first);

	/* SLR(1)分析表的构造：

		正数x    移进Sx
		负数x    规约Rx
		 0       错误
		-233     接受	*/
	//初始化
	vector<int> tem;
	for (int i = 0; i < factor + 1; i++)
		tem.push_back(0);
	for (int i = 0; i < DFA.size(); i++)
		LRtable.push_back(tem);

	//填入规约项
	for (int i = 0; i < DFA.size(); i++)
	{
		closure c = DFA[i];
		for (int j = 0; j < c.point_pos.size(); j++)
		{
			if (c.point_pos[j] == c.sentences[j].size()) //点在语句末尾
			{
				sentence reduce = c.sentences[j];
				int k = 0; //No:该规约语句在grammar中的序号

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

	//填入移进项
	for (int i = 0; i < go.size(); i++)
	{
		LRtable[go[i].from][go[i].by] = go[i].to;
	}

	//打印预测分析表
	//cout << "************************* 预测分析表 *************************" << endl;
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

void extend_closure(Grammar grammar, closure &c) //完成闭包的扩展
{
	for (int i = 0; i < c.sentences.size(); i++)
	{
		sentence thisline = c.sentences[i];
		mark nextone;
		if (c.point_pos[i] == thisline.size())
			nextone = null_;
		else
			nextone = thisline[c.point_pos[i]];

		if (nextone > Boundary) //如果是非终结符，则需要扩展
		{
			int k = 0;
			for (; k < grammar.size(); k++) //找到以nextone开头的语句
			{
				if (grammar[k][0] == nextone)
					break;
			}

			for (int j = k; j < grammar.size() && grammar[j][0] == nextone; j++)
			{
				sentence line = grammar[j];
				int point_pos;

				if (line[1] == null_)
				{
					point_pos = 2; //·放在null_后面
				}
				else
				{
					point_pos = 1; //·放在右侧的最左边
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

bool in_closure(closure c, sentence s, int pos) //判断一个闭包里是否包含该语句
{
	for (int i = 0; i < c.sentences.size(); i++)
	{
		sentence now = c.sentences[i];
		int now_pos = c.point_pos[i];
		int j = 0;

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

vector<int> control_program(LR_PredictTable LRtable, Grammar grammar, vector<token> &words) //进行分析动作
{
	ifstream token_file;
	string buffer;
	vector<token> tokens; //词法分析产生的记号流序列

	/*----------------------要改成词法分析生成文件的位置---------------------*/
	ifstream infile;
	infile.open("lexOut.txt");

	token tem;

	char buf[100];
	while (infile.getline(buf, sizeof(buf)) && buf[0]) //buf[0]为空是最后一个空行
	{
		int p = 0;
		string t = "";
		int n = 0; //识别到几个空格

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

	vector<int> reduces;   //产生式序列
	vector<int> state;	   //状态栈
	vector<int> symbol;	   //符号栈
	vector<int> token_num; //入栈的终结符对应的token序号
	int action;			   //当前表项的动作
	bool no_error = true;  //前面是否出现错误

	//初始化栈
	state.push_back(0);
	symbol.push_back(EOF_);

	for (int i = 0; i < tokens.size(); i++)
	{
		int now_mark = tokens[i].mark;
		int now_line = tokens[i].line;
		string now_content = tokens[i].content;

		if (now_line == 6)
			int aaa = 0;

		if (now_mark == -1) //词法分析识别到的错误
		{
			cout << "Lexical error:\t";
			cout << "<line " << now_line << ">\t";
			cout << "\"" << now_content << "\"\t";
			cout << "This is an illegal mark." << endl;
			continue;
		}

		//根据栈顶内容，找对应表项的动作
		action = LRtable[state.back()][now_mark];

		if (action > 0) //移进shift
		{
			state.push_back(action);
			symbol.push_back(now_mark);
			if (now_mark < Boundary)
			{
				token_num.push_back(i);
			}
		}
		else if (action < 0) //规约reduce
		{
			if (action == -233) //ACC接受动作
			{
				if (no_error)
				{
					cout << "Syntax analysis Success！" << endl;
				}
				else
				{
					cout << "Syntax analysis Failed! " << endl;
				}
				reduces.push_back(0);
				break;
			}

			sentence reduce = grammar[-action]; //规约的语句

			reduces.push_back(-action);

			vector<int> tem; //存放出栈的非终结符token序号
			int non_terminal_num = 0;

			//弹出栈顶的len个符号
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
			//将非终结符按照与出栈相反的顺序加入words中
			if (reduce[1] != null_)
			{
				for (int j = tem.size() - 1; non_terminal_num && j >= 0; j--, non_terminal_num--)
				{
					words.push_back(tokens[tem[j]]);
				}
			}

			state.push_back(LRtable[state.back()][reduce[0]]);
			symbol.push_back(reduce[0]);
			i--; //否则直接读取下一个符号
		}
		else //出错
		{
			no_error = false; //产生错误

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
			exit(1);
		}
	}

	return reduces;
}