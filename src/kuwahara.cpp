#include "kuwahara.h"
// #include "contour.h"

#define the_number_of_file_in_folder 100

#define class_number "0"


int determine_frame_size(Mat input, Point obj_center, int frame_size, int append_size ){
	bool is_tri=false;
	// cout<<"input.size(): "<<input.size()<<endl;
	// cout<<"obj_center.x: "<<obj_center.x<<endl;
	// cout<<"obj_center.y: "<<obj_center.y<<endl;

 	if( (obj_center.x-frame_size < 0) || (obj_center.y-frame_size < 0 ) ){//the case1 of out of frame boundary
 		// cout<<"return case1"<<endl;
 		// return -1;
 		return frame_size*2;
	}

	if( (obj_center.x+frame_size > input.cols) || (obj_center.y+frame_size > input.rows) ){//the case2 of out of frame boundary
		// cout<<"return case2"<<endl;
 		// return -2;
 		return frame_size*2;
	}

	while(true){
		if( (obj_center.x-frame_size<=0)||(obj_center.y-frame_size<=0) ){//The frame size bigger than the left and top side margin so finish expanding more frame size
			// cout<<"return case3"<<endl;
			return frame_size*2;
		}

		if( (obj_center.x+frame_size >= input.cols)||(obj_center.y+frame_size >= input.rows) ){//The frame size bigger than the left and top side margin so finish expanding more frame size
			// cout<<"return case4"<<endl;
			return frame_size*2;
		}

		for(int k=obj_center.x-frame_size;k<obj_center.x+frame_size;k++){			
			for(int l=obj_center.y-frame_size;l<obj_center.y+frame_size;l++){

				// cout<<"k: "<<k<<endl;
				// cout<<"l: "<<l<<endl;
				// cout<<"obj_center.x: "<<obj_center.x<<endl;
				// cout<<"obj_center.y: "<<obj_center.y<<endl;
				if( (k==obj_center.x-frame_size)||(k==obj_center.x+frame_size-1)||(l==obj_center.y-frame_size)||(l==obj_center.y+frame_size-1) ){
					// Point t_pp;
					// t_pp.x=k;
					// t_pp.y=l;
					// circle(subtracted_frame,t_pp,3,Scalar(255,255,255));
					if((int)Mpixel(input,k,l)!=0 ){
						is_tri=true;
					}
				}


			}	
		}

		
		
		frame_size=frame_size+append_size;

		// cout<<"frame_size: "<<frame_size<<endl;
		if(is_tri==false){
			// cout<<"normal case: "<<endl;
			// cout<<"frame_size: "<<frame_size*2<<endl;
			break;
		}
		is_tri=false;

		// if(frame_size>=250){//maximum size
		if(frame_size>=250){//maximum size this is for testing
			return frame_size*2;
			cout<<"error for size"<<endl;
		}
	}
	return frame_size*2;
}

bool creating_folder(string folder_name){
	int check;
	int size = folder_name.length(); 
	
    char dirname[size+1];
    strcpy(dirname, folder_name.c_str()); 
  
    check = mkdir(dirname,0777); 
  
    // check if directory is created or not 
    if(!check){ 
        printf("The folder %s Directory created\n", dirname);
        return true;
    }else{
    	printf("Fail to create %s Directory or the folder %s exist\n ", dirname, dirname);
        return false;
    }
}

bool creating_directories(string folder_name_roi, string folder_name_contour, string folder_name_image){
	int i=0;
	while(1){
		int size;
		string folder_name;
		if(i==0){
			size=folder_name_roi.length();
			folder_name=folder_name_roi;
		}else if(i==1){
			size=folder_name_contour.length();
			folder_name=folder_name_contour;
		}else if(i==2){
			size=folder_name_image.length();
			folder_name=folder_name_image;
		}else{
			cout<<"index error"<<endl;
			return false;
		}
		char dirname[size+1];
    	strcpy(dirname, folder_name.c_str());
		int check = mkdir(dirname,0777);
		if(!check){ 
        	printf("The folder %s Directory created\n", dirname);
    	}else{
    		printf("Fail to create %s Directory or the folder %s exist\n ", dirname, dirname);
        	return false;
        }
		if(i==2){break;}//loop breaker
		i++;
	}
  	return true;//success making three files
}



string write_img(Mat saving_img, string folder_name, string file_name, string file_type){
  	int delimiter=0;
  	while(delimiter!=-1){
  		delimiter = file_name.find('/');
  		file_name = file_name.substr(delimiter+1);
  	}
  	file_name = file_name.substr(0,file_name.find('.'));
  	cout<<"file_name: "<<file_name<<endl;

  	string output_name=file_name+file_type;
	string path_ROI=folder_name+file_name+file_type;

	imwrite( path_ROI, saving_img);
	return output_name;
}

int check_total_numb(int index, glob_t glob_result){
	bool is_the_end_numb_in_a_row=true;
	int total_numb=0;
	while(is_the_end_numb_in_a_row==true){
  		cout<<"index: "<<index<<endl;
  		if(index==glob_result.gl_pathc-1||(index==glob_result.gl_pathc-2)){
  			total_numb++;
  			break;
  		}
  		is_the_end_numb_in_a_row=check_numb_in_a_row(glob_result.gl_pathv[index], glob_result.gl_pathv[index+1]);
  		index++;
  		total_numb++;
  		if(total_numb>20){break;}
  	}
  	// cout<<"total_numb: "<<total_numb<<endl;

  	// for(int l=0;l<total_numb;l++){
  	// 	cout << glob_result.gl_pathv[i+l] << endl;
  	// }
  	return total_numb;
}


string Info_String_Merge(string class_numb, string dir_name , float x, float y, float w, float h, string CE){
	// string CE_val=ku_ROI.get_contour_txt();
	string text_info;
	string final_string=class_numb;
	final_string=final_string+';';
	final_string=final_string+dir_name;
	final_string=final_string+';';
	

	string yo_x=to_string(x);
	string yo_y=to_string(y);
	string yo_w=to_string(w);
	string yo_h=to_string(h);
	// cout<<"yo_x: "<<yo_x<<endl;
	// cout<<"float_yo_x: "<<ku_ROI.get_yolo_x()<<endl;
	// final_string=final_string+',';
	final_string=final_string+yo_x;
	// cout<<"final_string: "<<final_string<<endl;
	// cout<<"yo_x: "<<yo_x<<endl;
	final_string=final_string+',';
	final_string=final_string+yo_y;
	// cout<<"final_string: "<<final_string<<endl;
	final_string=final_string+',';
	final_string=final_string+yo_w;
	// cout<<"final_string: "<<final_string<<endl;
	final_string=final_string+',';
	final_string=final_string+yo_h;
	// cout<<"final_string: "<<final_string<<endl;
	final_string=final_string+';';
	final_string=final_string+CE;

	return final_string;
}

Mat Cropping_ROI(Mat imput_image,Point center_of_object, int kernel_size){
	Point temp_p;
	temp_p.x=center_of_object.x-(kernel_size/2);
	temp_p.y=center_of_object.y-(kernel_size/2);
	

	if(temp_p.x<0){
		temp_p.x=0;
	}

	if(temp_p.y<0){
		temp_p.y=0;	
	}

	if(temp_p.x+kernel_size>imput_image.cols){
		temp_p.x=imput_image.cols-kernel_size;
	}

	if(temp_p.y+kernel_size>imput_image.rows){
		temp_p.y=imput_image.rows-kernel_size;	
	}
	// cout<<"temp_p.x:"<<temp_p.x<<endl;
	// cout<<"temp_p.y:"<<temp_p.y<<endl;


	Mat ROI=imput_image(Rect(temp_p.x,temp_p.y,kernel_size,kernel_size)).clone();

	// rectangle(imput_image,Rect(temp_p.x,temp_p.y,kernel_size,kernel_size),Scalar(0,255,0),2);
	// Mat ROI=imput_image(Rect(abs(temp_p.x),abs(temp_p.y),abs(temp_p.y-center_of_object.y),abs(temp_p.y-center_of_object.y))).clone();
	return ROI;
}



bool check_numb_in_a_row(string prev_name, string cur_name){
	string str_prev_name=prev_name.substr(prev_name.size()-8,4);
	string str_cur_name=cur_name.substr(cur_name.size()-8,4);
	int prev_img_numb=stoi(str_prev_name);
	int cur_img_numb=stoi(str_cur_name);
	int diff_numb=cur_img_numb-prev_img_numb;
	
	if(diff_numb==1){
		return true;
	}else{
		return false;
	}

}



void thresholding_image(Mat image, int value, bool inverted, int window_size){
	if(inverted==true){
		for(int x=(window_size/2); x<image.cols-(window_size/2);x++){
			for(int y=(window_size/2); y<image.rows-(window_size/2);y++){
				if((Mpixel(image,x,y)>=value)){
					Mpixel(image,x,y)=255;
				}else{
					Mpixel(image,x,y)=0;
				}
			}
		}	
	}else{
		for(int x=(window_size/2); x<image.cols-(window_size/2);x++){
			for(int y=(window_size/2); y<image.rows-(window_size/2);y++){				
				if(Mpixel(image,x,y)>=value){
					Mpixel(image,x,y)=0;
				}else{
					Mpixel(image,x,y)=255;
				}
			}
		}
	}	
}


float binary_histogram(Mat image, int window_size, double histogram[]){
	for(int i=0;i<256;i++){
		histogram[i]=0;
	}

	// for(int x=( (window_size/2)+(image.cols*2/5) ); x<image.cols-( (window_size/2) +(image.cols*2/5));x++){
	// 	for(int y=((window_size/2) +(image.rows*2/5)); y<image.rows-( (window_size/2) +(image.rows*2/5) );y++){
	for(int x=(window_size/2); x<image.cols-(window_size/2);x++){
		for(int y=(window_size/2); y<image.rows-(window_size/2);y++){	
			int hist_val=(int)Mpixel(image,x,y);
			histogram[hist_val]++;
			// Point temp_p;
			// temp_p.x=x;
			// temp_p.y=y;
			// circle(image,temp_p,1,Scalar(255,255,255));
		}
	}	
	float total_sum=0;
	float total=0;
	int biggest=-9999;
	int big_i=-1;
	for(int i=0;i<100;i++){
		// if(i>0 && i<51){
			if(biggest<histogram[i]){
				biggest=histogram[i];
				big_i=i;
			}
		// }
		total_sum=total_sum+(histogram[i]*i);
		total=total+histogram[i];
		// cout<<i<<": "<<histogram[i]<<endl;
		// cout<<"total_sum: "<<total_sum<<endl;
	}
	// total_sum=total_sum/256;
	// return big_i;
	return total_sum/total;
}

