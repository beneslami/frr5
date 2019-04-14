#list of arguments: $@
#number of arguments: $#

for var in "$@"
do
    echo "./$var/$var -d"
    ./$var/$var -d
done
