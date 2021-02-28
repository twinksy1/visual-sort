OBJ=display.o gui.o global.o main.o
EXE=sort
HEADERS=*.h
LIBS=-lX11 -lXext
ARGS=-c
CC=g++

all: $(EXE)

%.o: %.cpp $(HEADERS)
	$(CC) $(ARGS) $< -o $@

$(EXE): $(OBJ)
	$(CC) $^ -o $@ $(LIBS)

clean:
	rm $(EXE) *.o ./*/*.o
