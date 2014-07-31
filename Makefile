Out = build

-include Makefile.in

CBIN = TextMiningCompiler
ABIN = TextMiningApp

CSRC = src/dict.cc src/trie.cc src/simpletriebuilder.cc
ASRC = src/dict.cc src/trie.cc

CXXFLAGS = -I include -std=c++11 -fstack-protector -Wno-write-strings -g
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
src/appmain.o: src/search.cc
${ABIN}: ${Out}/${ABIN}
	cp $< $@

depend: ${CDEPS}

%.deps: %.cc
	${CXX} ${CXXFLAGS} -MM -MT ${<:.cc=.o} $< -o $@

bench: ${Out}/my.dict ref build/tests/output
	./script/bench.sh tests/input/*.test

test: build/tests/test.xml

build/tests/test.xml:  ${Out}/my.dict build/test ${CBIN} ${ABIN}
	build/test --gtest_output=xml:$@  || true

${Out}:
	mkdir $@

build/tests/output: build
	mkdir -p $@

${Out}/ref.dict: ${RefCompiler} build
	./$< assignment/words.txt $@

${Out}/my.dict: ${CBIN} build
	./$< assignment/words.txt $@

build/test: tests/test.cc build ${CBIN} ${ABIN} external/gtest/lib/libgtest.a ${AOBJS}
	${CXX} ${CXXFLAGS} ${LDFLAGS} -I external/gtest/include/ -L external/gtest/lib -o $@ $< ${COBJS}  -lgtest -lpthread

external/gtest/lib/libgtest.a:
	mkdir external/gtest/lib
	cd external/gtest/lib; 	cmake ..; make

clean:
	${RM} -rf build ${CBIN} ${COBJS} ${CDEPS} ${ADEPS} ${AOBJS} src/main.o src/appmain.o

.PHONY: ref bench test
.PHONY: build/tests/test.xml

-include ${CDEPS} ${ADEPS}
