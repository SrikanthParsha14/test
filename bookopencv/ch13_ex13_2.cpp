//
// From OpenCV's samples/c directory
//   Example 13-2. Creating and training a decision tree
//
#include "cxcore.h"
#include "highgui.h"

#include "ml.h"
#include <stdio.h>
/* *************** License:**************************
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
 ************************************************** */

/*
 The sample demonstrates how to build a decision tree for classifying mushrooms.
 It uses the sample base agaricus-lepiota.data from UCI Repository, here is the link:

 Newman, D.J. & Hettich, S. & Blake, C.L. & Merz, C.J. (1998).
 UCI Repository of machine learning databases
 [http://www.ics.uci.edu/~mlearn/MLRepository.html].
 Irvine, CA: University of California, Department of Information and Computer Science.
 */

// loads the mushroom database, which is a text file, containing
// one training sample per row, all the input variables and the output variable are categorical,
// the values are encoded by characters.
int mushroom_read_database(const char* filename, CvMat** data, CvMat** missing,
		CvMat** responses) {
	const int M = 1024;
	FILE* f = fopen(filename, "rt");
	CvMemStorage* storage;
	CvSeq* seq;
	char buf[M + 2], *ptr;
	float* el_ptr;
	CvSeqReader reader;
	int i, j, var_count = 0;

	if (!f)
		return 0;

	// read the first line and determine the number of variables
	if (!fgets(buf, M, f)) {
		fclose(f);
		return 0;
	}

	for (ptr = buf; *ptr != '\0'; ptr++)
		var_count += *ptr == ',';
	assert( ptr - buf == (var_count+1)*2);

	// create temporary memory storage to store the whole database
	el_ptr = new float[var_count + 1];storage
	= cvCreateMemStorage();
	seq = cvCreateSeq(0, sizeof(*seq), (var_count + 1) * sizeof(float),
			storage);

	for (;;) {
		for (i = 0; i <= var_count; i++) {
			int c = buf[i * 2];
			el_ptr[i] = c == '?' ? -1.f : (float) c;
		}
		if (i != var_count + 1)
			break;
		cvSeqPush(seq, el_ptr);
		if (!fgets(buf, M, f) || !strchr(buf, ','))
			break;
	}
	fclose(f);

	// allocate the output matrices and copy the base there
	*data = cvCreateMat(seq->total, var_count, CV_32F);
	*missing = cvCreateMat(seq->total, var_count, CV_8U);
	*responses = cvCreateMat(seq->total, 1, CV_32F);

	cvStartReadSeq(seq, &reader);

	for (i = 0; i < seq->total; i++) {
		const float* sdata = (float*) reader.ptr + 1;
		float* ddata = data[0]->data.fl + var_count * i;
		float* dr = responses[0]->data.fl + i;
		uchar* dm = missing[0]->data.ptr + var_count * i;

		for (j = 0; j < var_count; j++) {
			ddata[j] = sdata[j];
			dm[j] = sdata[j] < 0;
		}
		*dr = sdata[-1];
		CV_NEXT_SEQ_ELEM( seq->elem_size, reader);
	}

	cvReleaseMemStorage(&storage);
	delete el_ptr;
	return 1;
}

CvDTree* mushroom_create_dtree(const CvMat* data, const CvMat* missing,
		const CvMat* responses, float p_weight) {
	CvDTree* dtree;
	CvMat* var_type;
	int i, hr1 = 0, hr2 = 0, p_total = 0;
	float priors[] = { 1, p_weight };

	var_type = cvCreateMat(data->cols + 1, 1, CV_8U);
	cvSet(var_type, cvScalarAll(CV_VAR_CATEGORICAL)); // all the variables are categorical

	dtree = new CvDTree;

	dtree->train(data, CV_ROW_SAMPLE, responses, 0, 0, var_type, missing,
			CvDTreeParams(8, // max depth
					10, // min sample count
					0, // regression accuracy: N/A here
					true, // compute surrogate split, as we have missing data
					15, // max number of categories (use sub-optimal algorithm for larger numbers)
					10, // the number of cross-validation folds
					true, // use 1SE rule => smaller tree
					true, // throw away the pruned tree branches
					priors // the array of priors, the bigger p_weight, the more attention
					       // to the poisonous mushrooms
					       // (a mushroom will be judjed to be poisonous with bigger chance)
					));

	// compute hit-rate on the training database, demonstrates predict usage.
	for (i = 0; i < data->rows; i++) {
		CvMat sample, mask;
		cvGetRow(data, &sample, i);
		cvGetRow(missing, &mask, i);
		double r = dtree->predict(&sample, &mask)->value;
		int d = fabs(r - responses->data.fl[i]) >= FLT_EPSILON;
		if (d) {
			if (r != 'p')
				hr1++;
			else
				hr2++;
		}
		p_total += responses->data.fl[i] == 'p';
	}

	printf("Results on the training database:\n"
			"\tPoisonous mushrooms mis-predicted: %d (%g%%)\n"
			"\tFalse-alarms: %d (%g%%)\n", hr1,
			(double) hr1 * 100 / p_total, hr2,
			(double) hr2 * 100 / (data->rows - p_total));

	cvReleaseMat(&var_type);

	return dtree;
}

