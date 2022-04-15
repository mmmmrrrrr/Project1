#include "grammar_analysis.h"
#include "semantic_analysis.h"
#include "code_generate.h"
#include "FlexLexer.h"
#include <windows.h>
#include <fstream>
#pragma execution_character_set("utf-8")
int main(int argc, char **argv)
{
	int runFlag = 0;
	string readFileName{"./runFile/test.pas"};
	for (int i = 1; i < argc; ++i)
	{
		// printf("%s", argv[i]);
		if (!strcmp(argv[i], "-r"))
			runFlag = 1;
		if (!strcmp(argv[i], "-w"))
		{
			if (i + 1 >= argc || argv[i + 1][0] == '-')
			{
				cerr << "Don't receive a file name!" << endl;
				return 1;
			}
			readFileName = argv[i + 1];
		}
	}
	freopen("./runFile/test.cpp", "w", stdout);
	freopen("./runFile/message.txt", "w", stderr);
	ifstream ifile(readFileName);
	ofstream ofile("./runFile/lexOut.txt");
	yyFlexLexer yy;
	yy.switch_streams(ifile, ofile);
	cerr << "[I]>>>Read data from file: " << readFileName << endl;
	yy.yylex();
	ifile.close();
	ofile.close();
	yy.yylex();
	ifstream productIn;
	// productIn.open(".\\production_sequence.pas");
	map<int, string> numToProduct;
	numToProduct.clear();
	/*
	for (int i = 1; i <= 90; ++i)
	{
		int x;
		string y;
		productIn >> x;
		getline(productIn, y, '\n');
		numToProduct[x] = y;
	}
	*/

	vector<token> token_seq;
	token_seq.clear();

	Grammar grammar = initGrammer();

	LR_PredictTable LRtable = getTable(grammar);

	vector<int> product_seq(control_program(LRtable, grammar, token_seq));

	initIdTable();

	/*
	if (debugFlag)
	{
		for (auto i : product_seq)
			cout << i << endl;
		for (auto i : token_seq)
			cout << i.content << endl;
	}
	*/
	semantic_analysis(product_seq, token_seq, numToProduct);
	cout << generate_Code(product_seq, token_seq, token_seq.size() - 1) << endl;
	cerr << "Semantic analysis Success!!" << endl;
	fclose(stdout);
	if (!system("g++ -o ./runFile/test ./runFile/test.cpp"))
	{
		cerr << "Code compiled successfully!" << endl;
		if (runFlag)
		{
			system("./runFile/test<./runFile/test.in>./runFile/test.out");
			cerr << " ---- test.in ---- " << endl;
			ifstream fin, fout;
			fin.open("./runFile/test.in");
			fout.open("./runFile/test.out");
			string str;
			while (getline(fin, str, '\n'))
			{
				cerr << str << endl;
			}
			cerr << " ---- test.out ---- " << endl;
			while (getline(fout, str, '\n'))
			{
				cerr << str << endl;
			}
		}
	}
	else
		cerr << "Code compilation failed" << endl;
	return 0;
}