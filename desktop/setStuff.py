# -*- coding: utf-8 -*-
"""
Created on Sun May 07 15:28:30 2017

@author: Jak
"""

import time
import datetime


hr = 6
minute = 30
alarmTime = datetime.time(hour=hr, minute=minute, second=0)

alarmDayOfWeek = [0, 1, 1, 1, 1, 1, 0] #Sunday -> Saturday






import serial

def send(con, uint8_t):
    b1 = chr(int(uint8_t[0:2], 16))
    con.write(b1)



port = "COM7"
baud = "9600"

con = serial.Serial(port=port, baudrate=baud)

#first, set the time

now = datetime.datetime.now()


#now = datetime.datetime(2017, 5, 8, 6, 29, 00)



yearString = hex(now.year)[-2:].zfill(2)
monthString = hex(now.month)[2:].zfill(2)
dayString = hex(now.day)[2:].zfill(2)
dayOfWeek = 1 << ([1, 2, 3, 4, 5, 6, 0][now.weekday()])
dayOfWeekString = hex(dayOfWeek)[2:].zfill(2)
hourString = hex(now.hour)[2:].zfill(2)
minuteString = hex(now.minute)[2:].zfill(2)
secondString = hex(now.second)[2:].zfill(2)

print "time"
print "year: ", now.year, yearString
print "month: ", now.month, monthString
print "day: ", now.day, dayString
print "hour: ", now.hour, hourString
print "minute: ", now.minute, minuteString
print "second: ", now.second, secondString
print "day of week",  bin(dayOfWeek), dayOfWeekString


#send a s
#then MSB first:
#   year
#   month
#   day
#   hour
#   second
#   dayOfWeek
con.write("s")
send(con, yearString)
send(con, monthString)
send(con, dayString)
send(con, hourString)
send(con, minuteString)
send(con, secondString)
send(con, dayOfWeekString)


#sending the second bit of data isn't currently working
#       The alarm default is 6:30, monday to friday -> bugger it
import sys
sys.exit()

time.sleep(1)


#set alarm
print "alarm"

hourString = hex(alarmTime.hour)[2:].zfill(2)
minuteString = hex(alarmTime.minute)[2:].zfill(2)
secondString = hex(alarmTime.second)[2:].zfill(2)
alarmDay = sum([alarmDayOfWeek[i] << i for i in xrange(7)])
alarmDayString = hex(alarmDay)[2:].zfill(2)


print "hour: ", now.hour, hourString
print "minute: ", now.minute, minuteString
print "second: ", now.second, secondString
print "alarmDay: ", bin(alarmDay), alarmDayString


#send a a
#then MSB first:
#   hour
#   second
#   alarm days
con.write("a")
send(con, hourString)
send(con, minuteString)
send(con, secondString)
send(con, alarmDayString)
#send some rubbish - as must send 7 btes at the moment
send(con, "00")
send(con, "00")
send(con, "00")





