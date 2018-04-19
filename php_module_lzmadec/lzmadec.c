/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_lzmadec.h"

/* If you declare any globals in php_lzmadec.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(lzmadec)
*/

/* True global resources - no need for thread safety here */
static int le_lzmadec;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("lzmadec.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_lzmadec_globals, lzmadec_globals)
    STD_PHP_INI_ENTRY("lzmadec.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_lzmadec_globals, lzmadec_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_lzmadec_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_lzmadec_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "lzmadec", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/

/* {{{ proto string lzmadec(string str)
    */
PHP_FUNCTION(lzmadec)
{
	char *str = NULL;
	int argc = ZEND_NUM_ARGS();
	size_t str_len;

	size_t srclen ;
	char *src = NULL ;
	char *dest = NULL ;
	long destlen = 0 ;

	if (zend_parse_parameters(argc TSRMLS_CC, "s", &str, &str_len) == FAILURE) 
		return;

	srclen = str_len / 2 ;
	
	src = (char *)emalloc( srclen ) ;
	
	if( strhex_to_numeral( str , src ) == -1 )
	{
		efree( src ) ;
		RETURN_STRINGL( dest, destlen ) ;
	}

	if( (destlen = get_extsize( src ) ) == 0 )
	{
		efree( src ) ;
		RETURN_STRINGL( dest, destlen ) ;
	}

	dest = (char *)emalloc( destlen+1 ) ;
	
	if( lzdecode( src, &srclen, dest, &destlen ) == -1 )
	{
		efree( src ) ;
		efree( dest ) ;
    dest = NULL ;
    destlen = 0 ;
		RETURN_STRINGL( dest, destlen ) ;
	}

	efree( src ) ;

	dest[destlen] = '\0' ;
	
	RETURN_STRINGL( dest, destlen ) ;
}
/* }}} */


/* {{{ php_lzmadec_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_lzmadec_init_globals(zend_lzmadec_globals *lzmadec_globals)
{
	lzmadec_globals->global_value = 0;
	lzmadec_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(lzmadec)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(lzmadec)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(lzmadec)
{
#if defined(COMPILE_DL_LZMADEC) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(lzmadec)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(lzmadec)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "lzmadec support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ lzmadec_functions[]
 *
 * Every user visible function must have an entry in lzmadec_functions[].
 */
const zend_function_entry lzmadec_functions[] = {
	PHP_FE(confirm_lzmadec_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(lzmadec,	NULL)
	PHP_FE_END	/* Must be the last line in lzmadec_functions[] */
};
/* }}} */

/* {{{ lzmadec_module_entry
 */
zend_module_entry lzmadec_module_entry = {
	STANDARD_MODULE_HEADER,
	"lzmadec",
	lzmadec_functions,
	PHP_MINIT(lzmadec),
	PHP_MSHUTDOWN(lzmadec),
	PHP_RINIT(lzmadec),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(lzmadec),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(lzmadec),
	PHP_LZMADEC_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_LZMADEC
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(lzmadec)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
