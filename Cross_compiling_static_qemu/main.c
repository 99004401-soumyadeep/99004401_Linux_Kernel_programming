WORK=/media/Work/qemu
cd $WORK/build/w32
(export PKG_CONFIG_PATH=$WORK/install/w32/lib/pkgconfig ; 
export PKG_CONFIG_PREFIX=$WORK/install/w32 ;
#export BASH_X=-x ;
bash $BASH_X ../../qemu.git/configure --target-list="i386-softmmu" \
--cross-prefix=i686-w64-mingw32- \
--extra-ldflags="-L$WORK/install/w32/lib" \
--extra-cflags="-I$WORK/install/w32/include" \
--static \
)

//error found:-zlib check failed
       Make sure to have the zlib libs and headers installed.
