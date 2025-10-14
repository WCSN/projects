#!/bin/ruby


#dir = "/var/"
dir = Dir.open("/")

#Dir.chdir(dir)
#puts Dir.pwd

#Dir.entries(dir).each { |e| p e }
#puts("======")
#Dir.foreach(dir) {| filename | p filename }

puts("======") 
p dir.path
p dir.tell
p dir.read
p dir.tell
p dir.rewind
 
dir.each {|e| p e}
 
dir.close
