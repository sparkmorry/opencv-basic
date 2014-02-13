一.开发软件说明*本作业是在 Mac OSX 系统下,以 Python-OpenCV 的方式开发。因为 Mac 在 OpenCV 的配置过程中出现一点问题,无法导入视频文件,所以 在这里有点偏题的采取了由电脑摄像头直接获取彩色视频,并实时输出视频图像。 由于选取了 Python 作为开发语言,所以没有可执行文件,只需直接执行.py 文件 即可(需要在 opencv 创建文件夹的.bash_profile 文件里添加 python 的 path)。 后期会尝试解决视频导入的问题。二.算法具体步骤
1. 用CaptureFromCAM函数对图像进行提取: capture = cv.CaptureFromCAM(0) 读取直接的视频文件只需将语句改变为: capture = cv.VideoCapture('videoname.avi')
2. 对每一帧图像进行读取: while True:img = cv.QueryFrame(capture)#如果按下 esc 键则终止程序退出 
if cv.WaitKey(10) == 27:	break
	3. 在循环中对读取的每一帧图像进行二值化处理:def binaryThreshold(Image, threshold):
	grey = cv.CreateImage(cv.GetSize(img),cv.IPL_DEPTH_8U, 1) 
	out = 	cv.CreateImage(cv.GetSize(img),cv.IPL_DEPTH_8U, 1)		cv.CvtColor(Image,grey,cv.CV_BGR2GRAY)
	cv.Threshold(grey, out , threshold , 255 ,cv.CV_THRESH_BINARY)
		return out
	其中,CreateImage 函数表示按原帧大小创建 256 值 1 通道的灰度图像, CvtColor 函数将传入的图像从 RGB 图像转换成灰度图,写入 grey 图像。 Threshold函数按程序开始时threshold = input("threshold=")语句获取的用 户输入的的阈值将图像进行二值化写入 out 图像。将 out 图像作为函数返回 值传入主函数。
4. 在每一帧图像上覆盖“3100102592 menglixia”的文字:(width, height) = cv.GetSize(img)
text_font = cv.InitFont(cv.CV_FONT_HERSHEY_DUPLEX, 2, 2) 
cv.PutText(img, "3100102592 menglixia", (50, height / 2), text_font,
cv.RGB(255, 0, 0))获取图像高度和宽度,并初始化文字字体,在 1/2 高度处写入“3100102592 menglixia”的字符串,以颜色红色(在二值化后的图像中显示为 黑色)
5. 显示每一帧图像:cv.ShowImage("camera",img)