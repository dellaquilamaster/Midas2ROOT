CFLAGS    :=`root-config --cflags --libs` -lGeom -lEve -lGed -lRGL -lEG -lTreePlayer -lMathMore

SrcSuf    := cpp
ObjSuf    := o
ExeSuf    := exe
ObjDir    := obj/
DepSuf    := h
PcmSuf    := _rdict.pcm

PROG      := exec_Midas2ROOT

M2RMIDASROOTEVENT_DICT  := M2RMidasRootEventDict.$(SrcSuf)
M2RMIDASROOTEVENT_DICTH := M2RMidasRootEventDict.$(DepSuf)
M2RMIDASROOTEVENT_DICTO := M2RMidasRootEventDict.$(ObjSuf)
M2RMIDASROOTEVENT_PCM   := M2RMidasRootEventDict$(PcmSuf)
M2RMIDASROOTEVENT_HDRS  := M2RMidasRootEventLinkDef.$(DepSuf)

OBJS      := $(ObjDir)M2RConverter.$(ObjSuf) $(ObjDir)M2RReader.$(ObjSuf) $(ObjDir)M2RMidasRootEvent.$(ObjSuf) $(ObjDir)shared.$(ObjSuf) $(ObjDir)$(M2RMIDASROOTEVENT_DICTO)

DEPS      := $(_OBJS:.$(ObjSuf)=.$(DepSuf))

INCLUDES  := -I./

CXXFLAGS  += $(INCLUDES) -std=c++11 -fPIC -O3

all: $(PROG)

$(PROG): $(OBJS)
		$(CXX) $(CXXFLAGS) -o ${@}.$(ExeSuf) ${@}.$(SrcSuf) $^ $(SYSLIB) $(CFLAGS) $(RLIBS)
		

$(ObjDir)M2RConverter.$(ObjSuf): M2RConverter.$(SrcSuf)  M2RConverter.$(DepSuf)
	$(CXX) $(CXXFLAGS) -c  M2RConverter.$(SrcSuf) -o $(ObjDir)M2RConverter.$(ObjSuf) $(SYSLIB) $(CFLAGS) $(RLIBS)	
	
$(ObjDir)M2RReader.$(ObjSuf): M2RReader.$(SrcSuf)  M2RReader.$(DepSuf)
	$(CXX) $(CXXFLAGS) -c  M2RReader.$(SrcSuf) -o $(ObjDir)M2RReader.$(ObjSuf) $(SYSLIB) $(CFLAGS) $(RLIBS)	
	
$(ObjDir)shared.$(ObjSuf): shared.$(SrcSuf)  shared.$(DepSuf)
	$(CXX) $(CXXFLAGS) -c  shared.$(SrcSuf) -o $(ObjDir)shared.$(ObjSuf) $(SYSLIB) $(CFLAGS) $(RLIBS)		

$(ObjDir)M2RMidasRootEvent.$(ObjSuf):  ./$(SrcDir)M2RMidasRootEvent.$(SrcSuf)  ./$(DepDir)M2RMidasRootEvent.$(DepSuf) ./$(ObjDir)$(M2RMIDASROOTEVENT_DICTO)
	$(CXX) $(CXXFLAGS) -c ./$(SrcDir)M2RMidasRootEvent.$(SrcSuf) -o $(ObjDir)M2RMidasRootEvent.$(ObjSuf) $(SYSLIB) $(CFLAGS) $(RLIBS)
	
$(ObjDir)$(M2RMIDASROOTEVENT_DICTO):  ./$(M2RMIDASROOTEVENT_DICT)
	$(CXX) $(CXXFLAGS) -c ./$(M2RMIDASROOTEVENT_DICT) -o ./$(ObjDir)$(M2RMIDASROOTEVENT_DICTO) $(SYSLIB) $(CFLAGS) $(RLIBS)
	
$(M2RMIDASROOTEVENT_DICT): ./$(DepDir)M2RMidasRootEvent.$(DepSuf) ./$(DepDir)$(M2RMIDASROOTEVENT_HDRS)
	rootcling -f $@ -p $(INCLUDES) $^
	
.PHONY: clean
clean:
	@$(RM) *.$(ExeSuf)
	@$(RM) $(ObjDir)*.$(ObjSuf)
	@$(RM) $(M2RMIDASROOTEVENT_DICT) $(M2RMIDASROOTEVENT_DICTH) $(M2RMIDASROOTEVENT_DICTO) $(M2RMIDASROOTEVENT_PCM)
