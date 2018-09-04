ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

#######################################
# -- DEFINE ARCH to something sensible!
#######################################

#
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

COREPATH       = ../CORE
TOOLSPATH      = $(COREPATH)/Tools

CXXFLAGS       += -I$(COREPATH)
CXXFLAGS       += -I$(TOOLSPATH)
CXXFLAGS       += -I./
CXXFLAGS       += -I./TFTopTagger

CORESOURCE     = $(COREPATH)/CMS3_CORE.so
CORELIB        = ./CMS3_CORE.so

STOPVARSOURCE  = $(wildcard stop_variables/*.cc)
STOPVARLIB     = StopVariables.so

TENSORFLOW_DIR = /cvmfs/cms.cern.ch/slc6_amd64_gcc700/external/tensorflow-c/1.1.0-mmelna
TENSORFLOWLIBS = -ltensorflow

ifdef TENSORFLOW_DIR
	CXXFLAGS += -I$(TENSORFLOW_DIR)/include

	CXXFLAGS += -DDOTENSORFLOW
	LIBS     += -L$(TENSORFLOW_DIR)/lib $(TENSORFLOWLIBS)
endif

#
# Make rules
#

runBabyMaker: runBabyMaker.o looper.o EventTree.o LeptonTree.o JetTree.o ResolvedTopMVA.o IsoTracksTree.o TauTree.o GenParticleTree.o PhotonTree.o StopSelections.o LinkDef_out.so $(CORELIB) $(STOPVARLIB) libTopTagger.so eventWeight_lepSF.o
	$(LD) $(LDFLAGS) -o runBabyMaker runBabyMaker.o looper.o EventTree.o ResolvedTopMVA.o JetTree.o LeptonTree.o eventWeight_lepSF.o PhotonTree.o IsoTracksTree.o TauTree.o GenParticleTree.o StopSelections.o libTopTagger.so $(LIBS) $(GLIBS) -lGenVector -lEG -lTMVA $(CORELIB) $(STOPVARLIB) LinkDef_out.so -Wl,-rpath,./

$(CORESOURCE):
	cd $(COREPATH) && $(MAKE) -j && cd ..

$(CORELIB): $(CORESOURCE)
	cp $(CORESOURCE) .

$(STOPVARLIB): $(STOPVARSOURCE) $(STOPVARSOURCE:.cc=.h)
	cd stop_variables && $(MAKE) -j4 && cd ..; \
	mv stop_variables/StopVariables.so .

runBabyMaker.o: runBabyMaker.cc looper.h
	$(CXX) $(CXXFLAGS) -c runBabyMaker.cc

looper.o: looper.cc looper.h EventTree.h TauTree.h IsoTracksTree.h GenParticleTree.h LeptonTree.h PhotonTree.h StopSelections.h
	$(CXX) $(CXXFLAGS) -c looper.cc

EventTree.o: EventTree.cc EventTree.h StopSelections.h
	$(CXX) $(CXXFLAGS) -c EventTree.cc

LeptonTree.o: LeptonTree.cc LeptonTree.h StopSelections.h
	$(CXX) $(CXXFLAGS) -c LeptonTree.cc

eventWeight_lepSF.o: ../StopCORE/eventWeight_lepSF.cc ../StopCORE/eventWeight_lepSF.h
	cd ../StopCORE/ && $(MAKE) -j4 && cd -; \
	cp ../StopCORE/eventWeight_lepSF.o  .

PhotonTree.o: PhotonTree.cc PhotonTree.h StopSelections.h
	$(CXX) $(CXXFLAGS) -c PhotonTree.cc

JetTree.o: JetTree.cc JetTree.h StopSelections.h
	$(CXX) $(CXXFLAGS) -c JetTree.cc

ResolvedTopMVA.o: ResTopTagger/ResolvedTopMVA.h ResTopTagger/ResolvedTopMVA.cc ResTopTagger/TMVAReader.h
	$(CXX) $(CXXFLAGS) -c ResTopTagger/ResolvedTopMVA.cc

TauTree.o: TauTree.cc TauTree.h
	$(CXX) $(CXXFLAGS) -c TauTree.cc

IsoTracksTree.o: IsoTracksTree.cc IsoTracksTree.h
	$(CXX) $(CXXFLAGS) -c IsoTracksTree.cc

GenParticleTree.o: GenParticleTree.cc GenParticleTree.h
	$(CXX) $(CXXFLAGS) -c GenParticleTree.cc

StopSelections.o: StopSelections.cc StopSelections.h
	$(CXX) $(CXXFLAGS) -c StopSelections.cc

LinkDef_out.so: LinkDef.h
	@rootcling -v -f LinkDef.cc -c -p Math/Vector4D.h LinkDef.h
	$(CXX) $(CXXFLAGS) $(SOFLAGS) LinkDef.cc -o $@

libTopTagger.so:
	cp TFTopTagger/libTopTagger.so .

.PHONY: clean
clean:
	rm -v -f \
	runBabyMaker \
	LinkDef.cc \
	LinkDef_rdict.pcm \
	../StopCORE/*.o \
	*.o *.d *.so
