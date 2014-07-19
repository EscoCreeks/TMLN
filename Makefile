Out = build

-include Makefile.in

ref: ${Out}/ref.dict

bench: ref build/tests/output
	./script/bench.sh tests/input/*.test

test: build/tests/test.xml

build/tests/test.xml: build/test
	$< --gtest_output=xml:$@

build:
	mkdir $@
build/tests/output: build
	mkdir -p $@

${Out}/ref.dict: ${RefCompiler} build
	$< assignment/words.txt $@

build/test: tests/test.cc build
	${CXX} ${CFLAGS} ${LDFLAGS} -l gtest -o $@ $<

clean:
	${RM} -rf build
.PHONY: ref bench test
.PHONY: build/tests/test.xml
