CFLAGS+=-Werror -Wall

all: oswdiag

oswdiag: oswdiag.o mdio.o atheros.o

$(wildcard *.o):$(wildcard *.h) $(lastword $(MAKEFILE_LIST))

install:$(DESTDIR)/sbin/oswdiag

$(DESTDIR)/sbin/oswdiag: oswdiag
	install -d $(DESTDIR)/sbin
	install oswdiag $(DESTDIR)/sbin/oswdiag

clean:
	rm -rf *.o oswdiag

.PHONY:all install clean