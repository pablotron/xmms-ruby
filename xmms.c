/************************************************************************/
/* Copyright (C) 2002 Paul Duncan                                       */
/*                                                                      */
/* Permission is hereby granted, free of charge, to any person          */
/* obtaining a copy of this software and associated documentation files */
/* (the "Software"), to deal in the Software without restriction,       */
/* including without limitation the rights to use, copy, modify, merge, */
/* publish, distribute, sublicense, and/or sell copies of the Software, */
/* and to permit persons to whom the Software is furnished to do so,    */
/* subject to the following conditions:                                 */
/*                                                                      */
/* The above copyright notice and this permission notice shall be       */
/* included in all copies of the Software, its documentation and        */
/* marketing & publicity materials, and acknowledgment shall be given   */
/* in the documentation, materials and software packages that this      */
/* Software was used.                                                   */
/*                                                                      */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY     */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE    */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.               */
/************************************************************************/

#include <stdio.h>
#include <xmms/xmmsctrl.h>
#include <ruby.h>

#define UNUSED(x)  ((void) (x))

#define VERSION "0.1.1"
#define NUM_BANDS 10
#define BAND_MAX 20.0
#define BAND_MIN -20.0
#define VOL_MAX 100
#define VOL_MIN 0

/****************************/
/* CLASS AND MODULE GLOBALS */
/****************************/
static VALUE mXmms,
             cRemote;

/*
 * Create a new Xmms::Remote object.
 *
 * Examples:
 *   # standard setup (one running copy of XMMS)
 *   remote = Xmms::Remote.new
 *
 *   # bind to the third instance of XMMS (for you nutcases who run 
 *   # multiple instances of XMMS)
 *   session = 2
 *   remote = Xmms::Remote.new session
 *
 */
VALUE xr_new(int argc, VALUE *argv, VALUE klass) {
  int *session;
  VALUE xr;
  
  session = malloc(sizeof(int));
  switch (argc) {
    case 0:
      *session = 0;
      break;
    case 1:
      *session = NUM2INT(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "invalid argument count (not 0 or 1)");
  }

  xr = Data_Wrap_Struct(klass, 0, free, session);
  rb_obj_call_init(xr, argc, argv);
  
  return xr;
}

/*
 * Xmms::Remote constructor.
 *
 * This function is currently just a placeholder.
 *
 */
static VALUE xr_init(int argc, VALUE *argv, VALUE self) {
  UNUSED(argc);
  UNUSED(argv);
  return self;
}

/*
 * Get the version of XMMS.
 *
 * Note that this is separate from Xmms::Remote::VERSION, which is the
 * version of Xmms-Ruby.
 * 
 * Examples:
 *   version = remote.version
 *   version = remote.get_version
 */
static VALUE xr_version(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);

  return INT2FIX(xmms_remote_get_version(*session));
}

/*************************/
/* BASIC CONTROL METHODS */
/*************************/

/*
 * Play current song.
 *
 * Example:
 *   remote.play
 *
 */
static VALUE xr_play(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_play(*session);

  return self;
}

/*
 * Pause current song.
 *
 * Example:
 *   remote.pause
 *
 */
static VALUE xr_pause(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_pause(*session);

  return self;
}

/*
 * Stop current song.
 *
 * Example:
 *   remote.stop
 *
 */
static VALUE xr_stop(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_stop(*session);

  return self;
}

/*
 * XMMS eject button (toggle add file dialog).
 *
 * Example:
 *   remote.eject
 *
 */
static VALUE xr_eject(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_eject(*session);

  return self;
}

/*
 * Quit XMMS.
 *
 * Example:
 *   remote.quit
 *
 */
static VALUE xr_quit(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_quit(*session);

  return self;
}

/*
 * Toggle Play/Pause status.
 *
 * Example:
 *   remote.play_pause
 *
 */
static VALUE xr_play_pause(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_play_pause(*session);

  return self;
}

/****************************/
/* PLAY/PAUSE QUERY METHODS */
/****************************/

/*
 * Is XMMS playing?
 *
 * Examples:
 *   puts 'playing' if remote.is_playing?
 *   puts 'playing' if remote.playing?
 *
 */
static VALUE xr_playing(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);

  return xmms_remote_is_playing(*session) ? Qtrue : Qfalse;
}

/*
 * Is XMMS paused?
 *
 * Examples:
 *   puts 'paused' if remote.is_paused?
 *   puts 'paused' if remote.paused?
 *
 */
static VALUE xr_paused(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);

  return xmms_remote_is_paused(*session) ? Qtrue : Qfalse;
}

/********************/
/* PLAYLIST METHODS */
/********************/

/*
 * Return the current playlist.
 *
 * Example:
 *   remote.playlist.each { |i|
 *     title, file = i
 *     puts "'#{title}', #{file}"
 *   }
 *
 */
