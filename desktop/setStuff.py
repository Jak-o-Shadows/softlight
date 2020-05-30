# -*- coding: utf-8 -*-
"""
Created on Sun May 07 15:28:30 2017

@author: Jak
"""

import serial
import time
import datetime
import binascii

hr = 6
minute = 20
alarmTime = datetime.datetime(
    year=2019, month=6, day=1, hour=hr, minute=minute, second=0)
#alarmTime = datetime.datetime.now() + datetime.timedelta(minutes=2)
#alarmTime = datetime.datetime.now() + datetime.timedelta(seconds=30)

alarmEnd = alarmTime + datetime.timedelta(minutes=30)

alarmDayOfWeek = [0, 1, 1, 1, 1, 1, 0]  # Sunday -> Saturday
alarmDayOfWeek = [1, 1, 1, 1, 1, 1, 1]


def send(con, uint8_t):
    # input("continue?")
    b1 = binascii.unhexlify(uint8_t)
    print("\t", b1)
    con.write(b1)


port = "COM4"
baud = "9600"

con = serial.Serial(port=port, baudrate=baud)


# for i in range(29+1):
# send(con, hex(i)[2:].zfill(2))

# import sys
# sys.exit()

# first, set the time

now = datetime.datetime.now()
#offset = datetime.timedelta(minutes=10)
#now = datetime.datetime(2017, 5, 6, 6, 29, 0) - offset
# print(now
#now = now + offset
print(now)
# now = datetime.datetime(year=2020, month=5, day=30,
#                        hour=23, minute=58, second=0)
#alarmTime = now + datetime.timedelta(minutes=3)
#alarmEnd = alarmTime + datetime.timedelta(minutes=30)

yearString = hex(now.year)[-2:].zfill(2)
monthString = hex(now.month)[2:].zfill(2)
dayString = hex(now.day)[2:].zfill(2)
dayOfWeek = 1 << ([1, 2, 3, 4, 5, 6, 0][now.weekday()])
dayOfWeekString = hex(dayOfWeek)[2:].zfill(2)
hourString = hex(now.hour)[2:].zfill(2)
minuteString = hex(now.minute)[2:].zfill(2)
secondString = hex(now.second)[2:].zfill(2)

print("time")
print("year: ", now.year, yearString)
print("month: ", now.month, monthString)
print("day: ", now.day, dayString)
print("hour: ", now.hour, hourString)
print("minute: ", now.minute, minuteString)
print("second: ", now.second, secondString)
print("day of week",  bin(dayOfWeek), dayOfWeekString)


# send a s
# then MSB first:
#   year
#   month
#   day
#   hour
#   second
#   dayOfWeek
start = "FF"
send(con, start)
send(con, hex(ord("s"))[2:])
length = "07"
send(con, length)
send(con, yearString)
send(con, monthString)
send(con, dayString)
send(con, hourString)
send(con, minuteString)
send(con, secondString)
send(con, dayOfWeekString)
crc = ["00", "00"]
end = "EF"
send(con, crc[0])
send(con, crc[1])
send(con, end)
print("sent")

# sending the second bit of data isn't currently working
#       The alarm default is 6:30, monday to friday -> bugger it
#import sys
# sys.exit()

time.sleep(4)


# set alarm
print("alarm")

hourString = hex(alarmTime.hour)[2:].zfill(2)
minuteString = hex(alarmTime.minute)[2:].zfill(2)
secondString = hex(alarmTime.second)[2:].zfill(2)
alarmDay = sum([alarmDayOfWeek[i] << i for i in range(7)])
alarmDayString = hex(alarmDay)[2:].zfill(2)
hourEndString = hex(alarmEnd.hour)[2:].zfill(2)
minuteEndString = hex(alarmEnd.minute)[2:].zfill(2)
secondEndString = hex(alarmEnd.second)[2:].zfill(2)

print("hour: ", alarmTime.hour, hourString)
print("minute: ", alarmTime.minute, minuteString)
print("second: ", alarmTime.second, secondString)
print("alarmDay: ", bin(alarmDay), alarmDayString)


# send a a
# then MSB first:
#   hour
#   second
#   alarm days
send(con, start)
send(con, hex(ord("a"))[2:])
length = "07"
send(con, length)
send(con, "02")
send(con, hourString)
send(con, minuteString)
send(con, secondString)
send(con, alarmDayString)
send(con, hourEndString)
send(con, minuteEndString)
send(con, secondEndString)
crc = ["00", "00"]
end = "EF"
send(con, crc[0])
send(con, crc[1])
send(con, end)

# override light
#send(con, start)
# con.write("o")
#send(con, "01")
#send(con, "FF")
#crc = ["0", "0"]
#send(con, crc[0])
#send(con, crc[1])
#send(con, end)
