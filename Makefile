.PHONY: build clean test

OUTDIR=./out
OUTFILE=libfiom
TESTDIR=./test

build: build-64 build-32

build-preparation:
	mkdir -p $(OUTDIR)

build-64: build-preparation
	$(CC) -Wall -m64 -shared -fPIC -o $(OUTDIR)/$(OUTFILE)-64.so src/*.c -ldl

build-32: build-preparation
	$(CC) -Wall -m32 -shared -fPIC -o $(OUTDIR)/$(OUTFILE)-32.so src/*.c -ldl

clean:
	rm -rf $(OUTDIR)
	rm -rf $(TESTDIR)

test: build
	mkdir -p $(TESTDIR)
	uname -a > $(TESTDIR)/uname
	echo "\n\n"
	LD_PRELOAD=$(OUTDIR)/$(OUTFILE)-64.so cat $(TESTDIR)/uname
	echo "\n\n"
	rm -rf $(TESTDIR)
