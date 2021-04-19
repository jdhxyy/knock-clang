// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// knock包用于通信时帧头和帧正文处理解耦
// 帧正文处理函数注册成knock包的服务,帧头处理调用服务,调用knock服务获取响应数据
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

// KnockLoad 模块载入.如果不调用本函数,模块会申请默认内存id
void KnockLoad(int mid) {
    if (list == 0) {
        gMid = mid;
        list = TZListCreateList(gMid);
        if (list == 0) {
            LE(KNOCK_TAG, "load failed!create list failed!");
        }
    }
}

// KnockCall 同步调用
// resp和respLen是应答字节流和字节流长度.注意resp需要释放.如果resp为NULL或者respLen为0表示无应答
// 支持可变参数.参数格式和数量由调用双方约定
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

// KnockRegister 注册服务回调函数
void KnockRegister(uint16_t protocol, uint16_t cmd, KncokCallbackFunc callback) {
    if (list == 0) {
        gMid = TZMallocRegister(0, KNOCK_TAG, KNOCK_TZMALLOC_SIZE);
        list = TZListCreateList(gMid);
        if (list == 0) {
            LE(KNOCK_TAG, "register failed!create list failed!");
            return;
        }
    }

    uint32_t rid = (uint32_t)(cmd + (protocol << 16));
    tItem* item = getItem(rid);
    if (item != NULL) {
        // 已存在,则替换回调函数
        LW(KNOCK_TAG, "replace callback.protocol:%d cmd:%d", protocol, cmd);
        item->callback = callback;
        return;
    }

    TZListNode* node = createNode();
    if (node == NULL) {
        LE(KNOCK_TAG, "register failed!create node failed!");
        return;
    }
    item = (tItem*)node->Data;
    item->rid = rid;
    item->callback = callback;
    TZListAppend(list, node);
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
