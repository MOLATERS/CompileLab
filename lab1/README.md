# 编译原理实验一
## 实验环境
WSL 版本： 2.1.5.0
内核版本： 5.15.146.1-2

## 实验功能

能够识别词法类型错误(Type A)与语法类型错误(Type B)。除此之外,还应能识别八进制数和十六进制数，指数形式的浮点数，识别“//”和“/**/”形式的注释。

## 实验亮点
### 数据结构
由于在执行遍历语法分析树的过程中需要进行树的遍历操作，使用结点的方式来存储数据，由于原始的树结构遍历的方式比较困难，所以转换成二叉树的方式进行遍历，采用孩子兄弟表示法来表示树的结构，由于树的先序遍历和整棵树对应的二叉树的先序遍历序列对应的结点排列顺序相同所以可以用这样的排布方法。
结点的数据类型定义如下：

```c
typedef struct TNode{
    int lineno; // 代表行号
    char* name; // 代表结点名称
    enum NODE_TYPE type; // 代表结点类型
    int value_int; // 如果结点的类型是TYPE_INT, 存储其值
    float value_float; // 如果结点的类型是TYPE_INT, 存储其值
    char* value_string; // 如果存储的结点是TYPE_ID或者TYPE_OTHER的时候，存储其值
    struct TNode* child; // 指向第一个孩子结点
    struct TNode* brother; // 指向第一个兄弟节点
}TNode,*TreeNode;
```
各个结点元素的定义见上注释
再一个是在构建语法分析树的过程中，参数是很长的，使用"stdarg.h"的库来对参数进行处理，将读入的参数自动整合称为结点的值并创建一个新的结点。
相关的代码在`parser.c`中，有关联的代码段如下所示：
```c
va_list args;
va_start(args, argc);
if (argc > 0){
    TreeNode child = va_arg(args, struct TNode*); // 创建第一个结点为孩子结点
    ...
        current->brother = va_arg(args, struct TNode*); // 将剩下的结点作为兄弟结点放在当前指针的brother指针的位置
    ...
    }
    va_end(args);
```
### 词法分析部分
**基本功能** 识别八进制，十进制和十六进制实现进制的转换，在`parser.c`的`string_to_num()`函数实现；识别正确的浮点数和整数的形式，并确实能够正确的识别注释；能够识别出ID，TYPE，INT，FLOAT并打印相应的信息。

在执行错误处理的时候，标记`lexError`表明错误类型A，并使用了正则表达式来匹配出错的错误项，匹配的原则如下所示：

```lex
INT8_ERROR 0[0-7]*[8-9]+[0-7]*
INT16_ERROR 0[Xx][a-fA-F0-9]*[g-zG-Z]+[a-fA-F0-9]*
FLOAT_ERROR [0]+(0|[1-9][0-9]*)\.[0-9]+|\.[0-9]+|[0-9]+\.|[0-9]+[Ee][+-]?[0-9]*|([+-]?([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)[Ee][+-]?[0-9]+\.[0-9]+)
NUM_ERROR {INT8_ERROR}|{INT16_ERROR}|{FLOAT_ERROR}
ID_ERROR [0-9]+[a-zA-Z_]+
AERROR .
```
使用的错误信息处理的方式为：
```lex
{NUM_ERROR} {...}
{ID_ERROR} {...}
{AERROR} {...}
```
使用一个测试用例来验证其正确性如下所示：
![18f157d51ef1b3c4812cdaf680dcee9](https://typora-slater.oss-cn-beijing.aliyuncs.com/picture18f157d51ef1b3c4812cdaf680dcee9.png)

另外单独对`COMMENT`部分进行了编程，使得可以正确的识别基础的注释语句，并且无法识别嵌套的注释语句。

### 语法分析的错误处理

**基本功能** 构建语法分析树，先序遍历打印节点信息（打印行号，若产生e，则不打印）在词法单元部分打印词法单元名称，不打印行号，能输出词法单元的“高度”，同时满足ID，TYPE，NUMBER的打印要求

通过研究Bison的相关文档，使用了`%define parse.lac full`和`%define parse.error verbose`两个语句来精确定位到错误文段。（错误提示也如上图所示）
在语法分析的.y文件中对每个非终结符进行操作，使用`InsertNode()`函数将该非终结符加入到语法树中，并使用`synError`来标记类型B的错误

## 程序构建

**测试单个用例**

```shell
cd src
make parser
./parser ../text/test.cmm
```

**全部测试命令**

```shell
bash test.sh # 基础测试
bash test.sh -o # 附加测试
bash test.sh -a # 全部测试
```

测试文件输出设置在output文件夹下的log文件中

![image-20240412103003854](https://typora-slater.oss-cn-beijing.aliyuncs.com/pictureimage-20240412103003854.png)
