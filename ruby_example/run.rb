#!/usr/bin/ruby

load "ourdate.rb"

d = OurDate.new(2011,1,4)
print "#{d.what_day}"
print "We started writing this file today.\n"
d.forward_time(365)
print "We are almost done now.\n"
print "#{d.what_day}"