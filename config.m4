dnl $Id$
dnl config.m4 for extension nabaztag

PHP_ARG_ENABLE(nabaztag, whether to enable nabaztag support,
dnl Make sure that the comment is aligned:
[  --enable-nabaztag       Enable nabaztag support])

if test "$PHP_NABAZTAG" != "no"; then

  PHP_SETUP_LIBXML(NABAZTAG_SHARED_LIBADD, [

    AC_MSG_CHECKING([for ext/libxml includes])
    ext_libxml_inc_path=""

    if test -f "$abs_srcdir/include/php/ext/libxml/php_libxml.h"; then
      ext_libxml_inc_path="$abs_srcdir/include/php"
    elif test -f "$abs_srcdir/ext/libxml/php_libxml.h"; then
      ext_libxml_inc_path="$abs_srcdir"
    elif test -f "$phpincludedir/ext/libxml/php_libxml.h"; then
      ext_libxml_inc_path="$phpincludedir"
    else
      for i in php php4 php5 php6; do
        if test -f "$prefix/include/$i/ext/libxml/php_libxml.h"; then
          ext_libxml_inc_path="$prefix/include/$i"
        fi
      done
    fi

    if test "$ext_libxml_inc_path" = ""; then
      AC_MSG_ERROR([Cannot find php_libxml.h. Is libxml extension installed?])
    else
      AC_MSG_RESULT([$ext_libxml_inc_path])
    fi

    PHP_SUBST(NABAZTAG_SHARED_LIBADD)
    PHP_ADD_EXTENSION_DEP(nabaztag, libxml, true)
    PHP_NEW_EXTENSION(nabaztag, nabaztag.c, $ext_shared)
  ], [
    AC_MSG_ERROR([xml2-config not found. Please check your libxml2 installation.])
  ])
  
  
fi
