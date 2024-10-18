## 指定编译器
CC = gcc

# 指定编译选项
CFLAGS = -Wall -g

# 源文件和目标文件
SRCS = src/main.c src/utils.c
OBJS = $(SRCS:.c=.o)

# 最终的可执行文件名
TARGET = my_program

# 默认目标
all: $(TARGET)

# 链接目标
$(TARGET): $(OBJS)
	$(CC) -o $@ $^

# 编译源文件为目标文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
