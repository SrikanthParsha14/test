/*
 * lsr.c
 *
 *  Created on: 2011-9-19
 *      Author: qcg
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#define MAX_PATH 1024

/* dirwalk:  apply fcn to all files in dir */
void dirwalk(char *dir, void(*fcn)(char *)) {
	char name[MAX_PATH];
	struct dirent *dp;
	DIR *dfd;

	if ((dfd = opendir(dir)) == NULL) {
		fprintf(stderr, "dirwalk: can't open %s\n", dir);
		return;
	}
	while ((dp = readdir(dfd)) != NULL) {
		if (strcmp(dp->d_name, ".") == 0
				|| strcmp(dp->d_name, "..") == 0)
			continue; /* skip self and parent */
		if (strlen(dir) + strlen(dp->d_name) + 2 > sizeof(name))
			fprintf(stderr, "dirwalk: name %s %s too long\n", dir,
					dp->d_name);
		else {
			sprintf(name, "%s/%s", dir, dp->d_name);
			(*fcn)(name);
		}
	}
	closedir(dfd);
}

/* fsize:  print the size and name of file "name" */
void fsize(char *name) {
	struct stat stbuf;

	if (stat(name, &stbuf) == -1) {
		fprintf(stderr, "fsize: can't access %s\n", name);
		return;
	}
	if ((stbuf.st_mode & S_IFMT) == S_IFDIR
	)
		dirwalk(name, fsize);
	printf("%8ld %s\n", stbuf.st_size, name);
}
/* default: current directory */
/*
int main(int argc, char **argv) {
	if (argc == 1)
		fsize(".");
	else
		while (--argc > 0)
			fsize(*++argv);
	return 0;
}
*/
