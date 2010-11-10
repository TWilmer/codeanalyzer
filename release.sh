#!/bin/bash -x
export PKG_CONFIG_PATH=/home/thorsten/gtk-target/lib/pkgconfig/
#export CFLAGS=-Wl,-subsystem,windows 
#export CXXFLAGS=-Wl,-subsystem,windows 
./configure  --prefix=/home/thorsten/gtk-target --host=i586-mingw32msvc   --with-libusb=/home/thorsten/gtk-target
make clean
make install

FILE+=codeanalyzer.exe
rm -rf /home/thorsten/gtk-target.relase
for a in $FILE
do
mkdir -p /home/thorsten/gtk-target.relase/bin/
cp /home/thorsten/gtk-target/bin/$a /home/thorsten/gtk-target.relase/bin/$a
done
cp /home/thorsten/gtk-target/bin/*.dll /home/thorsten/gtk-target.relase/bin/
cp -r /home/thorsten/gtk-target/etc /home/thorsten/gtk-target.relase/
mkdir -p /home/thorsten/gtk-target.relase/lib/gtk-2.0/2.10.0/
cp -r /home/thorsten/gtk-target/lib/gtk-2.0/2.10.0/engines/ /home/thorsten/gtk-target.relase/lib/gtk-2.0/2.10.0/engines
cp -r /home/thorsten/gtk-target/lib/pango /home/thorsten/gtk-target.relase/lib/pango
cp -r /home/thorsten/gtk-target.relase/* windows/SourceDir
