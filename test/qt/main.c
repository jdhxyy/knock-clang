#include <stdio.h>
#include <stdlib.h>
#include "knock.h"
#include "tzmalloc.h"

#pragma pack(1)

typedef struct {
    uint8_t a;
    uint16_t b;
    uint32_t c;
} tTest;

#pragma pack()

static int gMid = -1;

static void case1(void);
static void service1(uint8_t* req, int reqLen, uint8_t** resp, int* respLen, va_list argp);

static void case2(void);
static void service2(uint8_t* req, int reqLen, uint8_t** resp, int* respLen, va_list argp);

int main() {
    TZMallocLoad(0, 10, 10240, malloc(10240));
    gMid = TZMallocRegister(0, KNOCK_TAG, 1024);
    KnockLoad(gMid);

    case1();
    case2();
    return 0;
}

static void case1(void) {
    printf("case1\n");
    KnockRegister(0, 1, service1);

    uint8_t req[5] = {1, 2, 3, 4, 5};
    uint8_t* resp = NULL;
    int respLen = 0;
    KnockCall(0, 1, req, 5, &resp, &respLen);

    for (int i = 0; i < respLen; i++) {
        printf("0x%02x ", resp[i]);
    }
    printf("\n");

    TZFree(resp);
}

static void service1(uint8_t* req, int reqLen, uint8_t** resp, int* respLen, va_list argp) {
    for (int i = 0; i < reqLen; i++) {
        printf("0x%02x ", req[i]);
    }
    printf("\n");

    uint8_t* ack = TZMalloc(gMid, 3);
    for (int i = 0; i < 3; i++) {
        ack[i] = (uint8_t)i;
    }

    *resp = ack;
    *respLen = 3;
}

static void case2(void) {
    printf("case2\n");
    KnockRegister(1, 2, service2);

    uint8_t req[5] = {1, 2, 3, 4, 5};
    uint8_t* resp = NULL;
    int respLen = 0;

    tTest t;
    t.a = 6;
    t.b = 7;
    t.c = 8;

    KnockCall(1, 2, req, 5, &resp, &respLen, &t, 23);

    for (int i = 0; i < respLen; i++) {
        printf("0x%02x ", resp[i]);
    }
    printf("\n");

    TZFree(resp);
}

static void service2(uint8_t* req, int reqLen, uint8_t** resp, int* respLen, va_list argp) {
    for (int i = 0; i < reqLen; i++) {
        printf("0x%02x ", req[i]);
    }
    printf("\n");

    // È¡±ä²Î
    tTest* t = va_arg(argp, tTest*);
    printf("a=%d b=%d c=%d\n", t->a, t->b, t->c);
    printf("n=%d\n", va_arg(argp, int));


    uint8_t* ack = TZMalloc(gMid, 3);
    for (int i = 0; i < 3; i++) {
        ack[i] = (uint8_t)i;
    }

    *resp = ack;
    *respLen = 3;
}
