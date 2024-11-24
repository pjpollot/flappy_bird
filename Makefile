CPP=g++ -std=c++17
HEADERS=-I"./headers" -I"/opt/homebrew/include" -I"/opt/homebrew/include/eigen3"
LIBS=-L"/opt/homebrew/lib" -lSDL2 -lSDL2_image -lSDL2_mixer

OUT="./run"

all:
	${CPP} -c src/*.cpp src/*/*.cpp ${HEADERS}
	${CPP} *.o -o ${OUT} ${LIBS}

clean:
	rm *.o ${OUT}
