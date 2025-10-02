#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

enum TYPE {S, I, R};

int idx(int x, int y, int k)
{
    int side = 2 * k + 1;
    int xi = x + k;
    int yi = y + k;
    return xi * side + yi;
}

typedef struct Host
{
	int id;
	int x, y;
	int t;            // time since infected
	enum TYPE type;   // S, I, or R
} THost;

typedef struct node_tag {
   THost host;
   struct node_tag * next;
} node;

//Create a node whose value is a specific host
//Return a pointer to the created node
node * create_node(THost host)
{
    node *n = (node*)malloc(sizeof(node));
    if(!n) { perror("malloc"); exit(1); }
    n->host = host;
    n->next = NULL;
    return n;
}

//Add_first() should add to the beginning of a linked list
void add_first(node **head, node *newnode)
{
    if(!newnode) return;
    newnode->next = *head;
    *head = newnode;
}

//Remove the first node from the list and return it (caller frees)
node * remove_first(node **head)
{
    if(!head || !*head) return NULL;
    node *n = *head;
    *head = (*head)->next;
    n->next = NULL;
    return n;
}

//Remove all the nodes in the list and free memory
void remove_all(node **head)
{
    if(!head) return;
    node *cur = *head;
    while(cur)
    {
        node *nxt = cur->next;
        free(cur);
        cur = nxt;
    }
    *head = NULL;
}

//Location_match checks whether a linked list contains
//one or more hosts in the same location as 'host'
int location_match(node *head, THost host)
{
    for(node *p = head; p != NULL; p = p->next)
    {
        if(p->host.x == host.x && p->host.y == host.y)
            return 1;
    }
    return 0;
}

//Hash function (provided)
unsigned hash(unsigned a)
{
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}

//Summary prints out the proportions of different host types.
//It returns 1 if the number of infected hosts is not 0.
int summary(THost hosts[], int m)
{
    int S_n = 0, I_n = 0, R_n = 0;
    for(int i = 0; i < m; i++)
    {
        S_n += (hosts[i].type == S);
        I_n += (hosts[i].type == I);
        R_n += (hosts[i].type == R);
    }
    if(I_n == 0)
    {
        printf("S I R\n");
        printf("%lf %lf %lf\n",
               (double)S_n/(S_n + I_n + R_n),
               (double)I_n/(S_n + I_n + R_n),
               (double)R_n/(S_n + I_n + R_n));
    }
    return I_n > 0;
}


int one_round(THost *hosts, int m, node *p_arr[], int n_arr, int k, int T)
{
    // S -> I and I -> R
    for(int i = 0; i < m; i++)
    {
        if(hosts[i].type == S)
        {
            int index = (int)(hash((unsigned)idx(hosts[i].x, hosts[i].y, k)) % (unsigned)n_arr);
            if(location_match(p_arr[index], hosts[i]))
            {
                // Becomes infected now; will be contagious next step
                hosts[i].type = I;
                hosts[i].t = 0;
            }
        }
        else if(hosts[i].type == I)
        {
            hosts[i].t += 1;
            if(hosts[i].t >= T)
            {
                hosts[i].type = R;
            }
        }
    }

    // Reset all linked lists (hash buckets)
    for(int b = 0; b < n_arr; b++)
    {
        remove_all(&p_arr[b]);
    }

    //Move all hosts (random walk) with torus wrap-around
    for(int i = 0; i < m; i++)
    {
        int r = rand() % 4;
        switch(r)
        {
            case 0: //up: y++
                hosts[i].y += 1;
                if(hosts[i].y > k) hosts[i].y = -k;
                break;
            case 1: //right: x++
                hosts[i].x += 1;
                if(hosts[i].x > k) hosts[i].x = -k;
                break;
            case 2: //down: y--
                hosts[i].y -= 1;
                if(hosts[i].y < -k) hosts[i].y = k;
                break;
            case 3: //left: x--
                hosts[i].x -= 1;
                if(hosts[i].x < -k) hosts[i].x = k;
                break;
        }

        //Rebuild linked list only for infected hosts (contagious ones for next round)
        if(hosts[i].type == I)
        {
            node *rnode = create_node(hosts[i]);
            int index = (int)(hash((unsigned)idx(hosts[i].x, hosts[i].y, k)) % (unsigned)n_arr);
            add_first(&(p_arr[index]), rnode);
        }
    }

    return summary(hosts, m);
}

int main(int argc, char *argv[])
{
    if(argc != 5)
    {
        printf("Usage: %s k m T N\n", argv[0]);
        return 0;
    }
    int k = atoi(argv[1]);
    int m = atoi(argv[2]);
    int T = atoi(argv[3]);
    int N = atoi(argv[4]);

    assert(k >= 0 && k <= 1000);
    assert(m >= 1 && m <= 100000);
    assert(T >= 1);
    assert(N > 0 && N <= 100000);
    srand(12345);

    //Initialize hosts
    THost hosts[m];

    hosts[0].id = 0;
    hosts[0].x = 0;
    hosts[0].y = 0;
    hosts[0].t = 0;
    hosts[0].type = I;

    for(int i = 1; i < m; i ++)
    {
        hosts[i].id = i;
        hosts[i].x = rand() % (2*k + 1) - k;
        hosts[i].y = rand() % (2*k + 1) - k;
        hosts[i].t = 0;
        hosts[i].type = S;
    }

    //Initialize hash table buckets
    node *p_arr[N];
    for(int i = 0; i < N; i++)
    {
        p_arr[i] = NULL;
    }

    //Seed with the initial infected host at (0,0)
    node *r = create_node(hosts[0]);
    int index = (int)(hash((unsigned)idx(hosts[0].x, hosts[0].y, k)) % (unsigned)N);
    add_first(&(p_arr[index]), r);

    while(one_round(hosts, m, p_arr, N, k, T));


    return 0;
}
