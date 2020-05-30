OUT = bin

SEARCHPATH += src
vpath %.c $(SEARCHPATH)
vpath %.h $(SEARCHPATH)

DEPS +=

_OBJS += hisho_stack.o hisho_stack_test.o
_OBJS += hisho_first_fit.o hisho_first_fit_test.o
_OBJS += main.o

OBJS = $(patsubst %,$(OUT)/%,$(_OBJS))

# top-level rule to create the program.
all: $(PROG)

# compiling other source files.
$(OUT)/%.o: %.c %.h $(DEPS)
	@mkdir -p $(OUT)
	$(CC) $(CFLAGS) -c -o $@ $<

# cleaning everything that can be automatically recreated with "make".
clean:
	$(RM) -rf $(OUT) $(PROG)
