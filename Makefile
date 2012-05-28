CFLAGS=-pipe -O2 -g -Wall -Werror -Wchar-subscripts -Wundef -Wshadow -Wwrite-strings -Wsign-compare -Wunused -Wno-unused-parameter -Wuninitialized -Winit-self -Wpointer-arith -Wredundant-decls -Wformat-nonliteral -Wno-format-zero-length -Wno-format-y2k -Wmissing-format-attribute -Wsequence-point -Wparentheses -Wmissing-declarations

ifeq ('','$(shell pkg-config tokyocabinet || echo failed)')
    TC_CFLAGS:=$(shell pkg-config --cflags tokyocabinet)
    TC_LIBS:=$(shell pkg-config --libs tokyocabinet)
else
    $(error tokyocabinet library not found)
endif

M_LIBS := -lm

all: extract

extract: tri.o graphml.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) $(TC_CFLAGS) $(TC_LIBS) $(M_LIBS) -o $@

clean:
	-rm *.o
	-rm extract
