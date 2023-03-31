// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// knock包用于通信时帧头和帧正文处理解耦
// 帧正文处理函数注册成knock包的服务,帧头处理调用服务,调用knock服务获取响应数据
// Authors: jdh99 <jdh821@163.com>

#ifndef KNOCK_H
#define KNOCK_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

// 用户如果不调用载入函数传递内存id,则模块会在使用时自主申请内存id
#define KNOCK_TAG "knock"
#define KNOCK_TZMALLOC_SIZE 4096

// KncokCallbackFunc 注册服务回调函数
// 遵循谁调用谁释放原则,resp需要由回调函数使用TZMalloc开辟空间,调用方负责释放
// resp为NULL或者respLen为0表示不需要应答
// argp是可变参数,参数格式和数量由调用双方约定
typedef void (*KncokCallbackFunc)(uint8_t* req, int reqLen, uint8_t** resp, int* respLen, va_list argp);

// KnockLoad 模块载入.如果不调用本函数,模块会申请默认内存id
bool KnockLoad(int mid);

// KnockCall 同步调用
// resp和respLen是应答字节流和字节流长度.注意resp需要释放.如果resp为NULL或者respLen为0表示无应答
// 支持可变参数.参数格式和数量由调用双方约定
void KnockCall(uint16_t protocol, uint16_t cmd, uint8_t* req, int reqLen, uint8_t** resp, int* respLen, ...);

// KnockRegister 注册服务回调函数
bool KnockRegister(uint16_t protocol, uint16_t cmd, KncokCallbackFunc callback);

#endif