Point draw_rect_box(Mat input_image, Point* p1, Point* p2, int loop_number){
	int big_blob_i = -1;
	int big_blob_len = -9999;
	for(int i=0;i<loop_number;i++){
		if((p1[i].x!=9999)&&(p1[i].y!=9999)&&(p2[i].x!=-9999)&&(p2[i].y!=-9999)){
			int temp_value = abs( (p2[i].x-p1[i].x)*(p2[i].y-p1[i].y) );
			if(big_blob_len < temp_value){
				big_blob_len= temp_value;
				big_blob_i = i;
			}
		}
	}
	Rect rec(p1[big_blob_i].x-2, p1[big_blob_i].y-2, p2[big_blob_i].x-p1[big_blob_i].x+2, p2[big_blob_i].y-p1[big_blob_i].y+2);
	
	Point center_of_rect = (rec.br() + rec.tl())*0.5;

    // circle(input_image,center_of_rect,1,Scalar(0,0,255));
	// rectangle(input_image,rec,Scalar(0,255,0),2);
	// cout<<"centre: "<<center_of_rect.x<<", "<<center_of_rect.y<<endl;
	// cout<<"p1: "<<p1[big_blob_i].x<<", "<<p1[big_blob_i].y<<endl;
	// cout<<"p2: "<<p2[big_blob_i].x<<", "<<p2[big_blob_i].y<<endl;

	return center_of_rect;
}



int blob(Mat image2, Mat image3,Point *p1_rec, Point *p2_rec){

	int counterColour=0;

	// cout<<"image2.cols:"<<image2.cols<<endl;
	// cout<<"image2.rows:"<<image2.rows<<endl;
	// getchar();
	vector< tuple<int, int> > SET[image2.cols];
	// vector< tuple<int, int> > SET[2000];
	
	int A[image2.cols][image2.rows];

	
	int s1,s2;

	for(int x=0; x<image2.cols;x++){
		for(int y=0; y<image2.rows;y++){
			A[x][y]=-1;
		}
	}
	// cout<<"P1"<<endl;
	int i=0;
	int index=0, counter=-1;
	bool is_s1_in=false;
	// for(int y=1; y<image2.rows;y++){
	for(int y=image2.rows/50; y<image2.rows;y++){//Ignoring the numbers from top section
		for(int x=1; x<image2.cols;x++){
			// cout<<"counter: "<<counter<<endl;
			if(Mpixel(image2,x,y)!=0){
				if((Mpixel(image2,x-1,y)!=0)||(Mpixel(image2,x,y-1)!=0)){
					s1=A[x-1][y];
					s2=A[x][y-1];
					if(s1!=-1){
						SET[s1].push_back(make_tuple(x,y));
						
						A[x][y]=s1;
						is_s1_in=true;
					}
					if(s2!=-1){
						SET[s2].push_back(make_tuple(x,y));
						A[x][y]=s2;
						is_s1_in=false;

					}
					if((s1!=s2)&&(s1!=-1)&&(s2!=-1)){
						SET[s1].insert(SET[s1].end(),SET[s2].begin(),SET[s2].end());						
						for (int j = 0; j < SET[s2].size(); j++ ){
							A[get<0>(SET[s2][j])][get<1>(SET[s2][j])]=s1;
						}				
						SET[s2].clear();
					}
					is_s1_in=false;

				}else{
					counter++;
					SET[counter].push_back(make_tuple(x,y));
					A[x][y]=counter;
				}

			}

			// if(counter>150){return -100;}//This is using for avoiding segmental fault.
			if(counter>150){break;}//This is using for avoiding segmental fault.
		}


	}

	// cout<<"P2"<<endl;
	//colour counting
	// for(int i=0;i<image2.cols;i++){
	// 	colour_count(SET[i]);
	// }

	//put random colour to image3
	
	counter++;
	
	

	// Point p1_rec[counter];
	// Point p2_rec[counter];

	// p1_rec= new Point[counter];
	// p2_rec= new Point[counter];

	for(int i=0;i<counter;i++){
		p1_rec[i].x=9999;
		p1_rec[i].y=9999;

		p2_rec[i].x=-9999;
		p2_rec[i].y=-9999;
	}
	
	// cout<<"P3"<<endl;

	int rand_numb1 = rand() % 100; 
	int rand_numb2 = rand() % 100; 
	int rand_numb3 = rand() % 100;

	for(int y=image2.rows/50; y<image2.rows;y++){//Ignoring the numbers from top section
		for(int x=0; x<image2.cols;x++){
			if(y==0){
				if(SET[x].empty()==true){

				}else{
					counterColour++;
				}
				// colour_count(SET[x]);
			}
			if(A[x][y]==-1){
				pixelB(image3,x,y)=0;
				pixelG(image3,x,y)=0;
				pixelR(image3,x,y)=0;	
			}
			if(A[x][y]!=-1){	
				pixelB(image3,x,y)=(A[x][y]+1)*rand_numb1;
				pixelG(image3,x,y)=(A[x][y]+1)*rand_numb2;
				pixelR(image3,x,y)=(A[x][y]+1)*rand_numb3;

				if(x < p1_rec[ (A[x][y]) ].x){
					p1_rec[ (A[x][y]) ].x=x;
				}

				if(y < p1_rec[ (A[x][y]) ].y){
					p1_rec[ (A[x][y]) ].y=y;
				}


				if(x > p2_rec[ (A[x][y]) ].x){
					p2_rec[ (A[x][y]) ].x=x;
				}

				if(y > p2_rec[ (A[x][y]) ].y){
					p2_rec[ (A[x][y]) ].y=y;
				}

			}
		}
	}
	draw_rect_box(image3, p1_rec, p2_rec, counter);
	// cout<<"P4"<<endl;
	// for(int i=0;i<counter;i++){
	// 	if((p1_rec[i].x!=9999)&&(p1_rec[i].y!=9999)&&(p2_rec[i].x!=-9999)&&(p2_rec[i].y!=-9999)){
	// 		Rect rec(p1_rec[i].x, p1_rec[i].y, p2_rec[i].x-p1_rec[i].x+1, p2_rec[i].y-p1_rec[i].y+1);
	// 		rectangle(image3,rec,Scalar(0,255,0),1);
	// 	}
	// }
	// getchar();
	// for(int i=0;i<20;i++){
	// 	cout<<p1_rec->x<<endl;
	// }


	// return counterColour;
	return counter;
}

void Grey_to_Color(Mat3b source_image, Mat filtered_image,Mat3b output_image){
	for(int x=0; x<source_image.cols;x++){
		for(int y=0; y<source_image.rows;y++){
			if(Mpixel(filtered_image,x,y)!=0){//when pixel is not zero
				pixelB(output_image,x,y)=pixelB(source_image,x,y);
				pixelG(output_image,x,y)=pixelG(source_image,x,y);
				pixelR(output_image,x,y)=pixelR(source_image,x,y);
			}
		}	
	}
}
void Determining_ROI_Size(Mat source_image, int *small_x, int *small_y,int *large_x,int *large_y){
	//This is function for finding coordinates (p1 and p2) of rectangle before cropping Region of Interest area

	// * p1(small_x,small_y)
	// @ p2(large_x,large_y)
	//  |*| | |        
	//	| | | |
	//	| | |@|


	*small_x=9999;
	*small_y=9999;
	*large_x=-9999;
	*large_y=-9999;
	for(int x=0; x<source_image.cols;x++){
		for(int y=0; y<source_image.rows;y++){
			if(Mpixel(source_image,x,y)!=0){//when pixel is not zero
				if(x>*large_x){
					*large_x=x;
				}
				if(y>*large_y){
					*large_y=y;
				}
				if(x<*small_x){
					*small_x=x;
				}
				if(y<*small_y){
					*small_y=y;
				}
			}
		}	
	}
	if((*small_x==9999)||(*small_y==9999)||(*large_x==-9999)||(*large_y==-9999)){
		*small_x=0;
		*small_y=0;
		*large_x=0;
		*large_y=0;
	}
}