static VALUE xr_pl(VALUE self) {
  int i, len, *session;
  VALUE e, ary;
  
  Data_Get_Struct(self, int, session);

  ary = rb_ary_new();
  len = xmms_remote_get_playlist_length(*session);

  for (i = 0; i < len; i++) {
    e = rb_ary_new();
    rb_ary_push(e, rb_str_new2(xmms_remote_get_playlist_title(*session, i)));
    rb_ary_push(e, rb_str_new2(xmms_remote_get_playlist_file(*session, i)));
    rb_ary_push(e, INT2FIX(xmms_remote_get_playlist_time(*session, i)));
    rb_ary_push(ary, e);
  }

  return ary;
}

/*
 * Add one or more songs to the playlist.
 *
 * Examples:
 *   # replace the current playlist with one song
 *   enqueue = false
 *   remote.add 'path/to/cool_song.mp3', enqueue
 *
 *   # add several files to the current playlist
 *   files = %w{song01.mp3 song02.mp3 song03.mp3 song04.mp3 song05.mp3}
 *   remote.add *files
 *
 */
static VALUE xr_pl_add(int argc, VALUE *argv, VALUE self) {
  int i, max, *session;
  VALUE enqueue = Qtrue;
  gchar **list;

  if (argc < 1)
    rb_raise(rb_eArgError, "invalid argument count (must be >= 1)");
  
  list = g_malloc(sizeof(gchar*) * argc);

  for (i = 0, max = 0; i < argc; i++) {
    switch (TYPE(argv[i])) {
      case T_STRING:
        list[i] = (gchar*) RSTRING(argv[i])->ptr;
        max++;
        break;
      case T_TRUE:
      case T_FALSE:
        enqueue = argv[i];
        break;
      default:
        rb_raise(rb_eTypeError, "invalid argument type (not string or bool)");
    }
  }

  Data_Get_Struct(self, int, session);
  xmms_remote_playlist(*session, list, max, enqueue);

  /* free the list, but not the elements */
  g_free(list);

  return self;
}

/*
 * Add a URL to the current playlist.
 *
 * Examples:
 *   remote.add_url 'http://www.hhmecca.net/cool_song.mp3'
 *
 */
static VALUE xr_pl_add_url(VALUE self, VALUE url) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_playlist_add_url_string(*session, RSTRING(url)->ptr);

  return self;
}

/*
 * Insert a URL into the current playlist.
 *
 * Examples:
 *   # add a url after the 45th element in the playlist
 *   remote.ins_url 'http://www.hhmecca.net/cool_song.mp3', 45
 *
 */
static VALUE xr_pl_ins_url(VALUE self, VALUE url, VALUE pos) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_playlist_ins_url_string(*session,
                                      RSTRING(url)->ptr,
                                      NUM2INT(pos));

  return self;
}

/*
 * Delete the Nth element of the current playlist.
 *
 * Examples:
 *   remote.delete 26  # delete the 26th playlist element
 *
 */
static VALUE xr_pl_del(VALUE self, VALUE pos) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_playlist_delete(*session, NUM2INT(pos));

  return self;
}

/*
 * Clear the current playlist.
 *
 * Examples:
 *   remote.clear
 *
 */
static VALUE xr_pl_clear(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_playlist_clear(*session);

  return self;
}

/*
 * Get the current playlist position.
 *
 * Examples:
 *   song_number = remote.position
 *   song_number = remote.get_pos
 *   song_number = remote.pos
 *
 */
static VALUE xr_pl_pos(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);

  return INT2FIX(xmms_remote_get_playlist_pos(*session));
}

/*
 * Set the current playlist position.
 *
 * Examples:
 *   remote.pos = 45
 *   remote.position = 45
 *   remote.set_pos 45
 *
 */
static VALUE xr_pl_set_pos(VALUE self, VALUE pos) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_set_playlist_pos(*session, NUM2INT(pos));

  return self;
}

/*
 * Get the file path of a song.
 *
 * Examples:
 *   # get the path of the current song
 *   path = remote.file
 *
 *   # get the path of song 23 in the playlist
 *   path = remote.file 23
 *
 */
static VALUE xr_pl_file(int argc, VALUE *argv, VALUE self) {
  int pos, *session;

  Data_Get_Struct(self, int, session);

  switch (argc) {
    case 0:
      pos = xmms_remote_get_playlist_pos(*session);
      break;
    case 1:
      pos = NUM2INT(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "invalid argument count (not 0 or 1)");
  }
  
  return rb_str_new2(xmms_remote_get_playlist_file(*session, pos));
}

/*
 * Get the title of a song.
 *
 * Examples:
 *   # get the title of the current song
 *   path = remote.file
 *
 *   # get the title of song 23 in the playlist
 *   path = remote.file 23
 *
 */
static VALUE xr_pl_title(int argc, VALUE *argv, VALUE self) {
  int pos, *session;

  Data_Get_Struct(self, int, session);

  switch (argc) {
    case 0:
      pos = xmms_remote_get_playlist_pos(*session);
      break;
    case 1:
      pos = NUM2INT(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "invalid argument count (not 0 or 1)");
  }
  
  return rb_str_new2(xmms_remote_get_playlist_title(*session, pos));
}

