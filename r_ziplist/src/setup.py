from distutils.core import setup, Extension
setup(name="r_ziplist", version="1.0", ext_modules=[Extension("r_ziplist",
                                                              ["zmalloc.c", "sds.c", "endianconv.c", "ziplist.c",
                                                               "util.c", "sha1.c", "r_ziplistmodule.c"])])

