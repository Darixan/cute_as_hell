TMSRCS := abrahamC.cpp adrianT.cpp maribelM.cpp melanieC.cpp main.cpp
FRMWRKS := libggfonts.a log.cpp
CFLAGS := -I ./include
LFLAGS := -lrt -lX11 -lGLU -lGL -lm

all: cute_as_hell

cute_as_hell:$(TMSRCS)
	g++ $(CFLAGS) $(TMSRCS) $(FRMWRKS) -Wall $(LFLAGS) -o cute_as_hell

clean:
	rm -f cute_as_hell
