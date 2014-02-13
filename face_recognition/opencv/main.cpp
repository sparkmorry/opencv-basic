#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace cv;
using namespace std;

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            Mat src;
            equalizeHist(imread(path, 0), src );
            images.push_back(src);
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

int main(int argc, const char *argv[]) {
        // Check for valid command line arguments, print usage
        // if no arguments were given.
    if (argc != 4) {
        cout << "usage: " << argv[0] << " </path/to/haar_cascade> </path/to/csv.ext> </path/to/device id>" << endl;
        cout << "\t </path/to/haar_cascade> -- Path to the Haar Cascade for face detection." << endl;
        cout << "\t </path/to/csv.ext> -- Path to the CSV file with the face database." << endl;
        cout << "\t <device id> -- The webcam device id to grab frames from." << endl;
        exit(1);
    }
        // Get the path to your CSV:
    string fn_haar = string(argv[1]);
    string fn_csv = string(argv[2]);
    int deviceId = atoi(argv[3]);
        // These vectors hold the images and corresponding labels:
    vector<Mat> images;
    vector<int> labels;
        // Read in the data (fails if no valid input filename is given, but you'll get an error message):
    try {
        read_csv(fn_csv, images, labels);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
            // nothing more we can do
        exit(1);
    }
    
    //获取训练样本的尺寸大小用于后面对采集到人脸的尺寸修改
    int im_width = images[0].cols;
    int im_height = images[0].rows;
    
    // 创建特征脸识别器
    Ptr<FaceRecognizer> model = createEigenFaceRecognizer();    
    // 训练对应图像和标签
    model->train(images, labels);

    // 创建联级分类器用于摄像头中的人脸检测
    CascadeClassifier haar_cascade;
    haar_cascade.load(fn_haar);
    // 摄像头对象
    VideoCapture cap(deviceId);
        // Check if we can use this device at all:
    if(!cap.isOpened()) {
        cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
        return -1;
    }
        // Holds the current frame from the Video device:
    Mat frame;
    for(;;) {
        cap >> frame;
            // Clone the current frame:
        Mat original = frame.clone();
        // 对摄像头帧灰度化
        Mat gray;
        cvtColor(original, gray, CV_BGR2GRAY);
        
        // 存储摄像头获取的人脸，联级搜索视频中人脸位置
        vector< Rect_<int> > faces;
        haar_cascade.detectMultiScale(gray, faces); 
        
        //对检测到的每个人脸进行处理及匹配
        for(int i = 0; i < faces.size(); i++) {
            
            Rect face_i = faces[i];
            Mat face_g = gray(face_i);
            Mat face;
            //将人脸转换成灰度子块,并进行直方图均衡化处理
            equalizeHist(face_g, face );
            
            //将人脸与训练样本归一到统一尺寸    
            Mat face_resized;
            cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
                        
            //获得最匹配人脸标签
            int prediction = model->predict(face_resized);
            string dict[5]={"Me","TA","Grandma","Gao","Huo"};
            string name = dict[prediction/3];

                //cout<<(prediction-1)/3<<endl;
                cout<<prediction<<endl;
            rectangle(original, face_i, CV_RGB(0, 255,0), 1);
            
            int pos_x = std::max(face_i.tl().x - 10, 0);
            int pos_y = std::max(face_i.tl().y - 10, 0);
                // And now put it into the image:
            putText(original, name, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
            
            Mat matched_face=imread("/Users/sparkmorry/Study/13年秋冬学期/视觉/opencv/opencv/data/s3/2.jpg");
            int loc=50;
            matched_face.copyTo(original.colRange(loc,loc+im_width).rowRange(loc,loc+im_height));
    
        }
            // Show the result:
        imshow("face_recognizer", original);
            // And display it:
        char key = (char) waitKey(20);
            // Exit this loop on escape:
        if(key == 27)
            break;
    }
    return 0;
}