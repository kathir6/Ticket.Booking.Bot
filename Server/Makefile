INC = *.h\

SRC = *.cpp\

build: ${SRC}
	echo "Building ..."
	g++ ${SRC} ${INC} -o server.out -pthread
	echo "Running Server ..."
	./server.out 8001
	rm -rf server.out

