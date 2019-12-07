rm -rf debug
make -f Makefile_sbase
make -f Makefile_escprint
make -f Makefile_mdb
make -f Makefile_mysql
make -f Makefile_oracle
make -f Makefile_zip
mkdir debug
rm -rf libsbase_qtgui.a
make -f Makefile_qtgui
rm -rf debug
