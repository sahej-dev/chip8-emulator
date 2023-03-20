PROG = main

CXX = g++
CXXFLAGS = -std=c++17 -g
LIBS = -lSDL2

OBJDIR = obj

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)
DEPS = $(OBJS:.o=.d)

# .PHONY: $(PROG)

${PROG}: ${OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

-include $(DEPS)

$(OBJDIR)/%.o: %.cpp Makefile
	$(CXX) ${CXXFLAGS} -MMD -MP -c $< -o $@

clean:
	rm -f ${PROG} $(OBJDIR)/*.o $(OBJDIR)/*.d
