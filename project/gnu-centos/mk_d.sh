rm -rf debug
rm -rf *.a
make -f Makefile debug
make -f Makefile_escprint debug
make -f Makefile_mdb debug
make -f Makefile_mysql debug
make -f Makefile_oracle debug
make -f Makefile_zip debug
mkdir debug
rm -rf libsbase_qtguid.a
make -f Makefile_qtgui_debug
rm -rf debug
