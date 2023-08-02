CC=gcc
CFLAGS=
LDFLAGS=

PROGS :=
PROGS += roundup

all: $(PROGS)

PHONY :=
PHONY +=

roundup: roundup.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(PROGS)

PHONY += $(clean)

.PHONY: $(PHONY)
