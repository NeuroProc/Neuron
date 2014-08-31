#include <stdio.h>
#include <iostream>
#include <stdlib.h>

typedef long long int64;

static int N = 0;

bool findPath(int64 u1, bool **d, int64 *matching, bool *vis) {
    vis[u1] = true;
    for (int64 v = 0; v < N; ++v) {
      int64 u2 = matching[v];
      if (d[u1][v] && (u2 == -1 || !vis[u2] && findPath(u2, d, matching, vis))) {
        matching[v] = u1;
        return true;
      }
    }
    return false;
}

int maxMatching(bool **d) {
    int64 n1 = N;
    int64 n2 = n1 == 0 ? 0 : N;
    int64 *matching = (int64 *)malloc(sizeof(int64) * n2);
    //std::fill(matching, sizeof(int64) * n2, -1);
    for (int64 i = 0; i < n2; i++)
        matching[i] = -1;
    int matches = 0;
    for (int64 u = 0; u < n1; u++) {
        bool *a = (bool *)malloc(sizeof(bool) * n1);
        if (findPath(u, d, matching, a))
            ++matches;
        free(a);
    }

    free(matching);
    
    return matches;
}

  // Usage example
int main(void) {
	int i, j, a, b;
    scanf("%d", &N);

    bool **mas = (bool **)malloc(sizeof(bool *) * N);
    
    for (i = 0; i < N; i++)
    {
        mas[i] = (bool *)malloc(sizeof(bool) * N);
        /*
        for (j = 0; j < N; j++)
        {
            //mas[i][j] = false;
            printf("%d ", mas[i][j]);
        }
        printf("\n");
        */
    }
    
    //System.out.println(mas.getLong);
    
    for (i = 0; i < N-1; i++)
    {
        scanf("%d %d", &a, &b);
        
        //printf("%d %d\n",a, b);
        
        mas[a - 1][b - 1] = true;
        mas[b - 1][a - 1] = true;
    }

    /*
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
            printf("%d ",mas[i][j]);
        printf("\n");
    }
    */
    
    
    int res = maxMatching(mas);
    printf("%d\n",res >> 1);

    free(mas);
    
    return 0;

    /*
    int res = maxMatching(new boolean[][] { { false, true , false, false, false, false, false, false, false, false, false, false }, 
    										{ true , false, true , true , false, false, false, false, false, false, false, false },
    										{ false, true , false, false, false, false, false, false, false, false, false, false },
    										{ false, true , false, false, true , false, false, false, true , false, true , false },
    										{ false, false, false, true , false, true , true , false, false, false, false, false },
    										{ false, false, false, false, true , false, false, false, false, false, false, false },
    										{ false, false, false, false, true , false, false, true , false, false, false, false },
    										{ false, false, false, false, false, false, true , false, false, false, false, false },
    										{ false, false, false, true , false, false, false, false, false, true , false, false },
    										{ false, false, false, false, false, false, false, false, true , false, false, false },
    										{ false, false, false, true , false, false, false, false, false, false, false, true  },
    										{ false, false, false, false, false, false, false, false, false, false, true , false }
    										});
    System.out.println(res);
    */
    
}


