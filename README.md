# 课程设计报告

## 1.课程设计目标和要求

## 2.需求分析

### 1)数据流图

### 2)功能

#### c)语义分析

在语法分析的基础上，根据其产生式序列和以归约顺序重排的词法序列建立和维护符号表，检查是否存在重复定义，类型不一致等错误，并为代码生成提供对于符号表信息的查询接口。

支持的基本变量类型有:

- $integer$
- $real$
- $boolen$
- $char$

## 3.总体设计

### 1) 数据结构设计


#### a)符号表IdTable

设定符号表为多层次结构；


建立主符号表，用于存储全局的标识符；

以及子符号表用于存储局部的标识符；

- $father$指向上层符号表，主符号表指向空，子符号表指向主符号表

- $nameToId$使用了C++的STL库中的$unordered\_map$,为将一个字符串映射至一个标识符的存储方式。

- $now\_id\_table$指向当前块所在的符号表；

```c++
struct Id_Table
{
	Id_Table *father;		
	map<string, Id> name_to_id;
} * now_id_table;
```

- 标识符


``` c++
struct Id
{
	string name;
	Data_Type data_type;
	Id_Type id_type;
	vector<Param> param_list; 
	Data_Type return_value_data_type; 
	bool is_error;					  
} error_id, temp1;
```

 

|    属性    |    含义    |
| :--------: | :--------: |
|    name    | 标识符名字 |
| data_type  |  数据类型  |
|  id_type   | 标识符类型 |
| param_list |  参数列表  |
|  retDatat  | 返回值类型 |
|  is_error  | 是否有错误 |


### 标识符类型(ID)


- 常量($constant$)
- 变量($variable$)
- 过程($procedure$)
- 函数

### 支持操作

- 检索标识符

  对于给定的标识符进行查找，如果当前表不存在，则查找上一层符号表,直至主符号表。

- 插入标识符

  在符号表中新建对应的项。

- 定位

  建立新的子符号表，将指针指向当前符号表

- 重定位

  删除当前符号表，根据指针找到上层符号表。

### 错误处理

对于识别到的错误，程序会在错误信息表中记录错误的类型以及所在行的行号。

- 作用域检查

  查找标识符如果失败，则为标识符未定义错误。

- 赋值语句

  除$integer \to real$外，两边类型若不相同，则为赋值错误。

- 函数，过程调用

  调用时参数类型不一致错误。

- 运算符使用

  该运算符无法以对给定类型执行运算，如$x\ mod\ real$.

## 代码生成

使用S属性定义的语法制导翻译技术进行自顶向下的代码生成

对于每个文法符号定义一个string类型的属性值，用于记录代码。

### 类型对应

- $integer \to short$

- $real \to float$

- $boolean \to bool$

- $char \to char$ 

  ### 关键字对应

- $program$ 无实际对应转换

- $function$ 对应函数

- $procedure$ 对应返回值为void的函数









