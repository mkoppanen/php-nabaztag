dnl $Id$
dnl config.m4 for extension nabaztag

PHP_ARG_ENABLE(nabaztag, whether to enable nabaztag support,
dnl Make sure that the comment is aligned:
[  --enable-nabaztag       Enable nabaztag support])

if test "$PHP_NABAZTAG" != "no"; then
  PHP_NEW_EXTENSION(nabaztag, nabaztag.c, $ext_shared)
fi
