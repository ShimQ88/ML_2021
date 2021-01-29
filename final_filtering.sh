path_of_file=ROI_success/contour
# path_of_file=/home/q/Desktop/andre_file
# path_of_file=path_of_file3
echo $path_of_file

# The first process
./main -final_filtering

#read index
value=`cat Final_dataset/index.txt`

#read total number of item in directory
total=`ls $path_of_file -l | wc -l`
total=`expr $total - 4`

echo $total
echo $value

#loop work until index number over total number of item in directory
while [ $value -le $total ]
do
	./main -final_filtering
	value=`cat Final_dataset/index.txt`
	# value=`expr $value - 1`
	# echo $total
	# echo $value
done

echo "Terminate bash programming"