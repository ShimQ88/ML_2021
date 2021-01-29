path_of_file=ROI_images2/
number_count=ROI_images2/Contour

echo $path_of_file
# The first process
./main -filtering $path_of_file

#read index
value=`cat ROI_success/index.txt`

#read total number of item in directory
total=`ls $number_count -l | wc -l`

echo $total

total=`expr $total - 2`
#loop work until index number over total number of item in directory
while [ $value -le $total ]
do
	
	./main -filtering $path_of_file $value
	value=`cat ROI_success/index.txt`
done