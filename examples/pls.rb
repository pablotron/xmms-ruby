#!/usr/bin/env ruby

########################################################################
# m3u.rb - save current XMMS playlist to PLS file.                     #
# by Paul Duncan <pabs@pablotron.org>                                  #
########################################################################

require 'xmms'

module Xmms
  class Remote
    #
    # get playlist contents as pls-encoded string
    #
    def to_pls
      pls = playlist
      ret = "[playlist]\nNumberOfEntries=#{pls.size}\n"
      pls.each_with_index { |ary, i| ret << "File#{i + 1}=#{ary[1]}\n" }
      ret
    end
  end
end

# connect to XMMS and get output filename
xmms = Xmms::Remote.new
path = ARGV[0] || 'playlist.pls'

# save playlist to pls file
puts "Saving playlist to \"#{path}\"."
File::open(path, 'w') { |out| out.puts xmms.to_pls }
