#include <stdio.h>
#include <stdlib.h>

double two_d_random(int n)
{

	//Fill in code below
	//When deciding which way to go for the next step, generate a random number as follows.
	//r = rand() % 4;
	//Treat r = 0, 1, 2, 3 as up, right, down and left respectively.

	//The random walk should stop once the x coordinate or y coordinate reaches $-n$ or $n$. 
	//The function should return the fraction of the visited $(x, y)$ coordinates inside (not including) the square.

    long long total_interior = (long long)(2*n - 1) * (2*n - 1);

    // We count visits for interior points.
    int size = 2*n + 1;

    unsigned char visited[size][size];
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            visited[i][j] = 0;

    int x = 0, y = 0;            // Start at origin
    long long visited_interior = 0;

    // Mark an interior coordinate
    auto void mark_if_interior(int xx, int yy) {
        if (abs(xx) < n && abs(yy) < n) {
            int ix = xx + n;     // Shift to 0..2n
            int iy = yy + n;
            if (!visited[ix][iy]) {
                visited[ix][iy] = 1;
                visited_interior++;
            }
        }
    };

    mark_if_interior(x, y);

    // Walk until we hit any boundary line |x|==n or |y|==n
    while (1) {
        int r = rand() % 4;
        switch (r) {
            case 0: y -= 1; break;  // up (as per problem’s (x, y−1))
            case 1: x += 1; break;  // right
            case 2: y += 1; break;  // down
            case 3: x -= 1; break;  // left
        }

        // Stop if we’ve reached/touched the square boundary
        if (x == n || x == -n || y == n || y == -n) break;

        // Otherwise mark this interior point
        mark_if_interior(x, y);
    }

    // Fraction of interior points visited
    return (double)visited_interior / (double)total_interior;
}

//Do not change the code below
int main(int argc, char *argv[])
{
	int trials = 1000;
	int i, n, seed;
	if (argc == 2) seed = atoi(argv[1]);
	else seed = 12345;

	srand(seed);
	for(n=1; n<=64; n*=2)
	{	
		double sum = 0.;
		for(i=0; i < trials; i++)
		{
			double p = two_d_random(n);
			sum += p;
		}
		printf("%d %.3lf\n", n, sum/trials);
	}
	return 0;
}