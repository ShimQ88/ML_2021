#include "semi_auto_filtering.h"

string Ori_Img_Name_To_Contour_Name(string img_name){
	int delimiter = img_name.find('.');
	string output_string;
	output_string = img_name.substr(0,delimiter);
	output_string = output_string+"_contour.jpg";
 	return output_string;
}

string Jpg_To_Txt_Directory(string img_name){
	int delimiter = img_name.find('.');
	string output_string="ROI_success/yolo/";
	img_name = img_name.substr(0,delimiter);
	output_string = output_string+img_name+".txt";
 	return output_string;
}
void Splite_Data_Yolo_and_Contour(ifstream &file_contour, string* yolo_data, string* contour_data, string* img_name ){
		
	string contour_line;
	getline(file_contour, contour_line);
	cout<<"contour_line: "<<contour_line<<endl;

	
	int delimiter = contour_line.find(';');
	string class_name = contour_line.substr(0,delimiter);//extract out class
	
	*yolo_data=class_name;
	*contour_data=class_name;
	
	*yolo_data=*yolo_data+' ';
	*contour_data=*contour_data+',';

	contour_line=contour_line.substr(delimiter+1);

	delimiter = contour_line.find(';');
	
	*img_name=contour_line.substr(0,delimiter);

	contour_line=contour_line.substr(delimiter+1);

	delimiter = contour_line.find(';');
	
	// *yolo_data=*yolo_data+*img_name;

	// *yolo_data=*yolo_data+',';

	string yo_info=contour_line.substr(0,delimiter);//extract out yolo information
	contour_line=contour_line.substr(delimiter+1);
	string contour_info=contour_line;
	
	*yolo_data=*yolo_data+yo_info;
	*contour_data=*contour_data+contour_info;
}

bool Find_Contour_and_Name_File(glob_t glob_sub_result, string *path_contour, string *path_name){
	bool is_find_contour_path=false;
	bool is_find_name_path=false;

	for(unsigned int z=0; z<glob_sub_result.gl_pathc; z++){
		int delimiter=0;
		string file_name=glob_sub_result.gl_pathv[z];
		/*process extract the file name*/
		while(delimiter!=-1){
	  		delimiter = file_name.find('/');
	  		file_name = file_name.substr(delimiter+1);
	  	}
	  	/*********************************/
		delimiter = file_name.find('.');
		string type= file_name.substr(delimiter+1);
		if(type.compare("txt")==0){
			delimiter = file_name.find('_');
			string test=file_name.substr(0,delimiter);
			if(test.compare("contour")==0){
				*path_contour=glob_sub_result.gl_pathv[z];
				is_find_contour_path=true;
			
			}else if(test.compare("name")==0){
				*path_name=glob_sub_result.gl_pathv[z];
				is_find_name_path=true;
			}
		}
	}

	if(is_find_contour_path==true&&is_find_name_path==true){
		return true;//find both of path
	}else{
		return false;//couldn't find both of path
	}
}

void Semi_Auto_Filtering_info(){
	cout<<"-----------------------------------------------------------------"<<endl;
	cout<<"-                    The instruction of key                     -"<<endl;
	cout<<"-----------------------------------------------------------------"<<endl;
	cout<<"Key of 'Enter' = save contour image and contour CEs information,"<<endl;
	cout<<"		            save original image and yolo informations(class number, x_center, y_center, width and height of file)."<<endl;
	cout<<"Key of 'y' = save only yolo information."<<endl;
	cout<<"Key of 'c' = save only contour information."<<endl;
	cout<<"Key of 'esc' = terminate program"<<endl;
	cout<<"Key of 'Backspace' = Remove the previous CE and Yolo information and go to previous process."<<endl;
	cout<<"-----------------------------------------------------------------"<<endl;
	cout<<"-                                                               -"<<endl;
	cout<<"-----------------------------------------------------------------"<<endl;
}

