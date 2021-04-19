// Copyright 2021-2021 The jdh99 Authors. All rights reserved.
// knock������ͨ��ʱ֡ͷ��֡���Ĵ������
// ֡���Ĵ�����ע���knock���ķ���,֡ͷ������÷���,����knock�����ȡ��Ӧ����
// Authors: jdh99 <jdh821@163.com>

#ifndef KNOCK_H
#define KNOCK_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

// �û�������������뺯�������ڴ�id,��ģ�����ʹ��ʱ���������ڴ�id
#define KNOCK_TAG "knock"
#define KNOCK_TZMALLOC_SIZE 4096

// KncokCallbackFunc ע�����ص�����
// ��ѭ˭����˭�ͷ�ԭ��,resp��Ҫ�ɻص�����ʹ��TZMalloc���ٿռ�,���÷������ͷ�
// respΪNULL����respLenΪ0��ʾ����ҪӦ��
// argp�ǿɱ����,������ʽ�������ɵ���˫��Լ��
typedef void (*KncokCallbackFunc)(uint8_t* req, int reqLen, uint8_t** resp, int* respLen, va_list argp);

// KnockLoad ģ������.��������ñ�����,ģ�������Ĭ���ڴ�id
void KnockLoad(int mid);

// KnockCall ͬ������
// resp��respLen��Ӧ���ֽ������ֽ�������.ע��resp��Ҫ�ͷ�.���respΪNULL����respLenΪ0��ʾ��Ӧ��
// ֧�ֿɱ����.������ʽ�������ɵ���˫��Լ��
void KnockCall(uint16_t protocol, uint16_t cmd, uint8_t* req, int reqLen, uint8_t** resp, int* respLen, ...);

// KnockRegister ע�����ص�����
void KnockRegister(uint16_t protocol, uint16_t cmd, KncokCallbackFunc callback);

#endif