/*
 * Get the "playlist" time of a song.
 *
 * Examples:
 *   # get the "playlist" time of the current song
 *   time = remote.playlist_time
 *
 *   # get the "playlist" time of song 23 in the playlist
 *   time = remote.playlist_time 23
 *
 */
static VALUE xr_pl_time(int argc, VALUE *argv, VALUE self) {
  int pos, *session;

  Data_Get_Struct(self, int, session);

  switch (argc) {
    case 0:
      pos = xmms_remote_get_playlist_pos(*session);
      break;
    case 1:
      pos = NUM2INT(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "invalid argument count (not 0 or 1)");
  }
  
  return INT2FIX(xmms_remote_get_playlist_time(*session, pos));
}

/*
 * Return information about a specific song in the playlist.
 *
 * Examples:
 *   title, file, time = remote[45] # get info about the 45th song
 *   info = remote.get_entry 45 # get info about the 45th song
 *
 */
static VALUE xr_pl_ary(VALUE self, VALUE pos) {
  int p, *session;
  VALUE ary;

  Data_Get_Struct(self, int, session);

  p = NUM2INT(pos);
  ary = rb_ary_new();
  rb_ary_push(ary, rb_str_new2(xmms_remote_get_playlist_title(*session, p)));
  rb_ary_push(ary, rb_str_new2(xmms_remote_get_playlist_file(*session, p)));
  rb_ary_push(ary, INT2FIX(xmms_remote_get_playlist_time(*session, p)));

  return ary;
}

/****************/
/* TIME METHODS */
/****************/

/*
 * Get the output time of the current song.
 *
 * Examples:
 *   time = remote.time
 *   time = remote.output_time
 *
 */
static VALUE xr_time(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);

  return INT2FIX(xmms_remote_get_output_time(*session));
}

/*
 * Jump to a specific time of the current song.
 *
 * Examples:
 *   remote.jump 45   # jump 45 seconds into the current song
 *   remote.time = 45 # jump 45 seconds into the current song 
 *
 */
static VALUE xr_jump(VALUE self, VALUE pos) {
  int *session;

  Data_Get_Struct(self, int, session);

  xmms_remote_jump_to_time(*session, NUM2INT(pos));

  return self;
}

/*
 * Go to the previous song.
 *
 * Example:
 *   remote.prev
 *
 */
static VALUE xr_prev(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_playlist_prev(*session);

  return self;
}

/*
 * Go to the next song.
 *
 * Example:
 *   remote.next
 *
 */
static VALUE xr_next(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_playlist_next(*session);

  return self;
}

/******************/
/* VOLUME METHODS */
/******************/

/*
 * Get the stereo volume level.
 *
 * Example:
 *   left, right = remote.get_stereo_volume
 *   left, right = remote.stereo_volume
 *
 */
static VALUE xr_stereo_vol(VALUE self) {
  int l, r, *session;
  VALUE ary;

  Data_Get_Struct(self, int, session);
  xmms_remote_get_volume(*session, &l, &r);

  ary = rb_ary_new();
  rb_ary_push(ary, INT2FIX(l));
  rb_ary_push(ary, INT2FIX(r));

  return ary;
}

/*
 * Get the main volume level.
 *
 * Example:
 *   vol = remote.volume
 *   vol = remote.main_volume
 *   vol = remote.get_main_volume
 *
 */
static VALUE xr_main_vol(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);
  
  return INT2FIX(xmms_remote_get_main_volume(*session));
}

/* 
 * Set the stereo volume level.
 *
 * Examples:
 *   remote.set_stereo_volume left, right
 *
 *   stereo_volume = [10, 0]
 *   remote.set_stereo_volume *stereo_volume
 *   
 */
static VALUE xr_set_stereo_vol(VALUE self, VALUE l, VALUE r) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_set_volume(*session, NUM2INT(l), NUM2INT(r));

  return self;
}

/*
 * Set the main volume level.
 *
 * Examples:
 *   remote.volume = 23
 *   remote.main_volume = 23
 *   remote.set_main_volume 23
 *
 */
static VALUE xr_set_main_vol(VALUE self, VALUE vol) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_set_main_volume(*session, NUM2INT(vol));

  return self;
}


/*******************/
/* BALANCE METHODS */
/*******************/

/*
 * Get the balance level.
 *
 * Note: as of XMMS 1.2.6 xmms_remote_get_balance() does not appear to
 * work correctly when XMMS is paused or stopped, so this function doesn't
 * either.
 * 
 * Example:
 *   vol = remote.balance
 *
 */
static VALUE xr_balance(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);
  
  return INT2FIX(xmms_remote_get_balance(*session));
}

/*
 * Set the balance level.
 *
 * Note: as of XMMS 1.2.6 xmms_remote_set_balance() does not appear to
 * work correctly when XMMS is paused or stopped, so this function doesn't
 * either.
 * 
 * Example:
 *   remote.balance = 23
 *
 */
static VALUE xr_set_balance(VALUE self, VALUE bal) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_set_balance(*session, NUM2INT(bal));

  return self;
}

