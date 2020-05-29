CC = clang
PROG = hisho

include common.mk

CFLAGS += -g -fsanitize=address -std=c99
LDFLAGS +=

# linking the program.
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)
