#ifndef SEMI_AUTO_FILTERING
#define SEMI_AUTO_FILTERING

#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>


// System Headers
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <bits/stdc++.h>
#include <opencv2/core/core.hpp>
#include <vector>
#include <string>
#include <glob.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "contour.h"

//namespace
using namespace cv;
using namespace std;

string Jpg_To_Txt_Directory(string img_name);
string Ori_Img_Name_To_Contour_Name(string img_name);
bool Find_Contour_and_Name_File(glob_t glob_sub_result, string *path_contour, string *path_name);
int Semi_Auto_Filtering(int argc,char *argv[]);
void Semi_Auto_Filtering_info();
#endif