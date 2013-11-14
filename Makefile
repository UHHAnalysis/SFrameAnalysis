# test compiler version and complain if not recent enough:

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

#INCLUDES += -I$(SFRAME_DIR)/SFrameTools
#INCLUDES += -I$(SFRAME_DIR)/core
#INCLUDES += -I$(SFRAME_DIR)/core/include
#INCLUDES += -I$(SFRAME_DIR)/include
#INCLUDES += -I$(SFRAME_DIR)/NtupleWriter
#INCLUDES += -I$(SFRAME_DIR)/NtupleWriter/include

USERCXXFLAGS := -g -std=c++0x

#INCLUDES += -I$(LHAPDFDIR)/include
INCLUDES += -I/afs/naf.desy.de/user/m/mameyer/lhapdf/include/

# Include the generic compilation rules
include $(SFRAME_DIR)/Makefile.common
