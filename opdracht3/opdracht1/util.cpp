#include "util.h"

int maximum(int x, int y, int z) {
	int max = x; /* assume x is the largest */

	if (y > max) { /* if y is larger than max, assign y to max */
		max = y;
	} /* end if */

	if (z > max) { /* if z is larger than max, assign z to max */
		max = z;
	} /* end if */

	return max; /* max is the largest value */
}
int minimum(int x, int y, int z) {
	int min = x; /* assume x is the smallest */

	if (y < min) { /* if y is larger than max, assign y to max */
		min = y;
	} /* end if */

	if (z < min) { /* if z is larger than max, assign z to max */
		min = z;
	} /* end if */

	return min; /* max is the largest value */
}