static const char* var_desc[] =
		{
				"cap shape (bell=b,conical=c,convex=x,flat=f)",
				"cap surface (fibrous=f,grooves=g,scaly=y,smooth=s)",
				"cap color (brown=n,buff=b,cinnamon=c,gray=g,green=r,\n\tpink=p,purple=u,red=e,white=w,yellow=y)",
				"bruises? (bruises=t,no=f)",
				"odor (almond=a,anise=l,creosote=c,fishy=y,foul=f,\n\tmusty=m,none=n,pungent=p,spicy=s)",
				"gill attachment (attached=a,descending=d,free=f,notched=n)",
				"gill spacing (close=c,crowded=w,distant=d)",
				"gill size (broad=b,narrow=n)",
				"gill color (black=k,brown=n,buff=b,chocolate=h,gray=g,\n\tgreen=r,orange=o,pink=p,purple=u,red=e,white=w,yellow=y)",
				"stalk shape (enlarging=e,tapering=t)",
				"stalk root (bulbous=b,club=c,cup=u,equal=e,rhizomorphs=z,rooted=r)",
				"stalk surface above ring (ibrous=f,scaly=y,silky=k,smooth=s)",
				"stalk surface below ring (ibrous=f,scaly=y,silky=k,smooth=s)",
				"stalk color above ring (brown=n,buff=b,cinnamon=c,gray=g,orange=o,\n\tpink=p,red=e,white=w,yellow=y)",
				"stalk color below ring (brown=n,buff=b,cinnamon=c,gray=g,orange=o,\n\tpink=p,red=e,white=w,yellow=y)",
				"veil type (partial=p,universal=u)",
				"veil color (brown=n,orange=o,white=w,yellow=y)",
				"ring number (none=n,one=o,two=t)",
				"ring type (cobwebby=c,evanescent=e,flaring=f,large=l,\n\tnone=n,pendant=p,sheathing=s,zone=z)",
				"spore print color (black=k,brown=n,buff=b,chocolate=h,green=r,\n\torange=o,purple=u,white=w,yellow=y)",
				"population (abundant=a,clustered=c,numerous=n,\n\tscattered=s,several=v,solitary=y)",
				"habitat (grasses=g,leaves=l,meadows=m,paths=p\n\turban=u,waste=w,woods=d)",
				0 };

void print_variable_importance(CvDTree* dtree, const char** var_desc) {
	const CvMat* var_importance = dtree->get_var_importance();
	int i;
	char input[1000];

	if (!var_importance) {
		printf("Error: Variable importance can not be retrieved\n");
		return;
	}

	printf("Print variable importance information? (y/n) ");
	scanf("%1s", input);
	if (input[0] != 'y' && input[0] != 'Y')
		return;

	for (i = 0; i < var_importance->cols * var_importance->rows; i++) {
		double val = var_importance->data.db[i];
		if (var_desc) {
			char buf[100];
			int len = strchr(var_desc[i], '(') - var_desc[i] - 1;
			strncpy(buf, var_desc[i], len);
			buf[len] = '\0';
			printf("%s", buf);
		} else
			printf("var #%d", i);
		printf(": %g%%\n", val * 100.);
	}
}

void interactive_classification(CvDTree* dtree, const char** var_desc) {
	char input[1000];
	const CvDTreeNode* root;
	CvDTreeTrainData* data;

	if (!dtree)
		return;

	root = dtree->get_root();
	data = dtree->get_data();

	for (;;) {
		const CvDTreeNode* node;

		printf(
				"Start/Proceed with interactive mushroom classification (y/n): ");
		scanf("%1s", input);
		if (input[0] != 'y' && input[0] != 'Y')
			break;
		printf(
				"Enter 1-letter answers, '?' for missing/unknown value...\n");

		// custom version of predict
		node = root;
		for (;;) {
			CvDTreeSplit* split = node->split;
			int dir = 0;

			if (!node->left
					|| node->Tn
							<= dtree->get_pruned_tree_idx()
					|| !node->split)
				break;

			for (; split != 0;) {
				int vi = split->var_idx, j;
				int count = data->cat_count->data.i[vi];
				const int* map = data->cat_map->data.i
						+ data->cat_ofs->data.i[vi];

				printf("%s: ", var_desc[vi]);
				scanf("%1s", input);

				if (input[0] == '?') {
					split = split->next;
					continue;
				}

				// convert the input character to the normalized value of the variable
				for (j = 0; j < count; j++)
					if (map[j] == input[0])
						break;
				if (j < count) {
					dir = (split->subset[j >> 5]
							& (1 << (j & 31))) ?
							-1 : 1;
					if (split->inversed)
						dir = -dir;
					break;
				} else
					printf("Error: unrecognized value\n");
			}

			if (!dir) {
				printf("Impossible to classify the sample\n");
				node = 0;
				break;
			}
			node = dir < 0 ? node->left : node->right;
		}

		if (node)
			printf(
					"Prediction result: the mushroom is %s\n",
					node->class_idx == 0 ?
							"EDIBLE" : "POISONOUS");
		printf("\n-----------------------------\n");
	}
}

int main(int argc, char** argv) {
	CvMat *data = 0, *missing = 0, *responses = 0;
	CvDTree* dtree;
	const char* base_path = argc >= 2 ? argv[1] : "agaricus-lepiota.data";

	if (!mushroom_read_database(base_path, &data, &missing, &responses)) {
		printf(
				"Unable to load the training database\n"
						"Pass it as a parameter: dtree <path to agaricus-lepiota.data>\n");
		return 0;
		return -1;
	}

	dtree = mushroom_create_dtree(data, missing, responses, 10 // poisonous mushrooms will have 10x higher weight in the decision tree
			);
	cvReleaseMat(&data);
	cvReleaseMat(&missing);
	cvReleaseMat(&responses);

	print_variable_importance(dtree, var_desc);
	interactive_classification(dtree, var_desc);
	delete dtree;

	return 0;
}
