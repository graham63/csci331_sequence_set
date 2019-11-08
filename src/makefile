CXX = g++
IDIR = ../include
ODIR = obj
CXXFLAGS=-g -Wall -std=c++11 -I$(IDIR) -W -Wall -w
LIBS = -lm -std=c++11
_DEPS = $(wildcard *.h)
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
SRC = $(wildcard *.cpp)
_OBJ= $(SRC:.cpp=.o)
OBJ= $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

app: $(OBJ)
	g++ -o app $(SRC) -I$(IDIR) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
