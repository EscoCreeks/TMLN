Out = build

-include Makefile.in

ref: ${Out}/ref.dict

build:
	mkdir $@
build/bench_output: build
	mkdir -p $@

${Out}/ref.dict: ${RefCompiler} build
	$< assignment/words.txt $@

bench: ref build/bench_output
	./script/bench.sh tests/input/*.test

.PHONY: ref bench
