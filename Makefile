# test compiler version and complain if not recent enough:
GCCOK := $(shell $(SFRAME_DIR)/SFrameTools/gccok.sh)
ifneq ($(GCCOK),yes)
   $(error "Your compiler is too old; required is gcc version 46x or higher.")
endif

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
INCLUDES += -I/afs/cern.ch/sw/lcg/external/MCGenerators/lhapdf/5.8.8/x86_64-slc5-gcc46-opt/include

# Include the generic compilation rules
include $(SFRAME_DIR)/Makefile.common
