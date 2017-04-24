SRC_FILES = predictors.c makefile README.txt

predictors :predictors.c
	gcc -Wall -g -o predictors predictors.c

gdb : predictors
	gdb -x gdbcmds.txt predictors

clean :
	-rm predictors

submit : $(SRC_FILES)
	mkdir rrolsto1
	cp $(SRC_FILES) rrolsto1
	tar -czvf rrolsto1.tar.gz rrolsto1
	#mv rolsto1.tar.gz rrolsto1.tgz
	rm -r rrolsto1
