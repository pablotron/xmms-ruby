require 'mkmf'

xmms_config = with_config("xmms-config", "xmms-config")

$CFLAGS << ' ' << `#{xmms_config} --cflags`.chomp
$LDFLAGS << ' ' << `#{xmms_config} --libs`.chomp

have_library("xmms", "xmms_remote_get_version") and
  create_makefile("xmms")

