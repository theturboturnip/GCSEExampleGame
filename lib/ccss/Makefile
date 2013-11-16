SRC = ccss.c
OBJ = $(SRC:.c=.o)
OUT = ccss.a
INCLUDES = -I .
CCFLAGS = -g -O2
CCC = gcc
LDFLAGS = -g

.SUFFIXES: .c

all: $(OUT)

.c.o:
	$(CCC) $(INCLUDES) $(CCFLAGS) $(EXTRACCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)

clean:
	rm -f $(OBJ) $(OUT)