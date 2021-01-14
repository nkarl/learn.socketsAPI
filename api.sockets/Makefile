##################################################################
## MAKEFILE                                                     ##
##################################################################
vpath %hpp hdr
vpath %cpp src

HDIR		:= hdr
SDIR		:= src
ODIR		:= obj

EXET		:= a
OBJS		:= main.o

OSRC		:= $(addprefix $(ODIR)/, $(OBJS))

CXX		:= g++
CXXFLAGS	:= -I$(HDIR) -I$(TMPL) -g -Wall -std=c++17 \
			-Wextra -Wconversion -pedantic

##################################################################
## BUILD DIRECTIVE                                              ##
##################################################################
all : $(EXET)

$(EXET) : $(OSRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ 

$(ODIR)/%.o : $(SDIR)/%.cpp | $(ODIR)
	$(CXX) -c $(CXXFLAGS) $^ -o $@

$(ODIR):
	@if [ ! -d "$(ODIR)" ]; then mkdir $(ODIR); fi

##################################################################
## CLEAN TASK                                                   ##
##################################################################
.PHONY: clean
clean:
	rm -r $(EXET) $(ODIR)/*
