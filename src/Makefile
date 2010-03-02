CFLAGS = -O0 -g

SRCS = GCSpider.cpp
OBJS = $(SRCS:%.cpp=%.o)
DEPS = $(SRCS:%.cpp=%.P)

all: main

main: $(OBJS)

%.o: %.cpp
	g++ -c $(CFLAGS) $< -o $@

%.P: %.cpp
	g++ -MM $(CFLAGS) -MP -MF $@ $<

-include $(DEPS)

clean:
	rm -rf $(OBJS) $(DEPS)

