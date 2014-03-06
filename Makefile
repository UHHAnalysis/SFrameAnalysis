# Package information
LIBRARY = Analysis
OBJDIR  = obj
DEPDIR  = $(OBJDIR)/dep
SRCDIR  = src
INCDIR  = include

USERLDFLAGS += $(shell root-config --libs) -lGenVector

# Include definitions
include $(SFRAME_DIR)/SFrameTools/Makefile.defs
