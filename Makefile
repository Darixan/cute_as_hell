TMSRCS := abrahamC.cpp adrianT.cpp maribelM.cpp melanieC.cpp main.cpp

all: cute_as_hell

cute_as_hell:$(TMSRCS)
	g++ $(TMSRCS) -Wall -o cute_as_hell

clean:
	rm -f cute_as_hell