void Image_stitching(Mat source_image,Mat filtered_image,Mat output_image){
	
	//vertical way
	int sm[source_image.cols];
	int lar[source_image.cols];
	for(int x=0; x<source_image.cols;x++){
		sm[x]=9999;
		lar[x]=-9999;
		for(int y=0; y<source_image.rows;y++){
			if(Mpixel(filtered_image,x,y)<35){//thresholding
											  //this is better to do seperate but I put on here for performancing
				Mpixel(filtered_image,x,y)=0;
			}

			if((int)Mpixel(filtered_image,x,y)!=0){//The process for detecting the beginning point of pixel and the last point of pixel of each column. 
				if(y<sm[x]){
					sm[x]=y;
				}
				if(y>lar[x]){
					lar[x]=y;
				}
			}
			if(x!=0){
				if( (y>=sm[x-1])&&(y<=lar[x-1])){//The vertical way copy all previous line pixels from the point of the first detected pixcel to the last.  
					Mpixel(output_image,x-1,y)=Mpixel(source_image,x-1,y);
				}
			}
			if( (x==source_image.cols-1)&&(y==source_image.rows-1) ){//The same process of upper process but the last one is exception.
				for(int a=sm[x]; a<lar[x];a++){
					Mpixel(output_image,x,a)=Mpixel(source_image,x,a);	
				}						
			}

		}
	}
	//horizontal way
	int sm2[source_image.rows];
	int lar2[source_image.rows];
	for(int x=0; x<source_image.rows;x++){
		sm2[x]=9999;
		lar2[x]=-9999;
		for(int y=0; y<source_image.cols;y++){
			if((int)Mpixel(output_image,y,x)!=0){//The process for detecting the beginning point of pixel and the last point of pixel of each row.
				if(y<sm2[x]){
					sm2[x]=y;
				}
				if(y>lar2[x]){
					lar2[x]=y;
				}
			}
			if(x!=0){
				if( (y>sm2[x-1])&&(y<lar2[x-1])  ){//The horizontal way copy all previous line pixels from the point of the first detected pixcel to the last.  
					Mpixel(output_image,y,x-1)=Mpixel(source_image,y,x-1);	
				}							
			}
			if( (x==source_image.rows-1)&&(y==source_image.cols-1) ){//the case of last
				for(int a=sm2[x]; a<lar2[x];a++){
					Mpixel(output_image,a,x)=Mpixel(source_image,a,x);	
				}						
			}
		}
	}
}
void median_filter(Mat image1,Mat image2,int window_size){
    //picture elements(pels)
    int function_size_input=window_size;//this is for window size

    int picture_x_size=image1.cols;
    int picture_y_size=image1.rows;
    int mdn;//median value in a window
    int ltmdn=0;// number of pels having gray levels less than mdn in a window
    int window_x_size=function_size_input;
    int window_y_size=function_size_input;
    //int hist[window_x_size*window_y_size];
    int index_for_array=0;
    int count_of_less_than_median=0;
    int median_value;
    int left_column[window_y_size];
    int right_column[window_y_size];
    // int left_column[window_y_size-1];
    // int right_column[window_y_size-1];
    int hist_index=0;
    int g1;

    int th=(window_x_size*window_y_size/2);
    
    // for(int i=0;i<picture_y_size;i++){
    //      Mpixel(image2,637,i)=255;
    //  }
    
    for(int i=(window_x_size/2);i<picture_x_size-(window_x_size/2);i++){
        
        int hist[256];
        for(int q=0;q<256;q++){
            hist[q]=0;
        }

        int index_for_hist=0;
        int y_sizeof=i+window_y_size;
        int x_sizeof=(window_x_size/2)+window_x_size;
        for(int a=i;a<y_sizeof;a++){
            for(int b=(window_x_size/2);b<x_sizeof;b++){
                index_for_hist=(int)Mpixel(image1,a,b);
                hist[index_for_hist]=hist[index_for_hist]+1;
            }
        }


        int counter_for_find_median=(window_x_size*window_y_size/2)+1;
        int counter_for_find_less_than_median=0;

        for(int z=0;z<256;z++){
            if(hist[z]!=0){ 
                counter_for_find_median=counter_for_find_median-hist[z];
                if(counter_for_find_median<=0){
                    median_value=z;
                    mdn=median_value;
                    break;
                }else{
                    counter_for_find_less_than_median
                    =counter_for_find_less_than_median+hist[z];
                }
            }       
        }

        ltmdn=counter_for_find_less_than_median;

        //Mpixel(image2,i,(window_y_size/2))=mdn;

        for(int j=(window_y_size/2)+1;j<picture_y_size-(window_y_size/2);j++){//j indicates picture column number

    
            int index_for_left_column=0;
            int index_for_right_column=0;

            for(int l=i;l<i+window_x_size;l++){
                left_column[index_for_left_column]=(int)Mpixel(image1,l,j);
                index_for_left_column++;

                right_column[index_for_right_column]=(int)Mpixel(image1,l,j+window_x_size-1);               
                index_for_right_column++;   
            }
            
                    
            

            for(int k=0;k<window_y_size;k++){
                g1=left_column[k];
                hist[g1]=hist[g1]-1;
                if(g1<mdn){
                    ltmdn=ltmdn-1;
                }
                g1=right_column[k];
                hist[g1]=hist[g1]+1;
                if(g1<mdn){
                    ltmdn=ltmdn+1;
                }

                if(ltmdn>th){
                    while(true){
                        mdn=mdn-1;
                        ltmdn=ltmdn-hist[mdn];
                        if(ltmdn<=th){
                            break;
                        }
                    }
                }else{
                    while(ltmdn+hist[mdn]<=th){
                        ltmdn=ltmdn+hist[mdn];
                        mdn=mdn+1;
                    }
                }
            }
            Mpixel(image2,i,j)=mdn; //determine pixel
        }
    }

    for(int j=0;j<window_size/2+1;j++){ 
        for(int i=0;i<picture_y_size;i++){
            Mpixel(image2,j,i)=0;
            Mpixel(image2,picture_x_size-1-j,i)=0;
        }
    }
    for(int j=0;j<window_size/2+1;j++){ 
        for(int i=window_size/2;i<picture_x_size-(window_size/2);i++){
            Mpixel(image2,i,j)=0;
            Mpixel(image2,i,picture_y_size-1-j)=0;
        }
    }

}

int FindTheLargestContour2(std::vector<vector<Point>>contours){
    int largestcontour=0;
    long int largestsize=0;
    for(int i=0;i<contours.size();i++){
        if(largestsize<contours[i].size()){
            largestsize=contours[i].size();
            largestcontour=i;
        }
    }
    return largestcontour;
}


void Integral_test(Mat image1, double **integral_image, double **squared_integral_image){
//This is the function for printing out 1/20 size of original metrices values and summed-area table values.
//Checking whether properly summed or not. 	

	/*
		1/20 size of original metrices values
	*/
	cout<<"The First image"<<endl;
	for(int i=0;i<image1.cols;i++){
		cout<<"[ ";
		for(int j=0;j<image1.rows;j++){
			cout<<(int)Mpixel(image1,i,j)<<" ";
		}
		cout<<" ]"<<endl;
	}
	///////////////////////////////////////////

	/*
		summed-area table values
	*/
	cout<<"The Second image"<<endl;
	for(int i=0;i<image1.cols;i++){
		cout<<"[ ";
		for(int j=0;j<image1.rows;j++){
			cout<<integral_image[i][j]<<" ";
		}
		cout<<" ]"<<endl;
	}


	cout<<"The squared image"<<endl;
	for(int i=0;i<image1.cols/20;i++){
		cout<<"[ ";
		for(int j=0;j<image1.rows/20;j++){
			cout<<squared_integral_image[i][j]<<" ";
		}
		cout<<" ]"<<endl;
	}
	//////////////////////////////////////////
}

void Integral_Gray_Initialize(Mat image1, double **integral_image,double **squared_integral_image){
	//This is the function for creating summed-area table in gray image.
	for(int i=0;i<image1.cols+1;i++){
		for(int j=0;j<image1.rows+1;j++){
			if((i==0)||(j==0)){//The value of the first row and column is zero. 
				integral_image[i][j]=0;
				squared_integral_image[i][j]=0;
			}else{
				integral_image[i][j]=integral_image[i-1][j]+integral_image[i][j-1]-integral_image[i-1][j-1]+(int)Mpixel(image1,i-1,j-1);
				squared_integral_image[i][j]=squared_integral_image[i-1][j]+squared_integral_image[i][j-1]-squared_integral_image[i-1][j-1]
				+(int)Mpixel(image1,i-1,j-1)*(int)Mpixel(image1,i-1,j-1);
			}
			
		}
	}
}



void Integral_Color_Initialize(Mat3b image1, double **integral_image_B, double **integral_image_G, double **integral_image_R, double **squared_integral_image_B, double **squared_integral_image_G, double **squared_integral_image_R){
	//This is the function for creating summed-area table in color image.
	for(int i=0;i<image1.cols;i++){
		for(int j=0;j<image1.rows;j++){
			if((i==0)||(j==0)){//The value of the first row and column is zero.
				integral_image_B[i][j]=0;
				integral_image_G[i][j]=0;
				integral_image_R[i][j]=0;

				squared_integral_image_B[i][j]=0;
				squared_integral_image_G[i][j]=0;
				squared_integral_image_R[i][j]=0;
			}else{
				integral_image_B[i][j]=integral_image_B[i-1][j]+integral_image_B[i][j-1]-integral_image_B[i-1][j-1]+(int)pixelB(image1,i-1,j-1);
				integral_image_G[i][j]=integral_image_G[i-1][j]+integral_image_G[i][j-1]-integral_image_G[i-1][j-1]+(int)pixelG(image1,i-1,j-1);
				integral_image_R[i][j]=integral_image_R[i-1][j]+integral_image_R[i][j-1]-integral_image_R[i-1][j-1]+(int)pixelR(image1,i-1,j-1);

				squared_integral_image_B[i][j]=squared_integral_image_B[i-1][j]+squared_integral_image_B[i][j-1]-squared_integral_image_B[i-1][j-1]+(int)pixelB(image1,i-1,j-1)*(int)pixelB(image1,i-1,j-1);
				squared_integral_image_G[i][j]=squared_integral_image_G[i-1][j]+squared_integral_image_G[i][j-1]-squared_integral_image_G[i-1][j-1]+(int)pixelG(image1,i-1,j-1)*(int)pixelG(image1,i-1,j-1);
				squared_integral_image_R[i][j]=squared_integral_image_R[i-1][j]+squared_integral_image_R[i][j-1]-squared_integral_image_R[i-1][j-1]+(int)pixelR(image1,i-1,j-1)*(int)pixelR(image1,i-1,j-1);
			}
			
		}

	}
}


