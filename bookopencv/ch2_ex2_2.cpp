/* License:
 Oct. 3, 2008
 Right to use this code in any way you want without warrenty, support or any guarentee of it working.

 BOOK: It would be nice if you cited it:
 Learning OpenCV: Computer Vision with the OpenCV Library
 by Gary Bradski and Adrian Kaehler
 Published by O'Reilly Media, October 3, 2008
 
 AVAILABLE AT:
 http://www.amazon.com/Learning-OpenCV-Computer-Vision-Library/dp/0596516134
 Or: http://oreilly.com/catalog/9780596516130/
 ISBN-10: 0596516134 or: ISBN-13: 978-0596516130

 OTHER OPENCV SITES:
 * The source code is on sourceforge at:
 http://sourceforge.net/projects/opencvlibrary/
 * The OpenCV wiki page (As of Oct 1, 2008 this is down for changing over servers, but should come back):
 http://opencvlibrary.sourceforge.net/
 * An active user group is at:
 http://tech.groups.yahoo.com/group/OpenCV/
 * The minutes of weekly OpenCV development meetings are at:
 http://pr.willowgarage.com/wiki/OpenCV
 */
#include "highgui.h"

int main(int argc, char** argv) {
	cvNamedWindow("Example2", CV_WINDOW_AUTOSIZE);
	//CvCapture* capture = cvCaptureFromAVI( argv[1] ); // either one will work
	CvCapture* capture = cvCreateFileCapture(argv[1]);
	IplImage* frame;
	while (1) {
		frame = cvQueryFrame(capture);
		if (!frame)
			break;
		cvShowImage("Example2", frame);
		char c = cvWaitKey(33);
		if (c == 27)
			break;
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("Example2");
	return 0;
}
