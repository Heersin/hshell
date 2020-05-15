INCLUDE= -I ./include
INC_DIR= ./include/

hsh: hsh.o utils.o execute.o script.o builtin.o process.o varlib.o history.o pipe.o redirect.o autocomplete.o
	gcc -o hsh -g $(INCLUDE)  hsh.o utils.o execute.o script.o process.o builtin.o varlib.o history.o pipe.o redirect.o autocomplete.o

utils.o: $(INC_DIR)utils.h $(INC_DIR)history.h 
	gcc -c -g $(INCLUDE) utils.c

history.o: $(INC_DIR)history.h 
	gcc -c -g $(INCLUDE) history.c

varlib.o: $(INC_DIR)varlib.h
	gcc -c -g $(INCLUDE) varlib.c

execute.o: $(INC_DIR)execute.h $(INC_DIR)varlib.h
	gcc -c -g $(INCLUDE) execute.c

script.o: $(INC_DIR)utils.h $(INC_DIR)script.h $(INC_DIR)varlib.h
	gcc -c -g $(INCLUDE) script.c

builtin.o: $(INC_DIR)builtin.h $(INC_DIR)varlib.h
	gcc -c -g $(INCLUDE) builtin.c

process.o: $(INC_DIR)process.h $(INC_DIR)varlib.h $(INC_DIR)builtin.h $(INC_DIR)utils.h $(INC_DIR)execute.h $(INC_DIR)redirect.h $(INC_DIR)pipe.h
	gcc -c -g $(INCLUDE) process.c

pipe.o: $(INC_DIR)pipe.h $(INC_DIR)varlib.h 
	gcc -c -g $(INCLUDE) pipe.c

redirect.o: $(INC_DIR)redirect.h $(INC_DIR)execute.h 
	gcc -c -g $(INCLUDE) redirect.c

autocomplete.o: $(INC_DIR)autocomplete.h 
	gcc -c -g $(INCLUDE) autocomplete.c

hsh.o: hsh.c include/hsh.h include/varlib.h include/utils.h include/process.h include/script.h include/builtin.h include/execute.h include/history.h include/pipe.h include/redirect.h include/autocomplete.h
	gcc -c -g $(INCLUDE) hsh.c

clean:
	rm -rf *.o