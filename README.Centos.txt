On the CENTOS 5 machine:

 - on "<root_dir>
 - unpack gcc-4.9.3
 - cd gcc-4.9.3
 - update dependencies:  ./contrib/download_prerequisites
 - create a separate build dir: cd ..; mkdir gcc_build; cd gcc_build
 - configure gcc:
   NB: <install_dir> for destination directory, ex: $HOME/local_gcc-4.9.3/

 ../gcc-4.9.3/configure --prefix=<install_dire> --enable-languages=c,c++,fortran --disable-werror --disable-bootstrap --disable-multilib

 - make
 - make install
 	(NB: make install might need sudo if <install_dir> is in the superuser file space)


 -- INSTALL CMAKE 3.5
   from binaries:
   	- copy bin/* to /usr/bin
  - copy share/cmake-3.5/ to /usr/share

 -- INSTALL PYTHON 3 from sources:
	 yum install yum-utils
	     (yum-builddep python does not work, but does not seem to be needed)
	 curl -O https://www.python.org/ftp/python/3.5.0/Python-3.5.0.tgz
   tar -xvzf Python-3.5.0.tgz
   cd Python-3.5.0
   ./configure
   make
   sudo make install
    -- INSTALL Ply
      - tar -xvzf ply-3.10.tar.gz
      - cd ply-3.10
      - python3 setup.py install

  -- SIT_PARSER:
     copy generated files into SIT_parser directory to bypass flex


---------------------------------
run specifing the Makefile

make -f Makefile_Centos debug

---------------------------------

- If needed, set linking to static:
  target_link_libraries(<target_name> -static)

 NB: if compiled without "-static", set LD_LIBRARY_PATYH before execution:
    export LD_LIBRARY_PATH=<install_dir>/lib64



