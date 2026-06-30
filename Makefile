.PHONY: all clean

# Default rule compiles in parallel
all:
	cmake --build build -j

clean:
	cmake --build build --target clean
