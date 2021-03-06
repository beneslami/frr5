echo "!"
echo "! access-group commands"
for item in $( sudo vppctl show interface | tail -n +2 | awk '{print $1}');
do
	int_name=$(sudo vppctl show tap-inject | grep $item | awk '{print $3}')
	if test -z $int_name 
	then
		continue
	fi
	echo "interface $int_name"
	ind=$(sudo vppctl show interface | grep $item | awk '{print $2}')
#	echo "ind= $ind"
	acg=$(sudo vppctl show acl-plugin interface sw_if_index $ind | tail -n +2|awk '{print $3}')
	if test -z $acg
	then 
		 continue
	fi
	echo "access-group $acg"
done