void Filter_Gray_Integral(Mat image1, Mat image2, double** integral_image,double** squared_integral_image, int window_size){
	//This is function for applying the Kuwahara filter to image2, it must require summed-area table.

	//Mat image1 is the source image with gray scale. 
	//Mat image2 is the final output which is applied to Kuwahara filter in gray scale image.
	//double** integral_image is 2-dimensional array which stores the summed-area table.
	//int window_size is the size of window it is big more blurly when the value is high.

	int picture_x_size=image1.cols;//the x-axis length of source image.
	int picture_y_size=image1.rows;//the y-axis length of source image.
	int window_x_size=window_size;//the x-axis length of window.
	int window_y_size=window_size;//the y-axis length of window.


	/* The function of Kuhawara filter*/
			
	// mk=1(n+1)×(n+1)×∑(x,y)∈θkφ(f(x,y)

	// k∈{0,1,2,3},fis the source image function,
	// f(x,y)is the value of the pixel at coordinates(x,y),
	// φis a function calculating the value of a particular pixel,
	// 1/(n+1)×(n+1)is the number of pixels in the current area,
	// n is the value obtained directly from the filter windowsize.

	// reference by 
	// https://link-springer-com.ezproxy.massey.ac.nz/content/pdf/10.1007%2Fs11760-015-0791-3.pdf
	// page 665
	/**********************************/

	for(int i=(window_x_size/2);i<picture_x_size-(window_x_size/2+1);i++){
		for(int j=(window_y_size/2);j<picture_y_size-(window_y_size/2+1);j++){
			
			int f[4];
			int s_f[4];
			int result[4];
			int small_window_size=(window_x_size/2)*(window_y_size/2);

			int i1=i;
			int j1=j;

			int i2=i+(window_x_size/2);
			int j2=j;

			int i3=i;
			int j3=j+(window_y_size/2);

			int i4=i+(window_x_size/2);
			int j4=j+(window_y_size/2);

			int mean_f[4];

			



			// f[0]=integral_image[i1][j1]-integral_image[i1-(window_x_size/2)][j1]
			// -integral_image[i1][j1-(window_y_size/2)]+integral_image[i1-(window_x_size/2)][j1-(window_y_size/2)];


			f[0]=integral_image[i1-1][j1]-integral_image[i1][j1-1]
			-integral_image[i1-1][j1-1]+(int)Mpixel(image1,i1-1,j1-1);


			// s_f[0]=squared_integral_image[i1][j1]-squared_integral_image[i1-(window_x_size/2)][j1]
			// -squared_integral_image[i1][j1-(window_y_size/2)]+squared_integral_image[i1-(window_x_size/2)][j1-(window_y_size/2)];


			cout<<"s_f: "<<s_f[0]<<endl;
			cout<<"f[0]: "<<f[0]<<endl;
			// f[0]=f[0]/small_window_size;
			// cout<<"s_f: "<<s_f[0]<<endl;
			// cout<<"(f[0]*f[0])/small_window_size): "<<((f[0]*f[0])/small_window_size)<<endl;
			
			// result[0]=(f[0]*f[0]-((f[0]*f[0])/small_window_size))/small_window_size;

			result[0]=(s_f[0]-((f[0]*f[0])/small_window_size));
			// result[0]=sqrt(result[0]);


			f[1]=integral_image[i2][j2]-integral_image[i2-(window_x_size/2)][j2]
			-integral_image[i2][j2-(window_y_size/2)]+integral_image[i2-(window_x_size/2)][j2-(window_y_size/2)];

			s_f[1]=squared_integral_image[i2][j2]-squared_integral_image[i2-(window_x_size/2)][j2]
			-squared_integral_image[i2][j2-(window_y_size/2)]+squared_integral_image[i2-(window_x_size/2)][j2-(window_y_size/2)];
			// f[1]=f[1]/small_window_size;
			


			cout<<"s_f: "<<s_f[1]<<endl;
			cout<<"f[1]: "<<f[1]<<endl;
			// result[1]=(f[1]*f[1]-((f[1]*f[1])/small_window_size))/small_window_size;
			result[1]=(s_f[1]-((f[1]*f[1])/small_window_size));
			// result[1]=sqrt(result[1]);


			f[2]=integral_image[i3][j3]-integral_image[i3-(window_x_size/2)][j3]
			-integral_image[i3][j3-(window_y_size/2)]+integral_image[i3-(window_x_size/2)][j3-(window_y_size/2)];

			s_f[2]=squared_integral_image[i3][j3]-squared_integral_image[i3-(window_x_size/2)][j3]
			-squared_integral_image[i3][j3-(window_y_size/2)]+squared_integral_image[i3-(window_x_size/2)][j3-(window_y_size/2)];
			// f[2]=f[2]/small_window_size;


			cout<<"s_f: "<<s_f[2]<<endl;
			cout<<"f[2]: "<<f[2]<<endl;
			
			// result[2]=(f[2]*f[2]-((f[2]*f[2])/small_window_size))/small_window_size;
			result[2]=(s_f[2]-((f[2]*f[2])/small_window_size));
			// result[2]=sqrt(result[2]);


			f[3]=integral_image[i4][j4]-integral_image[i4-(window_x_size/2)][j4]
			-integral_image[i4][j4-(window_y_size/2)]+integral_image[i4-(window_x_size/2)][j4-(window_y_size/2)];

			s_f[3]=squared_integral_image[i4][j4]-squared_integral_image[i4-(window_x_size/2)][j4]
			-squared_integral_image[i4][j4-(window_y_size/2)]+squared_integral_image[i4-(window_x_size/2)][j4-(window_y_size/2)];
			// f[3]=f[3]/small_window_size;


			cout<<"s_f: "<<s_f[3]<<endl;
			cout<<"f[3]: "<<f[3]<<endl;

			
			// result[3]=(f[3]*f[3]-((f[3]*f[3])/small_window_size))/small_window_size;
			result[3]=(s_f[3]-((f[3]*f[3])/small_window_size));
			// result[3]=sqrt(result[3]);
			
			/*
				The process finding the smallest value between f[0] and f[3].
			*/
			int final=9999;
			for(int l=0;l<4;l++){
				cout<<"result: "<<result[l]<<endl;
				// if(final>f[l]){
				// 	final=f[l];
				// }
				if(final>result[l]){
					final=result[l];
				}
			}
			cout<<"final: "<<final<<endl;
			getchar();

			Mpixel(image2,i,j)=final;
			//////////////////////////// 

		}
	}

}

void Filter_Gray_Integral2(Mat image1, Mat image2, double** integral_image,double** squared_integral_image, int window_size){
	//This is function for applying the Kuwahara filter to image2, it must require summed-area table.

	//Mat image1 is the source image with gray scale. 
	//Mat image2 is the final output which is applied to Kuwahara filter in gray scale image.
	//double** integral_image is 2-dimensional array which stores the summed-area table.
	//int window_size is the size of window it is big more blurly when the value is high.

	int picture_x_size=image1.cols;//the x-axis length of source image.
	int picture_y_size=image1.rows;//the y-axis length of source image.
	int window_x_size=window_size;//the x-axis length of window.
	int window_y_size=window_size;//the y-axis length of window.


	/* The function of Kuhawara filter*/
			
	// mk=1(n+1)×(n+1)×∑(x,y)∈θkφ(f(x,y)

	// k∈{0,1,2,3},fis the source image function,
	// f(x,y)is the value of the pixel at coordinates(x,y),
	// φis a function calculating the value of a particular pixel,
	// 1/(n+1)×(n+1)is the number of pixels in the current area,
	// n is the value obtained directly from the filter windowsize.

	// reference by 
	// https://link-springer-com.ezproxy.massey.ac.nz/content/pdf/10.1007%2Fs11760-015-0791-3.pdf
	// page 665
	/**********************************/

	for(int i=(window_x_size/2);i<picture_x_size-(window_x_size/2+1);i++){
		for(int j=(window_y_size/2);j<picture_y_size-(window_y_size/2+1);j++){
			
			int f[4];
			int s_f[4];
			int result[4];
			int small_window_size=(window_x_size/2+1)*(window_y_size/2+1);

			int i0=i;
			int j0=j;

			int i1=i+(window_x_size/2);
			int j1=j;

			int i2=i;
			int j2=j+(window_y_size/2);

			int i3=i+(window_x_size/2);
			int j3=j+(window_y_size/2);

			int mean_f[4];

			// cout<<"small_window_size: "<<small_window_size<<endl;




			// f[0]=integral_image[i1][j1]-integral_image[i1-(window_x_size/2)][j1]
			// -integral_image[i1][j1-(window_y_size/2)]+integral_image[i1-(window_x_size/2)][j1-(window_y_size/2)];


			f[0]=integral_image[i0+(window_x_size/2)][j0+(window_y_size/2)]-integral_image[i0-1][j0+(window_y_size/2)]
			-integral_image[i0+(window_y_size/2)][j0-1]+integral_image[i0-1][j0-1];

			s_f[0]=squared_integral_image[i0+(window_x_size/2)][j0+(window_y_size/2)]-squared_integral_image[i0-1][j0+(window_y_size/2)]
			-squared_integral_image[i0+(window_y_size/2)][j0-1]+squared_integral_image[i0-1][j0-1];

			// cout<<"integral_image[i1+(window_x_size/2)][j1+(window_y_size/2)]:"<<integral_image[i1+(window_x_size/2)][j1+(window_y_size/2)]<<endl;
			// cout<<"-integral_image[i1][j1+(window_y_size/2)]:"<<integral_image[i1][j1+(window_y_size/2)]<<endl;
			// cout<<"-integral_image[i1+(window_y_size/2)][j1]:"<<integral_image[i1+(window_y_size/2)][j1]<<endl;
			// cout<<"integral_image[i1][j1]:"<<integral_image[i1][j1]<<endl;

			result[0]=(s_f[0]-((f[0]*f[0])/small_window_size))/small_window_size;

			// cout<<"s_f[0]: "<<s_f[0]<<endl;
			// cout<<"f[0]: "<<	f[0]<<endl;


			f[1]=integral_image[i1+(window_x_size/2)][j1+(window_y_size/2)]-integral_image[i1-1][j1+(window_y_size/2)]
			-integral_image[i1+(window_y_size/2)][j1-1]+integral_image[i1-1][j1-1];

			s_f[1]=squared_integral_image[i1+(window_x_size/2)][j1+(window_y_size/2)]-squared_integral_image[i1-1][j1+(window_y_size/2)]
			-squared_integral_image[i1+(window_y_size/2)][j1-1]+squared_integral_image[i1-1][j1-1];

			result[1]=(s_f[1]-((f[1]*f[1])/small_window_size))/small_window_size;


			f[2]=integral_image[i2+(window_x_size/2)][j2+(window_y_size/2)]-integral_image[i2-1][j2+(window_y_size/2)]
			-integral_image[i2+(window_y_size/2)][j2-1]+integral_image[i2-1][j2-1];

			s_f[2]=squared_integral_image[i2+(window_x_size/2)][j2+(window_y_size/2)]-squared_integral_image[i2-1][j2+(window_y_size/2)]
			-squared_integral_image[i2+(window_y_size/2)][j2-1]+squared_integral_image[i2-1][j2-1];

			result[2]=(s_f[2]-((f[2]*f[2])/small_window_size))/small_window_size;


			f[3]=integral_image[i3+(window_x_size/2)][j3+(window_y_size/2)]-integral_image[i3-1][j3+(window_y_size/2)]
			-integral_image[i3+(window_y_size/2)][j3-1]+integral_image[i3-1][j3-1];

			s_f[3]=squared_integral_image[i3+(window_x_size/2)][j3+(window_y_size/2)]-squared_integral_image[i3-1][j3+(window_y_size/2)]
			-squared_integral_image[i3+(window_y_size/2)][j3-1]+squared_integral_image[i3-1][j3-1];

			result[3]=(s_f[3]-((f[3]*f[3])/small_window_size))/small_window_size;

			// cout<<"s_f[3]: "<<s_f[3]<<endl;
			// cout<<"f[3]: "<<	f[3]<<endl;
			// cout<<"small_window_size: "<<small_window_size<<endl; 
			// cout<<"result[3]: "<<result[3]<<endl;			
			// cout<<"f[3]*f[3]: "<<f[3]*f[3]<<endl;
			// cout<<"f[3]*f[3])/small_window_size: "<<(f[3]*f[3])/small_window_size<<endl;
			/*
				The process finding the smallest value between f[0] and f[3].
			*/
			int final=9999;
			for(int l=0;l<4;l++){
				// cout<<"result: "<<result[l]<<endl;
				// if(final>f[l]){
				// 	final=f[l];
				// }
				if(final>result[l]){
					final=result[l];
				}
			}
			cout<<"final: "<<final<<endl;
			
			// getchar();

			Mpixel(image2,i,j)=final;
			//////////////////////////// 

		}
	}

}

