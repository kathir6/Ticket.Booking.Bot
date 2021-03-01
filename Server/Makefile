INC = *.h\

SRC = *.cpp\

build: ${SRC}
		g++ ${SRC} ${INC} -o server -pthread
		./server 8001
		rm -rf server

