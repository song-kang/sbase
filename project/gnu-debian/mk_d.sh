#!/bin/bash
rm -rf debug GeneratedFiles
make -f Makefile_sbase debug
make -f Makefile_sbase clean
make -f Makefile_escprint debug
make -f Makefile_escprint clean
make -f Makefile_mdb debug
make -f Makefile_mdb clean
make -f Makefile_mysql debug
make -f Makefile_mysql clean
make -f Makefile_oracle debug
make -f Makefile_oracle clean
make -f Makefile_zip debug
make -f Makefile_zip clean
make -f Makefile_rawsock debug
make -f Makefile_rawsock clean
make -f Makefile_qtgui_debug
rm -rf debug GeneratedFiles
