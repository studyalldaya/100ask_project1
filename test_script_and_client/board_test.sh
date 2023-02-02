#!/bin/sh

#WIFI是一个USB模块，能识别出来就认为正常
test_wifi() {
	while :
	do
		lsusb | grep -q "0bda:b720"  
		if [ $? -eq 0 ]
		then
			detect_dev 127.0.0.1 "wifi ok"
			break
		fi
		sleep 0.1
	done
}
#让声卡录音，如果能监测到比较大的音量就认为正常
test_record() {
	echo clean > record.out
	while :
	do
		arecord -vvv  -d 5 test.wav > record.out 2>&1 
		grep -q "[3-7][0-9]%" record.out
		if [ $? -eq 0 ]
		then
			detect_dev 127.0.0.1 "record ok"
			break
		fi
		sleep 0.1
	done
}

test_key1() {
	while :
	do
		hexdump /dev/input/event2 -n 15 | grep -q "0001 0002 0001 0000"
		if [ $? -eq 0 ]
		then
			detect_dev 127.0.0.1 "key1 ok"
			break
		fi
		sleep 0.1
	done
}
test_key2() {
	while :
	do
		hexdump /dev/input/event2 -n 15 | grep -q "0001 0003 0001 0000"
		if [ $? -eq 0 ]
		then
			detect_dev 127.0.0.1 "key2 ok"
			break
		fi
		sleep 0.1
	done
}


test_wifi
test_key1
test_key2
