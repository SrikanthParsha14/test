#!/usr/bin/awk -f

BEGIN	{ FS = "\t" }
	{ pop[$4] += $3 }
END	{ for (c in pop)
		printf("%15s\t%6d\n", c, pop[c]) | "sort -t'\t' -k 2,2 -rn"
	}
