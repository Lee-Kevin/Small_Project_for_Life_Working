#!/bin/sh
set -x
vio_pid=`hbipc-utils --bifsd run "ps" | grep vio_service | awk '{print$1}'`
fetch_av_pid=`perpls | grep "fetch_av" | awk '{print$8}'| awk -F / '{print$1}'`

hbipc-utils --bifsd run "kill -9 $vio_pid"
sleep 30
vif_ints=`cat /sys/class/mstar/vif0/vif_ints | grep VREF_RISING | awk {'print$3'} | awk 'NR==1'`
sleep 1
now_vif_ints=`cat /sys/class/mstar/vif0/vif_ints | grep VREF_RISING | awk {'print$3'} | awk 'NR==1'`

vio_reboot=1
while [ ${vio_reboot} -le 1000 ]
do	
	echo "vio reboot $vio_reboot times"
	new_fetch_av_pid=`perpls | grep "fetch_av" | awk '{print$8}'| awk -F / '{print$1}'`
	if [ x${fetch_av_pid} != x${new_fetch_av_pid} ];then
		fetch_av_pid=`perpls | grep "fetch_av" | awk '{print$8}'| awk -F / '{print$1}'`
		echo "fetch_av reboot"
	fi
	let vio_reboot+=1
	if [ x${now_vif_ints} != x${vif_ints} ];then
		vio_pid=`hbipc-utils --bifsd run "ps" | grep vio_service | awk '{print$1}'`
		hbipc-utils --bifsd run "kill -9 $vio_pid"
		sleep 45
		vif_ints=`cat /sys/class/mstar/vif0/vif_ints | grep VREF_RISING | awk {'print$3'} | awk 'NR==1'`
		sleep 1
		now_vif_ints=`cat /sys/class/mstar/vif0/vif_ints | grep VREF_RISING | awk {'print$3'} | awk 'NR==1'`
	else
		sleep 20
		vif_ints=`cat /sys/class/mstar/vif0/vif_ints | grep VREF_RISING | awk {'print$3'} | awk 'NR==1'`               
                sleep 1                                                                                                        
                now_vif_ints=`cat /sys/class/mstar/vif0/vif_ints | grep VREF_RISING | awk {'print$3'} | awk 'NR==1'`
		if [ x${now_vif_ints} != x${vif_ints} ];then
			echo "I can get picture now"
		else
			echo "cat't get picture again"
			break
		fi
	fi
	sleep 1
done

echo "The test is done"
