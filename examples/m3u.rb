#!/usr/bin/env ruby

########################################################################
# m3u.rb - save current XMMS playlist to WinAmp-style M3U file         #
# by Paul Duncan <pabs@pablotron.org>                                  #
########################################################################

require 'xmms'

module Xmms
  class Remote
    #
    # get playlist contents as m3u-encoded string
    #
    def to_m3u
      "#EXTM3U\n" + playlist.map { |title, file, time| 
        "#EXTINF:#{time / 1000},#{title}\n#{file}"
      }.join("\n")
    end
  end
end

# connect to XMMS and get output filename
xmms = Xmms::Remote.new
path = ARGV[0] || 'playlist.m3u'

# save playlist to m3u file
puts "Saving playlist to \"#{path}\"."
File::open(path, 'w') { |out| out.puts xmms.to_m3u }
