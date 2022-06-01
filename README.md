## 一个简单的Pascal编译器： Simple-Pascal-Compiler

### 一、项目介绍

本仓库为ZJU计算机学院2022春夏编译原理课程的大程序设计。

本项目旨在学习编译器原理与实现，仅供参考，基于MIT协议开源。

该Pascal编译器（工具链）完全使用C++实现，有以下组件：

- Pascal编译器前端（生成AST）
- 抽象语法树可视化工具
- 目标机汇编代码生成
- 目标机汇编器
- 目标机机器码仿真

编译工具链完整的构建过程为：

单个Pascal源代码文件 `*.pas` -> 目标机汇编文件 `*.S` -> 十六进制字符串文本文件 `*.hex` -> 二进制程序文件 `*.bin`

树可视化工具的输出为 `*.png` 或 `*.svg` 格式的图像。如果有条件，实现一个交互式语法树浏览器。

目标机ISA尽可能简单，有基本的算术、逻辑、访存和控制指令以及基本I/O。暂时选择RISC-V RV32I作为目标ISA。

Pascal语言具有以下基本语言特性，我们将选择一部分实现：

- 字面量与常量的实现
  - 无符号或有符号数
  - Maxint/True/False
  - 字符串
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
  - 字符串类型
- 表达式的实现
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
- 块和作用域控制
- Procedure和Function
- 部分内置函数（输入、输出）

### 二、工程结构

``` bash
---- / ---- bin 预先构建的编译器二进制文件
       |
       ---- doc 参考文档
       |
       ---- pas 测试使用的Pascal源代码
       |
       ---- src ---- as 汇编器源码
       |        |
       |        ---- pc 编译器前后端源码
       |        |
       |        ---- sim 仿真器源码
       |        |
       |        ---- util 实用工具源码
       |
       ---- tester TA提供的测试工具
       |
       ---- .clang-format
       |
       ---- .gitignore
       |
       ---- LICENSE 
       |
       ---- Makefile
       |
       ---- README.md 本说明文档
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
- [ ] 实现目标机汇编生成
- [x] 实现汇编器
- [x] 实现仿真器
- [ ] 完成仿真器或实机测试

目前已经实现的语言特性：

字面量与常量：
|        |   词法/语法分析    |      语义分析      |    汇编代码生成    |
| :----- | :----------------: | :----------------: | :----------------: |
| 字面量 | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| 常量   | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |

类型与变量定义：
|            |   词法/语法分析    |      语义分析      |                  汇编代码生成                   |
| :--------- | :----------------: | :----------------: | :---------------------------------------------: |
| 实类型     | :heavy_check_mark: | :heavy_check_mark: | :heavy_exclamation_mark:<br>(布尔类型有对齐Bug) |
| 字符串类型 | :heavy_check_mark: | :heavy_check_mark: |     :heavy_exclamation_mark:<br>(有对齐Bug)     |
| 枚举类型   | :heavy_check_mark: | :heavy_check_mark: |                                                 |
| 子界类型   | :heavy_check_mark: | :heavy_check_mark: |                                                 |
| 集合类型   |                    |                    |                                                 |
| 数组类型   | :heavy_check_mark: | :heavy_check_mark: |               :heavy_check_mark:                |
| 记录类型   | :heavy_check_mark: | :heavy_check_mark: |               :heavy_check_mark:                |
| 指针类型   | :heavy_check_mark: | :heavy_check_mark: |                                                 |
| 过程和函数 | :heavy_check_mark: | :heavy_check_mark: |               :heavy_check_mark:                |

结构化变量访问：
|              |   词法/语法分析    |      语义分析      |    汇编代码生成    |
| :----------- | :----------------: | :----------------: | :----------------: |
| 数组成员访问 | :heavy_check_mark: | :heavy_minus_sign: | :heavy_check_mark: |
| 记录成员访问 | :heavy_check_mark: | :heavy_minus_sign: | :heavy_check_mark: |
| 指针变量访问 | :heavy_check_mark: | :heavy_minus_sign: |                    |

表达式：
|          |   词法/语法分析    |      语义分析      |    汇编代码生成    |
| :------- | :----------------: | :----------------: | :----------------: |
| 算术运算 | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| 关系运算 | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| 逻辑运算 | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |

语句：
|                |   词法/语法分析    |      语义分析      |    汇编代码生成    |
| :------------- | :----------------: | :----------------: | :----------------: |
| 赋值语句       | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| goto语句       |        :x:         |        :x:         |        :x:         |
| if语句         | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| case语句       | :heavy_check_mark: | :heavy_check_mark: |                    |
| while语句      | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| repeat语句     | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| for语句        | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| with语句       |        :x:         |        :x:         |        :x:         |
| 调用语句       | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| read/write语句 | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| 复合语句       | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |

作用域相关：
|                      |   词法/语法分析    |      语义分析      |    汇编代码生成    |
| :------------------- | :----------------: | :----------------: | :----------------: |
| 全局变量定义和访问   | :heavy_minus_sign: | :heavy_check_mark: | :heavy_check_mark: |
| 局部变量定义和访问   | :heavy_minus_sign: | :heavy_check_mark: | :heavy_check_mark: |
| 非全局非局部变量访问 | :heavy_minus_sign: | :heavy_check_mark: |                    |

编译器特性：
|          |   词法/语法分析    |         语义分析         |    汇编代码生成    |
| :------- | :----------------: | :----------------------: | :----------------: |
| 类型检查 | :heavy_minus_sign: |    :heavy_check_mark:    | :heavy_minus_sign: |
| 错误恢复 |                    | :heavy_exclamation_mark: | :heavy_minus_sign: |

### 四、开发环境

项目使用到以下工具：

- 词法分析器自动生成工具Lex（Flex）
- 语法分析器自动生成工具Yacc（Bison）
- 源码构建工具GNU make和Clang/LLVM
- 可视化工具Graphviz

建议在Linux环境下（Docker/VM/wsl）进行开发。Ubuntu下环境搭建：

```bash
# gnu tool-chain
sudo apt install build-essential

