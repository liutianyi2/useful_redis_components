#include <stdio.h>
#include "ziplist.h"
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define ROUNDS 1000000

int main(int argc, char **argv){
    unsigned char *zl = ziplistNew(), *ori;
    char buf[100] = "liutianyi001";
    int reallocate_change_count = 0;
    clock_t start, end;
//    assert(zl != NULL);
//    ziplistPush(zl, (unsigned char *)buf, strlen(buf), ZIPLIST_TAIL);
//    strcpy(buf, "12345");
//    ziplistPush(zl, (unsigned char *)buf, strlen(buf), ZIPLIST_TAIL);
//    ziplistRepr(zl);
//    printf("%d\n", sizeof(zl));
    start = clock();
    for(int i = 0;i < ROUNDS; i++){
//        printf("ininininin, %d\n", i);
        strcpy(buf, "12345");
        ori = zl;
        zl = ziplistPush(zl, (unsigned char *)buf, strlen(buf), ZIPLIST_TAIL);
        if(ori != zl)
            reallocate_change_count++;
        strcpy(buf, "liutianyi001");
        ori = zl;
        zl = ziplistPush(zl, (unsigned char *)buf, strlen(buf), ZIPLIST_TAIL);
        if(ori != zl)
            reallocate_change_count++;
    }
    ziplistRepr(zl);
    end = clock();
    printf("%d rounds, reallocate_change_count: %d, using %f s\n", ROUNDS, reallocate_change_count, (double)(end - start) / CLOCKS_PER_SEC);
    sleep(100);
}