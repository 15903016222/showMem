#!/bin/bash

FILE="/tmp/mem.xml"

write_xml()
{
	echo "<disk>" >> $FILE
	echo "<name>$1</name>" >> $FILE
	echo "<free>$3</free>" >> $FILE
	echo "<total>$2</total>" >> $FILE
	echo "</disk>" >> $FILE
}

get_disk_info()
{
	disk=`cat /proc/mounts | grep "$1" | awk '{print $2}'`
	total=`df $disk | grep "$disk" | awk '{print $2}'`
	free=`df $disk | grep "$disk" | awk '{print $4}'`
	if [ "/" == "$disk" ]
	then
		disk="system"
	else
		disk=${disk##*/}
	fi
}


test -f "$FILE" && rm "$FILE"

contents=`cat /proc/mounts | awk '{print $1}' | grep "/dev/root\|/dev/sd\|/dev/mmc"`

echo "<disks>" > $FILE
for content in `echo $contents`
do
	get_disk_info $content
	write_xml $disk $total $free
done
echo "</disks>" >> $FILE
