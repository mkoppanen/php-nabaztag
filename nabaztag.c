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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_nabaztag.h"

ZEND_DECLARE_MODULE_GLOBALS(nabaztag);

#define NABAZTAG_SET_MSG(intern, message) snprintf(intern->message, 512, message);

zend_class_entry *nabaztag_ce_nabaztag;
static zend_object_handlers nabaztag_object_handlers;
static void nabaztag_register_classes(TSRMLS_D);

/* {{{ ARG_INFO */
ZEND_BEGIN_ARG_INFO(arginfo_nabaztag_method_getSerial, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(arginfo_nabaztag_method_setSerial, 0)
ZEND_ARG_INFO(0, serial)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(arginfo_nabaztag_method_getToken, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(arginfo_nabaztag_method_setToken, 0)
ZEND_ARG_INFO(0, token)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(arginfo_nabaztag_method_getLastMessage, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(arginfo_nabaztag_method_tts, 0)
ZEND_ARG_INFO(0, string)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(arginfo_nabaztag_method___construct, 0)
ZEND_ARG_INFO(0, serial)
ZEND_ARG_INFO(0, token)
ZEND_END_ARG_INFO()
/* }}} */

const zend_function_entry nabaztag_class_methods[] = {
  PHP_ME(Nabaztag, __construct, arginfo_nabaztag_method___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
  PHP_ME(Nabaztag, getSerial, arginfo_nabaztag_method_getSerial, ZEND_ACC_PUBLIC)
  PHP_ME(Nabaztag, setSerial, arginfo_nabaztag_method_setSerial, ZEND_ACC_PUBLIC)
  PHP_ME(Nabaztag, getToken, arginfo_nabaztag_method_getToken, ZEND_ACC_PUBLIC)
  PHP_ME(Nabaztag, setToken, arginfo_nabaztag_method_setToken, ZEND_ACC_PUBLIC)
  PHP_ME(Nabaztag, getLastMessage, arginfo_nabaztag_method_getLastMessage, ZEND_ACC_PUBLIC)
  PHP_ME(Nabaztag, tts, arginfo_nabaztag_method_tts, ZEND_ACC_PUBLIC)
  {NULL, NULL, NULL}
};

zend_module_entry nabaztag_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
    PHP_NABAZTAG_EXTNAME,
	nabaztag_class_methods,
	PHP_MINIT(nabaztag),
	PHP_MSHUTDOWN(nabaztag),
	PHP_RINIT(nabaztag),
	PHP_RSHUTDOWN(nabaztag),
	PHP_MINFO(nabaztag),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_NABAZTAG_EXTVER,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_NABAZTAG
ZEND_GET_MODULE(nabaztag)
#endif

PHP_INI_BEGIN()
    PHP_INI_ENTRY("nabaztag.api_url", "http://api.nabaztag.com/vl/FR/api.jsp?",
        PHP_INI_ALL, NULL)
PHP_INI_END()

PHP_MINIT_FUNCTION(nabaztag)
{
    nabaztag_register_classes(TSRMLS_C);
    REGISTER_INI_ENTRIES();
	return SUCCESS;
}

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(nabaztag)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION */
PHP_RSHUTDOWN_FUNCTION(nabaztag)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(nabaztag)
{
    UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(nabaztag)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "nabaztag support", "enabled");
    DISPLAY_INI_ENTRIES();
	php_info_print_table_end();
}
/* }}} */

static void nabaztag_object_free_storage_nabaztag(void *object TSRMLS_DC);
static zend_object_value nabaztag_object_new_nabaztag(zend_class_entry *class_type TSRMLS_DC);
static zend_object_value nabaztag_object_clone_nabaztag(zval *this_ptr TSRMLS_DC);
static int nabaztag_initialize(php_nabaztag_obj *nabaztagobj, char *serial, int serial_len, char *token, int token_len, int ctor TSRMLS_DC);

/* {{{ Object handling */
static void php_nabaztag_object_init(php_nabaztag_obj *intern TSRMLS_DC)
{
    intern->serial = NULL;
    intern->token = NULL;
}

static void nabaztag_object_free_storage_nabaztag(void *object TSRMLS_DC)
{
    php_nabaztag_obj *intern = (php_nabaztag_obj *)object;

    if (!intern) {
        return;
    }

    efree(intern->serial);
    efree(intern->token);
    zend_object_std_dtor(&intern->zo TSRMLS_CC);
    efree(intern);
}

static inline zend_object_value nabaztag_object_new_nabaztag_ex(zend_class_entry *class_type, php_nabaztag_obj **ptr TSRMLS_DC)
{
    zend_object_value retval;
    php_nabaztag_obj *intern;
    zval *tmp;

    intern = emalloc(sizeof(php_nabaztag_obj));
    memset(intern, 0, sizeof(php_nabaztag_obj));

    php_nabaztag_object_init(intern TSRMLS_CC);

    if (ptr) {
        *ptr = intern;
    }    

    zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
    zend_hash_copy(intern->zo.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));
    retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t) nabaztag_object_free_storage_nabaztag, NULL TSRMLS_CC);
    retval.handlers = &nabaztag_object_handlers;
    return retval;
}

static zend_object_value nabaztag_object_new_nabaztag(zend_class_entry *class_type TSRMLS_DC)
{
    return nabaztag_object_new_nabaztag_ex(class_type, NULL TSRMLS_CC);
}

static zend_object_value nabaztag_object_clone_nabaztag(zval *this_ptr TSRMLS_DC)
{
    php_nabaztag_obj *new_obj = NULL;
    php_nabaztag_obj *old_obj = (php_nabaztag_obj *) zend_object_store_get_object(this_ptr TSRMLS_CC);
    zend_object_value new_ov = nabaztag_object_new_nabaztag_ex(old_obj->zo.ce, &new_obj TSRMLS_CC);
    
    zend_objects_clone_members(&new_obj->zo, new_ov, &old_obj->zo, Z_OBJ_HANDLE_P(this_ptr) TSRMLS_CC);
    return new_ov;
}

