../configure --enable-encoders=lzma2 --enable-decoders=lzma2 --disable-assembler --disable-threads --disable-xz --disable-xzdec --disable-lzmadec --disable-lzmainfo --disable-lzma-links --disable-scripts --disable-unaligned-access
make
gcc ../benchmark.c -I../src/liblzma/api src/liblzma/.libs/liblzma.a

