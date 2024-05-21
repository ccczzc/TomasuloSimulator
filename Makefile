all:
	mkdir -p build && cd build && cmake .. && make
	./build/TomasuloSimulator ./tests/CODE1.S
	# ./build/TomasuloSimulator ./tests/CODE1.S
	# ./build/TomasuloSimulator ./tests/CODE2.S
	