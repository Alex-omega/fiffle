# 主文件
MAIN = main.c

# 自动检测其他源文件（排除 main.c）
SRCS = $(filter-out $(MAIN), $(wildcard *.c))

# 所有目标文件
OBJS = $(SRCS:.c=.o) main.o

# 主程序
TARGET = program

# 默认目标
all: $(TARGET)

# 链接目标文件
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# 通用规则
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
