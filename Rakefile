# -*- ruby -*-

SKETCHBOOK = File.join(ENV['HOME'], 'sketchbook')
LIBRARIES = File.join(SKETCHBOOK, 'libraries')

# Must have a lib directory
directory 'lib'

directory 'lib/IRremote' do
  ln_s File.join(LIBRARIES, 'IRremote'), 'lib'
end

task :default => [ :libraries ]

task :libraries => [ 'lib/IRremote' ]

