CFLAGS=-pipe -O2 -g -Wall -Werror -Wchar-subscripts -Wundef -Wshadow -Wwrite-strings -Wsign-compare -Wunused -Wno-unused-parameter -Wuninitialized -Winit-self -Wpointer-arith -Wredundant-decls -Wformat-nonliteral -Wno-format-zero-length -Wno-format-y2k -Wmissing-format-attribute -Wsequence-point -Wparentheses -Wmissing-declarations

INPUT?=input

ifeq ('','$(shell pkg-config tokyocabinet || echo failed)')
    TC_CFLAGS:=$(shell pkg-config --cflags tokyocabinet)
    TC_LIBS:=$(shell pkg-config --libs tokyocabinet)
else
    $(error tokyocabinet library not found)
endif

all: corrolated mixed sum_b sum_c sum_j

tri: tri.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) $(TC_CFLAGS) $(TC_LIBS) -o $@

output: $(INPUT) tri
	cat $< | ./tri > $@

corrolated: output
	sed -n "s/^c \(.*\)/\1/p" < $< | sort -rn > $@ 

mixed: output
	sed -n "s/^s \(.*\)/\1/p" < $< | sort -rn > $@ 

sum_b: output
	sed -n "s/^s.b \(.*\)/\1/p" < $< | sort -rn > $@ 

sum_c: output
	sed -n "s/^s.c \(.*\)/\1/p" < $< | sort -rn > $@ 

sum_j: output
	sed -n "s/^s.j \(.*\)/\1/p" < $< | sort -rn > $@ 

clean:
	-rm output
	-rm corrolated
	-rm mixed
	-rm sum_b sum_c sum_j
	-rm tri

test: graphml.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -lm -o $@
