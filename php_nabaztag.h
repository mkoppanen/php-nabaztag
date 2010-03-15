/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2008 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Felix De Vliegher <felixdv@php.net>                          |
  +----------------------------------------------------------------------+
*/

/* $Id: $ */

#ifndef PHP_NABAZTAG_H
#define PHP_NABAZTAG_H

#define PHP_NABAZTAG_EXTNAME "nabaztag"
#define PHP_NABAZTAG_EXTVER "@PACKAGE_VERSION@"

extern zend_module_entry nabaztag_module_entry;
#define phpext_nabaztag_ptr &nabaztag_module_entry

#ifdef PHP_WIN32
#	define PHP_NABAZTAG_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_NABAZTAG_API __attribute__ ((visibility("default")))
#else
#	define PHP_NABAZTAG_API
#endif

#include "ext/libxml/php_libxml.h"
#include <libxml/parser.h>
#include <libxml/tree.h>

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_METHOD(Nabaztag, __construct);
PHP_METHOD(Nabaztag, getSerial);
PHP_METHOD(Nabaztag, setSerial);
PHP_METHOD(Nabaztag, getToken);
PHP_METHOD(Nabaztag, setToken);
PHP_METHOD(Nabaztag, getLastMessage);
PHP_METHOD(Nabaztag, tts);

PHP_MINIT_FUNCTION(nabaztag);
PHP_MSHUTDOWN_FUNCTION(nabaztag);
PHP_RINIT_FUNCTION(nabaztag);
PHP_RSHUTDOWN_FUNCTION(nabaztag);
PHP_MINFO_FUNCTION(nabaztag);

ZEND_BEGIN_MODULE_GLOBALS(nabaztag)
    char *api_url;
ZEND_END_MODULE_GLOBALS(nabaztag)

typedef struct _php_nabaztag_obj php_nabaztag_obj;
struct _php_nabaztag_obj {
	zend_object   zo;
	char         *serial;
	char         *token;

    /* Store last message here */
    char         message[512];
};


/* Return statuses */
#define NABAZTAG_STATUS_TTSSENT             "TTSSENT"
#define NABAZTAG_STATUS_NOGOODTOKENORSERIAL "NOGOODTOKENORSERIAL"

#ifdef ZTS
#define NABAZTAG_G(v) TSRMG(nabaztag_globals_id, zend_nabaztag_globals *, v)
#else
#define NABAZTAG_G(v) (nabaztag_globals.v)
#endif

#endif	/* PHP_NABAZTAG_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