/****************/
/* SKIN METHODS */
/****************/

/*
 * Get the current skin file.
 *
 * Examples:
 *   skin = remote.get_skin
 *   skin = remote.skin
 *
 */
static VALUE xr_skin(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);

  return rb_str_new2(xmms_remote_get_skin(*session));
}

/*
 * Set the current skin file.
 *
 * Examples:
 *   remote.set_skin 'path/to/cool_skin.zip'
 *   remote.skin = 'path/to/cool_skin.zip'
 *
 */
static VALUE xr_set_skin(VALUE self, VALUE skin) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_set_skin(*session, RSTRING(skin)->ptr);

  return self;
}

/******************/
/* WINDOW METHODS */
/******************/

/*
 * Set the visibility of the main window.
 *
 * Examples:
 *   remote.set_main_toggle true
 *   remote.main_visible = true
 *   remote.main = true
 *
 */
static VALUE xr_main_toggle(VALUE self, VALUE vis) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_main_win_toggle(*session, vis);

  return self;
}

/*
 * Get the visibility of the main window.
 *
 * Examples:
 *   puts 'main window visible' if remote.main_visible?
 *   puts 'main window visible' if remote.main?
 *
 */
static VALUE xr_is_main(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);

  return xmms_remote_is_main_win(*session) ? Qtrue : Qfalse;
}

/*
 * Set the visibility of the playlist window.
 *
 * Examples:
 *   remote.set_playlist_toggle true
 *   remote.playlist_visible = true
 *   remote.pl_visible = true
 *   remote.pl = true
 *
 */
static VALUE xr_pl_toggle(VALUE self, VALUE vis) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_pl_win_toggle(*session, vis);

  return self;
}

/*
 * Get the visibility of the playlist window.
 *
 * Examples:
 *   puts 'playlist window visible' if remote.playlist_visible?
 *   puts 'playlist window visible' if remote.pl_visible?
 *   puts 'playlist window visible' if remote.playlist?
 *   puts 'playlist window visible' if remote.pl?
 *
 */
static VALUE xr_is_pl(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);

  return xmms_remote_is_pl_win(*session) ? Qtrue : Qfalse;
}

/*
 * Set the visibility of the equalizer window.
 *
 * Examples:
 *   remote.set_equalizer_toggle true
 *   remote.equalizer_visible = true
 *   remote.eq_visible = true
 *   remote.eq = true
 *
 */
static VALUE xr_eq_toggle(VALUE self, VALUE vis) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_eq_win_toggle(*session, vis);

  return self;
}

/*
 * Get the visibility of the equalizer window.
 *
 * Examples:
 *   puts 'equalizer window visible' if remote.equalizer_visible?
 *   puts 'equalizer window visible' if remote.eq_visible?
 *   puts 'equalizer window visible' if remote.equalizer?
 *   puts 'equalizer window visible' if remote.eq?
 *
 */
static VALUE xr_is_eq(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);

  return xmms_remote_is_eq_win(*session) ? Qtrue : Qfalse;
}

/*
 * Show the preferences dialog.
 *
 * Examples:
 *   remote.show_prefs
 *   remote.prefs
 *
 */
static VALUE xr_prefs(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_show_prefs_box(*session);

  return self;
}

/*
 * Set the "always on top" flag.
 *
 * Examples:
 *   remote.set_always_on_top true
 *   remote.always_on_top = true
 *   remote.aot = true
 *
 */
static VALUE xr_set_aot(VALUE self, VALUE aot) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_toggle_aot(*session, aot);

  return self;
}

/**************************/
/* REPEAT/SHUFFLE METHODS */
/**************************/

/*
 * Toggle the repeat flag.
 *
 * Example:
 *   remote.toggle_repeat
 *
 */
static VALUE xr_toggle_repeat(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_toggle_repeat(*session);

  return self;
}

/*
 * Is the repeat flag set?
 *
 * Examples:
 *   puts 'repeating' if remote.is_repeating?
 *   puts 'repeating' if remote.repeating?
 *   puts 'repeating' if remote.is_repeat?
 *   puts 'repeating' if remote.repeat?
 *
 */
static VALUE xr_repeat(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);

  return xmms_remote_is_repeat(*session) ? Qtrue : Qfalse;
}

/*
 * Toggle the shuffle flag.
 *
 * Example:
 *   remote.toggle_shuffle
 *
 */
static VALUE xr_toggle_shuffle(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_toggle_shuffle(*session);

  return self;
}

/*
 * Is the shuffle flag set?
 *
 * Examples:
 *   puts 'shuffling' if remote.is_shuffling?
 *   puts 'shuffling' if remote.shuffling?
 *   puts 'shuffling' if remote.is_shuffle?
 *   puts 'shuffling' if remote.shuffle?
 *
 */
static VALUE xr_shuffle(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);

  return xmms_remote_is_shuffle(*session) ? Qtrue : Qfalse;
}

/****************/
/* MISC METHODS */
/****************/

