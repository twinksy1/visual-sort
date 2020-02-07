PROG_NAME=sort
SOURCE=$(PROG_NAME).cpp
ARGS=-Wall -lX11 -lXext -lm

all: $(PROG_NAME)

$(PROG_NAME): $(SOURCE)
	g++ $< -o $@ $(ARGS)

RUN: $(SOURCE)
	g++ $< -o $(PROG_NAME) $(ARGS); ./$(PROG_NAME)

clean:
	rm -f $(PROG_NAME)
