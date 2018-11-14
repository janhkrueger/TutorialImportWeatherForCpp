EXECUTABLE=ReadWeatherSource

SRC=src
BIN=bin
BINCOV=coverage

CC=g++
CFLAGS=-std=c++17 -flto -pthread -D_FORTIFY_SOURCE=2 -pie -fPIE -ftrapv -Wl,-z,relro,-z,now -fpic -fvisibility=hidden -fstack-check -fstack-protector-all --param ssp-buffer-size=1 -fmessage-length=0
CWARNINGS=-Wextra -Wconversion -Wsign-conversion -Wformat-nonliteral -Wstack-protector -Wno-unused-parameter -Wformat=2 -Wformat-security -Winit-self -Wuninitialized -Wswitch-default -Wswitch-enum -Wunused-but-set-parameter -Wunused-parameter -Wunused-but-set-variable -Wfloat-equal -Wshadow -Wunreachable-code
CINCLUDES=-I$(SRC) -I/usr/include/postgresql -I frameworks/rapidjson/include/ -I frameworks/spdlog/include/ -I/usr/include -Iinclude -includelibpq-fe.h
CLIBS=-lpqxx -lpq -Llib -lcurlpp -lcurl

SOURCES=$(SRC)/Batch.cpp $(SRC)/Weather.cpp $(SRC)/WeatherFactory.cpp $(SRC)/$(EXECUTABLE).cpp
OBJECTS=$(SOURCES:.cpp=.o)

all:
	export GLIBCXX_FORCE_NEW=true
	$(CC) $(CFLAGS) -O3 $(SOURCES) $(CLIBS) $(CINCLUDES) -o $(BIN)/$(EXECUTABLE)
	export GLIBCXX_FORCE_NEW=false

.PHONY: coverage
coverage:
	export GLIBCXX_FORCE_NEW=true
	$(CC) $(CFLAGS) -O0 -fprofile-arcs -ftest-coverage -g $(SOURCES) $(CLIBS) $(CINCLUDES) -o $(BINCOV)/$(EXECUTABLE)
	export GLIBCXX_FORCE_NEW=false

full:
	export GLIBCXX_FORCE_NEW=true
	$(CC) $(CFLAGS) -O3 $(CWARNINGS) $(SOURCES) $(CLIBS) $(CINCLUDES) -o $(BIN)/$(EXECUTABLE)
	export GLIBCXX_FORCE_NEW=false

security:
	export GLIBCXX_FORCE_NEW=true
	$(CC) $(CFLAGS) -O3 $(SOURCES) $(CLIBS) $(CINCLUDES) -o $(BIN)/$(EXECUTABLE)
	valgrind --undef-value-errors=yes --time-stamp=yes --leak-check=full --track-fds=yes --show-reachable=yes --log-file=code-maintenance/valgrind_$(EXECUTABLE).log -v $(BIN)/$(EXECUTABLE)
	export GLIBCXX_FORCE_NEW=false