int Semi_Auto_Filtering(int argc,char *argv[]){
	
	int index_from_file;
	
	if(argc==3){//the case without index value
		index_from_file=0;
	}else if(argc==4){//the case of running with index value
		index_from_file=stoi(argv[3]);
	}else{//error
		return 10000000;
	}

	int size=strlen(argv[2]);
	cout<<"size:"<<size<<endl;
	// char path[size+10];

	char path_ori_img[size+8];
	char path_contour[size+8];

	// strcpy(path,argv[2]);
	strcpy(path_ori_img,argv[2]);
	strcat(path_ori_img,"/Image/*");

	strcpy(path_contour,argv[2]);
	strcat(path_contour,"/Contour/*");
	

	glob_t glob_result, glob_result_ori_img;
	glob(path_contour,GLOB_TILDE,NULL,&glob_result);
	glob(path_ori_img,GLOB_TILDE,NULL,&glob_result_ori_img);
	

	ofstream save_contour;
	// ofstream save_yolo;
	unsigned int i=index_from_file;
	// for(unsigned int i=index_from_file; i<glob_result.gl_pathc; i++){
		
	// cout << glob_result.gl_pathv[i] << endl;
	// cout << /glob_result_ori_img.gl_pathv[i] << endl;
	int sub_size_contour=strlen(glob_result.gl_pathv[i]);
	int sub_size_ori_img=strlen(glob_result_ori_img.gl_pathv[i]);
	// cout<<"sub_size_ori_img: "<<sub_size_ori_img<<endl;
	// cout<<"sub_size_contour: "<<sub_size_contour<<endl;

	char sub_path_contour[sub_size_contour+2];
	char sub_path_ori_img[sub_size_ori_img+2];

	strcpy(sub_path_contour, glob_result.gl_pathv[i]);
	strcat(sub_path_contour,"/*");

	strcpy(sub_path_ori_img, glob_result_ori_img.gl_pathv[i]);
	strcat(sub_path_ori_img,"/*");
	



	glob_t glob_sub_result;
	glob_t glob_sub_result_ori_img;
	glob(sub_path_contour,GLOB_TILDE,NULL,&glob_sub_result);

	// cout<<"sub_path_contour: "<<sub_path_contour<<endl;
	// cout<<"sub_path_ori_img: "<<sub_path_ori_img<<endl;
	glob(sub_path_ori_img,GLOB_TILDE,NULL,&glob_sub_result_ori_img);
	cout<<"glob_sub_result.gl_pathc:"<<glob_sub_result.gl_pathc<<endl;
	// Mat image_contour[glob_sub_result.gl_pathc-2];//except to contour and name files
	Mat image_contour;

	// Mat image_ori_img[glob_sub_result.gl_pathc-2];//except to contour and name files
	Mat image_ori_img;//except to contour and name files

	string p_contour;
	string path_name;
	ifstream file_contour;
	
	cout<<"haha"<<endl;

	// cout<<"glob_result.gl_pathv[i]: "<<glob_result.gl_pathv[i]<<endl;
	// cout<<"glob_sub_result+1: "<<glob_result.gl_pathv[i+1]<<endl;
	// cout<<"hoho"<<endl;
	bool is_press_wrong_or_back_btn=false;
	
	int lock_opener=0;
	
	string prev_file_name_ori_img;
	string prev_file_name_contour;
	string prev_txt_name;
	Mat prev_file_img_contour;
	Mat prev_file_img_ori;
	string yolo_data, contour_data, name_ori_img;
	int counter=0;
	for(unsigned int j=0; j<glob_sub_result.gl_pathc-2; j++){
		if(j==0&&is_press_wrong_or_back_btn==false){
			cout<<"glob_sub_result: "<<glob_sub_result.gl_pathv[j]<<endl;
			cout<<"glob_sub_result+1: "<<glob_sub_result.gl_pathv[j+1]<<endl;
			Find_Contour_and_Name_File(glob_sub_result, &p_contour, &path_name);
			cout<<"p_contour: "<<p_contour<<endl;
			file_contour.open(p_contour);
			
			if(file_contour.is_open()==true){
				cout<<"success to load contour file"<<endl;
			}else{
				cout<<"fail to load contour file"<<endl;
			}
		}

		if(j==0&&is_press_wrong_or_back_btn==false){
			save_contour.open("ROI_success/contour/final_contour.txt",std::ios_base::app);
		}

		
		if(is_press_wrong_or_back_btn==false){
			Splite_Data_Yolo_and_Contour(file_contour, &yolo_data, &contour_data,&name_ori_img);			
		}
		
		string name_contour=Ori_Img_Name_To_Contour_Name(name_ori_img);
		// cout<<"path_contour: "<<path_contour<<endl;
		// cout<<"path_name: "<<path_name<<endl;

		// cout << glob_sub_result.gl_pathv[j] << endl;

		image_contour=imread(glob_sub_result.gl_pathv[j],1);
		image_ori_img=imread(glob_sub_result_ori_img.gl_pathv[j],1);



		// glob_sub_result.gl_pathv[j]
		// if(is_press_wrong_or_back_btn==true){
			// imshow("image_contour", prev_file_img_contour);
			// imshow("image_ori", prev_file_img_ori);
		// }else{
		imshow("image_contour", image_contour);
		// imshow("image_ori", image_ori_img);	
		// }
		
		int key=waitKey(0);

		cout<<"the key: "<<key<<endl;
		// getchar();  
		string path1="ROI_success/contour/"+name_contour;
		string path2="ROI_success/yolo/"+name_ori_img;

		string y_path=Jpg_To_Txt_Directory(name_ori_img);
		lock_opener++;
		if(lock_opener>=2){
			is_press_wrong_or_back_btn=false;	
		}
		if(key==10||key==13){//when press enter

					//create and save yolo and contour information
			ofstream save_yolo;//yolo
		  	
		  	save_yolo.open(y_path);

		  	//writing process
			imwrite( path1, image_contour);
			imwrite( path2, image_ori_img);

			save_contour << contour_data;
			save_contour << endl;
			save_yolo << yolo_data;
			save_yolo << endl;

			save_yolo.close();
		}else if(key==255){//when press delete
			cout<<"you press delete"<<endl;
			// getchar();
			
		}else if(key==8){//when press backspace
			cout<<"you press backspace"<<endl;
			if(j==0){

			}else{
				is_press_wrong_or_back_btn=true;
				lock_opener=0;
				char contour_delete_file[prev_file_name_contour.size()+1];
				strcpy(contour_delete_file,prev_file_name_contour.c_str());

				char yolo_delete_file[prev_file_name_ori_img.size()+1];
				strcpy(yolo_delete_file,prev_file_name_ori_img.c_str());

				char yolo_delete_txt[prev_txt_name.size()+1];
				strcpy(yolo_delete_txt,prev_txt_name.c_str());

				int status_contour=remove(contour_delete_file);
				int status_yolo=remove(yolo_delete_file);
				int status_yolo_txt=remove(yolo_delete_txt);
				// cout<<"game: "<<game<<endl;
				// getchar();
				if(status_contour==0){
					cout<<"The contour file "<<prev_file_name_contour<<" is deleted"<<endl;
				}

				if(status_yolo==0){
					cout<<"The yolo file "<<prev_file_name_ori_img<<" is deleted"<<endl;
				}

				if(status_yolo_txt==0){
					cout<<"The yolo txt file "<<prev_txt_name<<" is deleted"<<endl;
				}

				j=j-2;
			}
			
		}else if(key==27){//when press esc
			cout<<"you press esc"<<endl;
			file_contour.close();
			save_contour.close();
			return 20000000;
			// getchar();
		}else if(key==121){//when press y:yolo
			ofstream save_yolo;//yolo
		  	save_yolo.open(y_path);

			imwrite( path2, image_ori_img);
			save_yolo<<yolo_data;
			save_yolo<<endl;
			save_yolo.close();
		}else if(key==99){//when press c:Contour

			imwrite( path1, image_contour);
			save_contour << contour_data;
			save_contour << endl;
		}else{
			cout<<"you press wrong button"<<endl;
			cout<<"try again"<<endl;
			is_press_wrong_or_back_btn=true;
			lock_opener=1;
			j--;
		}
		prev_file_name_contour=path1;
		prev_file_name_ori_img=path2;
		prev_txt_name=y_path;
		cout<<"key: "<<key<<endl;

	}
	file_contour.close();
	save_contour.close();
	return i+1;

}
