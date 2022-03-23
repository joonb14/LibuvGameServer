CC = g++

# C++ 컴파일러 옵션
CXXFLAGS = -g -Wall -O2 

# Warning 안뜨게 하기위함
NOWARN = -Wno-unused-function -Wno-format-security

# 링커 옵션
LDFLAGS = /usr/local/lib/libuv.a -L./ -lstdc++ -lpthread -ldl -lm -lservercore

# 헤더파일 경로
INCLUDE = -Iinclude/

# 소스 파일 디렉토리
LIB_DIR = ./ServerCore
SRC_DIR = ./GameServer

# 오브젝트 파일 디렉토리
OBJ_DIR = ./obj

# 생성하고자 하는 실행 파일 이름
LIB_NAME = libservercore
TARGET = MetaGameServer
# DUMMY_CLIENT = DummyClient

# Make 할 소스 파일들
# wildcard 로 SRC_DIR 에서 *.cc 로 된 파일들 목록을 뽑아낸 뒤에
# notdir 로 파일 이름만 뽑아낸다.
LIB_SRCS = $(notdir $(wildcard $(LIB_DIR)/*.cpp))
SRC_SRCS = $(notdir $(wildcard $(SRC_DIR)/*.cpp))

LIB_OBJS = $(LIB_SRCS:.cpp=.o)
SRC_OBJS = $(SRC_SRCS:.cpp=.o)
DEPS = $(LIB_SRCS:.cpp=.d)

# OBJS 안의 object 파일들 이름 앞에 $(OBJ_DIR)/ 을 붙인다.
OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o,$(LIB_OBJS))
DEPS = $(OBJECTS:.o=.d)
SRC_OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o,$(LIB_OBJS)) $(patsubst %.o,$(OBJ_DIR)/%.o,$(SRC_OBJS))
SRC_DEPS = = $(SRC_OBJECTS:.o=.d)

all: $(TARGET)

# $(DUMMY_CLIENT) : 
# 	$(CC) -o $(DUMMY_CLIENT) $(DUMMY_CLIENT).cpp -luv

$(OBJ_DIR)/%.o : $(LIB_DIR)/%.cpp
	$(CC) $(CXXFLAGS) $(INCLUDE) -fPIC -c $< -o $@ -MD

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CXXFLAGS) $(NOWARN) $(INCLUDE) -c $< -o $@ -MD

$(LIB_NAME) : $(OBJECTS)
	$(CC) $(CXXFLAGS) -shared $(OBJECTS) -o $(LIB_NAME).so 

$(TARGET) : $(SRC_OBJECTS) $(LIB_NAME) # $(DUMMY_CLIENT)
	$(CC) $(CXXFLAGS) $(SRC_OBJECTS) -o $(TARGET) $(LDFLAGS)

.PHONY: clean all

clean:
	rm -f $(SRC_OBJECTS) $(SRC_DEPS) $(LIB_NAME).so $(TARGET) $(DUMMY_CLIENT)

-include $(DEPS)