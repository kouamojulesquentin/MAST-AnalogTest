-) untar MAST--ATE_Server
-) build xmlrpc locally: 
         - ./configure  --prefix=$(pwd)/../xmlrpc-c/build
        - make
        - make install
   NB: the syntax $(pwd) might not work, replace with absolute path if neededexit
   NB may need a chmod a+x for ./configure and ./install-sh   
   
In SmarTest:
   
File->Import
   General->File System
   
From directory: ATE_Server
	In the hierarchy, select	
	 ATE_Server->External_Libs->xmlrpc-c->build
 		all subdirs
	 ATE_Server->Mast_Headers
	 ATE_Server->Optional_Libs
	 		->MastXmlRpc
			
	 
	
Into folder:mast_dev_tml	
 
 "External_Libs", "Mast_Headers" and "Optional_libs" folder should appear in Project Explorer

In Project Explorer: 
   rigth-click->Properties
   	GCC C++ compiler -> Include
	Add from workspace "Mast_Headers"
	and External_Libs->xmlrpc-c->build->include
	and Optional_Libs->MastXmlRpc

For the headers: Import
	 ATE_Server->Optional_Libs->ATE_Server
	 	NB: deselect ATE_Loppback_server
				JTAG_Example*.cpp
	

		
---------------------------------------------
---LIBCURL-----------------------------------
---------------------------------------------

If you need to locally install CURL, xmlprc-c this option must be added to xmlrpc-c configure command: 
	--enable-curl-client=<path to local install>
 
