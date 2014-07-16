Out = build

-include Makefile.in

ref: ${Out}/ref.dict

build:
	mkdir $@
build/tests: build
	mkdir $@

${Out}/ref.dict: ${RefCompiler} build
	$< assignment/words.txt $@

bench: ref build/tests
	./script/bench.sh tests/*.test

.PHONY: ref bench
