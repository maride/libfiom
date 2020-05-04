.PHONY: build clean test

OUTDIR=./out
OUTFILE=libfiom
TESTDIR=./test

build:
	mkdir -p $(OUTDIR)
	$(CC) -Wall -shared -o $(OUTDIR)/$(OUTFILE) src/*.c -ldl

clean:
	rm -rf $(OUTDIR)
	rm -rf $(TESTDIR)

test: build
	mkdir -p $(TESTDIR)
	uname -a > $(TESTDIR)/uname
	echo "\n\n"
	LD_PRELOAD=$(OUTDIR)/$(OUTFILE) cat $(TESTDIR)/uname
	echo "\n\n"
	rm -rf $(TESTDIR)
