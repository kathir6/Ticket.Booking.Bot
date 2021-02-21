INC = *.h\

SRC = *.cpp\

build: ${SRC}
		g++ ${SRC} ${INC} -o server -pthread
		./server 8000
		rm rf server

