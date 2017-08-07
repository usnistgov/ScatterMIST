#//******************************************************************************
#//** MIST: Modeled Integrated Scatter Tool 
#//**
#//** File: Makefile
#//**
#//** Thomas A. Germer
#//** Sensor Science Division, National Institute of Standards and Technology
#//** 100 Bureau Dr. Stop 8443; Gaithersburg, MD 20899-8443
#//** Phone: (301) 975-2876
#//** Email: thomas.germer@nist.gov
#//**
#//** Version: 4.00 (January 2015)
#//**
#//******************************************************************************

#
# This is the Makefile for a command-line version of MIST
#

SPECIALUSER = germer
ifeq ($(USERNAME),$(SPECIALUSER)) 
SCATMECHDIR = /cygdrive/c/programming/scatmech
SCATPROGDIR = /cygdrive/c/programming/scatprog
INCLUDE=-I $(SCATMECHDIR) -I $(SCATPROGDIR) 
SCATPROG = $(SCATPROGDIR)/scatprog.a
SCATMECH = $(SCATMECHDIR)/scatmech.a
LIBRARIES = $(SCATPROG) $(SCATMECH) -lstdc++
OPTIONS = -O3 -mtune=pentium4 -DGERMER_MODELS
#LIBRARIES = $(SCATPROG) $(SCATMECH) -lstdc++ -lgomp
else
SCATMECHDIR = /cygdrive/c/programming/scatmech
SCATPROG =
INCLUDE=-I $(SCATMECHDIR)
SCATMECH = $(SCATMECHDIR)/scatmech.a
LIBRARIES = $(SCATMECH) -lstdc++
OPTIONS = -O3 -mtune=pentium4 
endif




COMPILE = gcc $(OPTIONS) $(INCLUDE) -c
LINK = gcc $(OPTIONS) 

mist.o : mist.cpp mist.h mist0.h 
	 $(COMPILE) mist.cpp

shapes.o : shapes.cpp shapes.h mist.h mist0.h 
	 $(COMPILE) shapes.cpp

loops.o : loops.cpp loops.h mist.h mist0.h 
	 $(COMPILE) loops.cpp

evaluate.o : evaluate.cpp evaluate.h
	 $(COMPILE) evaluate.cpp

mist0.exe : mist.o evaluate.o loops.o shapes.o $(SCATPROG) $(SCATMECH)
	$(LINK) -o mist0.exe mist.o evaluate.o loops.o shapes.o $(LIBRARIES)
ifeq ($(USERNAME),$(SPECIALUSER)) 
	cp mist0.exe /z/Projects/MIST/bin/mist0.exe
endif

clean:
	rm evaluate.o mist.o loops.o shapes.o

all: 
	make -C $(SCATMECHDIR) scatmech.a
ifeq ($(USERNAME),$(SPECIALUSER)) 
	make -C $(SCATPROGDIR) scatprog.a
endif
	make mist0.exe