static void nabaztag_register_classes(TSRMLS_D)
{
  zend_class_entry nabaztag_ce;
  INIT_CLASS_ENTRY(nabaztag_ce, "Nabaztag", nabaztag_class_methods);
  nabaztag_ce.create_object = nabaztag_object_new_nabaztag;
  nabaztag_ce_nabaztag = zend_register_internal_class_ex(&nabaztag_ce, NULL, NULL TSRMLS_CC);
  memcpy(&nabaztag_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  nabaztag_object_handlers.clone_obj = nabaztag_object_clone_nabaztag;
}
/* }}} */

/* {{{ proto Nabaztag __construct(string $serial, string $token)
   Class constructor, takes the unique sn and token */
PHP_METHOD(Nabaztag, __construct)
{
    char *serial = NULL;
    char *token = NULL;
    int serial_len = 0;
    int token_len = 0;
    zend_error_handling error_handling;

    zend_replace_error_handling(EH_THROW, NULL, &error_handling TSRMLS_CC);
    if (SUCCESS == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &serial, &serial_len, &token, &token_len)) {
        nabaztag_initialize(zend_object_store_get_object(getThis() TSRMLS_CC), serial, serial_len, token, token_len, 1 TSRMLS_CC);
    }
    zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */

/* {{{ proto string getSerial()
   Returns the serial number for your nabaztag */
PHP_METHOD(Nabaztag, getSerial)
{
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    php_nabaztag_obj *nabobj = zend_object_store_get_object(getThis() TSRMLS_CC);
    RETURN_STRING(nabobj->serial, 1);
}
/* }}} */

/* {{{ proto string setSerial(string $serial)
   Sets the serial number for your nabaztag */
PHP_METHOD(Nabaztag, setSerial)
{
    char *serial;
    int serial_len;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &serial, &serial_len)) {
        return;
    }

    php_nabaztag_obj *nabobj = zend_object_store_get_object(getThis() TSRMLS_CC);
    nabobj->serial = erealloc(nabobj->serial, serial_len + 1);
    strcpy(nabobj->serial, serial);

    RETURN_TRUE;
}
/* }}} */

/* {{{ proto string getToken()
   Returns the token for your nabaztag */
PHP_METHOD(Nabaztag, getToken)
{
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    php_nabaztag_obj *nabobj = zend_object_store_get_object(getThis() TSRMLS_CC);
    RETURN_STRING(nabobj->token, 1);
}
/* }}} */

/* {{{ proto string setToken(string $token)
   Sets the token for your nabaztag */
PHP_METHOD(Nabaztag, setToken)
{
    char *token;
    int token_len;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &token, &token_len)) {
        return;
    }

    php_nabaztag_obj *nabobj = zend_object_store_get_object(getThis() TSRMLS_CC);
    nabobj->token = erealloc(nabobj->token, token_len + 1);
    strcpy(nabobj->token, token);

    RETURN_TRUE;
}
/* }}} */

/* {{{ proto string getLastMessage()
   Returns the message from the last executed method */
PHP_METHOD(Nabaztag, getLastMessage)
{
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    php_nabaztag_obj *nabobj = zend_object_store_get_object(getThis() TSRMLS_CC);
    if(strlen(nabobj->message)) {
        RETURN_STRING(nabobj->message, 1);
    }

    return;
}
/* }}} */

/* {{{ proto void Nabaztag::tts(string $string)
   Send a string to the text-to-speech system */
PHP_METHOD(Nabaztag, tts)
{
   php_stream *stream;
   char *string, *url, retval[512];
   int string_len;
   int options = ENFORCE_SAFE_MODE | STREAM_USE_URL | REPORT_ERRORS;
   php_nabaztag_obj *nabobj;
   xmlDocPtr docp;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &string, &string_len)) {
        return;
    }

    nabobj = zend_object_store_get_object(getThis() TSRMLS_CC);
    ap_php_asprintf(&url, "%ssn=%s&token=%s&tts=%s", INI_STR("nabaztag.api_url"), nabobj->serial, nabobj->token, string);
    stream = php_stream_open_wrapper(url, "r", options, NULL);
    free(url);

    if (!stream) {
        RETURN_FALSE;
    }

    memcpy(retval, "", sizeof(""));
    while(!php_stream_eof(stream)) {
        char buf[1024];

        if (php_stream_gets(stream, buf, sizeof(buf))) {
            /* Check for buffer overflows */
            if (strlcat(retval, buf, sizeof(retval)) >= sizeof(retval)) {
                php_stream_close(stream);
                RETURN_FALSE;
            }
        } else {
            break;
        }
    }
    php_stream_close(stream);
    
    long xml_opts = 0;
    docp = xmlReadMemory(retval, strlen(retval), NULL, NULL, xml_opts);
    char *status = estrdup((char *)docp->children->children->children->content);
    char *message = estrdup((char *)docp->children->children->next->children->content);
    xmlFreeDoc(docp);

    NABAZTAG_SET_MSG(nabobj, message);

    RETVAL_BOOL(!strcmp(status, NABAZTAG_STATUS_TTSSENT));
    efree(status);
    efree(message);
}
/* }}} */

static int nabaztag_initialize(php_nabaztag_obj *nabaztagobj, char *serial, int serial_len, char *token, int token_len, int ctor TSRMLS_DC)
{
    nabaztagobj->serial = estrdup(serial);
    nabaztagobj->token = estrdup(token);
    return 1;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
