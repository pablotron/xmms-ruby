require 'rubygems'

spec = Gem::Specification.new do |s|

  #### Basic information.

  s.name = 'XMMS-Ruby'
  s.version = '0.1.2'
  s.summary = <<-EOF
    XMMS bindings for Ruby.
  EOF
  s.description = <<-EOF
    XMMS bindings for Ruby.
  EOF

  s.requirements << 'XMMS, version 1.2.6 (or newer)'
  s.requirements << 'Ruby, version 1.6.7 (or newer)'

  #### Which files are to be included in this gem?  Everything!  (Except CVS directories.)

  s.files = Dir.glob("**/*").delete_if { |item| item.include?("CVS") }

  #### C code extensions.

  s.require_path = 'lib' # is this correct?
  s.extensions << "extconf.rb"

  #### Load-time details: library and application (you will need one or both).
  s.autorequire = 'fam'
  s.has_rdoc = true
  s.rdoc_options = ['xmms.c', 'README', 'ChangeLog', 'AUTHORS', 'COPYING', 'TODO']

  #### Author and project details.

  s.author = 'Paul Duncan'
  s.email = 'pabs@pablotron.org'
  s.homepage = 'http://www.pablotron.org/software/xmms-ruby/'
  s.rubyforge_project = 'xmms-ruby'
end