void Kuwahara_Filter_Gray_With_Sum_Table(Mat source_image, Mat output_image, double** integral_image, double** squared_integral_image, int window_size){
	//This is function for applying the Kuwahara filter to output_image, it must require summed-area table.

	//Mat source_image is the source image with gray scale. 
	//Mat output_image is the final output which is applied to Kuwahara filter in gray scale image.
	//double** integral_image is 2-dimensional array which stores the summed-area table.
	//int window_size is the size of window it is big more blurly when the value is high.

	int picture_x_size=source_image.cols;//the x-axis length of source image.
	int picture_y_size=source_image.rows;//the y-axis length of source image.
	

	for(int i=0;i<=source_image.cols-(window_size);i++){
		for(int j=0;j<=source_image.rows-(window_size);j++){
			double f[4];
			double s_f[4];
			double result[4];
			int small_window_size=(window_size/2+1)*(window_size/2+1);

			int i_col[4];
			int i_row[4];

			i_col[0]=i;
			i_row[0]=j;

			i_col[1]=i+(window_size/2);
			i_row[1]=j;

			i_col[2]=i;
			i_row[2]=j+(window_size/2);

			i_col[3]=i+(window_size/2);
			i_row[3]=j+(window_size/2);

			double mean_fa[4];

			
			for(int a=0;a<4;a++){
				f[a]=integral_image[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-integral_image[i_col[a]][i_row[a]+(window_size/2)+1]
				-integral_image[i_col[a]+(window_size/2)+1][i_row[a]]+integral_image[i_col[a]][i_row[a]];

				s_f[a]=squared_integral_image[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-squared_integral_image[i_col[a]][i_row[a]+(window_size/2)+1]
				-squared_integral_image[i_col[a]+(window_size/2)+1][i_row[a]]+squared_integral_image[i_col[a]][i_row[a]];

				mean_fa[a]=f[a]/small_window_size;
				// cout<<"mean_fa["<<a<<"]: "<<mean_fa[a]<<endl;
			}
			for(int a=0;a<4;a++){
				result[a]=(s_f[a]-((f[a]*f[a])/small_window_size))/small_window_size;
			}


			double final=9999;
			int min_index=0;
			for(int l=0;l<4;l++){
				if(final>result[l]){
					final=result[l];
					min_index=l;
				}
			}
			Mpixel(output_image,i+window_size/2,j+window_size/2)=(int)mean_fa[min_index];
		}
	}

}

void Kuwahara_Filter_Color_With_Sum_Table(Mat3b source_image, Mat3b output_image, double **integral_image_B, double **integral_image_G, double **integral_image_R, double **squared_integral_image_B, double **squared_integral_image_G, double **squared_integral_image_R, int window_size){
	//This is function for applying the Kuwahara filter to output_image, it must require summed-area table.

	//Mat source_image is the source image with gray scale. 
	//Mat output_image is the final output which is applied to Kuwahara filter in gray scale image.
	//double** integral_image is 2-dimensional array which stores the summed-area table.
	//int window_size is the size of window it is big more blurly when the value is high.

	int picture_x_size=source_image.cols;//the x-axis length of source image.
	int picture_y_size=source_image.rows;//the y-axis length of source image.
	

	for(int i=0;i<=source_image.cols-(window_size);i++){
		for(int j=0;j<=source_image.rows-(window_size);j++){
			double f_B[4];
			double f_G[4];
			double f_R[4];
			double s_f_B[4];
			double s_f_G[4];
			double s_f_R[4];
			double result_B[4];
			double result_G[4];
			double result_R[4];
			int small_window_size=(window_size/2+1)*(window_size/2+1);

			int i_col[4];
			int i_row[4];

			i_col[0]=i;
			i_row[0]=j;

			i_col[1]=i+(window_size/2);
			i_row[1]=j;

			i_col[2]=i;
			i_row[2]=j+(window_size/2);

			i_col[3]=i+(window_size/2);
			i_row[3]=j+(window_size/2);

			double mean_fa_B[4];
			double mean_fa_G[4];
			double mean_fa_R[4];

			for(int a=0;a<4;a++){
				f_B[a]=integral_image_B[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-integral_image_B[i_col[a]][i_row[a]+(window_size/2)+1]
				-integral_image_B[i_col[a]+(window_size/2)+1][i_row[a]]+integral_image_B[i_col[a]][i_row[a]];

				s_f_B[a]=squared_integral_image_B[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-squared_integral_image_B[i_col[a]][i_row[a]+(window_size/2)+1]
				-squared_integral_image_B[i_col[a]+(window_size/2)+1][i_row[a]]+squared_integral_image_B[i_col[a]][i_row[a]];

				mean_fa_B[a]=f_B[a]/small_window_size;
				// cout<<"mean_fa_B["<<a<<"]: "<<mean_fa_B[a]<<endl;
			}

			for(int a=0;a<4;a++){
				f_G[a]=integral_image_G[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-integral_image_G[i_col[a]][i_row[a]+(window_size/2)+1]
				-integral_image_G[i_col[a]+(window_size/2)+1][i_row[a]]+integral_image_G[i_col[a]][i_row[a]];

				s_f_G[a]=squared_integral_image_G[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-squared_integral_image_G[i_col[a]][i_row[a]+(window_size/2)+1]
				-squared_integral_image_G[i_col[a]+(window_size/2)+1][i_row[a]]+squared_integral_image_G[i_col[a]][i_row[a]];

				mean_fa_G[a]=f_G[a]/small_window_size;
				// cout<<"mean_fa_G["<<a<<"]: "<<mean_fa_G[a]<<endl;
			}
			
			for(int a=0;a<4;a++){
				f_R[a]=integral_image_R[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-integral_image_R[i_col[a]][i_row[a]+(window_size/2)+1]
				-integral_image_R[i_col[a]+(window_size/2)+1][i_row[a]]+integral_image_R[i_col[a]][i_row[a]];

				s_f_R[a]=squared_integral_image_R[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-squared_integral_image_R[i_col[a]][i_row[a]+(window_size/2)+1]
				-squared_integral_image_R[i_col[a]+(window_size/2)+1][i_row[a]]+squared_integral_image_R[i_col[a]][i_row[a]];

				mean_fa_R[a]=f_R[a]/small_window_size;
				// cout<<"mean_fa_R["<<a<<"]: "<<mean_fa_R[a]<<endl;
			}

			for(int a=0;a<4;a++){
				result_B[a]=(s_f_B[a]-((f_B[a]*f_B[a])/small_window_size))/small_window_size;
			}

			for(int a=0;a<4;a++){
				result_G[a]=(s_f_G[a]-((f_G[a]*f_G[a])/small_window_size))/small_window_size;
			}

			for(int a=0;a<4;a++){
				result_R[a]=(s_f_R[a]-((f_R[a]*f_R[a])/small_window_size))/small_window_size;
			}



			double final=9999;
			int min_index=0;
			for(int l=0;l<4;l++){
				if(final>result_B[l]){
					final=result_B[l];
					min_index=l;
				}
			}

			pixelB(output_image,i+window_size/2,j+window_size/2)=(int)mean_fa_B[min_index];

			final=9999;
			min_index=0;
			for(int l=0;l<4;l++){
				if(final>result_G[l]){
					final=result_G[l];
					min_index=l;
				}
			}

			pixelG(output_image,i+window_size/2,j+window_size/2)=(int)mean_fa_G[min_index];

			final=9999;
			min_index=0;

			for(int l=0;l<4;l++){
				if(final>result_R[l]){
					final=result_R[l];
					min_index=l;
				}
			}

			pixelR(output_image,i+window_size/2,j+window_size/2)=(int)mean_fa_R[min_index];
		}
	}

}




void Filter_Gray_Integral4(Mat image1, Mat image2, double** integral_image,double** squared_integral_image, int window_size){
	//This is function for applying the Kuwahara filter to image2, it must require summed-area table.

	//Mat image1 is the source image with gray scale. 
	//Mat image2 is the final output which is applied to Kuwahara filter in gray scale image.
	//double** integral_image is 2-dimensional array which stores the summed-area table.
	//int window_size is the size of window it is big more blurly when the value is high.

	int picture_x_size=image1.cols;//the x-axis length of source image.
	int picture_y_size=image1.rows;//the y-axis length of source image.
	int window_x_size=window_size;//the x-axis length of window.
	int window_y_size=window_size;//the y-axis length of window.


	for(int i=(window_x_size/2);i<picture_x_size-(window_x_size/2+1);i++){
		for(int j=(window_y_size/2);j<picture_y_size-(window_y_size/2+1);j++){
			float f[4];
			float s_f[4];
			float result[4];
			int small_window_size=(window_x_size/2+1)*(window_y_size/2+1);

			int i0=i;
			int j0=j;

			int i1=i+(window_x_size/2);
			int j1=j;

			int i2=i;
			int j2=j+(window_y_size/2);

			int i3=i+(window_x_size/2);
			int j3=j+(window_y_size/2);

			float mean_f[4];

			cout<<"small_window_size: "<<small_window_size<<endl;

			mean_f[0]=0;
			for(int l=i-(window_x_size/2);l<=i;l++){
				for(int k=j-(window_y_size/2);k<=j;k++){
					mean_f[0]=mean_f[0]+(float)Mpixel(image1,l,k);
				}
			}
			mean_f[0]=mean_f[0]/small_window_size;

			cout<<"mean_f[0]: "<<mean_f[0]<<endl;
			
			//the section of f[1]
			//  | |*|*|        
			//	| |*|*|
			//	| | | |
			mean_f[1]=0;
			for(int l=i;l<=i+(window_x_size/2);l++){
				for(int k=j-(window_y_size/2);k<=j;k++){
					mean_f[1]=mean_f[1]+(float)Mpixel(image1,l,k);
				}
			}
			mean_f[1]=mean_f[1]/small_window_size;

			cout<<"mean_f[1]: "<<mean_f[1]<<endl;

			//the section of f[2]
			//  | | | |        
			//	|*|*| |
			//	|*|*| |
			mean_f[2]=0;
			for(int l=i-(window_x_size/2);l<=i;l++){
				for(int k=j;k<=j+(window_y_size/2);k++){
					mean_f[2]=mean_f[2]+(float)Mpixel(image1,l,k);
				}
			}
			mean_f[2]=mean_f[2]/small_window_size;


			//the section of f[3]
			//  | | | |        
			//	| |*|*|
			//	| |*|*|
			mean_f[3]=0;
			for(int l=i;l<=i+(window_x_size/2);l++){
				for(int k=j;k<=j+(window_x_size/2);k++){
					mean_f[3]=mean_f[3]+(float)Mpixel(image1,l,k);
				}
			}

			mean_f[3]=mean_f[3]/small_window_size;





			//the section of f[0]
			//  |*|*| |        
			//	|*|*| |
			//	| | | |

			float temp=0;
			result[0]=0;


			for(int l=i-(window_x_size/2);l<=i;l++){
				for(int k=j-(window_y_size/2);k<=j;k++){
					temp=mean_f[0]-(float)Mpixel(image1,l,k);
					temp=temp*temp;
					result[0]=result[0]+temp;
				}
			}

			result[0]=result[0]/small_window_size;


			
			//the section of f[1]
			//  | |*|*|        
			//	| |*|*|
			//	| | | |
			result[1]=0;
			for(int l=i;l<=i+(window_x_size/2);l++){
				for(int k=j-(window_y_size/2);k<=j;k++){
					temp=mean_f[1]-(float)Mpixel(image1,l,k);
					temp=temp*temp;
					result[1]=result[1]+temp;
				}
			}
			result[1]=result[1]/small_window_size;


			//the section of f[2]
			//  | | | |        
			//	|*|*| |
			//	|*|*| |
			result[2]=0;
			for(int l=i-(window_x_size/2);l<=i;l++){
				for(int k=j;k<=j+(window_y_size/2);k++){
					temp=mean_f[2]-(float)Mpixel(image1,l,k);
					temp=temp*temp;
					result[2]=result[2]+temp;
				}
			}
			result[2]=result[2]/small_window_size;


			//the section of f[3]
			//  | | | |        
			//	| |*|*|
			//	| |*|*|
			result[3]=0;
			for(int l=i;l<=i+(window_x_size/2);l++){
				for(int k=j;k<=j+(window_x_size/2);k++){
					temp=mean_f[3]-(float)Mpixel(image1,l,k);
					temp=temp*temp;
					// cout<<"temp: "<<temp<<endl;
					result[3]=result[3]+temp;
				}
			}
			// getchar();

			result[3]=result[3]/small_window_size;












			// cout<<"s_f[3]: "<<s_f[3]<<endl;
			// cout<<"f[3]: "<<	f[3]<<endl;
			// cout<<"small_window_size: "<<small_window_size<<endl; 
			cout<<"result[0]: "<<result[0]<<endl;			
			cout<<"result[1]: "<<result[1]<<endl;			
			cout<<"result[2]: "<<result[2]<<endl;			
			cout<<"result[3]: "<<result[3]<<endl;
			cout<<"i: "<<i<<endl;
			cout<<"j: "<<j<<endl;			
			// cout<<"f[3]*f[3]: "<<f[3]*f[3]<<endl;
			// cout<<"f[3]*f[3])/small_window_size: "<<(f[3]*f[3])/small_window_size<<endl;
			/*
				The process finding the smallest value between f[0] and f[3].
			*/
			float final=9999;
			for(int l=0;l<4;l++){
				// cout<<"result: "<<result[l]<<endl;
				// if(final>f[l]){
				// 	final=f[l];
				// }
				if(final>result[l]){
					final=result[l];
				}
			}

			for(int l=0;l<4;l++){
				if(final==result[l]){
					Mpixel(image2,i,j)=mean_f[l];
					break;
				}
			}
			// cout<<"final: "<<final<<endl;
			// if(final>255){
			// 	// getchar();
			// 	Mpixel(image2,i,j)=(int)Mpixel(image1,i,j);
			// }else{
			// Mpixel(image2,i,j)=(int)final;	
			// }

			
			//////////////////////////// 

		}
	}

}

void Kuwahara_Filter_Gray_Without_Sum_Table(Mat source_image, Mat output_image, int window_size){
	//This is function for applying the Kuwahara filter to output_image, it must require summed-area table.

	//Mat source_image is the source image with gray scale. 
	//Mat output_image is the final output which is applied to Kuwahara filter in gray scale image.
	//double** integral_image is 2-dimensional array which stores the summed-area table.
	//int window_size is the size of window it is big more blurly when the value is high.

	int picture_x_size=source_image.cols;//the x-axis length of source image.
	int picture_y_size=source_image.rows;//the y-axis length of source image.


	
	for(int i=0;i<=source_image.cols-(window_size);i++){
		for(int j=0;j<=source_image.rows-(window_size);j++){
			double f[4];
			double s_f[4];
			double result[4];
			int small_window_size=(window_size/2+1)*(window_size/2+1);

			int i_col[4];
			int i_row[4];

			i_col[0]=i;
			i_row[0]=j;

			i_col[1]=i+(window_size/2);
			i_row[1]=j;

			i_col[2]=i;
			i_row[2]=j+(window_size/2);

			i_col[3]=i+(window_size/2);
			i_row[3]=j+(window_size/2);

			double mean_f[4];

			// cout<<"small_window_size: "<<small_window_size<<endl;

			mean_f[0]=0;
			for(int l=i;l<=i+(window_size/2);l++){
				for(int k=j;k<=j+(window_size/2);k++){
					mean_f[0]=mean_f[0]+(float)Mpixel(source_image,l,k);
				}
			}
			mean_f[0]=mean_f[0]/small_window_size;

			// cout<<"mean_f[0]: "<<mean_f[0]<<endl;
			
			//the section of f[1]
			//  | |*|*|        
			//	| |*|*|
			//	| | | |
			mean_f[1]=0;
			for(int l=i+(window_size/2);l<i+window_size;l++){
				for(int k=j;k<=j+(window_size/2);k++){
					mean_f[1]=mean_f[1]+(float)Mpixel(source_image,l,k);
				}
			}
			mean_f[1]=mean_f[1]/small_window_size;

			// cout<<"mean_f[1]: "<<mean_f[1]<<endl;

			//the section of f[2]
			//  | | | |        
			//	|*|*| |
			//	|*|*| |

			mean_f[2]=0;
			for(int l=i;l<=i+(window_size/2);l++){
				for(int k=j+(window_size/2);k<j+window_size;k++){
					mean_f[2]=mean_f[2]+(float)Mpixel(source_image,l,k);
				}
			}
			mean_f[2]=mean_f[2]/small_window_size;
			// cout<<"mean_f[2]: "<<mean_f[2]<<endl;


			//the section of f[3]
			//  | | | |        
			//	| |*|*|
			//	| |*|*|

			mean_f[3]=0;
			for(int l=i+(window_size/2);l<i+window_size;l++){
				for(int k=j+(window_size/2);k<j+window_size;k++){
					mean_f[3]=mean_f[3]+(float)Mpixel(source_image,l,k);
				}
			}

			mean_f[3]=mean_f[3]/small_window_size;
			// cout<<"mean_f[3]: "<<mean_f[3]<<endl;
			
			

			double temp=0;
			result[0]=0;
			for(int l=i;l<=i+(window_size/2);l++){
				for(int k=j;k<=j+(window_size/2);k++){
					temp=mean_f[0]-(double)Mpixel(source_image,l,k);
					temp=temp*temp;
					result[0]=result[0]+temp;
				}
			}

			result[0]=result[0]/small_window_size;


			
			//the section of f[1]
			//  | |*|*|        
			//	| |*|*|
			//	| | | |
			result[1]=0;
			for(int l=i+(window_size/2);l<i+window_size;l++){
				for(int k=j;k<=j+(window_size/2);k++){
					temp=mean_f[1]-(double)Mpixel(source_image,l,k);
					temp=temp*temp;
					result[1]=result[1]+temp;
				}
			}
			result[1]=result[1]/small_window_size;


			//the section of f[2]
			//  | | | |        
			//	|*|*| |
			//	|*|*| |
			result[2]=0;
			for(int l=i;l<=i+(window_size/2);l++){
				for(int k=j+(window_size/2);k<j+window_size;k++){
					temp=mean_f[2]-(double)Mpixel(source_image,l,k);
					temp=temp*temp;
					result[2]=result[2]+temp;
				}
			}
			result[2]=result[2]/small_window_size;


			//the section of f[3]
			//  | | | |        
			//	| |*|*|
			//	| |*|*|
			result[3]=0;
			for(int l=i+(window_size/2);l<i+window_size;l++){
				for(int k=j+(window_size/2);k<j+window_size;k++){
					temp=mean_f[3]-(double)Mpixel(source_image,l,k);
					temp=temp*temp;
					// cout<<"temp: "<<temp<<endl;
					result[3]=result[3]+temp;
				}
			}
			// getchar();

			result[3]=result[3]/small_window_size;

			double final=9999;
			int min_index=0;
			for(int l=0;l<4;l++){
				if(final>result[l]){
					final=result[l];
					min_index=l;
				}
			}
			Mpixel(output_image,i+window_size/2,j+window_size/2)=(int)mean_f[min_index];
		}
	}

}



void Filter_Gray(Mat image1, Mat image2, int window_size){
	// This is function for applying the Kuwahara filter to gray-image2 (without summed-table).

	// Mat image1 is the source image with Gray scale. 
	// Mat image2 is the final output which is applied to Kuwahara filter in Gray image.

	// int window_size is the size of window it is big more blurly when the value is high.


	int picture_x_size=image1.cols;//the x-axis length of source image.
	int picture_y_size=image1.rows;//the y-axis length of source image.
	int window_x_size=window_size;//the x-axis length of window. 
	int window_y_size=window_size;//the y-axis length of window.

	for(int i=(window_x_size/2);i<picture_x_size-(window_x_size/2);i++){
		for(int j=(window_y_size/2);j<picture_y_size-(window_y_size/2);j++){
			int value[4];
			int f[4];
			int small_window_size=((window_x_size/2)+1)*((window_y_size/2)+1);

			//	This is 3*3 window
			//  | | | |        
			//	| | | |
			//	| | | |



			//the section of f[0]
			//  |*|*| |        
			//	|*|*| |
			//	| | | |

			value[0]=0;
			for(int l=i-(window_x_size/2);l<=i;l++){
				for(int k=j-(window_y_size/2);k<=j;k++){
					value[0]=value[0]+(int)Mpixel(image1,l,k);
				}
			}

			f[0]=value[0]/small_window_size;


			//the section of f[1]
			//  | |*|*|        
			//	| |*|*|
			//	| | | |

			value[1]=0;
			for(int l=i;l<=i+(window_x_size/2);l++){
				for(int k=j-(window_y_size/2);k<=j;k++){
					value[1]=value[1]+(int)Mpixel(image1,l,k);
				}
			}

			f[1]=value[1]/small_window_size;


			//the section of f[2]
			//  | | | |        
			//	|*|*| |
			//	|*|*| |

			value[2]=0;
			for(int l=i-(window_x_size/2);l<=i;l++){
				for(int k=j;k<=j+(window_y_size/2);k++){
					value[2]=value[2]+(int)Mpixel(image1,l,k);
				}
			}

			f[2]=value[2]/small_window_size;


			//the section of f[3]
			//  | | | |        
			//	| |*|*|
			//	| |*|*|

			value[3]=0;
			for(int l=i;l<=i+(window_x_size/2);l++){
				for(int k=j;k<=j+(window_x_size/2);k++){
					value[3]=value[3]+(int)Mpixel(image1,l,k);
				}
			}

			f[3]=value[3]/small_window_size;

			
			/*
				The process finding the smallest value between f[0] and f[3].
			*/
			int final=9999;
			for(int l=0;l<4;l++){
				if(final>f[l]){
					final=f[l];
				}
			}

			Mpixel(image2,i,j)=final;
			/////////////////////////

		}
	}

}

int run_kuwahara(int argc,char *argv[]){
	/*The First image*/
	double fps=0.0;	
	if((argc==3)||(argc==4)){
		int index_from_file;
		if(argc==3){//The case of there are no index information
			index_from_file=0;
		}else if(argc==4){//including index information
			index_from_file=stoi(argv[3]);
		}else{
			return -1; //error code
		}

		int size=strlen(argv[2]);
		// cout<<"size:"<<size<<endl;
		char path[size+2];

		strcpy(path,argv[2]);
		strcat(path,"/*");
		cout<<"path:"<<path<<endl;
		int file_numb=0;
		glob_t glob_result;
		glob(path,GLOB_TILDE,NULL,&glob_result);

		int key=0;
		ofstream contour_file;
		
		// unsigned int i;
		bool are_files_reached_max_capacity=false;
		unsigned int i;
		
		string folder_name_root="ROI_images/";
        string folder_name_roi="ROI/";
		string folder_name_contour="Contour/";
        string folder_name_ori_img="Image/";
		Directory_Creater write_file(the_number_of_file_in_folder,
			folder_name_root,
			 folder_name_roi,
			 folder_name_contour,
			  folder_name_ori_img);
      
		for(i=index_from_file; i<glob_result.gl_pathc-1; ++i){
			// cout<<"glob_result.gl_pathc: "<<glob_result.gl_pathc<<endl;
		  	int total_numb=check_total_numb(i,glob_result);

		  	if(total_numb>20){
		  		cout<<"The last collection of pictures"<<endl;
		  		continue;
		  	}
		  	cout<<"total_numb: "<<total_numb<<endl;

		  	for(int l=0;l<total_numb;l++){
		  		cout << glob_result.gl_pathv[i+l] << endl;
		  	}

		  	if(total_numb==1){
		  		cout<<"skip just one picture in a row"<<endl;
		  		continue;
		  	}

		  	// total_numb=5;
		  	/*The First image process*/
		  	Mat samp_output[total_numb];
		  	if(total_numb>4){
		  		total_numb=4;
		  	}

		  	Kuhawara_ROI2 ku_ROI;
		  	int loop_break=0;

		  	while(true){
		  		cout<<"index: "<<loop_break<<endl;

		  		cout<<"total_numb: "<<total_numb<<endl;
		  		

			  	// ku_ROI.main(ku,total_numb,loop_break);
			  	ku_ROI.main(glob_result,i,total_numb,loop_break);

			  	cout<<"p1"<<endl;
			  	
			  	// Mat *ROI_t=new Mat[total_numb-1];
			  	// Mat *ROI_real=new Mat[total_numb];
			  	// ROI_t=ku_ROI.get_samp_output();
			  	// ROI_real=ku_ROI.get_ROI_img();
			  	// contour_file << "haha"<<endl;
			  	if(ku_ROI.get_initalization_result()==false){

			  	}else{
			  		// if(file_numb>=the_number_of_file_in_folder){
			  		// 	are_files_reached_max_capacity=true;
			  		// 	break;
			  		// }
			  		cout<<"p4"<<endl;
			  		string file_name = glob_result.gl_pathv[i+loop_break];
			  	
			  		are_files_reached_max_capacity=
			  		write_file.Main_Process(
			  			ku_ROI.get_target_ROI_img(),
			  			ku_ROI.get_ROI_and_drawing(),
			  			ku_ROI.get_target_original_img(),
			  			file_name,
			  			class_number,
			  			ku_ROI.get_yolo_x(),
			  			ku_ROI.get_yolo_y(),
			  			ku_ROI.get_yolo_width(),
			  			ku_ROI.get_yolo_height(),
			  			ku_ROI.get_contour_txt()
			  			);
			  		if(are_files_reached_max_capacity==true){
			  			break;
			  		}
			  		file_numb++;
			  		cout<<"p5"<<endl;
			  	}
			  	loop_break++;
			  	if(loop_break==total_numb){
			  		cout<<"loop break"<<endl;
			  		break;
		  		}	
		  	}
		  	i=i+total_numb-1;
		  	if(are_files_reached_max_capacity==true){
		  		break;
		  	}
		}
		return i;
	}else{//dynamic mode with camera
		/*Information*/
		cout<<"dynamic mode"<<endl;
		// const string about =
	 //        "This is Mastewr Project in Massey University.\n"
	 //        "The example file can be downloaded from:\n"
	 //        "https://github.com/ShimQ88/Kuwahara-filter";
	 //    const string keys =
	 //        "{ h help |      | print this help message }"
	 //        "{ @image |<none>| path to image file }";
  //       CommandLineParser parser(argc, argv, keys);
	 //    parser.about(about);
	 //    if (parser.has("help"))
	 //    {
	 //        parser.printMessage();
	 //        return 0;
	 //    }
  //       string filename = parser.get<string>("@image");
	 //    if (!parser.check())
	 //    {
	 //        parser.printErrors();
	 //        return 0;
	 //    }

		/*Camera setup*/
		VideoCapture cap;
	  	cap.open(0);
	 	if (!cap.isOpened()){
	        cout << "Failed to open camera" << endl;
	        return 0;
	    }

	    cout << "Opened camera" << endl;
	    int cap_width=640;
	    int cap_height=480;
	   	cap.set(CAP_PROP_FRAME_WIDTH, cap_width);
        cap.set(CAP_PROP_FRAME_HEIGHT, cap_height);
        /**************/

        /*capture*/
     //    const string about =
	    //     "This sample demonstrates the camshift algorithm.\n"
	    //     "The example file can be downloaded from:\n"
	    //     "  https://www.bogotobogo.com/python/OpenCV_Python/images/mean_shift_tracking/slow_traffic_small.mp4";
	    // const string keys =
	    //     "{ h help |      | print this help message }"
	    //     "{ @image |<none>| path to image file }";
     //    CommandLineParser parser(argc, argv, keys);
	    // parser.about(about);
	    // if (parser.has("help"))
	    // {
	    //     parser.printMessage();
	    //     return 0;
	    // }
     //    string filename = parser.get<string>("@image");
	    // if (!parser.check())
	    // {
	    //     parser.printErrors();
	    //     return 0;
	    // }
	    // VideoCapture cap(filename);
	    // if (!cap.isOpened()){
	    //     //error in opening the video input
	    //     cerr << "Unable to open file!" << endl;
	    //     return 0;
	    // }

        /*The First image*/
        Mat3b image1;
        Mat gray_image1;
	   	int key=0;
	   	cap >> image1;
	   	if(!image1.data){printf("Could not open the file\n"); exit(0);}
	   	cvtColor(image1,gray_image1, COLOR_BGR2GRAY);//copy camera color image to gray scale
	   	/*****************/

	   	//////////////////////////////////////
		//Kuwahara filter using Summed-table//
		//////////////////////////////////////
    	
    	

		

	   	/*The Second image process*/
	   	int j=0;
	   	while (1){
	   		system_clock::time_point start = system_clock::now();//start clock
	   		
	   		Mat image2;
	   		Mat gray_image2;
	   		Mat output;
	   		Mat output1;
	   		Mat output2;
	   		Mat3b final_output;
	   		cap >> image2;
			
			cvtColor(image2,gray_image2, COLOR_BGR2GRAY);//copy camera color image to gray scale
			
			if(!image2.data){printf("Could not open the file\n"); exit(0);}
			output=Mat::zeros(gray_image2.size(),IMREAD_GRAYSCALE);//initialize the value of output metrices to zero
			output1=Mat::zeros(gray_image1.size(),IMREAD_GRAYSCALE);//initialize the value of output metrices to zero
			output2=Mat::zeros(gray_image2.size(),IMREAD_GRAYSCALE);//initialize the value of output metrices to zero
    		final_output=Mat::zeros(gray_image2.size(),CV_8UC3);//initialize the value of final_output metrices to zero

    		//////////////////////////////////////
    		//Kuwahara filter using Summed-table//
    		//////////////////////////////////////

			/*Memory Allocation*/
			double** integral_image1=new double*[gray_image1.cols+1];
			double** squared_integral_image1=new double*[gray_image1.cols+1];

			double** integral_image2=new double*[gray_image2.cols+1];
			double** squared_integral_image2=new double*[gray_image2.cols+1];
			for(int i = 0; i < gray_image2.cols+1; ++i){
				integral_image1[i] = new double[gray_image1.rows+1];
				squared_integral_image1[i] = new double[gray_image1.rows+1];

				integral_image2[i] = new double[gray_image2.rows+1];
				squared_integral_image2[i] = new double[gray_image2.rows+1];
			}
    		/*********************/

    		Integral_Gray_Initialize(gray_image2,integral_image2,squared_integral_image2);//create summed-table to integral_image array.
    		Kuwahara_Filter_Gray_With_Sum_Table(gray_image2,output2,integral_image2,squared_integral_image2,11);//Applying kuwahara filter to output using integral_image.
    		Integral_Gray_Initialize(gray_image1,integral_image1,squared_integral_image1);//create summed-table to integral_image array.
    		Kuwahara_Filter_Gray_With_Sum_Table(gray_image1,output1,integral_image1,squared_integral_image1,11);//Applying kuwahara filter to output using integral_image.
		
			/*Memory deallocation*/
			for(int i = 0; i < gray_image1.cols+1; ++i) {
				delete [] integral_image1[i];
				delete [] squared_integral_image1[i];

				delete [] integral_image2[i];
				delete [] squared_integral_image2[i];
			}
			delete [] integral_image1;
			delete [] squared_integral_image1;
			delete [] integral_image2;
			delete [] squared_integral_image2;
			/***************/
			
			///////////////////////////////////////



			////////////////////////////////////////
    		//Kuwahara filter without Summed-table//
    		////////////////////////////////////////
    		
    		// Filter_Gray(gray_image2,output,5);

    		
    		///////////////////////////////////////
    		

			/*subtraction process between The first image and the second image*/
			output=gray_image1-gray_image2;
			/********************************/
			



			/***Cropping Object by outline of object***/
			Mat temp_window=Mat::zeros(image2.size(),IMREAD_GRAYSCALE);//gray case
			// Image_stitching(gray_image2,output,temp_window);

			thresholding_image(output, 50,true,0);
			// cv::threshold(output, output, 0, 255, THRESH_BINARY | THRESH_OTSU);
			// median_filter(subtracted_frame,subtracted_frame,7);
			medianBlur(output,output,7);

			

			/******************************************/
			Point* p_start_roi_window;
			Point* p_end_roi_window;
			p_start_roi_window=new Point[200];//approx numb
			p_end_roi_window=new Point[200];//approx numb
			// Mat blob_window;
			Mat blob_window=Mat::zeros(image2.size(),CV_8UC3);//gray case

			int count_numb=blob(output, blob_window, p_start_roi_window, p_end_roi_window);
			
			//filename
			time_t rawtime;
			struct tm * timeinfo;
			char buffer[80];

			time (&rawtime);
			timeinfo = localtime(&rawtime);

			strftime(buffer,sizeof(buffer),"%H:%M:%S;%d:%m:%Y",timeinfo);
			string str(buffer);
			cout << str;
			string file_name;
			file_name="capture/";
			file_name=file_name+str+".jpg";
			//filename end

			// cout<<"THis is point 2-1"<<endl;
			

			Point p_center_of_object=draw_rect_box(blob_window, p_start_roi_window, p_end_roi_window, 200);
			int cropping_size=determine_frame_size(blob_window,p_center_of_object,20,20);

			cout<<"cropping size: "<<cropping_size<<endl;
			if((count_numb==-100)||(cropping_size>100)){
				// cout<<"Skip: too much blob"<<endl;
				// is_initialize_success=false;
				// return;
				imwrite(file_name,image2);
				// continue;
			}//segmental fault

			/***ROI Section***/
			// int small_x,small_y,large_x,large_y;//size of
			// Determining_ROI_Size(temp_window,&small_x,&small_y,&large_x,&large_y);
			// // median_filter(temp_window,temp_window,15);
			// Rect window(small_x, small_y, large_x-small_x, large_y-small_y); // simply hardcoded the values
			// rectangle(final_output, window, Scalar(0, 255, 255), 2);
				

			// int x_value=(large_x-small_x-small_x);
			// int y_value=(large_y-small_y-small_y);
			// int trigger=x_value+y_value;
			
			// if( trigger>400 ){
			// 	// auto time = std::chrono::system_clock::now();
			// 	// string time_str=to_string(time);  
			// 	// imwrite("2.jpg",image1);
			// }


			/*Debugging for values*/
			// cout<<"1.small_x:"<<small_x<<endl;
			// cout<<"1.large_x:"<<large_x<<endl;
			// cout<<"2.small_y:"<<small_y<<endl;
			// cout<<"2.large_y:"<<large_y<<endl;
			/**********************/

			/*Mean shift part*/
			// if( trigger>200 ){//trigger to work meanshift.(question1)
			// 	cout<<"mean shift working"<<endl;
			// 	namedWindow(window_detection_name);
			// 	namedWindow(window_capture_name);
			// 	createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
			//     createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
			//     createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
			//     createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
			//     createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
			//     createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);
   // 				Mat frame, frame_HSV, frame_threshold;
			// 	// cout<<"1.small_x:"<<small_x<<endl;
			// 	// cout<<"1.large_x:"<<large_x<<endl;
			// 	// cout<<"2.small_y"<<small_y<<endl;
			// 	// cout<<"2.large_y"<<large_y<<endl;
			// 	Rect track_window(small_x, small_y, large_x-small_x, large_y-small_y);
			// 	// rectangle(image2,track_window,Scalar(0,255,0),3);
			// 	float range_1[] = {0, 90};
			// 	// float range_2[] = {0, 90};
			// 	// float range_3[] = {0, 90};
			//     const float* range[] = {range_1};
			//     Mat roi_hist;
			//     Mat roi, mask;

			//     Mat hsv;
			//     cvtColor(image2, frame_HSV, COLOR_BGR2HSV);
			//     cvtColor(image2, hsv, COLOR_BGR2HSV);
			//     // inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);


			// 	Mat mask1, mask2;
			//     // inRange(hsv, Scalar(161, 155, 84), Scalar(179, 255, 255), mask1);
			//     // inRange(hsv, Scalar(179, 255, 255), Scalar(180, 255, 255), mask2);
			//     // inRange(hsv, Scalar(94, 80, 2), Scalar(126, 255, 255), mask);
			//     // mask = mask1 + mask2;
			// 	// mask=zeros(image2.size(),IMREAD_GRAYSCALE);

			//     // Mat mask;
			//     roi = image2(track_window);
			//     // roi = image2(window);
			//     int histSize[] = {180};
			//     int channels[] = {0};
			//     // calcHist(&image2, 1, channels, mask, roi_hist, 1, histSize, range);
			//     calcHist(&hsv, 1, channels, mask, roi_hist, 1, histSize, range);
			//     normalize(roi_hist, roi_hist, 0, 255, NORM_MINMAX);
			//     TermCriteria term_crit(TermCriteria::EPS | TermCriteria::COUNT, 100, 1);
			// 	int k=0;
			// 	while(1){
			// 		k++;
			// 		system_clock::time_point start = system_clock::now();//start clock
			// 		Mat hsv1, dst;
			//         cap >> image2;
			//         if (image2.empty())
			//             break;
			//         cvtColor(image2, hsv, COLOR_BGR2HSV);
			//         // calcBackProject(&image2, 1, channels, roi_hist, dst, range);
			//         calcBackProject(&hsv, 1, channels, roi_hist, dst, range);
			        
			//         // apply camshift to get the new location
			//         RotatedRect rot_rect = CamShift(dst, track_window, term_crit);
			        
			//         // Draw it on image
			//         Point2f points[4];
			//         rot_rect.points(points);
			//         int loop_breaker=0;

			//         //find smallest point 
			//         Point2f pt_small,pt_large;
			//         pt_small.x=9999;
			//         pt_small.y=9999;
			//         pt_large.x=-9999;
			//         pt_large.y=-9999;

			//         for (int i = 0; i < 4; i++){
			//             // line(image2, points[i], points[(i+1)%4], 255, 2);
			//             line(hsv, points[i], points[(i+1)%4], 255, 2);
			//             if((points[i].x<0)||(points[i].x>640)||(points[i].y<0)||(points[i].y>480)){
			//             	loop_breaker++;
			//             }
			//             cout<<"P"<<i<<": x,y: "<<points[i].x<<", "<<points[i].y<<endl;

			//             if(points[i].x<pt_small.x){
			//             	pt_small.x=points[i].x;
			//             }

			//             if(points[i].y<pt_small.y){
			//             	pt_small.y=points[i].y;
			//             }

			//             if(points[i].x>pt_large.x){
			//             	pt_large.x=points[i].x;
			//             }

			//             if(points[i].y>pt_large.y){
			//             	pt_large.y=points[i].y;
			//             }

			//         }
			//         // Rect t_rec(points[3].x,points[3].y,points[1].x-points[3].x,points[1].y-points[3].y);
			//         Rect t_rec(pt_small.x, pt_small.y, pt_large.x-pt_small.x, pt_large.y-pt_small.y);
			        
			//         // rectangle(image2,t_rec,Scalar(0,255,0),3);
			//         rectangle(hsv,t_rec,Scalar(0,255,0),3);

			//         cout<<"small"<<": x,y: "<<pt_small.x<<", "<<pt_small.y<<endl;
			//         cout<<"large"<<": x,y: "<<pt_large.x<<", "<<pt_large.y<<endl;
			        
			//         Point center_of_rect = (t_rec.br() + t_rec.tl())*0.5;

			        

   //  				// circle(image2,center_of_rect,3,Scalar(0,0,255));
   //  				circle(hsv,center_of_rect,3,Scalar(0,0,255));

			//         // if(loop_breaker>=){//breaking rule (question2)
			//         // 	break;
			//         // }
			     
			//         string directory_name="captured_image/"+to_string(k)+".jpg";
			//         string file_name="captured_image/"+to_string(k)+".txt";

			//         float file_width=abs(points[2].x-points[0].x)/cap_width;
			//         float file_height=abs(points[2].y-points[0].y)/cap_height;
			//         float file_x=center_of_rect.x/cap_width;
			//         float file_y=center_of_rect.y/cap_height;
			//         string file_content="0, "+to_string(file_x)+", "
			//         +to_string(file_y)+", "+to_string(file_width)+", "
			//         +to_string(file_height)+'\n';

			//         ofstream myfile (file_name);
			// 		if (myfile.is_open())
			// 		{
			// 			myfile << file_content;
			// 		    myfile.close();
			// 		}

			//         imshow("object following window", image2);
			//         // imshow("object following window", hsv1);
			//         imshow("hsv", hsv);
			//         // imshow("mask2", mask2);
			//         // imshow("mask1", mask1);
			//         // imshow("mask", mask);
			//         imshow("dst of object", dst);
			//         // cvtColor(image2, frame_HSV, COLOR_BGR2HSV);
			//         // inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
			//         // imshow(window_detection_name, frame_threshold);
			//         // imshow("hellow world", frame_HSV);
			//         // imwrite(directory_name, image2);//write
			//         // imshow("roi", roi);
			        
			//         key=waitKey(1);
			//         system_clock::time_point end = system_clock::now();
			//        	double seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			//      	fps = 1000000/seconds;
			//      	cout << "frames " << fps << " seconds " << seconds << endl;
			// 	}
			// }
			/**********************/

			Grey_to_Color(image2,temp_window,final_output);
	    	// imshow("temp_window" ,temp_window);
	    	imshow("final_output" ,final_output);
	    	imshow("blob_window" ,blob_window);

	    	// imshow("image1" ,output1);
	    	// imshow("image2" ,output2);
	    	/*************/
	    	
	    	/*program termination*/
	    	key=waitKey(1);
	       	if(key==113 || key==27) return 0;//either esc or 'q'
	       	/****************/

	       	/*Caculate performance of program*/
	       	system_clock::time_point end = system_clock::now();
	       	double seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	     	fps = 1000000/seconds;
	     	cout << "frames " << fps << " seconds " << seconds << endl;
	     	/*********************************/
	     	
	     	/***Delay for image1***/
	     	// j++;
	     	// if(j==3){
	     	// 	image1=image2;
	     	// 	cvtColor(image1,gray_image1, CV_BGR2GRAY);//copy camera color image to gray scale
	     	// 	j=0;
	     	// }
	     	/**********************/
	     	image1=image2;
	     	cvtColor(image1,gray_image1, COLOR_BGR2GRAY);//copy camera color image to gray scale
	    }
	    /************/
	}
}

