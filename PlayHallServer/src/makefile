#makefile 文件 第一版 最简单版本
#语法
#目标：依赖
#	命令
#app:while.c
#	gcc while.c -o app

#makefile 版本1.0 缩短编译时间
#app:main.o add.o sub.o mul.o des.o
#	gcc *.o -o app
#
#main.o:main.c
#	gcc -I../include -c main.c
#
#add.o:add.c
#	gcc -I../include -c add.c
#
#sub.o:sub.c
#	gcc -I../include -c sub.c
#
#mul.o:mul.c
#	gcc -I../include -c mul.c
#
#des.o:des.c
#	gcc -I../include -c des.c

#makefile 变量的使用
#TMP_VAR=test codes
#
#output:
#	echo "$(TMP_VAR)"

#内置变量
#$@ 代替目标
#$^ 获取当前目标的所有依赖项
#$< 表示依赖中的第一项


#makefile 版本2.0 使用变量 自动获取源码文件名，使用内建语法，简化.o文件的生成

#INCLUDE_PATH=../include

#SFILE=$(wildcard *.c)
#该函数可以遍历文件夹，将.c后缀的文件名都获取到，并返回存储在sfile变量中

#DFILE=$(patsubst %.c,%.o,$(SFILE))
#该函数可以对一个字符串进行处理和替换，例如将sfile中所有.c后缀的文件名替换为.o 替换完毕后存储到dfile变量中

#app:$(DFILE)
#	gcc $^ -o $@

#%.o:%.c
#	gcc -I$(INCLUDE_PATH) -c $<
#该语法可以自动遍历迭代当前目录，并生成.o文件

#output:
#	@echo $(DFILE)

#makefile 版本2.1 添加常用变量 增加复用，便于后期维护调整
TARGET=server#用于存储目标名
CC=g++#用于存储编译器版本
INCLUDE_PATH=../include#用于存储头文件路径
INSTALL_PATH=/usr/bin/#安装位置
LIBRARY_PATH= #存放共享库或静态库位置
LIBRARY=-lpthread -lmysqlclient
CFLAGS=-I$(INCLUDE_PATH) -c -g -Wall#编译选项
CPPFLAGS=-I$(INCLUDE_PATH) -c -pipe -g -std=gnu++11 -Wall -W -fPIC #编译选项
#CPPFLAGS=-D -L -l -O1 -O2 -O3 #预处理选项，编译优化及库使用
RM=sudo rm -rf
COPY=sudo cp

SFILE=$(wildcard *.cpp)
#该函数可以遍历文件夹，将.c后缀的文件名都获取到，并返回存储在sfile变量中

DFILE=$(patsubst %.cpp,%.o,$(SFILE))
#该函数可以对一个字符串进行处理和替换，例如将sfile中所有.c后缀的文件名替换为.o 替换完毕后存储到dfile变量中

$(TARGET):$(DFILE)
	$(CC) $^ -o $@ $(LIBRARY)

%.o:%.cpp
	$(CC) $(CPPFLAGS) $<
#该语法可以自动遍历迭代当前目录，并生成.o文件

clean:
	$(RM) $(DFILE) $(TARGET)

output: 
	echo $(INSTALL_PATH)$(TARGET)

install:
	$(COPY) $(TARGET) $(INSTALL_PATH)
#拷贝需要权限
	
#distclean:
#	$(RM) $(INSTALL_PATH)$(TARGET)
#将安装拷贝的应用删除，注意文件名或变量名，防止误删除，最好先outut确认一下

