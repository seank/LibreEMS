# Top level convenience make file

# Detect windows, set variables to handle platform differences
ifdef ComSpec
VAR_SET = SET
SPECIAL_QUOTE = "
PATHSEP = \\
DESCEND = cd src\\main
else
VAR_SET = export
SPECIAL_QUOTE = '
PATHSEP = /
DESCEND = cd src/main
endif


Help:
	@echo
	@echo "######################## Welcome to FreeEMS! ########################"
	@echo
	@echo "To build, choose a target from the below list and type 'make target'!"
	@echo "For example, to build blank unconfigured firmware type 'make BLANK'"
	@echo "Note: For BenchTest to work you MUST build using the BENCHTEST target!"
	@echo "Advanced users can find more options available in the src/main directory!"
	@echo
	@echo "Options:"
	@echo
	@make -qp | awk -F':' $(SPECIAL_QUOTE)/^[a-zA-Z0-9][^$$#\/\t=]*:([^=]|$$)/ {split($$1,A,/ /);for(i in A)print A[i]}$(SPECIAL_QUOTE)

TEST_AND_SAVE_ALL:
	$(DESCEND) && ..$(PATHSEP)..$(PATHSEP)bin$(PATHSEP)testclibuilds.bash

BLANK:
	$(DESCEND) && make clean s19

SPECIAL: # Can use a dependency here instead. 
	$(DESCEND) && make clean EvenTeeth-Distributor-4of6and1

# Other special builds
BENCHTEST: 
	$(VAR_SET) CLIFLAGS="DEFAULT" && $(DESCEND) && make clean BenchTest 
TRUCK: 
	$(VAR_SET) CLIFLAGS="$@" && $(DESCEND) && make clean MitsiAndMazda-CAS-4and1
HOTEL: 
	$(VAR_SET) CLIFLAGS="$@" && $(DESCEND) && make clean HallOrOptical-Distributor-4of69
PRESTO: 
	$(VAR_SET) CLIFLAGS="$@" && $(DESCEND) && make clean EvenTeeth-Cam-24and1
SEANKLT1: 
	$(VAR_SET) CLIFLAGS="$@" && $(DESCEND) && make clean GM-LT1-CAS-360and8
SEANKR1: 
	$(VAR_SET) CLIFLAGS="$@" && $(DESCEND) && make clean SEANKR1
SNOTROCKET: 
	$(VAR_SET) CLIFLAGS="$@" && $(DESCEND) && make clean EvenTeeth-Cam-24and1
SPUDMN: 
	$(VAR_SET) CLIFLAGS="$@" && $(DESCEND) && make clean MissingTeeth-Crank-8minus1
SLATER: 
	$(VAR_SET) CLIFLAGS="$@" && $(DESCEND) && make clean MissingTeeth-Crank-12minus1
PETERJSERIES: 
	$(VAR_SET) CLIFLAGS="$@" && $(DESCEND) && make clean JSeries-12CrankWith6-2Cam
DEUCECOUPE: 
	$(VAR_SET) CLIFLAGS="$@" && $(DESCEND) && make clean EvenTeeth-Cam-6and1
DEUCES10: 
	$(VAR_SET) CLIFLAGS="$@" && $(DESCEND) && make clean EvenTeeth-Cam-4and1
PETERTRUCK: 
	$(VAR_SET) CLIFLAGS="$@" && $(DESCEND) && make clean MissingTeeth-Cam-12minus1
SCAVENGER: 
	$(VAR_SET) CLIFLAGS="$@" && $(DESCEND) && make clean MitsiAndMazda-CAS-4and2

