# Package information
LIBRARY = Analysis
OBJDIR  = obj
DEPDIR  = $(OBJDIR)/dep
SRCDIR  = src
INCDIR  = include



USERCXXFLAGS := -g

#INCLUDES += -I$(LHAPDFDIR)/include
#INCLUDES += -I/nfs/dust/cms/user/marchesi/LHAPDF/install/include/
INCLUDES += -I/afs/desy.de/user/m/mameyer/xxl-af-cms/lhapdf/include

# Include the generic compilation rules
include $(SFRAME_DIR)/Makefile.common

include $(SFRAME_DIR)/SFrameTools/Makefile.defs