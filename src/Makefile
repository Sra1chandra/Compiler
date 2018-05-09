bcc:	parser.tab.c lex.yy.c node.h
	g++ -w -g node.cpp  parser.tab.c lex.yy.c  `llvm-config-3.8 --cxxflags --cppflags mcjit native --ldflags --system-libs --libs core` -lfl -o bcc 
parser.tab.c: parser.y
	bison -d parser.y
parser.tab.h: parser.y
	bison -d parser.y
lex.yy.c: scanner.l parser.tab.h
	flex scanner.l

.PHONY: clean
clean:
	-@rm -rf parser.tab.c parser.tab.h lex.yy.c bcc 2>/dev/null || true
