Out = build

-include Makefile.in

ref: ${Out}/ref.dict

${Out}/ref.dict: ${RefCompiler}
	$< assignment/words.txt $@

.PHONY: ref