# llvm & clang
sudo apt install llvm clang

# lex & yacc
sudo apt install flex bison

# visualization tool
sudo apt install graphviz
```

### 五、构建与测试

构建可执行文件：
```bash
# Pascal Compiler
make pc

# Assembler
make as

# Hex2Bin
make util

# RV32I Simulator
make sim
```

构建上述所有可执行文件：
```bash
make build

# alias
make all
```

清理构建产物：
```bash
make clean
```

AST可视化：
```bash
make visual PAS_SRC=fib_iterative.pas
```

编译Pascal代码：
```bash
make compile PAS_SRC=fib_iterative.pas
```

编译Pascal代码并输出诊断信息：
```bash
make diagnose PAS_SRC=fib_iterative.pas
```

使用模拟器运行目标代码：
```bash
make run PAS_SRC=fib_iterative.pas
```

使用模拟器调试目标代码：
```bash
make debug PAS_SRC=fib_iterative.pas
```

使用TA提供的快速排序测试工具：
```bash
make test1
```

使用TA提供的矩阵乘法测试工具：
```bash
make test2
```

pas目录下示例代码说明

| 文件名                                     | 功能                     |
| :----------------------------------------- | :----------------------- |
| array_record_nest.pas                      | 测试数组记录嵌套寻址     |
| error_detect.pas                           | 测试错误检测             |
| fib_iterative.pas                          | 斐波那契数列（循环）     |
| fib_recursive.pas                          | 斐波那契数列（递归）     |
| letter_count.pas                           | 统计单词中字母出现的次数 |
| mat_mul.pas（TestCase2:heavy_check_mark:） | 矩阵乘法                 |
| multiple_table.pas                         | 打印乘法表（三种循环）   |
| nest.pas                                   | 测试作用域               |
| parser_test.pas                            | 测试语法分析             |
| qsort.pas（TestCase1:heavy_check_mark:）   | 快速排序                 |
