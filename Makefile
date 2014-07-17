Out = build

-include Makefile.in

ref: ${Out}/ref.dict

build:
	mkdir $@
build/tests/output: build
	mkdir -p $@

${Out}/ref.dict: ${RefCompiler} build
	$< assignment/words.txt $@

bench: ref build/tests/output
	./script/bench.sh tests/input/*.test

.PHONY: ref bench
