gcc -fPIC -c zmalloc.c
gcc -fPIC -c sds.c
gcc -fPIC -c endianconv.c
gcc -fPIC -c ziplist.c
gcc -fPIC -c util.c
gcc -fPIC -c sha1.c
gcc sds.o ziplist.o zmalloc.o endianconv.o util.o sha1.o simple_test.c
python3 setup.py install
