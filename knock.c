// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// knock������ͨ��ʱ֡ͷ��֡���Ĵ������
// ֡���Ĵ�����ע���knock���ķ���,֡ͷ������÷���,����knock�����ȡ��Ӧ����
// Authors: jdh99 <jdh821@163.com>

#include "knock.h"

#include "tzlist.h"
#include "tzmalloc.h"
#include "lagan.h"

#include <string.h>

#pragma pack(1)

typedef struct {
    uint32_t rid;
    KncokCallbackFunc callback;
} tItem;

#pragma pack()

static intptr_t list = 0;
static int gMid = -1;

static tItem* getItem(uint32_t rid);
static TZListNode* createNode(void);

// KnockLoad ģ������.��������ñ�����,ģ�������Ĭ���ڴ�id
bool KnockLoad(int mid) {
    if (list == 0) {
        gMid = mid;
        list = TZListCreateList(gMid);
        if (list == 0) {
            LE(KNOCK_TAG, "load failed!create list failed!");
            return false;
        }
    }
    return true;
}

// KnockCall ͬ������
// resp��respLen��Ӧ���ֽ������ֽ�������.ע��resp��Ҫ�ͷ�.���respΪNULL����respLenΪ0��ʾ��Ӧ��
// ֧�ֿɱ����.������ʽ�������ɵ���˫��Լ��
void KnockCall(uint16_t protocol, uint16_t cmd, uint8_t* req, int reqLen, uint8_t** resp, int* respLen, ...) {
    uint32_t rid = (uint32_t)(cmd + (protocol << 16));
    tItem* item = getItem(rid);
    if (item == NULL) {
        return;
    }

    va_list argp;
    va_start(argp, respLen);
    item->callback(req, reqLen, resp, respLen, argp);
    va_end(argp);
}

// KnockRegister ע�����ص�����
bool KnockRegister(uint16_t protocol, uint16_t cmd, KncokCallbackFunc callback) {
    if (list == 0) {
        gMid = TZMallocRegister(0, KNOCK_TAG, KNOCK_TZMALLOC_SIZE);
        list = TZListCreateList(gMid);
        if (list == 0) {
            LE(KNOCK_TAG, "register failed!create list failed!");
            return false;
        }
    }

    uint32_t rid = (uint32_t)(cmd + (protocol << 16));
    tItem* item = getItem(rid);
    if (item != NULL) {
        // �Ѵ���,���滻�ص�����
        LW(KNOCK_TAG, "replace callback.protocol:%d cmd:%d", protocol, cmd);
        item->callback = callback;
        return true;
    }

    TZListNode* node = createNode();
    if (node == NULL) {
        LE(KNOCK_TAG, "register failed!create node failed!");
        return false;
    }
    item = (tItem*)node->Data;
    item->rid = rid;
    item->callback = callback;
    TZListAppend(list, node);
    return true;
}

static tItem* getItem(uint32_t rid) {
    TZListNode* node = TZListGetHeader(list);
    tItem* item = NULL;
    for (;;) {
        if (node == NULL) {
            break;
        }
        
        item = (tItem*)node->Data;
        if (item->rid == rid) {
            return item;
        }
        
        node = node->Next;
    }
    return NULL;
}

static TZListNode* createNode(void) {
    TZListNode* node = TZListCreateNode(list);
    if (node == NULL) {
        return NULL;
    }
    node->Data = TZMalloc(gMid, sizeof(tItem));
    if (node->Data == NULL) {
        TZFree(node);
        return NULL;
    }
    return node;
}
