path_of_file=~/Desktop/rodents_data
# path_of_file=~/Desktop/test_img
# path_of_file2=~/Desktop/ML2/sample_img
# path_of_file=/home/q/Desktop/andre_2/other
# path_of_file=/home/q/Desktop/andre_file
# path_of_file=path_of_file3
echo $path_of_file
# The first process
./main -detect $path_of_file

#read index
value=`cat ROI_images/index.txt`

#read total number of item in directory
total=`ls $path_of_file -l | wc -l`

echo $total

total=`expr $total - 5`
#loop work until index number over total number of item in directory
while [ $value != $total ]
do
	
	./main -detect $path_of_file $value
	value=`cat ROI_images/index.txt`
done
echo "Terminate bash programming"