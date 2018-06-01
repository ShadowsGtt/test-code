CC := gcc
CPPFLAGS := -Iinclude
LIB := -lm 
LIBDIR = -L

src := $(wildcard ./src/*.c)
allobj := $(patsubst %.c,%.o,$(src))



servobj := $(filter-out ./src/client.o,$(allobj) )
cliobj  := $(filter-out ./src/server.o ./src/Socket.o,$(allobj) )

.PHONY:target
target:client server


server:$(servobj)
	$(CC) $^ -o $@ $(LIB)
client:$(cliobj)
	$(CC) $^ -o $@ $(LIB)
%.o:%.c
	$(CC) -c $^ $(CPPFLAGS) -o $@

.PHONY:clean
clean:
	-rm -f $(allobj)
	-rm -f client
	-rm -f server

