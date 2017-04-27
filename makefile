SRC_FILES = predictors.c makefile README.md statistics.txt members.txt problem_2.xlsx

predictors :predictors.c
	gcc -Wall -g -o predictors predictors.c

gdb : predictors
	gdb -x gdbcmds.txt predictors

clean :
	-rm predictors

submit : $(SRC_FILES)
	mkdir rrolsto1-sbailey6-pr1
	cp $(SRC_FILES) rrolsto1-sbailey6-pr1
	tar -zcvf rrolsto1-sbailey6-pr1.tar.gz rrolsto1-sbailey6-pr1
	#mv rolsto1.tar.gz rrolsto1.tgz
	rm -r rrolsto1-sbailey6-pr1
