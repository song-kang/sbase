rm -rf debug
make -f Makefile
make -f Makefile_escprint
make -f Makefile_mdb
make -f Makefile_mysql
make -f Makefile_zip
mkdir debug
make -f Makefile_qtgui
rm -rf debug