/*
 * Get various about the current song.
 *
 * Returns an array of bitrate, frequency, and numchannels,
 * respectively.
 *
 * Examples:
 *   info = remote.info
 *   rate, freq, nch = remote.info
 *
 */
static VALUE xr_info(VALUE self) {
  int rate, freq, nch, *session;
  VALUE ary;

  Data_Get_Struct(self, int, session);
  xmms_remote_get_info(*session, &rate, &freq, &nch);

  ary = rb_ary_new();
  rb_ary_push(ary, NUM2INT(rate));
  rb_ary_push(ary, NUM2INT(freq));
  rb_ary_push(ary, NUM2INT(nch));

  return ary;
}

/*
 * Is XMMS running?
 *
 * Examples:
 *   puts 'xmms is running' if remote.is_running?
 *   puts 'xmms is running' if remote.running?
 *
 */
static VALUE xr_running(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);

  return xmms_remote_is_running(*session) ? Qtrue : Qfalse;
}

/*********************/
/* EQUALIZER METHODS */
/*********************/

/*
 * Get the equalizer settings.
 *
 * This method returns an array.  The first element in the array is the
 * preamp setting, and the second element in the array is an 10 element
 * array of band settings.  So the result would look something like
 * this:
 *
 *   [1.0, [0.0, -0.5, 0.9, 0.0, 0.0, 0.0, 0.2, 0.5, -0.1, 0.0]]
 * 
 * Examples:
 *   preamp, bands = remote.get_equalizer
 *   eq = remote.eq
 *
 */
static VALUE xr_eq(VALUE self) {
  int i, *session;
  VALUE ary, band_ary;
  gfloat preamp, *bands;

  Data_Get_Struct(self, int, session);
  xmms_remote_get_eq(*session, &preamp, &bands);

  band_ary = rb_ary_new();
  for (i = 0; i < NUM_BANDS; i++)
    rb_ary_push(band_ary, rb_float_new(bands[i]));
  
  ary = rb_ary_new();
  rb_ary_push(ary, rb_float_new(preamp));
  rb_ary_push(ary, band_ary);
  
  return ary;
}

/*
 * Get the equalizer preamp value.
 *
 * Examples:
 *   preamp = remote.get_preamp
 *   preamp = remote.preamp
 *
 */
static VALUE xr_eq_preamp(VALUE self) {
  int *session;

  Data_Get_Struct(self, int, session);

  return rb_float_new(xmms_remote_get_eq_preamp(*session));
}

/*
 * Get an equalizer band value.
 *
 * Note that legal band indexes range from 0 to 9 instead of 1 to 10.
 * 
 * Examples:
 *   band = remote.get_band 2
 *   band = remote.band 8
 *
 */
static VALUE xr_eq_band(VALUE self, VALUE band) {
  int *session;
  int b;

  Data_Get_Struct(self, int, session);
  b = NUM2INT(band);

  if (b < 0 || b >= NUM_BANDS)
    rb_raise(rb_eArgError, "band out of range (band < 0 or band >= 10)");

  return rb_float_new(xmms_remote_get_eq_band(*session, b));
}

/*
 * Set the equalizer values.
 *
 * This method takes either a preamp value and 10 band values, or a
 * preamp value and an array of band values.
 *
 * Examples:
 *   bands = [0.0, -0.5, 0.9, 0.0, 0.0, 0.0, 0.2, 0.5, -0.1, 0.0]
 *   remote.set_equalizer 0.0, bands
 *
 *   remote.set_eq 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
 *   
 */
static VALUE xr_set_eq(int argc, VALUE *argv, VALUE self) {
  int i, *session;
  gfloat bands[10];

  switch (argc) {
    case 11:
      for (i = 0; i < NUM_BANDS; i++) 
        bands[i] = NUM2DBL(argv[i + 1]);
      break;
    case 2:
      for (i = 0; i < NUM_BANDS; i++)
        bands[i] = NUM2DBL(rb_ary_entry(argv[1], 2));
      break;
    default:
      rb_raise(rb_eArgError,"invalid argument count (not 2 or 11)");
  }
  
  Data_Get_Struct(self, int, session);
  xmms_remote_set_eq(*session, NUM2DBL(argv[0]), bands);

  return self;
}

/*
 * Set the equalizer preamp value.
 *
 * Examples:
 *   remote.set_preamp 0.5
 *   remote.preamp = 0.5
 *
 */
static VALUE xr_eq_set_preamp(VALUE self, VALUE preamp) {
  int *session;

  Data_Get_Struct(self, int, session);
  xmms_remote_set_eq_preamp(*session, NUM2DBL(preamp));

  return self;
}

/*
 * Set an equalizer band value.
 *
 * Note that legal band indexes range from 0 to 9 instead of 1 to 10.
 * 
 * Example:
 *   remote.set_band 2, 0.5
 *
 */
