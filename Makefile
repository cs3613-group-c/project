INCLUDES=-Iinclude
DEPS=$(wildcard include/*.h)
SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:.c=.o)

train_simulation: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o train_simulation

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f src/*.o train_simulation
