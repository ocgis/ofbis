# Configure paths for oFBis
# Christer Gustavsson 98-09-20

dnl AM_PATH_OFBIS([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl Test for oFBis, and define OFBIS_CFLAGS and OFBIS_LIBS

AC_DEFUN(AM_PATH_OFBIS,
[dnl 
dnl Get the cflags and libraries from the ofbis-config script
dnl
AC_ARG_WITH(ofbis-prefix,[  --with-ofbis-prefix=PFX   Prefix where oFBis is installed (optional)],
            ofbis_config_prefix="$withval", ofbis_config_prefix="")
AC_ARG_WITH(ofbis-exec-prefix,[  --with-ofbis-exec-prefix=PFX Exec prefix where oFBis is installed (optional)],
            ofbis_config_exec_prefix="$withval", ofbis_config_exec_prefix="")
AC_ARG_ENABLE(ofbistest, [  --disable-ofbistest       Do not try to compile and run a test oFBis program],
		    , enable_ofbistest=yes)

  if test x$ofbis_config_exec_prefix != x ; then
     ofbis_config_args="$ofbis_config_args --exec-prefix=$ofbis_config_exec_prefix"
     if test x${OFBIS_CONFIG+set} != xset ; then
        OFBIS_CONFIG=$ofbis_config_exec_prefix/bin/ofbis-config
     fi
  fi
  if test x$ofbis_config_prefix != x ; then
     ofbis_config_args="$ofbis_config_args --prefix=$ofbis_config_prefix"
     if test x${OFBIS_CONFIG+set} != xset ; then
        OFBIS_CONFIG=$ofbis_config_prefix/bin/ofbis-config
     fi
  fi

  AC_PATH_PROG(OFBIS_CONFIG, ofbis-config, no)
  min_ofbis_version=ifelse([$1], ,0.0.8,$1)
  AC_MSG_CHECKING(for oFBis - version >= $min_ofbis_version)
  no_ofbis=""
  if test "$OFBIS_CONFIG" = "no" ; then
    no_ofbis=yes
  else
    OFBIS_CFLAGS=`$OFBIS_CONFIG $ofbis_config_args --cflags`
    OFBIS_LIBS=`$OFBIS_CONFIG $ofbis_config_args --libs`
    ofbis_config_major_version=`$OFBIS_CONFIG $ofbis_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    ofbis_config_minor_version=`$OFBIS_CONFIG $ofbis_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    ofbis_config_micro_version=`$OFBIS_CONFIG $ofbis_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_ofbistest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $OFBIS_CFLAGS"
      LIBS="$LIBS $OFBIS_LIBS"
     fi
  fi
  if test "x$no_ofbis" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$OFBIS_CONFIG" = "no" ; then
       echo "*** The ofbis-config script installed by oFBis could not be found"
       echo "*** If oFBis was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the OFBIS_CONFIG environment variable to the"
       echo "*** full path to ofbis-config."
     else
        :
     fi
     OFBIS_CFLAGS=""
     OFBIS_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(OFBIS_CFLAGS)
  AC_SUBST(OFBIS_LIBS)
])
