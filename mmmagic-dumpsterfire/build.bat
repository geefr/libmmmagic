gcc mmmagic.c mm_lowlevel.c asyncthreads.c minimaidhid.c update.c main.c -lhid -lsetupapi -std=gnu99 -o test.exe

gcc mmmagic.c mm_lowlevel.c asyncthreads.c minimaidhid.c update.c ledutil.c -lhid -lsetupapi -std=gnu99 -o mmmame.exe