
prefix = /usr/local
exec_prefix = $(prefix)
sbindir = $(exec_prefix)/sbin

src = $(wildcard src/*.c)
obj = $(src:.c=.o)

cram-md5-pwconv: $(obj)
	$(CC) -o $@ $^ $(shell pkg-config --cflags --libs openssl)

install: cram-md5-pwconv
	mkdir -p $(DESTDIR)$(sbindir)
	install -m 0755 $< $(DESTDIR)$(sbindir)/cram-md5-pwconv

.PHONY: clean
	rm -f $(obj) cram-md5-pwconv

