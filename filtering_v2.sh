path_of_file=ROI_images2/Contour
# path_of_file=/home/q/Desktop/andre_file
# path_of_file=path_of_file3
echo $path_of_file

# The first process
./main -filtering_v2

#read index
value=`cat ROI_success/index.txt`

#read total number of item in directory
total=`ls $path_of_file -l | wc -l`

echo $total

# total=`expr $total - 2`
#loop work until index number over total number of item in directory
while [ $value -le $total ]
do
	./main -filtering_v2
	value=`cat ROI_success/index.txt`
	# value=`expr $value - 1`
	# echo $total
	# echo $value
done

echo "Terminate bash programming"