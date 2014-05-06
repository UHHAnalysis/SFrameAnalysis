# Package information
LIBRARY = Analysis
OBJDIR  = obj
DEPDIR  = $(OBJDIR)/dep
SRCDIR  = src
INCDIR  = include



USERCXXFLAGS := -g

#INCLUDES += -I$(LHAPDFDIR)/include
INCLUDES += -I/nfs/dust/cms/user/marchesi/LHAPDF/install/include/


include $(SFRAME_DIR)/SFrameTools/Makefile.defs