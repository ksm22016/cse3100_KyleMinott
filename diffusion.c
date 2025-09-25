#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//TODO
//Implement the below function
//Simulate one particle moving n steps in random directions
//Use a random number generator to decide which way to go at every step
//When the particle stops at a final location, use the memory pointed to by grid to 
//record the number of particles that stop at this final location
//Feel free to declare, implement and use other functions when needed
static inline int grid_idx(int n, int x, int y, int z) {
    int L = 2*n + 1;
    return (x + n) * L * L + (y + n) * L + (z + n);
}
void one_particle(int *grid, int n)
{
    int x = 0, y = 0, z = 0;
    int L = 2*n + 1;

    for (int step = 0; step < n; ++step) {
        int r = rand() % 6;
        switch (r) {
            case 0: x--; break; //Left
            case 1: x++; break; //Right
            case 2: y++; break; //Forward/up on y
            case 3: y--; break; //Backward/down on y
            case 4: z++; break; //Up on z
            default: z--; break; //Down on z
        }
    }

    grid[ (x + n)*L*L + (y + n)*L + (z + n) ]++;
}

//TODO
//Implement the following function
//This function returns the fraction of particles that lie within the distance
//r*n from the origin (including particles exactly r*n away)
//The distance used here is Euclidean distance
//Note: you will not have access to math.h when submitting on Mimir
double density(int *grid, int n, double r)
{
    int L = 2*n + 1;
    long total = 0;
    long inside = 0;
    double rad2 = (r * n) * (r * n);

    for (int x = -n; x <= n; ++x) {
        for (int y = -n; y <= n; ++y) {
            for (int z = -n; z <= n; ++z) {
                int idx = (x + n)*L*L + (y + n)*L + (z + n);
                int cnt = grid[idx];
                if (!cnt) continue;
                total += cnt;

                int d2 = x*x + y*y + z*z;
                if ((double)d2 <= rad2) inside += cnt;
            }
        }
    }

    if (total == 0) return 0.0;
    return (double)inside / (double)total;    
}

//use this function to print results
void print_result(int *grid, int n)
{
    printf("radius density\n");
    for(int k = 1; k <= 20; k++)
    {
        printf("%.2lf   %lf\n", 0.05*k, density(grid, n, 0.05*k));
    }
}

//TODO
//Finish the following function
//See the assignment decription on Piazza for more details
void diffusion(int n, int m)
{
	//fill in a few line of code below
    int L = 2*n + 1;
    size_t cells = (size_t)L * L * L;

    int *grid = calloc(cells, sizeof *grid);
    assert(grid != NULL);

	for(int i = 1; i<=m; i++) one_particle(grid, n);

	print_result(grid, n);
	//fill in some code below
    free (grid);

}

int main(int argc, char *argv[])
{
	
	if(argc != 3)
	{
		printf("Usage: %s n m\n", argv[0]);
		return 0; 
	}
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);

	assert(n >= 1 && n <=50);
	assert(m >= 1 && m <= 1000000);
	srand(12345);
	diffusion(n, m);
	return 0;
}