## 一个简单的Pascal编译器： Simple-Pascal-Compiler

### 一、项目介绍

本仓库为ZJU计算机学院2022春夏编译原理课程的大程序设计。

本项目旨在学习编译器原理与实现，仅供参考，基于MIT协议开源。

该***单文件***Pascal编译器有以下组件：

- Pascal编译器前端，生成AST（C++）
- 抽象语法树可视化工具（C++/Python）
- 目标机汇编代码生成（可选，C++）
- 目标机汇编器和机器码仿真（可选，C++）

编译过程为：`*.pas`->`*.hex`或`*.bin`，同时产生目标机汇编代码（或三地址码）文件`*.S`

树可视化工具的输出为`*.png`格式的图片。如果有条件，可以实现一个基于Python的交互式可视化工具，建立源代码与AST节点的对应关系

目标机ISA尽可能简单，汇编指令不超过50条（如70年代的8位MPU，或者现代RISC CPU），有基本的算术（add）、逻辑（and or xor not）、访存（load和store）和控制指令（branch和call）以及基本I/O（read和print）。

暂时选择RISC-V RV32-I（with OpenSBI）作为目标ISA。

Pascal语言具有以下基本语言要素，我们将选择一部分实现：

- 常量的实现
  - 无符号或有符号数常量
  - 标识符常量（Maxint/True/False）
  - 字符串常量
- 类型系统的实现（类型定义和变量定义）
  - 简单数据类型
    - 实数类型：
      - Boolean
      - Integer
      - Char
      - Real
    - 序数类型：
      - 枚举类型
      - 子界类型
  - 结构化数据类型
    - 数组类型
    - 集合类型
    - 记录类型
  - 指针类型
- 运算符的实现
  - 算术运算
  - 关系运算
  - 布尔运算
- 语句的实现
  - 赋值语句（... := ...）
  - 过程调用语句
  - goto语句
  - 结构化语句
    - 复合语句（begin ... end）
    - 条件语句
      - if ... then ... else ...
      - case ... of ...
    - 循环语句
      - while ... do ...
      - repeat ... until ...
      - for ... := ... to ... do ...
      - with ... do ...
- 块和作用域的精细控制
- Procedure和Function
- 部分内置函数（输入、输出）

### 二、工程结构

``` bash
---- / ---- src ---- pc 编译器前后端源码
       |        |
       |        ---- （asm 汇编器源码）
       |        |
       |        ---- （sim 仿真器源码）
       |
       ---- bin 本工具链可以直接执行的二进制文件
       |
       ---- doc 实现参考文档
       |
       ---- test 测试过程中使用到的Pascal源代码
       |
       ---- README.md 本说明文档
       |
       ---- LICENSE 
       |
       ---- Makefile
       |
       ---- .gitignore
```

### 三、项目进度

目前已经完成的模块：

- [x] 建立项目仓库，确定需要实现的Pascal语言标准
  - 语言标准参考`./doc/Pascal_User_Manual_and_Report_Fourth_Edition.pdf`
  - 词法分析（Lex）实现参考：
    - <https://opensource.apple.com/source/flex/flex-26/flex/examples/manual/pascal.lex>
    - <https://github.com/yinxiaojian/pascal-compiler/blob/master/lex.l>
  - 语法分析（Yacc）实现参考：
    - <https://github.com/yinxiaojian/pascal-compiler/blob/master/yacc.y>
- [ ] 实现编译器前端
  - [x] 词法分析器（需要移除注释）
  - [x] 语法分析器
  - [ ] 语义分析器
- [x] 实现语法树可视化
- [ ] 实现编译器后端、汇编器、仿真器
- [ ] 完成仿真器或实机测试

目前已经实现的语言特性：

|                  |   词法/语法分析    | 语义分析 | 后端支持 |
| :--------------: | :----------------: | :------: | :------: |
|    自定义常量    | :heavy_check_mark: |          |          |
|   自定义实类型   | :heavy_check_mark: |          |          |
| 自定义字符串类型 |                    |          |          |
|  自定义枚举类型  | :heavy_check_mark: |          |          |
|  自定义子界类型  | :heavy_check_mark: |          |          |
|  自定义集合类型  | :heavy_check_mark: |          |          |
|  自定义数组类型  | :heavy_check_mark: |          |          |
|  自定义记录类型  | :heavy_check_mark: |          |          |
|  自定义指针类型  |                    |          |          |
|   数组成员访问   | :heavy_check_mark: |          |          |
|   记录成员访问   | :heavy_check_mark: |          |          |
|   指针变量访问   |                    |          |          |
|     变量定义     | :heavy_check_mark: |          |          |
|     过程定义     | :heavy_check_mark: |          |          |
|     函数定义     | :heavy_check_mark: |          |          |
|     算术运算     | :heavy_check_mark: |          |          |
|     关系运算     | :heavy_check_mark: |          |          |
|     逻辑运算     | :heavy_check_mark: |          |          |
|     复合语句     | :heavy_check_mark: |          |          |
|     赋值语句     | :heavy_check_mark: |          |          |
|  过程/函数调用   | :heavy_check_mark: |          |          |
|     goto语句     |                    |          |          |
|      if语句      | :heavy_check_mark: |          |          |
|     case语句     | :heavy_check_mark: |          |          |
|    while语句     | :heavy_check_mark: |          |          |
|    repeat语句    | :heavy_check_mark: |          |          |
|     for语句      | :heavy_check_mark: |          |          |
|     with语句     |                    |          |          |
|  read/write语句  |                    |          |          |
|    作用域控制    | :heavy_minus_sign: |          |          |
|     递归调用     | :heavy_minus_sign: |          |          |
|       RTTI       | :heavy_minus_sign: |          |          |

### 四、开发环境

项目使用到以下工具：

- 词法分析器自动生成工具Lex（Flex）
- 语法分析器自动生成工具Yacc（Bison）
- 构建工具GNU make和GCC/G++
- 可视化工具Graphviz

建议在Linux环境下（Docker/VM/wsl）进行开发。Ubuntu下环境搭建：

```bash
# gnu tool-chain
sudo apt install build-essential

# lex
sudo apt install flex

# yacc
sudo apt install bison

# visualization tool
sudo apt install graphviz
```

构建可执行文件：
```bash
make all
```

清理构建产物：
```bash
make clean
```

测试编译器（指定输入文件并产生可视化结果）：
```bash
make debug SRC=test/fib.pas
```

