#!/bin/bash
rm -rf debug GeneratedFiles
make -f Makefile_sbase
make -f Makefile_sbase clean
make -f Makefile_escprint
make -f Makefile_escprint clean
make -f Makefile_mdb
make -f Makefile_mdb clean
make -f Makefile_mysql
make -f Makefile_mysql clean
make -f Makefile_oracle
make -f Makefile_oracle clean
make -f Makefile_zip
make -f Makefile_zip clean
make -f Makefile_rawsock
make -f Makefile_rawsock clean
make -f Makefile_qtgui
rm -rf debug GeneratedFiles
