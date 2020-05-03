.PHONY: build clean

OUT=./out

build:
	mkdir -p $(OUT)
	$(CC) -Wall -shared -o $(OUT)/libfiom src/*.c -ldl

clean:
	rm -rf $(OUT)
