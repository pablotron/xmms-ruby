#!/usr/bin/env ruby

########################################################################
# get_playlist.rb - create an XML playlist from the XMMS playlist      #
# by Paul Duncan <pabs@pablotron.org>                                  #
########################################################################

# load XMMS bindings
require 'xmms'

# create a new Xmms::Remote object
r = Xmms::Remote.new

# add String#xml_escape! (used below to escape values)
class String
  def xml_escape!
    gsub '&', '&amp;'
    gsub '<', '&lt;'
    gsub '>', '&gt;'
  end
end

# start the XML output 
puts '<?xml version="1.0" encoding="ISO-8859-1">',
     '<XmmsPlaylist>'

# step through each element in the playlist and print it's information
# in XML form
r.playlist do |title, file, time|
  puts '  <entry>',
       "    <title>#{title.xml_escape!}</title>",
       "    <filename>#{file.xml_escape!}</filename>",
       "    <duration>#{time}</duration>",
       '  </entry>',
end

# close the XML output
puts '</XmmsPlaylist>'
