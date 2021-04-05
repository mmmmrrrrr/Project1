#include "semantic_analysis.h"
#include "grammar_analysis.h"
Identifier search_identifier(string name){
    for (Id_Table *t=now_ID_table; t != NULL;t=t->father)
    {
        if(t->name_to_id.find(name)!=t->name_to_id.end())
            return t->name_to_id[name];
    }
    return error_id;
}//根据名字进行查找，返回标识符信息
bool insert_identifier(Identifier id){
    if(now_ID_table->name_to_id.find(id.name)!=now_ID_table->name_to_id.end())
        return 0;
    now_ID_table->name_to_id[id.name] = id;
    return 1;
}//返回1表示正确插入，返回0表示已经有重复的标识符
void location(){
    auto t = now_ID_table;
    now_ID_table = new Id_Table;
    now_ID_table->father = t;
}//定位
void relocation(){
    auto t = now_ID_table->father;
    delete now_ID_table;
    now_ID_table = t;
}//重定位
void init_ID_Table(){
    error_id.is_error = 1;
    location();
}
int main()
{	
    vector<token>token_sequence=getToken();

    Grammar grammar=initGrammer();

	LR_PredictTable LRtable = getTable(grammar);

	vector<int>product_sequence(control_program(LRtable,grammar,token_sequence));

    init_ID_Table();

    for(auto i:product_sequence)
        cout<<i<<endl;
    
    
    return 0;
}
