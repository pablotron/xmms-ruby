#!/usr/bin/env ruby

require 'xmms'

PAUSE_DURATION = 2

# allocate a new Xmms::Remote object
$stderr.puts 'testing Xmms-Ruby version ' << Xmms::Remote::VERSION
xr = Xmms::Remote::new

# test pause method
$stderr.puts 'testing Xmms::Remote#pause'
xr.pause
puts 'paused'
sleep PAUSE_DURATION

# test play method
$stderr.puts 'testing Xmms::Remote#play'
xr.play
puts 'playing'

# test play_pause method
$stderr.puts 'testing Xmms::Remote#play_pause'
xr.play_pause
puts 'paused'
sleep PAUSE_DURATION
xr.play_pause
puts 'playing'

# test playlist method
$stderr.puts 'testing Xmms::Remote#playlist'
xr.playlist.each { |entry|
  title, file, time = entry
  puts "'#{title}', #{time}, #{file}"
}

# test playlist block method
$stderr.puts 'testing Xmms::Remote#playlist (block)'
xr.playlist { |title, file, time| puts "'#{title}', #{time}, #{file}" }

# test [] method
$stderr.puts 'testing Xmms::Remote#[]'
1.upto(5) { |i|
  title, file, time = xr[i]
  puts "'#{title}', #{time}, #{file}"
}

# test eq methods
$stderr.puts 'test equalizer settings'

# save current equalizer settings
$stderr.puts 'testing Xmms::Remote#eq'
old_eq = xr.eq
puts old_eq

# make eq look like sine wave
$stderr.puts 'testing Xmms::Remote#set_eq'
0.upto(40) { |x|
  bands = [0.0] * 10
  0.upto(9) { |i| 
    bands[i] = 20.0 * Math::sin((i * 1.1 + x) / 5.0 * Math::PI)
    puts "setting band[#{i}] to #{bands[i]}"
  }
  xr.set_eq old_eq[0], bands
  sleep 0.1
}

puts *old_eq

# restore equalizer settings
xr.set_eq *old_eq
