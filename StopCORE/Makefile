ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

#######################################
# -- DEFINE ARCH to something sensible!
#######################################

CXX           = g++
CXXFLAGS      = -g -Wall -fPIC
LD            = g++
LDFLAGS       = -g
SOFLAGS       = -shared


CXXFLAGS      += $(ROOTCFLAGS)
LIBS           = $(ROOTLIBS)

NGLIBS         = $(ROOTGLIBS)
NGLIBS        += -lMinuit
GLIBS          = $(filter-out -lNew, $(NGLIBS))

#COREPATH       = $(HOME)/CORE
COREPATH       = ../CORE
TOOLSPATH      = $(COREPATH)/Tools
DORKYPATH      = $(TOOLSPATH)/dorky


CXXFLAGS       += -I$(COREPATH)
CXXFLAGS       += -I$(TOOLSPATH)
CXXFLAGS       += -I$(DORKYPATH)

CORELIB        = $(COREPATH)/CMS3_CORE.so

TOOLSLIB = ../CORE/Tools/btagsf/BTagCalibrationStandalone.o

# stopCORE.so: sysInfo.o eventWeight_bTagSF.o eventWeight_lepSF.o selectionInfo.o categoryInfo.o genClassyInfo.o sampleInfo.o stop_1l_babyAnalyzer.o
# 	$(LD) $(LDFLAGS) $(SOFLAGS) -o stopCORE.so sysInfo.o eventWeight_bTagSF.o eventWeight_lepSF.o selectionInfo.o categoryInfo.o genClassyInfo.o sampleInfo.o stop_1l_babyAnalyzer.o $(GLIBS) -lGenVector -lEG $(CORELIB) $(STOPVARLIB) -Wl,-rpath,./

stopCORE.so: eventWeight_bTagSF.o eventWeight_lepSF.o stop_1l_babyAnalyzer.o sampleInfo.o
	$(LD) $(LDFLAGS) $(SOFLAGS) -o stopCORE.so eventWeight_bTagSF.o eventWeight_lepSF.o stop_1l_babyAnalyzer.o sampleInfo.o $(GLIBS) -lGenVector -lEG $(TOOLSLIB) -Wl,-rpath,./

sysInfo.o: sysInfo.h sysInfo.cc stop_1l_babyAnalyzer.h stop_1l_babyAnalyzer.cc eventWeight_lepSF.h eventWeight_lepSF.cc eventWeight_bTagSF.h eventWeight_bTagSF.cc
	$(CXX) $(CXXFLAGS) -c sysInfo.cc

%.o: 	%.cc %.h
	$(QUIET) echo "Compiling $<"; \
	$(CXX) $(CXXFLAGS) $< -c -o $@

# STOPVARSOURCE  = $(wildcard stop_variables/*.cc)
# StopVariables.so: $(STOPVARSOURCE) $(STOPVARSOURCE:.cc=.h)
# 	cd stop_variables && $(MAKE) -j4 && cd ..; \
# 	mv stop_variables/StopVariables.so .

.PHONY: clean
clean:
	rm -v -f \
	*.o *.d *.so *.pcm *~ \
	echo "Done"
