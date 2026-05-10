CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -I.
LDFLAGS   :=

TARGET    := piny
SRCS      := main.cpp dictionary.cpp tone.cpp utils.cpp searcher.cpp
OBJS      := $(SRCS:.cpp=.o)

TEST_DIR  := test
TEST_SRC  := $(TEST_DIR)/test.cpp
TEST_BIN  := $(TEST_DIR)/test_runner
# 不链接 main.o（测试有独立入口）
LIB_OBJS  := $(filter-out main.o, $(OBJS))
TEST_OBJ  := $(TEST_SRC:.cpp=.o)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TEST_OBJ): $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TEST_BIN): $(LIB_OBJS) $(TEST_OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

# 运行单元测试，然后做集成冒烟测试
test: $(TARGET) $(TEST_BIN)
	@echo ""
	@echo "═══ 单元测试 ═══"
	./$(TEST_BIN)
	@echo ""
	@echo "═══ 集成冒烟测试 ═══"
	@echo "  演示模式..."
	@echo "2" | timeout 5 ./$(TARGET) 2>&1 | grep -q "功能演示" && echo "  ✓ 菜单启动" || (echo "  ✗ 菜单启动" && false)
	@echo "  搜索模式..."
	@echo "3" | timeout 5 ./$(TARGET) 2>&1 | grep -q "拼音搜索" && echo "  ✓ 搜索菜单" || (echo "  ✗ 搜索菜单" && false)
	@echo "  命令行 demo 模式..."
	@timeout 5 ./$(TARGET) demo 2>&1 | grep -q "功能演示" && echo "  ✓ demo 命令" || (echo "  ✗ demo 命令" && false)
	@echo "  成语接龙模式..."
	@echo "exit" | timeout 5 ./$(TARGET) game 2>&1 | grep -q "成语接龙" && echo "  ✓ game 命令" || (echo "  ✗ game 命令" && false)
	@echo ""
	@echo "所有测试通过。"

clean:
	rm -f $(OBJS) $(TARGET) $(TEST_OBJ) $(TEST_BIN)
