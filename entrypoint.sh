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
		tail -f /dev/null
		/ebpf_exporter --config.file=/build/ebpf_exporter/examples/timers.yaml &
		;;
	*)
		exec "$@"
		;;
esac
