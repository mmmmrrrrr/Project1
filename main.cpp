#include "grammar_analysis.h"
#include "semantic_analysis.h"
#include "code_generate.h"
#include "FlexLexer.h"
int main()
{
	freopen("a.out","w",stderr);
	ifstream ifile("test3.pas");
	ofstream ofile("lexOut.txt");
	yyFlexLexer yy;
	yy.switch_streams(ifile, ofile);
	printf("[I]>>>Read data from file:test.pas\n");
	yy.yylex();
	ifile.close();
	ofile.close();
	yy.yylex();
	ifstream productIn;
	productIn.open("D:\\coding\\Project1\\production_sequence.pas");
	map<int, string> numToProduct;
	numToProduct.clear();
	for (int i = 1; i <= 90; ++i)
	{
		int x;
		string y;
		productIn >> x;
		getline(productIn, y, '\n');
		numToProduct[x] = y;
	}
	vector<token> token_seq;
	token_seq.clear();

	Grammar grammar = initGrammer();

	LR_PredictTable LRtable = getTable(grammar);

	vector<int> product_seq(control_program(LRtable, grammar, token_seq));

	initIdTable();


	cout<<semantic_analysis(product_seq, token_seq, numToProduct)<<endl;
	cout << "Success!!" << endl;
	return 0;
}