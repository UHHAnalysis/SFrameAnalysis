# Package information
LIBRARY = Analysis
OBJDIR  = obj
DEPDIR  = $(OBJDIR)/dep
SRCDIR  = src
INCDIR  = include
#INCLUDES += -I$(SFRAME_DIR)/SFrameTools/include
#INCLUDES += -I$(SFRAME_DIR)/SFrameTools/JetMETObjects/interface

# configure FastJet
#INCLUDES += -I$(FASTJETDIR)/include
INCLUDES += -I$(FASTJETDIR)/../include

USERCXXFLAGS := -g

#INCLUDES += -I$(LHAPDFDIR)/include
INCLUDES += -I/afs/naf.desy.de/user/m/mameyer/lhapdf/include

# Include the generic compilation rules
include $(SFRAME_DIR)/Makefile.common
