# $^ 依赖 不重复
# @$ 目标
# @ 不显示命令执行 失败不停止

TARGET1=./bin/main
OBJS1=./build/add.o ./build/main.o
CXXFLAGS=-I./include -Wall -g


$(TARGET1):$(OBJS1)
	$(CXX) $(CXXFLAGS) $^ -o $@

./build/%.o:./src/%.cpp
	$(CXX)  -c $^ -o $@ -I./include

clean:
	$(RM) $(OBJS1) $(TARGET1)
.PHONY:clean *clean