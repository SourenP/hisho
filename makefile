CC = clang
PROG = hisho_test

include common.mk

CFLAGS += -Wall -g -fsanitize=address -std=c99
LDFLAGS +=

# linking the program.
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)
