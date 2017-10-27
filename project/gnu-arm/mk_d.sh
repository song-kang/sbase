rm -rf debug
make -f Makefile debug
make -f Makefile_escprint debug
make -f Makefile_mdb debug
make -f Makefile_mysql debug
make -f Makefile_zip debug
mkdir debug
make -f Makefile_qtgui_debug
rm -rf debug