Out = build

-include Makefile.in

CBIN = TextMiningCompiler
ABIN = TextMiningApp

CSRC = src/dict.cc src/trie.cc src/simpletriebuilder.cc src/lockfreecpp11triebuilder.cc src/lockedcpp11triebuilder.cc src/tbbparalleltriebuilder.cc

CXXFLAGS = -I include -std=c++11
LDFLAGS = -ltbb

COBJS = ${CSRC:.cc=.o}
CDEPS = ${CSRC:.cc=.deps}

all:  ${Out} ${CBIN}

ref: ${Out}/ref.dict

${Out}/${CBIN}: depend ${COBJS}
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o $@ ${COBJS} src/main.cc

${CBIN}: ${Out}/${CBIN}
	cp $< $@

depend: ${CDEPS}

%.deps: %.cc
	${CXX} ${CXXFLAGS} -MM -MT ${<:.cc=.o} $< -o $@

bench: ref build/tests/output
	./script/bench.sh tests/input/*.tes

test: build/tests/test.xml

build/tests/test.xml:  build/test ${CBIN}
	$< --gtest_output=xml:$@ || true

${Out}:
	mkdir $@

build/tests/output: build
	mkdir -p $@

${Out}/ref.dict: ${RefCompiler} build
	$< assignment/words.txt $@

build/test: tests/test.cc build ${CBIN} external/gtest/lib/libgtest.a
	${CXX} ${CXXFLAGS} ${LDFLAGS} -I external/gtest/include/ -L external/gtest/lib -o $@ $< ${COBJS} -lgtest -lpthread

external/gtest/lib/libgtest.a:
	mkdir external/gtest/lib
	cd external/gtest/lib; 	cmake ..; make

clean:
	echo ${COJBS}
	${RM} -rf build ${CBIN} ${COBJS} ${CDEPS}

.PHONY: ref bench test
.PHONY: build/tests/test.xml

-include ${CDEPS}
