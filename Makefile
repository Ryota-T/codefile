T1 = code

CC = g++
CFLAGS = -c -g -O2 -Wall
LDFLAGS = `mecab-config --cflags` `mecab-config --libs`

OBJ1 = code.o

#Rules for make

all: $(T1)

$(T1): $(OBJ1)
	$(CC) -o $@ $^ $(LDFLAGS)

.cc.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f *~ *.o *.exe *.stackdump $(T1)
