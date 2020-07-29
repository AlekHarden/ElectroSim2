# Usage:
# make			# compile all binaries
# make clean 	# remove all binaries and objects

CC = g++
LINKERFLAGS = -Iinclude

PROJECT = ElectroSim

SRCDIR = src
OBJDIR = object
ifeq ($(shell uname), Linux)
LIBS = -lpthread -lglfw -lGLEW -lGL
else
LIBS = -lglfw3dll -lglew32s -lopengl32
endif

SRCS = $(shell find $(SRCDIR) -name '*.cpp')
SRCDIRS = $(shell find . -name '*.cpp' | dirname {} | sort | uniq | sed 's/\/$(SRCDIR)//g' )
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

$(PROJECT): buildrepo $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(LINKERFLAGS) -c $< -o $@

clean:
	rm $(PROJECT) $(OBJDIR)/*.o -Rf

buildrepo:
	@$(call make-repo)

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef
