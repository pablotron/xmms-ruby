#!/usr/bin/ruby

#########################################################################
# get_playlist.rb - create an XML playlist from the XMMS playlist       #
#########################################################################

# load the XMMS bindings
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
puts '<?xml version="1.0">'
puts '<XmmsPlaylist>'

# step through each element in the playlist and print it's information
# in XML form
r.playlist.each { |entry|
  title, file, time = entry

  puts '  <entry>'
  puts "    <title>#{title.xml_escape!}</title>"
  puts "    <filename>#{file.xml_escape!}</filename>"
  puts "    <duration>#{time}</duration>"
  puts '  </entry>'
}

# close the XML output
puts '</XmmsPlaylist>'
