CC = gcc
CFLAGS = `pkg-config --cflags cairo` -lm
LIBS = `pkg-config --libs cairo`

# Define source/exe files
SRC = ccurve.c
EXE = ccurve

# Define build and clean targets
all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) -o $(EXE) $(SRC) $(LIBS)

clean:
	rm -f $(EXE)