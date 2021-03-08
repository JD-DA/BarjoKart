# Compilteur
CPPC := g++
#options de compilation
#CPPFLAGS := --std=c++98 -Wall -Wextra -pedantic -ggdb
# Sources & objets
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)
# executable
#EXEC = main

# Règle de compilation
all : clean main

mem : main
	valgrind --leak-check=full ./$<

main : $(OBJ)
	g++ --std=c++17 -Wall -Wextra -pedantic -ggdb -o $@  $^	`libpng-config --ldflags`

%.o: %.cpp
	g++ --std=c++17 -Wall -Wextra -pedantic -ggdb -o $@ -c  $< `libpng-config --cflags`

.PHONY : clean

clean:
	rm -f *.o
