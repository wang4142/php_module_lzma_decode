#include "lzmadecode.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int 
main( int argc, char *argv[] )
{
  Byte *src_stream = NULL ;
  int srclen ;
  uint64_t datasize = 0 ;
  Byte *dest = NULL ;
  char *src ;
  //int fd ;


 if( argc < 2 )
    exit(-1) ;

  src = argv[1] ;


  srclen = strlen( src ) / 2 ;

  if( ( src_stream = (Byte *)malloc( srclen ) ) == NULL )
    goto __return ;


  if( strhex_to_numeral( src, src_stream ) == __FALSE )
    goto __return ;

/*
  fd = open( "tmpfile", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH ) ;
  write( fd, src_stream, srclen ) ;
  close(fd) ;
*/

  if( ( datasize = get_extsize( src_stream ) ) == 0 )
    goto __return ;


  if( ( dest = (Byte *)malloc( datasize+1 ) ) == NULL )
    goto __return ;



  if( lzdecode( (const Byte *)src_stream, (SizeT *)&srclen, dest, (SizeT *)&datasize ) == __FALSE )  
    goto __return ;


  dest[datasize] = '\0' ;

  printf( "%s\n", dest ) ;


  __return :
    free( src_stream ) ;
    free( dest ) ;
    return 0;
}