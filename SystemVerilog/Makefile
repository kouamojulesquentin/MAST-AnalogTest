#VCS configuration
CPP=g++
CPP_FLAGS=-w  -pipe -O -fPIC -std=c++14
VCS_INCLUDE=-I/softslin/vcs_mx_vH2013_06_sp1_64b/include

CC_OPTION= $(CPP_FLAGS)
CPP_OPTION=$(CPP_FLAGS) 


#MAST configuration
OBJ_DIR=./Objs
EXE_DIR=./Bin
OPT_DIR=../Optional_Libs
RPC_DIR=$(OPT_DIR)/MastRpc
XMLRPC_DIR=$(OPT_DIR)/MastXmlRpc/
MAST_H=../Mast_Core/include/
XMLRPC_INSTALL=../External_Libs/xmlrpc-c/build
XMLRPC_H=$(XMLRPC_INSTALL)/include
XMLRPC_LIB=$(XMLRPC_INSTALL)/lib

SV_LB_OBJS=$(OBJ_DIR)/Remote_Loopback_Protocol.o \
            $(OBJ_DIR)/XmlRpc_Protocol_Server.o \
	    $(OBJ_DIR)/XmlRpc_Protocol_Client.o \
	    $(OBJ_DIR)/SV_Loopback_Server.o

SV_OBJS=$(OBJ_DIR)/Remote_Loopback_Protocol.o \
            $(OBJ_DIR)/XmlRpc_Protocol_Server.o \
	    $(OBJ_DIR)/XmlRpc_Protocol_Client.o \
	    $(OBJ_DIR)/DPI_Protocol.o \
	    $(OBJ_DIR)/DPI_interface.o \
	    $(OBJ_DIR)/SV_Server.o

INCLUDE_LIBS= -lxmlrpc -lxmlrpc++ -lxmlrpc_abyss -lxmlrpc_abyss++ -lxmlrpc_client++ -lxmlrpc_client -lxmlrpc_cpp -lxmlrpc_packetsocket -lxmlrpc_server++ -lxmlrpc_server -lxmlrpc_server_abyss++ -lxmlrpc_server_abyss -lxmlrpc_server_cgi++ -lxmlrpc_server_cgi -lxmlrpc_server_pstream++ -lxmlrpc_util++ -lxmlrpc_util -lxmlrpc_xmlparse -lxmlrpc_xmltok 


SV_DIR=$(OPT_DIR)/SV_Server

INCLUDES=-I$(RPC_DIR) -I$(XMLRPC_DIR) -I$(MAST_H)

all: SV_compile SV_Server 

test: SV_test_compile SV_Loopback_Server SV_Server 
	$(CPP) $(CPP_OPTION) -o $(EXE_DIR)/SV_Loopback_Server $(INCLUDE_LIBS) -L$(XMLRPC_LIB) $(INCLUDES) $(SV_LB_OBJS)
	

SV_Loopback_Server: XmlRpC RpC DPI_RpC $(SV_DIR)/SV_Loopback_Server.cpp
	$(CPP) $(CPP_OPTION) -c -o $(OBJ_DIR)/SV_Loopback_Server.o $(INCLUDES) $(SV_DIR)/SV_Loopback_Server.cpp

SV_Server: XmlRpC RpC DPI_RpC $(SV_DIR)/SV_Server.cpp
	$(CPP) $(CC_OPTION) $(INCLUDES) $(VCS_INCLUDE) -c -o $(OBJ_DIR)/SV_Server.o $(SV_DIR)/SV_Server.cpp 
	$(CPP)  -shared -fPIC -o $(OBJ_DIR)/DPI_SV_Server.so $(SV_OBJS)
	
XmlRpC: $(XMLRPC_DIR)/XmlRpc_Protocol_Client.cpp $(XMLRPC_DIR)/XmlRpc_Protocol_Server.cpp
	$(CPP) $(CPP_OPTION) -c -o $(OBJ_DIR)/XmlRpc_Protocol_Client.o -I$(XMLRPC_H) $(INCLUDES) $(XMLRPC_DIR)/XmlRpc_Protocol_Client.cpp
	$(CPP) $(CPP_OPTION) -c -o $(OBJ_DIR)/XmlRpc_Protocol_Server.o -I$(XMLRPC_H) $(INCLUDES) $(XMLRPC_DIR)/XmlRpc_Protocol_Server.cpp

RpC: $(RPC_DIR)/Remote_Loopback_Protocol.cpp
	$(CPP) $(CPP_OPTION) -c -o $(OBJ_DIR)/Remote_Loopback_Protocol.o -I$(XMLRPC_H) $(INCLUDES) $(RPC_DIR)/Remote_Loopback_Protocol.cpp

DPI_RpC: DPI_interface	$(SV_DIR)/DPI_Protocol.cpp 
	$(CPP) $(CPP_OPTION) -c -o $(OBJ_DIR)/DPI_Protocol.o -I$(XMLRPC_H) $(VCS_INCLUDE) $(INCLUDES) $(SV_DIR)/DPI_Protocol.cpp

DPI_interface: $(SV_DIR)/DPI_interface.cpp
	$(CPP) $(CPP_OPTION) -c -o $(OBJ_DIR)/DPI_interface.o -I$(XMLRPC_H) $(VCS_INCLUDE) $(INCLUDES) $(SV_DIR)/DPI_interface.cpp

SV_test_compile: RPC_test.sv
	vhdlan -f RTL_filelist.txt -work work
	vlogan -sverilog  RPC_test.sv 
	vcs  -debug_all work.top -l comp.log	
# vcs  -debug_all -sverilog RPC_test.sv -l comp.log

SV_compile: RPC_server.sv
	vhdlan -f RTL_filelist.txt -work work
	vlogan -sverilog  RPC_server.sv 
	vcs  -debug_all work.top -l comp.log	
# vcs  -debug_all -sverilog RPC_test.sv -l comp.log

run_test:
	./simv -sv_liblist DPI_bootstrap.txt -gui

run:
	./simv -sv_liblist DPI_bootstrap.txt -gui
	
compile:  test.cpp
	$(CPP) $(CPP_FLAGS) $(VCS_INCLUDE) -c test.cpp -o $(OBJ_DIR)/test.o

rtl: test.sv compile
	vcs test.sv $(OBJ_DIR)/test.o -sverilog -l comp.log
	
run_rtl: rtl
	./simv -l run.log

clean:
	rm -rf simv* csrc *.log vc_hdrs.h $(OBJ_DIR)/*.o $(OBJ_DIR)/*.so DVEfiles dve_report.* inter.vpd ucli.key 64 AN.DB
