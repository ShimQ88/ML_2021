path_of_file=Yolo_txt

dataset_path=/home/kyu/Desktop/Final_dataset_collection/Extracted_by_Video/bird/img_false/

# path_of_file=/home/q/Desktop/andre_file
# path_of_file=path_of_file3
echo $path_of_file 

# The first process
./main -video_yolo $dataset_path

#read index
value=`cat ROI_images_from_yolo_txt/index.txt`

#read total number of item in directory
total=`ls $path_of_file -l | wc -l`
total=`expr $total`

echo $total
echo $value

#loop work until index number over total number of item in directory
while [ $value -le $total ]
do
	./main -video_yolo $dataset_path
	value=`cat ROI_images_from_yolo_txt/index.txt`
	# value=`expr $value - 1`
	# echo $total
	# echo $value
done

echo "Terminate bash programming"