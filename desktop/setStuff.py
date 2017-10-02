# -*- coding: utf-8 -*-
"""
Created on Sun May 07 15:28:30 2017

@author: Jak
"""

import time
import datetime


hr = 6
minute = 20
alarmTime = datetime.datetime(year=2017, month=2, day=10, hour=hr, minute=minute, second=0)
alarmEnd = alarmTime + datetime.timedelta(minutes=30)

alarmDayOfWeek = [0, 1, 1, 1, 1, 1, 0] #Sunday -> Saturday






import serial

def send(con, uint8_t):
    b1 = chr(int(uint8_t[0:2], 16))
    con.write(b1)



port = "COM8"
baud = "9600"

con = serial.Serial(port=port, baudrate=baud)

#first, set the time

now = datetime.datetime.now()
#offset = datetime.timedelta(minutes=10)
#now = datetime.datetime(2017, 5, 6, 6, 29, 0) - offset
#print now
#now = now + offset
print now



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
start = "FF"
send(con, start)
con.write("s")
length = "07"
send(con, length)
send(con, yearString)
send(con, monthString)
send(con, dayString)
send(con, hourString)
send(con, minuteString)
send(con, secondString)
send(con, dayOfWeekString)
crc = ["0", "0"]
end ="EF"
send(con, crc[0])
send(con, crc[1])
send(con, end)
print "sent"

#sending the second bit of data isn't currently working
#       The alarm default is 6:30, monday to friday -> bugger it
#import sys
#sys.exit()

time.sleep(1)


#set alarm
print "alarm"

hourString = hex(alarmTime.hour)[2:].zfill(2)
minuteString = hex(alarmTime.minute)[2:].zfill(2)
secondString = hex(alarmTime.second)[2:].zfill(2)
alarmDay = sum([alarmDayOfWeek[i] << i for i in xrange(7)])
alarmDayString = hex(alarmDay)[2:].zfill(2)
hourEndString = hex(alarmEnd.hour)[2:].zfill(2)
minuteEndString = hex(alarmEnd.minute)[2:].zfill(2)
secondEndString = hex(alarmEnd.second)[2:].zfill(2)

print "hour: ", now.hour, hourString
print "minute: ", now.minute, minuteString
print "second: ", now.second, secondString
print "alarmDay: ", bin(alarmDay), alarmDayString


#send a a
#then MSB first:
#   hour
#   second
#   alarm days
send(con, start)
con.write("a")
length = "07"
send(con, length)
send(con, "2")
send(con, hourString)
send(con, minuteString)
send(con, secondString)
send(con, alarmDayString)
send(con, hourEndString)
send(con, minuteEndString)
send(con, secondEndString)
crc = ["0", "0"]
end ="EF"
send(con, crc[0])
send(con, crc[1])
send(con, end)






