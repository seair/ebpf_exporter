#!/bin/bash

if [ -d /root/init ];then

	for x in $(ls /root/init)
	do
		if [ -f /root/init/$x ];then
			chmod u+x /root/init/$x
			/bin/bash /root/init/$x
			rm -rf /root/init/$x
		fi
	done
fi


case ${1} in
	init)
		;;
	start)
		/usr/sbin/init
		source /etc/profile
		source ~/.bashrc
		/ebpf_exporter --config.file=/examples/timers.yaml &
		tail -f /dev/null
		;;
	*)
		exec "$@"
		;;
esac