static VALUE xr_eq_set_band(VALUE self, VALUE band, VALUE val) {
  int *session;
  int b;

  Data_Get_Struct(self, int, session);

  b = NUM2INT(band);
  if (b < 0 || b >= NUM_BANDS)
    rb_raise(rb_eArgError, "band out of range (band < 0 or band >= 10)");

  xmms_remote_set_eq_band(*session, b, NUM2DBL(val));

  return self;
}

void Init_xmms(void) {
  mXmms = rb_define_module("Xmms");
  
  /***********************/
  /* define Remote class */
  /***********************/
  cRemote = rb_define_class_under(mXmms, "Remote", rb_cObject);

  rb_define_singleton_method(cRemote, "new", xr_new, -1);
  rb_define_singleton_method(cRemote, "connect", xr_new, -1);

  rb_define_method(cRemote, "initialize", xr_init, -1);

  /* initialize constants */
  rb_define_const(cRemote, "VERSION", rb_str_new2(VERSION));
  rb_define_const(cRemote, "NUM_BANDS", INT2FIX(NUM_BANDS));

  rb_define_const(cRemote, "BAND_MAX", rb_float_new(BAND_MAX));
  rb_define_const(cRemote, "BAND_MIN", rb_float_new(BAND_MIN));

  rb_define_const(cRemote, "VOL_MAX", rb_float_new(VOL_MAX));
  rb_define_const(cRemote, "VOL_MIN", rb_float_new(VOL_MIN));

  /* basic control methods */
  rb_define_method(cRemote, "play", xr_play, 0);
  rb_define_method(cRemote, "pause", xr_pause, 0);
  rb_define_method(cRemote, "stop", xr_stop, 0);
  rb_define_method(cRemote, "eject", xr_eject, 0);
  rb_define_method(cRemote, "quit", xr_quit, 0);
  rb_define_method(cRemote, "play_pause", xr_play_pause, 0);

  /* play/pause query methods */
  rb_define_method(cRemote, "playing?", xr_playing, 0);
  rb_define_alias(cRemote, "is_playing", "playing?");
  rb_define_alias(cRemote, "is_playing?", "playing?");

  rb_define_method(cRemote, "paused?", xr_paused, 0);
  rb_define_alias(cRemote, "is_paused", "paused?");
  rb_define_alias(cRemote, "is_paused?", "paused?");
  
  /* playlist methods */
  rb_define_method(cRemote, "playlist", xr_pl, 0);
  rb_define_alias(cRemote, "list", "playlist");
  rb_define_alias(cRemote, "pl", "playlist");

  rb_define_method(cRemote, "add", xr_pl_add, -1);
  rb_define_alias(cRemote, "playlist_add", "add");
  rb_define_alias(cRemote, "add_files", "add");

  rb_define_method(cRemote, "add_url", xr_pl_add_url, 1);
  rb_define_alias(cRemote, "playlist_add_url", "add_url");
  
  rb_define_method(cRemote, "ins_url", xr_pl_ins_url, 2);
  rb_define_alias(cRemote, "playlist_ins_url_string", "ins_url");
  rb_define_alias(cRemote, "playlist_ins_url", "ins_url");

  rb_define_method(cRemote, "delete", xr_pl_del, 1);
  rb_define_alias(cRemote, "playlist_delete", "delete");
  rb_define_alias(cRemote, "playlist_del", "delete");
  rb_define_alias(cRemote, "pl_del", "delete");
  rb_define_alias(cRemote, "del", "delete");

  rb_define_method(cRemote, "clear", xr_pl_clear, 0);
  rb_define_alias(cRemote, "playlist_clear", "clear");
  rb_define_alias(cRemote, "pl_clear", "clear");

  rb_define_method(cRemote, "playlist_pos", xr_pl_pos, 0);
  rb_define_alias(cRemote, "get_playlist_position", "playlist_pos");
  rb_define_alias(cRemote, "get_playlist_pos", "playlist_pos");
  rb_define_alias(cRemote, "get_position", "playlist_pos");
  rb_define_alias(cRemote, "get_pos", "playlist_pos");
  rb_define_alias(cRemote, "position", "playlist_pos");
  rb_define_alias(cRemote, "pos", "playlist_pos");

  rb_define_method(cRemote, "set_playlist_pos", xr_pl_set_pos, 1);
  rb_define_alias(cRemote, "set_playlist_position", "set_playlist_pos");
  rb_define_alias(cRemote, "set_position", "set_playlist_pos");
  rb_define_alias(cRemote, "set_pos", "set_playlist_pos");
  rb_define_alias(cRemote, "playlist_position=", "set_playlist_pos");
  rb_define_alias(cRemote, "playlist_pos=", "set_playlist_pos");
  rb_define_alias(cRemote, "position=", "set_playlist_pos");
  rb_define_alias(cRemote, "pos=", "set_playlist_pos");
  
  rb_define_method(cRemote, "get_playlist_file", xr_pl_file, -1);
  rb_define_alias(cRemote, "playlist_file", "get_playlist_file");
  rb_define_alias(cRemote, "file", "get_playlist_file");

  rb_define_method(cRemote, "get_playlist_title", xr_pl_title, -1);
  rb_define_alias(cRemote, "playlist_title", "get_playlist_title");
  rb_define_alias(cRemote, "title", "get_playlist_title");

  rb_define_method(cRemote, "get_playlist_time", xr_pl_time, -1);
  rb_define_alias(cRemote, "playlist_time", "get_playlist_time");
  rb_define_alias(cRemote, "pl_time", "get_playlist_time");

  rb_define_method(cRemote, "[]", xr_pl_ary, 1);

  /* time methods */
  rb_define_method(cRemote, "time", xr_time, 0);
  rb_define_alias(cRemote, "output_time", "time");
  rb_define_alias(cRemote, "get_output_time", "time");

  rb_define_method(cRemote, "jump_to_time", xr_jump, 1);
  rb_define_alias(cRemote, "to_time", "jump_to_time");
  rb_define_alias(cRemote, "jump", "jump_to_time");
  rb_define_method(cRemote, "time=", xr_jump, 1);

  rb_define_method(cRemote, "playlist_prev", xr_prev, 0);
  rb_define_alias(cRemote, "prev", "playlist_prev");

  rb_define_method(cRemote, "playlist_next", xr_next, 0);
  rb_define_alias(cRemote, "next", "playlist_next");
  
  /* volume methods */
  rb_define_method(cRemote, "get_stereo_volume", xr_stereo_vol, 0);
  rb_define_alias(cRemote, "stereo_volume", "get_stereo_volume");

  rb_define_method(cRemote, "get_main_volume", xr_main_vol, 0);
  rb_define_alias(cRemote, "main_volume", "get_main_volume");
  rb_define_alias(cRemote, "get_volume", "get_main_volume");
  rb_define_alias(cRemote, "volume", "get_main_volume");

  rb_define_method(cRemote, "set_stereo_volume", xr_set_stereo_vol, 1);
  rb_define_alias(cRemote, "stereo_volume=", "set_stereo_volume");

  rb_define_method(cRemote, "set_main_volume", xr_set_main_vol, 1);
  rb_define_alias(cRemote, "main_volume=", "set_main_volume");
  rb_define_alias(cRemote, "volume=", "set_main_volume");

  /* balance methods */
  rb_define_method(cRemote, "get_balance", xr_balance, 0);
  rb_define_alias(cRemote, "balance", "get_balance");

  rb_define_method(cRemote, "set_balance", xr_set_balance, 1);
  rb_define_method(cRemote, "balance=", xr_set_balance, 1);

  /* skin methods */
  rb_define_method(cRemote, "get_skin", xr_skin, 0);
  rb_define_alias(cRemote, "skin", "get_skin");

  rb_define_method(cRemote, "set_skin", xr_set_skin, 1);
  rb_define_alias(cRemote, "skin=", "set_skin");

  /* window methods */
  rb_define_method(cRemote, "main_win_toggle", xr_main_toggle, 1);
  rb_define_alias(cRemote, "main_win=", "main_win_toggle");
  rb_define_alias(cRemote, "main_visible=", "main_win_toggle");
  rb_define_alias(cRemote, "main=", "main_win_toggle");
  
  rb_define_method(cRemote, "pl_win_toggle", xr_pl_toggle, 1);
  rb_define_alias(cRemote, "playlist_win_toggle", "pl_win_toggle");
  rb_define_alias(cRemote, "playlist_win=", "pl_win_toggle");
  rb_define_alias(cRemote, "playlist_visible=", "pl_win_toggle");
  rb_define_alias(cRemote, "pl_visible=", "pl_win_toggle");
  rb_define_alias(cRemote, "pl_win=", "pl_win_toggle");
  
  rb_define_method(cRemote, "eq_win_toggle", xr_eq_toggle, 1);
  rb_define_alias(cRemote, "equalizer_win_toggle", "eq_win_toggle");
  rb_define_alias(cRemote, "equalizer_win=", "eq_win_toggle");
  rb_define_alias(cRemote, "equalizer_visible=", "eq_win_toggle");
  rb_define_alias(cRemote, "eq_visible=", "eq_win_toggle");
  rb_define_alias(cRemote, "eq_win=", "eq_win_toggle");

  rb_define_method(cRemote, "is_main_win?", xr_is_main, 0);
  rb_define_alias(cRemote, "is_main?", "is_main_win?");
  rb_define_alias(cRemote, "is_main_visible?", "is_main_win?");
  rb_define_alias(cRemote, "main_visible?", "is_main_win?");
  rb_define_alias(cRemote, "main?", "is_main_win?");
  
  rb_define_method(cRemote, "is_pl_win?", xr_is_pl, 0);
  rb_define_alias(cRemote, "is_pl?", "is_pl_win?");
  rb_define_alias(cRemote, "is_playlist_win?", "is_pl_win?");
  rb_define_alias(cRemote, "is_playlist_visible?", "is_pl_win?");
  rb_define_alias(cRemote, "is_playlist?", "is_pl_win?");
  rb_define_alias(cRemote, "is_pl?", "is_pl_win?");
  rb_define_alias(cRemote, "playlist_visible?", "is_pl_win?");
  rb_define_alias(cRemote, "pl_visible?", "is_pl_win?");
  rb_define_alias(cRemote, "playlist?", "is_pl_win?");
  rb_define_alias(cRemote, "pl?", "is_pl_win?");
  
  rb_define_method(cRemote, "is_eq_win?", xr_is_eq, 0);
  rb_define_alias(cRemote, "is_eq?", "is_eq_win?");
  rb_define_alias(cRemote, "is_equalizer_win?", "is_eq_win?");
  rb_define_alias(cRemote, "is_equalizer_visible?", "is_eq_win?");
  rb_define_alias(cRemote, "is_equalizer?", "is_eq_win?");
  rb_define_alias(cRemote, "is_eq?", "is_eq_win?");
  rb_define_alias(cRemote, "equalizer_visible?", "is_eq_win?");
  rb_define_alias(cRemote, "eq_visible?", "is_eq_win?");
  rb_define_alias(cRemote, "equalizer?", "is_eq_win?");
  rb_define_alias(cRemote, "eq?", "is_eq_win?");
  
  rb_define_method(cRemote, "show_prefs", xr_prefs, 0);
  rb_define_alias(cRemote, "show_prefs_box", "show_prefs");
  rb_define_alias(cRemote, "prefs", "show_prefs");
  
  rb_define_method(cRemote, "toggle_aot", xr_set_aot, 1);
  rb_define_alias(cRemote, "toggle_always_on_top", "toggle_aot");
  rb_define_alias(cRemote, "set_always_on_top", "toggle_aot");
  rb_define_alias(cRemote, "set_aot", "toggle_aot");
  rb_define_alias(cRemote, "always_on_top=", "toggle_aot");
  rb_define_alias(cRemote, "aot=", "toggle_aot");

  /* repeat/shuffle methods */
  rb_define_method(cRemote, "toggle_repeat", xr_toggle_repeat, 0);
  rb_define_method(cRemote, "is_repeat?", xr_repeat, 0);
  rb_define_alias(cRemote, "is_repeating?", "is_repeat?");
  rb_define_alias(cRemote, "repeating?", "is_repeat?");
  rb_define_alias(cRemote, "repeat?", "is_repeat?");

  rb_define_method(cRemote, "toggle_shuffle", xr_toggle_shuffle, 0);
  rb_define_method(cRemote, "is_shuffle?", xr_shuffle, 0);
  rb_define_alias(cRemote, "is_shuffling?", "is_shuffle?");
  rb_define_alias(cRemote, "shuffling?", "is_shuffle?");
  rb_define_alias(cRemote, "shuffle?", "is_shuffle?");

  /* equalizer methods */
  rb_define_method(cRemote, "get_eq", xr_eq, 0);
  rb_define_alias(cRemote, "get_equalizer", "get_eq");
  rb_define_alias(cRemote, "equalizer", "get_eq");
  rb_define_alias(cRemote, "eq", "get_eq");
  
  rb_define_method(cRemote, "set_eq", xr_set_eq, -1);
  rb_define_alias(cRemote, "set_equalizer", "set_eq");
  
  rb_define_method(cRemote, "get_eq_preamp", xr_eq_preamp, 0);
  rb_define_alias(cRemote, "get_equalizer_preamp", "get_eq_preamp");
  rb_define_alias(cRemote, "equalizer_preamp", "get_eq_preamp");
  rb_define_alias(cRemote, "eq_preamp", "get_eq_preamp");
  rb_define_alias(cRemote, "preamp", "get_eq_preamp");

  rb_define_method(cRemote, "set_eq_preamp", xr_eq_set_preamp, 1);
  rb_define_alias(cRemote, "set_equalizer_preamp", "set_eq_preamp");
  rb_define_alias(cRemote, "equalizer_preamp=", "set_eq_preamp");
  rb_define_alias(cRemote, "eq_preamp=", "set_eq_preamp");
  rb_define_alias(cRemote, "preamp=", "set_eq_preamp");
  
  rb_define_method(cRemote, "get_eq_band", xr_eq_band, 1);
  rb_define_alias(cRemote, "get_equalizer_band", "get_eq_band");
  rb_define_alias(cRemote, "equalizer_band", "get_eq_band");
  rb_define_alias(cRemote, "eq_band", "get_eq_band");
  rb_define_alias(cRemote, "band", "get_eq_band");

  rb_define_method(cRemote, "set_eq_band", xr_eq_set_band, 2);
  rb_define_alias(cRemote, "set_equalizer_band", "set_eq_band");
  rb_define_alias(cRemote, "set_band", "set_eq_band");

  /* misc methods */
  rb_define_method(cRemote, "get_info", xr_info, 0);
  rb_define_alias(cRemote, "info", "get_info");

  rb_define_method(cRemote, "is_running?", xr_running, 0);
  rb_define_alias(cRemote, "running?", "is_running?");

  rb_define_method(cRemote, "get_version", xr_version, 0);
  rb_define_alias(cRemote, "version", "get_version");
}
