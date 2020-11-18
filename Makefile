CC:=g++
ifneq (,$(findstring Darwin,$(shell uname)))
        exist = $(shell if [ -e '/usr/local/bin/g++-10' ]; then echo "exist"; else echo "notexist"; fi;)
        ifeq ($(exist),exist)
                CC:=g++-10
        else
                exist = $(shell if [ -e '/usr/local/bin/g++-9' ]; then echo "exist"; else echo "notexist"; fi;)
                ifeq ($(exist),exist)
                        CC:=g++-9
                else
                        CC:=g++-8
                endif
        endif
endif
OMPFLG=-fopenmp
BUILDFLG=-w -ffunction-sections -fdata-sections -fmodulo-sched -msse
HASHFLG=-Wno-deprecated
EXE_CMF=bin/MS-comp-func
EXE_PC=bin/MS-get-pc

all:
	$(CC) -o $(EXE_CMF) src/comp_sam_func.cpp $(HASHFLG) $(BUILDFLG) $(OMPFLG)
	$(CC) -o $(EXE_PC) src/get_pc.cpp $(HASHFLG) $(OMPFLG) -O3

clean:
	rm -rf bin/* src/*.o
