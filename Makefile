# Package information
LIBRARY = Analysis
OBJDIR  = obj
DEPDIR  = $(OBJDIR)/dep
SRCDIR  = src
INCDIR  = include
INCLUDES += -I$(SFRAME_DIR)/SFrameTools/include

# Include the generic compilation rules
include $(SFRAME_DIR)/Makefile.common

# configure FastJet
FASTJETDIR = /afs/naf.desy.de/user/p/peiffer/fastjet-3.0.2/install
INCLUDES += -I$(FASTJETDIR)/include
