Out = build

-include Makefile.in

ref: ${Out}/ref.dict

build:
	mkdir -f $@

${Out}/ref.dict: ${RefCompiler} build
	$< assignment/words.txt $@

.PHONY: ref
