#ifndef __LZMA_DECODE_H
#define __LZMA_DECODE_H

#include "7zTypes.h"
#include "LzmaDec.h"
#include "Compiler.h"
#include <stdint.h>

#define __EXTRA_SIZE_LEN  8           //在以压缩的二进制流中,表示解压后长度的位数
#define __EXTRA_SIZE_BEGIN (LZMA_PROPS_SIZE)
#define __LZMA_HEADER_LEN  (LZMA_PROPS_SIZE + __EXTRA_SIZE_LEN )
#define __LZMA_DATA_BEGIN (__LZMA_HEADER_LEN)
#define __BITS_PER_BYTE  8

#define __TRUE 1
#define __FALSE -1

/*
The lzma file format layout:

Offset Size Description

  0     1   LZMA model properties (lc, lp, pb) in encoded form
  1     4   Dictionary size (32-bit unsigned integer, little-endian)
  5     8   Uncompressed size (64-bit unsigned integer, little-endian)
 13         Compressed data (LZMA stream)
*/

// 此函数将一串字符串方式表示的16进制流(src),转换成二进制形式存入dest中
// src, dest的内存空间由调用者管理, 成功换回 __TRUE, 失败返回 __FALSE
int strhex_to_numeral( const char *src, Byte *dest ) ;

// 此函数从被lzma压缩后的二进制流中获取解压后大小
// stream 的内存空间由调用者管理
uint64_t get_extsize( const Byte *stream ) ;

// 此函数将被lzma压缩后的二进制流(src),解压缩到dest中, 
// srclen表示压缩数据长度
// destlen传入dest的大小,调用结束后destlen表示解压后的大小
// src, srclen,dest, destlen内存空间由调用者管理
int lzdecode( const Byte *src, SizeT *srclen, Byte *dest, SizeT *destlen ) ;


#endif