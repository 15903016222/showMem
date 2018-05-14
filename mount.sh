#!/bin/bash

MOUNT="/bin/mount"
UMOUNT="/bin/umount"
SEND="/usr/bin/send"
RM="/bin/rm -rf"

name=$1
name=${name%_*}

DIR="/media/$name"
i=0

DRIVER=$DEVNAME
LAST_NUM=`echo $DRIVER | wc -L`
LAST_NUM1=$LAST_NUM-1 
DRIVER1=${DRIVER:LAST_NUM1:LAST_NUM}
DRIVER2=${DRIVER:0:LAST_NUM1}

for DEV in `ls $DRIVER2[0-9]`
do
	if [ -e $DEV ]
	then
		if [ "$DEV" == "$DRIVER" ]
		then
			echo "$DEV $DRIVER"
			echo "mount"
			break
		else
			echo "$DEV $DRIVER"
			echo "no mount"
			exit
		fi
	fi
done

if ! mount | awk '{print $1}' | grep -q "$DEVNAME"
then
	while cat /proc/mounts | awk '{print $2}' | grep -q "$DIR" 
	do
		i=`expr $i + 1`
		DIR="/media/$name$i"
	done
	mkdir -p $DIR

	# 发送挂载信号
	$SEND $DIR
else
	return
fi

