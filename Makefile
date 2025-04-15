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
	@mkdir -p output/
	@$(CC) -o output/message_test src/message.o tests/message_test.o
	./output/message_test

deadlock_test: src/rag.o tests/deadlock_test.o 
	@mkdir -p output/
	@$(CC) -o output/deadlock_test src/rag.o tests/deadlock_test.o
	./output/deadlock_test

parse_test: src/parse.o tests/parse_test.o
	@mkdir -p output
	@$(CC) -o output/parse_test src/parse.o tests/parse_test.o
	./output/parse_test

logger_test: src/logger.o tests/logger_test.o
	@mkdir -p output
	@$(CC) -o output/logger_test src/logger.o tests/logger_test.o
	./output/logger_test


table_test: src/table.o src/parse.o tests/table_test.o
	@mkdir -p output
	@$(CC) -o output/table_test src/table.o tests/table_test.o src/parse.o
	./output/table_test
