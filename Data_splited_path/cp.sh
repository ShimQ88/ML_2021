folder=splited
for i in `cat /home/kyu/Desktop/ML3/Data_splited_path/The_first_cycle_test0.txt`
do 
	echo "downloading" $i
	cp $i $folder

done