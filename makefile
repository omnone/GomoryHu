LEDAROOT := /usr/local/LEDA
BOOSTROOT := /usr/local/include/
SRCDIR = src

TARGET := ask3
OBJS   := $(SRCDIR)/ask3.cpp
HEADERS := $(SRCDIR)/setup.cpp 

CXX := g++ -w -g

#CXXFLAGS := -O3
CXXFLAGS := $(CXXFLAGS) -I  $(BOOSTROOT) -I $(LEDAROOT)/incl
LIBS     += -L $(LEDAROOT) -L/usrX11R6/lib64/ -lm -lX11 -lleda

all: run

compile: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(HEADERS) $(LIBS) -o $(TARGET)
  
run:  ask3
	./ask3
   
clean: rm -rf $(TARGET) 