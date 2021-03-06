## How to make

Run the command as below

aclocal; autoconf; autoheader;automake --add-missing;

Then config the platform and compiler

./configure --host=arm-linux CC=/home/ljk/ipc022/mijia_camera_mstar323/toolchain/host/bin/arm-linux-gcc 

