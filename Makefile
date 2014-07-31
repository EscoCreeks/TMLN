Out = build

-include Makefile.in

CBIN = TextMiningCompiler
ABIN = TextMiningApp

CSRC = src/dict.cc src/trie.cc src/simpletriebuilder.cc src/locklesstriebuilder.cc src/lockedtrie.cc src/tbbparalleltriebuilder.cc
ASRC = src/dict.cc src/trie.cc src/search.cc

CXXFLAGS = -I include -std=c++11 -fstack-protector -g
LDFLAGS = -ltbb

COBJS = ${CSRC:.cc=.o}
AOBJS = ${ASRC:.cc=.o}
CDEPS = ${CSRC:.cc=.deps}
ADEPS = ${ASRC:.cc=.deps}

all:  ${Out} ${CBIN} ${ABIN}

ref: ${Out}/ref.dict

${Out}/${CBIN}: depend ${COBJS} src/main.o
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o $@ ${COBJS} src/main.o

${CBIN}: ${Out}/${CBIN}
	cp $< $@

${Out}/${ABIN}: ${AOBJS} src/appmain.o
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o $@ ${AOBJS} src/appmain.o

${ABIN}: ${Out}/${ABIN}
	cp $< $@

depend: ${CDEPS}

%.deps: %.cc
	${CXX} ${CXXFLAGS} -MM -MT ${<:.cc=.o} $< -o $@

bench: ${Out}/my.dict ref build/tests/output
	./script/bench.sh tests/input/*.test

test: build/tests/test.xml

build/tests/test.xml:  build/test ${CBIN} ${ABIN}
	$< --gtest_output=xml:$@  || true

${Out}:
	mkdir $@

build/tests/output: build
	mkdir -p $@

${Out}/ref.dict: ${RefCompiler} build
	./$< assignment/words.txt $@

${Out}/my.dict: ${CBIN} build
	./$< assignment/words.txt $@

build/test: tests/test.cc build ${CBIN} ${ABIN} external/gtest/lib/libgtest.a ${AOBJS}
	${CXX} ${CXXFLAGS} ${LDFLAGS} -I external/gtest/include/ -L external/gtest/lib -o $@ $< ${COBJS} src/search.o  -lgtest -lpthread

external/gtest/lib/libgtest.a:
	mkdir external/gtest/lib
	cd external/gtest/lib; 	cmake ..; make

clean:
	${RM} -rf build ${CBIN} ${COBJS} ${CDEPS} ${ADEPS} ${AOBJS} src/main.o src/appmain.o

.PHONY: ref bench test
.PHONY: build/tests/test.xml

-include ${CDEPS} ${ADEPS}
