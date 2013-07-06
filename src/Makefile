LIBS = libcppredis.a
RM = rm
CCC = g++
AR = ar

CXXFLAGS = -g -fPIC -std=c++11
LDOPTIONS = 

ALLINCLUDES = -I../deps
LDLIBS = ../deps/hiredis/libhiredis.a

CCFLAGS = $(CXXFLAGS) $(ALLINCLUDES)

OBJS = \
	reply.o \
	redis_connection.o\
	redis_connection_pool.o \
	redis_client.o \
	uri.o \
	async_connection.o \
	async_service.o \
	pubsub.o

all : build 
	@echo
	@echo "========== cpp-redis build complete =========="
	@echo

build: $(LIBS)  

$(LIBS): $(OBJS)
	$(RM) -f $@
	$(AR) -cru $@ $^

%.o:%.cpp
	$(CCC) $(CCFLAGS) -c $<

clean:
	$(RM) core *.o $(LIBS)  -f
