
#!/bin/bash
num=1
until [ $num -gt 5 ]
do
	echo "number : $num"
	((num++))
done
