LIBS = libcppredis.so
RM = rm
CCC = g++

CDEBUGFLAGS =  -g -fPIC
LDOPTIONS = -shared

ALLINCLUDES = -I../deps/hiredis
LDLIBS = -L../deps/hiredis/ -lhiredis

CCFLAGS = $(CDEBUGFLAGS) $(ALLINCLUDES)

OBJS = \
	redis_reply.o \
	redis_connection.o\
	redis_connection_pool.o \
	redis_client.o

all : build 
	@echo
	@echo "========== cpp-redis build complete =========="
	@echo

build: $(LIBS)  

$(LIBS): $(OBJS)
	$(RM) -f $@
	$(CCC) -o $@ $(OBJS) $(LDOPTIONS) $(LDLIBS)

%.o:%.cpp
	$(CCC) $(CCFLAGS) -c $<

clean:
	$(RM) core *.o $(LIBS)  -f
