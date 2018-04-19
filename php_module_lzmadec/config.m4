dnl $Id$
dnl config.m4 for extension lzmadec

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(lzmadec, for lzmadec support,
dnl Make sure that the comment is aligned:
dnl [  --with-lzmadec             Include lzmadec support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(lzmadec, whether to enable lzmadec support,
Make sure that the comment is aligned:
[  --enable-lzmadec           Enable lzmadec support])

if test "$PHP_LZMADEC" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-lzmadec -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/lzmadec.h"  # you most likely want to change this
  dnl if test -r $PHP_LZMADEC/$SEARCH_FOR; then # path given as parameter
  dnl   LZMADEC_DIR=$PHP_LZMADEC
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for lzmadec files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       LZMADEC_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$LZMADEC_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the lzmadec distribution])
  dnl fi

  dnl # --with-lzmadec -> add include path
  dnl PHP_ADD_INCLUDE($LZMADEC_DIR/include)

  dnl # --with-lzmadec -> check for lib and symbol presence
  dnl LIBNAME=lzmadec # you may want to change this
  dnl LIBSYMBOL=lzmadec # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $LZMADEC_DIR/$PHP_LIBDIR, LZMADEC_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_LZMADECLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong lzmadec lib version or lib not found])
  dnl ],[
  dnl   -L$LZMADEC_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(LZMADEC_SHARED_LIBADD)

  PHP_NEW_EXTENSION(lzmadec, lzmadec.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
