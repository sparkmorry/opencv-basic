#coding=utf-8
import cv2.cv as cv

def binaryThreshold(Image, threshold):
    grey = cv.CreateImage(cv.GetSize(img),cv.IPL_DEPTH_8U, 1)
    out = cv.CreateImage(cv.GetSize(img),cv.IPL_DEPTH_8U, 1)
 
    cv.CvtColor(Image,grey,cv.CV_BGR2GRAY)
    cv.Threshold(grey, out ,threshold , 255 ,cv.CV_THRESH_BINARY)

    return out

if __name__ == '__main__':

    #threshold = input("threshold=")
    cv.NamedWindow("camera",1)
    capture = cv.CaptureFromCAM(0)  

    
    while True:
        """ capture image from camera """
        img = cv.QueryFrame(capture)

        """ convert color image to grey """
        #img = binaryThreshold(img, threshold)

        """ Get the width and height of the image """
        (width, height) = cv.GetSize(img)

        """ put text id and name in image """        
        text_font = cv.InitFont(cv.CV_FONT_HERSHEY_DUPLEX, 2, 2)
        cv.PutText(img, "3100102592 menglixia", (50, height / 2), text_font, cv.RGB(255, 255, 0))

        """ show each frame """
        cv.ShowImage("camera",img)

        """ press esc to quit the script """
        if cv.WaitKey(10) == 27:
            break

    del(capture)
    cv.DestroyWindow("camera")
