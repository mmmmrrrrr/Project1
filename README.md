# 课程设计报告

## 1.课程设计目标和要求

## 2.需求分析

### 2.1数据流图

![image-20210507102735560](C:\Users\17231\AppData\Roaming\Typora\typora-user-images\image-20210507102735560.png)

### 2.2功能

#### 1)词法分析

#### 2)语法分析

#### 3)语义分析

在语法分析的基础上，根据其产生式序列和以归约顺序重排的词法序列建立和维护符号表，检查是否存在重复定义，类型不一致等错误，并为代码生成提供对于符号表信息的查询接口。

#### 4）代码生成

## 3.总体设计

### 3.1 数据结构设计


#### 1)符号表IdTable

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

- 标识符**Id**

| 属性        | 类型                    | 含义       |
| ----------- | ----------------------- | ---------- |
| name        | string                  | 标识符名字 |
| dataType    | DataType                | 数据类型   |
| idType      | IdType                  | 标识符类型 |
| paramList   | vector\<Param>paramList | 参数列表   |
| retDataType | DataType                | 返回值类型 |
| isError     | bool                    | 是否有错误 |

- 参数(**Param**)

  | 属性     | 类型     | 含义       |
  | -------- | -------- | ---------- |
  | name     | string   | 标识符名字 |
  | dataType | DataType | 数据类型   |

- 数据类型(**DataType**)

  | 属性       | 类型        | 含义           |
  | ---------- | ----------- | -------------- |
  | basicType  | BasicType   | 基础数据类型   |
  | constVal   | int         | 整数型常量数值 |
  | dimension  | int         | 数组维度       |
  | upperBound | vector<int> | 数组上限       |
  | lowerBound | vector<int> | 数组下限       |
  | paramType  | ParamType   | 参数类型       |

- 枚举类型 **BasicType** 

  ```c++
  enum BasicType : int
  {
  	_integer = 1,
  	_real,
  	_boolean,
  	_char,
  };	
  ```

  对应四种支持的数据类型

- 枚举类型 **ParamType**
  
  ```c++
  enum ParamType : int
  {
	_reference = 1,
	_value
  };
  ```
  对应引用参数，传值参数

- 枚举类型 **IdType**
  ```c++
  enum IdType : int
  {
	  _constant = 1,
	  _variable,
	  _procedure,
	  _function
  };
  ```
  对应5种标识符类型

## 4.总体设计

### 4.3 语义分析

#### 1) 接口描述

输入： 语法分析生成的依照SLR分析归约顺序的产生式序列和记号序列。

输出： 供代码生成的符号表，给语义分析提供所需的列表。

#### 2) 功能描述

- 检索标识符

  对于给定的标识符进行查找，如果当前表不存在，则查找上一层符号表,直至主符号表。

- 插入标识符

  在符号表中新建对应的项。

- 定位

  建立新的子符号表，将指针指向当前符号表

- 重定位

  删除当前符号表，根据指针找到上层符号表。

- 错误检查
  
  对于作用域，赋值语句，函数和过程调用，运算符使用进行检查。

#### 3) 所用数据结构说明

- **idStack**(vector\<Id\>)

用于记录标识符信息的栈。

- **idList**(vector\<string\>)

用于变量定义时存储标识符列表。


#### 4) 详细设计

整体来看，程序根据产生式的序号，以及该产生式所归约的词法符号，决定如何进行对于符号表的修改和

- 常量定义

  $const\_value \to ...$

  $idStack$根据产生式记录下常量类型.

  $const\_declaration\to id\ relop\_e\ const\_value$

  根据$idStack$中常量类型和$id$，将其插入符号表中。


- 变量定义

  $type \to ...$

  $idStack$根据产生式记录下变量类型.

  $idlist \to id | idList,id$

  $idList$记录下id名。

  $var\_declaration->idlist\ punc\_colon\ type$

  根据$idStack$中变量量类型和$id$，将其插入符号表中。

- 运算表达式

  $x \to y\ op \ z$

  根据$op$类型以及$y$和$z$的类型得出$x$的类型，以及判断是否存在运算符的使用错误

  | 运算符          | 类型       | 子表达式y,z类型限制              | x表达式类型 |
  |-----------------|------------|----------------------------------|-------------|
  |relop           | 关系运算符 | y,z类型相同，或者为real和integer | bool        |
  | not             | 取非运算符 | bool                             | bool        |
  | minus           | 取反运算符 | integer或real                    | z类型       |
  | "+","-","*","/" | 算术运算符 | integer或real                    | 有real      |
  | "div","mod"     | 算术运算符 | integer                          | integer    |
  | "and","or"      | 与或运算符 | bool                             | bool       |

- if与else语句

  $statement\to if\ expression\ then\ statement else\_part$
  
  检查$expression$的类型是否为$bool$

- for 语句

  $statement\to for\ id\ assignop\ expression\ to\ expression\ do\ statement$

  检查$id$类型是否为$integer$

- 数组使用

  $period\to period\ punc\_comma\ digits\ punc\_point\ punc\_point\ digits$

  记录数组维度增加一维，并且记录上限

- 过程及函数使用

- read与write处理