#!/bin/sh
#set -x
fetch_av_runtime=`perpls | grep "fetch_av" | awk '{print$6}' | awk -F / '{print$1}' | tr -cd "[0-9]"`
echo "fetch_av_runtime : ${fetch_av_runtime}"
fv_pid=`perpls | grep "fetch_av" | awk '{print$8}'| awk -F / '{print$1}'`etch_av_pid=`perpls | grep "fetch_av" | awk '{print$8}'| awk -F / '{print$1}'`
miio_x2_algo_pid=`perpls | grep "miio_x2_algo" | awk '{print$8}'| awk -F / '{print$1}'`
last_miio_x2_pid=1
fetch_av_reboot=1
while [ ${fetch_av_reboot} -le 1000 ]
do
	fetch_av_runtime=`perpls | grep "fetch_av" | awk '{print$6}' | awk -F / '{print$1}' | tr -cd "[0-9]"`
	if [ ${fetch_av_runtime} -gt 20 ];then
		fetch_av_pid=`perpls | grep "fetch_av" | awk '{print$8}'| awk -F / '{print$1}'`
		kill -9 $fetch_av_pid
		sleep 3
		miio_x2_algo_pid=`perpls | grep "miio_x2_algo" | awk '{print$8}'| awk -F / '{print$1}'`
		if [ x$last_miio_x2_pid == x$miio_x2_algo_pid ]; then
			echo "ERROR:miio_x2_algo cat't exit"
			break
		fi
		echo "last miio_x2_pid=$last_miio_x2_pid new pid=$miio_x2_algo_pid"
		last_miio_x2_pid=$miio_x2_algo_pid
		let fetch_av_reboot+=1
		echo "fetch reboot $fetch_av_reboot times"
	fi
	sleep 5
done
echo "The test is done"

