#include "lzmadecode.h"
#include <string.h>
#include <stdlib.h>


/*lzma 要求定义分配函数*/
static void *myAlloc(void *p, size_t size)
{
  UNUSED_VAR(p);
  if (size == 0)
    return 0;
  #ifdef _SZ_ALLOC_DEBUG
  fprintf(stderr, "\nAlloc %10u bytes; count = %10d", (unsigned)size, g_allocCount);
  g_allocCount++;
  #endif
  return malloc(size);
}

/*lzma 要求定义释放函数*/
static void myFree(void *p, void *address)
{
  UNUSED_VAR(p);
  #ifdef _SZ_ALLOC_DEBUG
  if (address != 0)
  {
    g_allocCount--;
    fprintf(stderr, "\nFree; count = %10d", g_allocCount);
  }
  #endif
  free(address);
}


// 此函数将一串字符串方式表示的16进制流(src),转换成二进制形式存入dest中
// src, dest的内存空间由调用者管理, 成功换回 __TRUE, 失败返回 __FALSE
// 并且 dest的长度一定是src长度的一半
int 
strhex_to_numeral( const char *src, Byte *dest )
{
  int srclen ;
  char *srctmp ;
  int i ;

  if( src == NULL || dest == NULL )
    return __FALSE ;

  srclen = strlen( src ) ;

  if( ( srctmp = (char *)malloc( srclen + 1 ) ) == NULL )
    return __FALSE ;

  strcpy( srctmp, src ) ;

  for( i = 0 ; i < srclen ; i += 2 )
  {
    if( srctmp[i] <= '9' )
      srctmp[i] = srctmp[i] - '0' ;
    else if( srctmp[i] <= 'F' )
      srctmp[i] = srctmp[i] - 'A' + 10 ;
    else if( srctmp[i] <= 'f' )
      srctmp[i] = srctmp[i] - 'a' + 10 ;

    if( srctmp[i+1] <= '9' )
      srctmp[i+1] = srctmp[i+1] - '0' ;
    else if( srctmp[i+1] <= 'F' )
      srctmp[i+1] = srctmp[i+1] - 'A' + 10 ;
    else if( srctmp[i+1] <= 'f' )
      srctmp[i+1] = srctmp[i+1] - 'a' + 10 ;

    dest[i/2] = (srctmp[i] << 4 )| (srctmp[i+1]) ;
  }

  free( srctmp ) ;

  return __TRUE ;
}

// 此函数从被lzma压缩后的二进制流中获取解压后大小
// stream 的内存空间由调用者管理
uint64_t
get_extsize( const Byte *stream )
{
  int i ;
  uint64_t extsize = 0 ;

  if( stream == NULL )
    return (uint64_t )0 ;

  for( i = 0 ; i < __EXTRA_SIZE_LEN ; i++ )
    extsize |= ( stream[LZMA_PROPS_SIZE+i] << ( i * __BITS_PER_BYTE ) ) ;

  return extsize ;
}

// 此函数将被lzma压缩后的二进制流(src),解压缩到dest中, 
// srclen表示压缩数据长度
// destlen传入dest的大小,调用结束后destlen表示解压后的大小
// src, srclen,dest, destlen内存空间由调用者管理
int
lzdecode( const Byte *src, SizeT *srclen, Byte *dest, SizeT *destlen )
{
  ELzmaStatus status ;
  ISzAlloc defalloc = { myAlloc, myFree } ;
  SRes res ;
  SizeT tmpslen ;

  if( src == NULL || srclen == NULL || dest == NULL || destlen == NULL )
    return __FALSE ;

  tmpslen = *srclen - __LZMA_HEADER_LEN ;

  res = LzmaDecode( dest, destlen, \
                    src+__LZMA_DATA_BEGIN, &tmpslen, \
                    src, (unsigned int)LZMA_PROPS_SIZE, \
                    LZMA_FINISH_END, &status, \
                    &defalloc ) ;

  if( res == SZ_OK && status == LZMA_STATUS_FINISHED_WITH_MARK )
    return __TRUE ;
  else
    return __FALSE ;
}









