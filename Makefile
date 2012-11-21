SOURCES=sirbonville.o core.o gui.o
CFLAGS=`pkg-config gtk+-2.0 --cflags`
LIBS=`pkg-config gtk+-2.0 --libs`
BINARY=sirbonville


all: $(SOURCES)

clean:
	rm $(SOURCES)

link:
	gcc $(SOURCES) $(LIBS) -o sirbonville


.c.o:
	gcc -c $< -o $@ $(CFLAGS)



