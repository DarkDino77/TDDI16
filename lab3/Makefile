#
# Makefile
#

CCC      = g++ -pipe
CCFLAGS += -std=c++17 -pedantic -g -O3 -Wall -Wextra -ggdb



wordchain.o: wordchain.cpp 
	$(CCC) $(CCFLAGS) wordchain.cpp

clean:
	@ \rm -f *.o

zap: clean
	@ \rm -rf avl_tree-test *.o *~ 
