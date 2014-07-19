Out = build

-include Makefile.in

CBIN = TextMiningCompiler
ABIN = TextMiningApp

CSRC = src/main.cc

CXXFLAGS = -I include -std=c++11

COBJS = ${CSRC:.cc=.o}

all:  ${Out} ${CBIN}

ref: ${Out}/ref.dict

${Out}/${CBIN}: ${COBJS}
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o $@ $^

${CBIN}: ${Out}/${CBIN}
	cp $< $@

bench: ref build/tests/output
	./script/bench.sh tests/input/*.test

test: build/tests/test.xml

build/tests/test.xml: build/test
	$< --gtest_output=xml:$@

${Out}:
	mkdir $@

build/tests/output: build
	mkdir -p $@

${Out}/ref.dict: ${RefCompiler} build
	$< assignment/words.txt $@

build/test: tests/test.cc build
	${CXX} ${CFLAGS} ${LDFLAGS} -l gtest -o $@ $<

clean:
	${RM} -rf build ${CBIN}

.PHONY: ref bench test
.PHONY: build/tests/test.xml
