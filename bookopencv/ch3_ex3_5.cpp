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

#include "cv.h"
#include <stdio.h>

int main() {
	CvMat* mat = cvCreateMat(5, 5, CV_32FC1);
	float element_3_2 = 7.7;
	*((float*) CV_MAT_ELEM_PTR(*mat, 3, 2)) = element_3_2;

	// below from example ch3_ex3_8.txt
	cvmSet(mat, 2, 2, 0.5000);
	cvSetReal2D(mat, 3, 3, 0.3300);

	printf(
			"Exercise 3_5, matrix created and accessed [3,2]=%f, [2,2]=%f, [3,3]=%f\n",
			CV_MAT_ELEM( *mat, float, 3, 2 ),
			CV_MAT_ELEM( *mat, float, 2, 2 ),
			CV_MAT_ELEM( *mat, float, 3, 3 ));
	return 0;

}
