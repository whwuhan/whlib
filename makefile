# SRC := src/%.cpp src/%.cpp algorithm/%.cpp basic/%.cpp utils/%.cpp

#获取src下所有的cpp文件
SRCS = $(wildcard src/algorithm/*.cpp src/basic/*.cpp src/utils/*.cpp src/utils/io/*.cpp)
#所有的.cpp文件的后缀改为.o
OBJS = $(patsubst %.cpp, %.o, $(SRCS))  
#是否编译成静态库
STATIC_LIB = FALSE

#ifeq后面必须有空格
#编译成静态库
ifeq ($(STATIC_LIB), TRUE)
libwhlib : $(OBJS)
	ar cr libwhlib.a $(OBJS)
else
#编译成可执行程序
main : $(OBJS) main.cpp
	g++ -std=c++11 -Iinclude -Ideps/eigen-3.3.7 -Ideps/boost/1.72.0_3/include -Ideps/glm \
	$(OBJS) main.cpp -o main
	./main
endif

# $^:所有的依赖文件 $<:第一个依赖文件 $@:目标文件
$(OBJS) : %.o : %.cpp
	g++ -std=c++11 -Iinclude -Ideps/eigen-3.3.7 -Ideps/boost/1.72.0_3/include -Ideps/glm $< -c -o $@
	
.PHONY : clean
clean : 
	-rm -rf Coolender $(OBJS)