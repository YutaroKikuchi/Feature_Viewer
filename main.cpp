#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <time.h>
#include <iostream>
#include <string>
#include <string.h>

#include "ImageKeeper.h"
#include "Drawer.h"
#include "FeaturePoint.h"
#include "Reader.h"
#include "LineReader.h"
//#include "FeatureViewer.h"
#include "ImageViewer.h"

void checkCommand(int* output,char key){	//キーコマンドを解析

	switch(key){
		case 'n':
		case 'N':
			break;
		case 'p':
		case 'P':
			*output -= 2;
			break;
		case 'q':
		case 'Q':
			exit(0);
			break;
		default :
			*output -= 1;
			break;
	}
}

int readPath(char* in, std::string *imgpath, std::string *nvmpath,int *cam, int *lengh, int *numofimg, int *numofLine){	//引数で渡したテキストファイルを読み込む

	std::ifstream ifs(in);
	std::string buff;

	if(ifs.fail()){
		std::cout << "File Not Found:" << in << std::endl;
		return 0;
	}else{
		while(std::getline(ifs,buff)){
			if(strstr(buff.c_str(),"data_dir")){
				size_t pos1 = buff.find("\""), pos2 = buff.rfind("\"");
				*imgpath = buff.substr(pos1 + 1, pos2 - pos1 - 1);
			}

			if(strstr(buff.c_str(),"nvm_dir")){
				size_t pos1 = buff.find("\""), pos2 = buff.rfind("\"");
				*nvmpath = buff.substr(pos1 + 1, pos2 - pos1 - 1);
			}

			if(strstr(buff.c_str(),"pertition_number")){
				size_t pos1 = buff.find("\""), pos2 = buff.rfind("\"");
				*cam = std::stoi(buff.substr(pos1 + 1, pos2 - pos1 - 1));
			}

			if(strstr(buff.c_str(),"rought_lengh")){
				size_t pos1 = buff.find("\""), pos2 = buff.rfind("\"");
				*lengh = std::stoi(buff.substr(pos1 + 1, pos2 - pos1 - 1));
			}

			if(strstr(buff.c_str(), "num_of_imgs")){
				size_t pos1 = buff.find("\""), pos2 = buff.rfind("\"");
				*numofimg = std::stoi(buff.substr(pos1 + 1, pos2 - pos1 - 1));
			}

			if(strstr(buff.c_str(), "num_of_lines")){
				size_t pos1 = buff.find("\""), pos2 = buff.rfind("\"");
				*numofLine = std::stoi(buff.substr(pos1 + 1, pos2 - pos1 - 1));
			}
		}
	}

	return 1;
}

void showData(std::vector<ImageKeeper> &viewed){

	for(int i=0; i<3; i++){

		std::vector<std::string> shownStr(6);
		std::vector<int> numModel(6);
		for(int j=0; j<6; j++){

			switch(i){
				case 0:
					switch(j){
						case 0:
							shownStr[j] = viewed[5].getName();
							numModel[j] = viewed[5].getModelID();
							break;
						case 1:
							shownStr[j] = viewed[4].getName();
							numModel[j] = viewed[4].getModelID();
							break;
						case 3:
							shownStr[j] = viewed[10].getName();
							numModel[j] = viewed[10].getModelID();
							break;
						case 4:
							shownStr[j] = viewed[11].getName();
							numModel[j] = viewed[11].getModelID();
							break;
						default:
							shownStr[j] = "No Image";
							numModel[j] = -1;
					}
					break;
				case 1:
					switch(j){
						case 0:
							shownStr[j] = viewed[3].getName();
							numModel[j] = viewed[3].getModelID();
							break;
						case 1:
							shownStr[j] = viewed[0].getName();
							numModel[j] = viewed[0].getModelID();
							break;
						case 2:
							shownStr[j] = viewed[1].getName();
							numModel[j] = viewed[1].getModelID();
							break;
						case 3:
							shownStr[j] = viewed[9].getName();
							numModel[j] = viewed[9].getModelID();
							break;
						case 4:
							shownStr[j] = viewed[6].getName();
							numModel[j] = viewed[6].getModelID();
							break;
						case 5:
							shownStr[j] = viewed[7].getName();
							numModel[j] = viewed[7].getModelID();
							break;
						default:
							shownStr[j] = "No Image";
							numModel[j] = -1;
					}
					break;
				case 2:
					switch(j){
						case 1:
							shownStr[j] = viewed[2].getName();
							numModel[j] = viewed[2].getModelID();
							break;
						case 4:
							shownStr[j] = viewed[8].getName();
							numModel[j] = viewed[8].getModelID();
							break;
						default:
							shownStr[j] = "No Image";
							numModel[j] = -1;
							
					}
					break;
				default:
					break;
			}

		}

		for(int j=0; j < 6; j++){
			std::cout << "|" << shownStr[j] << "\t|";
		}
		std::cout << std::endl;
		for(int j=0; j < 6; j++){
			std::cout << "|" << "Model:" << numModel[j] << "\t|";
		}
		std::cout << std::endl << "-------------------------------------------------------------------------------------------------" << std::endl;
	}
	std::cout << std::endl;
}

