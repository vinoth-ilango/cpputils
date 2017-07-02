# makefile basics
# target: dependencies
# 	command
# $@ - target
# $
CC = g++ -Wall
CCFLAGs  = --std=c++11
INCLUDES = -I./

PROGRAM = ./bin/test
SRCDIR  = ./
OBJDIR  = ./obj
CPPLIB  = ./bin/cpputils.a

LIBS    = -pthread
LIBPATH = -L/usr/lib/x86_64-linux-gnu

OBJS = \
    $(OBJDIR)/ConfigReader.o \
    $(OBJDIR)/Ini.o \
    $(OBJDIR)/Logger.o \
    $(OBJDIR)/ObjectPool.o \
    $(OBJDIR)/ProcessEngine.o \
    $(OBJDIR)/Utils.o \
    $(OBJDIR)/xmlparser.o \
    $(OBJDIR)/TimeUtil.o \

all: mkdirectories $(CPPLIB) $(PROGRAM)

mkdirectories:
	@mkdir -p obj
	@mkdir -p bin

$(CPPLIB):$(OBJS)
	@echo creating $@ from $(OBJS)
	@ar -r $@ $(OBJS)

$(PROGRAM):$(OBJDIR)/main.o
	@echo creating $@ binary
	@$(CC) $(CCFLAGs) $(LIBPATH) $(LIBS) $(OBJDIR)/main.o $(CPPLIB) -o $@

$(OBJDIR)/%.o:$(SRCDIR)/%.cpp
	@echo compiling $<...
	@$(CC) $(CCFLAGs) $(INCLUDES) -c $< -o $@

clean:
	@echo removing objs and binary files
	@rm -f $(OBJDIR)/*.o 
	@rm -f $(PROGRAM) $(CPPLIB)

