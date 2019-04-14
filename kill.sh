daemons="zebra ospfd ldpd bgpd isisd ripd vtysh"
if [ $# -eq 0 ]; then
	echo "Kill whom?"
	exit
fi
if [ $1 = "all" ]; then
	for var in $daemons
	do
            echo "sudo pkill $var"
	    sudo pkill $var
	done
	sudo ip link del dev left

else
	for var in "$@"
	do
            echo "sudo pkill $var"
	    sudo pkill $var
	done
fi
