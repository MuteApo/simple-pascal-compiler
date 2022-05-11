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

目标机ISA尽可能简单，汇编指令不超过50条（如70年代的8位MPU，或者现代RISC CPU），有基本的算术（add）、逻辑（and or xor not）、访存（load和store）和控制指令（branch和call）以及基本I/O（read和print）。可以参考：

- RISC-V RV32-I（需要设计简单的I/O）
- Intel MCS-51（使用UART串口作为I/O）
- MOS6502（使用6520 PIA的ASCII I/O）

项目使用到以下工具：

- 词法分析器自动生成工具Lex（Flex）
- 语法分析器自动生成工具Yacc
- 构建工具GNU make和GCC/g++
- 可视化工具Graphviz（或OpenCV）

建议在Linux环境下（Docker/VM/wsl）进行开发。

### 二、工程结构

``` bash
---- / ---- src ---- pc 编译器前后端源码
       |        |
       |        ---- vis 可视化工具源码
       |        |
       |        ---- （asm 汇编器源码）
       |        |
       |        ---- （sim 仿真器源码）
       |
       ---- release 本工具链可以直接执行的二进制文件
       |
       ---- doc 实现参考文档
       |
       ---- demo 测试过程中使用到的Pascal源代码
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

- [x] 建立项目仓库，确定需要实现的Pascal语言标准
  - 语言标准参考`./doc/Pascal_User_Manual_and_Report_Fourth_Edition.pdf`
  - 词法分析（Lex）实现参考：
    - <https://opensource.apple.com/source/flex/flex-26/flex/examples/manual/pascal.lex>
    - <https://github.com/yinxiaojian/pascal-compiler/blob/master/lex.l>
  - 语法分析（Yacc）实现参考：
    - <https://github.com/yinxiaojian/pascal-compiler/blob/master/yacc.y>
- [ ] 实现编译器前端
  - [ ] 词法分析器（需要移除注释）
  - [ ] 语法分析器
    - [ ] 常量的实现
      - [ ] 无符号或有符号数常量
      - [ ] 标识符常量（Maxint/True/False）
      - [ ] 字符串常量
    - [ ] 类型系统的实现（类型定义和变量定义）
      - [ ] 简单数据类型
        - [ ] 实数类型：
          - [ ] Boolean
          - [ ] Integer
          - [ ] Char
          - [ ] Real
        - [ ] 序数类型：
          - [ ] 枚举类型
          - [ ] 子域类型
      - [ ] 结构化数据类型
        - [ ] 数组类型
        - [ ] 集合类型
        - [ ] 记录类型
      - [ ] 指针类型
    - [ ] 运算符的实现
      - [ ] 算术运算
      - [ ] 关系运算
      - [ ] 布尔运算
    - [ ] 语句的实现
      - [ ] 赋值语句（... := ...）
      - [ ] 过程调用语句
      - [ ] goto语句
      - [ ] 结构化语句
        - [ ] 复合语句（begin ... end）
        - [ ] 条件语句
          - [ ] if ... then ... else ...
          - [ ] case ... of ...
        - [ ] 循环语句
          - [ ] while ... do ...
          - [ ] repeat ... until ...
          - [ ] for ... := ... to ... do ...
          - [ ] with ... do ...
    - [ ] 块和作用域的精细控制
    - [ ] Procedure和Function
    - [ ] 部分内置函数（输入、输出）
- [ ] 实现编译器后端
- [ ] 实现语法树可视化
- [ ] 实现汇编器、仿真器
- [ ] 完成仿真器或实机测试

### 三、词法分析 - 正则表达式

### 四、语法分析 - EBNF
