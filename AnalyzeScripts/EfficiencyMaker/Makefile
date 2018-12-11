ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

#######################################
# -- DEFINE ARCH to something sensible!
#######################################

#
CXX           = g++
CXXFLAGS      = -g -Wall -fPIC -O3
LD            = g++
LDFLAGS       = -g -O3
SOFLAGS       = -shared


CXXFLAGS      += $(ROOTCFLAGS)
LIBS           = $(ROOTLIBS)

NGLIBS         = $(ROOTGLIBS)
NGLIBS        += -lMinuit
GLIBS          = $(filter-out -lNew, $(NGLIBS))

# COREPATH       = $(HOME)/CORE
COREPATH       = ../../CORE

CXXFLAGS       += -I$(COREPATH)

CORELIB        = $(COREPATH)/CMS3_CORE.so

# runBTagEffMaker: runBTagEffMaker.o looper.o  LinkDef_out.so
# 	$(LD) $(LDFLAGS)  -o runBTagEffMaker runBTagEffMaker.o looper.o $(GLIBS) -lGenVector -lEG $(CORELIB) LinkDef_out.so -Wl,-rpath,./

# runBTagEffMaker.o: runBTagEffMaker.C looper.h
# 	$(CXX) $(CXXFLAGS) -c runBTagEffMaker.C

# looper.o: looper.C looper.h
# 	$(CXX) $(CXXFLAGS) -c looper.C

# LinkDef_out.so: LinkDef.h
# 	@rootcling -v -f LinkDef.cc -c -p Math/Vector4D.h LinkDef.h
# 	$(CXX) $(CXXFLAGS) $(SOFLAGS) LinkDef.cc -o $@

runLeptonEffMaker: makeLeptonEfficiencies.o $(CORELIB)
	$(LD) $(LDFLAGS) -o runLeptonEffMaker makeLeptonEfficiencies.o $(CORELIB) $(GLIBS) -lGenVector -lEG -Wl,-rpath,./

makeLeptonEfficiencies.o: makeLeptonEfficiencies.cc
	$(CXX) $(CXXFLAGS) -c makeLeptonEfficiencies.cc

.PHONY: clean
clean:
	rm -v -f \
	LinkDef.cc \
	LinkDef_rdict.pcm \
	*.o *.d *.so; echo "Done"