int main(int argc ,char* argv[]){

	if(argc != 2){
		std::cout << "Usage:" << std::endl << "%s ./FeatureViewer input.txt"<<std::endl<<std::endl;
		return 0;
	}

	std::string nvmpath;
	std::string imgpath;
	int lengh,cam,numofimg,numofLines;

	if(readPath(argv[1], &imgpath, &nvmpath, &cam, &lengh, &numofimg, &numofLines) == 0){
		return 0;
	}

	std::cout << "****************************************" << std::endl << "How to use..." << std::endl << " * [n] key : Move to Next Frame" << std::endl;
	std::cout << " * [p] key : Move to previous Frame" << std::endl;
	std::cout << " * [q] key : Quit thie application" << std::endl << "****************************************" << std::endl<< std::endl;

	std::cout << "　* imgpath:" << imgpath << std::endl;
	std::cout << " * nvmpath:" << nvmpath << std::endl;
	std::cout << "　* pertitition_number:" << cam << std::endl;
	std::cout << " * rought_lengh:" << lengh << std::endl;
	std::cout << "　* num_of_imgs:" << numofimg << std::endl;
	std::cout << " * num_of_lines:" << numofLines << std::endl << std::endl;

	int numpic=0;

	ImageViewer imgv(cam,lengh, "FeatureViewer");
	Reader reader(cam,imgpath,nvmpath);

	Drawer drawer;

	if(reader.setFeaturePoint(1,numofLines) != 0){
		exit(0);
	}


	reader.setImg(cam, 0, numofimg-1);


	reader.sortKeyFrameID();

	imgv.setWindow();
	
	//全てのフレームに対してループを実行
	for(int i=lengh+1; i<reader.ik.size();i++){
		
		std::vector<std::vector<ImageKeeper>> drawnCam(cam);

		//表示したい12枚の画像を取得
		for(int j=0; j<12; j++){
			//指定された線の長さ分だけ、前フレームの画像を取得
			for(int k=0; k<lengh+1;k++){
				drawnCam[j].push_back(reader.getIKbyID(j,reader.keyFrameID[i-k]));
			}
		}

		//featureFlagのベクトル
		std::vector< std::vector<bool> > flagCam(cam);

		//表示する画像に対してすべての点を描写
		//flagのベクトルに特徴点の部分だけ割り当てる
		for(int j=0;j<cam;j++){
			drawer.DrawPoints(drawnCam[j][0]);
			flagCam[j].resize(drawnCam[j][0].getFeaturesSize());

		}

		//表示する画像に対して、Featureflagを割り当てる
		for(int j=0;j<cam;j++){
			//1つ前の画像の特徴点同士のIDを比較
			for(int k=0;k<cam;k++){
               			if(j!=k){
                    			drawnCam[k][1].getFeatureFlags(drawnCam[j][0],flagCam[j]);
                		}
            		}
		}

		for(int j=0;j<cam;j++){
			drawer.DrawRoute(drawnCam[j],drawnCam[j][0].getIMG(),flagCam[j]);
		}

		std::vector<ImageKeeper> preimg;
		std::vector<ImageKeeper> viewedimg;


		for(int j=0;j<cam;j++){
			preimg.push_back(drawnCam[j][1]);
			viewedimg.push_back(drawnCam[j][0]);
		}

		showData(viewedimg);

		imgv.showImgsTheta(viewedimg, preimg);

		checkCommand(&i,cv::waitKey(0));
		
	}

	return 0;
	
}

