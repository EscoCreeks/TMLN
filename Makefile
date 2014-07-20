Out = build

-include Makefile.in

CBIN = TextMiningCompiler
ABIN = TextMiningApp

CSRC = src/dict.cc src/trie.cc

CXXFLAGS = -I include -std=c++11

COBJS = ${CSRC:.cc=.o}

all:  ${Out} ${CBIN}

ref: ${Out}/ref.dict

${Out}/${CBIN}: ${COBJS}
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o $@ $^ src/main.cc

${CBIN}: ${Out}/${CBIN}
	cp $< $@

bench: ref build/tests/output
	./script/bench.sh tests/input/*.tes

test: build/tests/test.xml

build/tests/test.xml: build/test ${CBIN}
	$< --gtest_output=xml:$@ || true

${Out}:
	mkdir $@

build/tests/output: build
	mkdir -p $@

${Out}/ref.dict: ${RefCompiler} build
	$< assignment/words.txt $@

build/test: tests/test.cc build ${CBIN}
	${CXX} ${CXXFLAGS} ${LDFLAGS} -l gtest -o $@ $< ${COBJS}

clean:
	${RM} -rf build ${CBIN}

.PHONY: ref bench test
.PHONY: build/tests/test.xml
