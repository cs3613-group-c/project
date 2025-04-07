INCLUDES=-Iinclude
DEPS=$(wildcard include/*.h)
SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:.c=.o)

train_simulation: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o train_simulation

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

# TODO: Grab all output binaries?
clean:
	rm -f src/*.o tests/*.o output/* train_simulation 

# TODO: Figure out how to make this more expandable
message_test: src/message.o tests/message_test.o 
	@$(CC) -o output/message_test src/message.o tests/message_test.o
	./output/message_test

parse_test: src/parse.o tests/parse_test/parse_test.o
	@mkdir -p output
	@$(CC) -o output/parse_test src/parse.o tests/parse_test/parse_test.o
	./output/parse_test
