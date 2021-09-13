#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

double calculate_difference(double **current_clusters, double* vector, int cluster, int d);
int check_min_cluster(double **current_clusters, double *vector, int k, int d);
int num_of_lines(FILE *fp);
int num_of_columns(FILE *fp);

double calculate_difference(double **current_clusters, double* vector, int cluster, int d){
    double sum = 0;
    int j;
    for (j = 0; j<d; j++){
        sum += ((vector[j] - current_clusters[cluster][j])*(vector[j] - current_clusters[cluster][j]));
    }
    return sum;
}


int check_min_cluster(double **current_clusters, double *vector, int k, int d){
    int i;
    int cluster;
    double new_dist;
    double dist;
    i = 0;
    dist = calculate_difference(current_clusters, vector, i, d);
    for (cluster = 1; cluster<k; cluster++){
        new_dist = calculate_difference(current_clusters, vector, cluster, d);
        if (new_dist<dist){
            i = cluster;
            dist = new_dist;
        }
    }
    return i;
}

int num_of_lines(FILE *fp){
    int ch;
    int lines = 0;

    while(!feof(fp))
    {
        ch = fgetc(fp);
        if(ch == '\n'){
            lines++;
        }
    }
return lines;
}

int num_of_columns(FILE *fp){
    int ch;
    int columns = 1;

    while(!feof(fp))
    {
        ch = fgetc(fp);
        if(ch == ','){
            columns++;
        }
        if(ch == '\n'){
            break;
        }
    }
    return columns;
}



int main(int argc, char* argv[]){
    int k, N, d, MAX_ITER, i, j, counter, centroid, prev, change, cluster;
    double new_val;
    double *p1;
    double **obs;
    int *counts;
    double *p2;
    double **sums;
    int *members;
    double *p3;
    double **current_clusters;
    double n1;
    char c;
    long int bOfFile;

    if(argc == 2){
        MAX_ITER = INT_MAX;
        k = atoi(argv[1]);
    }
    else {
        if(argc == 1){
            printf("%s\n", "K has to be input");
            return 0;
        }
        if(argc > 3){
            printf("%s\n", "invalid input");
            return 0;
        }
        else{
            k = atoi(argv[1]);
            MAX_ITER = atoi(argv[2]);
        }

    }
    
    bOfFile = ftell(stdin);/*save the address of the beginning of the file */
    N = num_of_lines(stdin);

    fseek(stdin, bOfFile, SEEK_SET);/*set the file position back to the beginning */
    d = num_of_columns(stdin);
    fseek(stdin, bOfFile, SEEK_SET);/*set the file position back to the beginning */

    if(k >= N || d <= 0 || k <= 0 || N <= 0 || MAX_ITER <= 0){
        printf("%s\n", "invalid input");
        return 0;
    }

    p1 = calloc(N*d, sizeof(double));
    obs = calloc(N, sizeof(double*));
    assert(p1 != NULL && obs != NULL);
    for (i = 0; i<N; i++){
        obs[i] = p1 + i*d;
    }

    counts = calloc(k, sizeof(int));
    assert(counts != NULL);

    p2 = calloc(k*d, sizeof(double));
    sums = calloc(k, sizeof(double*));
    assert(p2 != NULL && sums != NULL);
    for (i = 0; i<k; i++){
        sums[i] = p2 + i*d;
    }

    members = calloc(N, sizeof(int));
    assert(members != NULL);

    p3 = calloc(k*d, sizeof(double));
    current_clusters = calloc(k, sizeof(double*));
    assert(p3 != NULL && current_clusters != NULL);
    for (i = 0; i<k; i++){
        current_clusters[i] = p3 + i*d;
    }

    while (scanf("%lf%c", &n1, &c) == 2){
        p1[counter] = n1;
        counter++;
    }


    for(i = 0; i<N; i++){
        members[i] = k;
    }

    for(i = 0; i < k; i++) {
        for(j = 0; j < d; j++) {
            sums[i][j] = obs[i][j];
            current_clusters[i][j] = obs[i][j];
        }
        counts[i] = 1;
        members[i] = i;
    }

    for(counter = 0; counter < MAX_ITER; counter++){
        change = 1;
        for(i = 0; i < N; i++){
            centroid = check_min_cluster(current_clusters, obs[i], k, d);
            prev = members[i];
            if(prev != centroid){
                for(j = 0; j < d; j++){
                    if (prev != k){
                        sums[prev][j] -= obs[i][j];
                    }
                    sums[centroid][j] += obs[i][j];
                }
                if (prev != k){
                    counts[prev] -= 1;
                }
                counts[centroid] += 1;
                members[i] = centroid;
            }
        }
        for(cluster = 0; cluster < k; cluster++){
            for(j = 0; j<d; j++){
                new_val = sums[cluster][j]/counts[cluster];
                if (current_clusters[cluster][j] != new_val){
                    change = 0;
                }
                current_clusters[cluster][j] = new_val;
            }
        }

        if(change){
            break;
        }
    }

    for(i = 0; i < k; i++) {
        printf("%.4f", current_clusters[i][0]);
        for(j = 1; j < d; j++) {
            printf("%s%.4f", ",", current_clusters[i][j]);
        }
        printf("\n");
    }


    free(obs);
    free(counts);
    free(sums);
    free(members);
    free(current_clusters);
    free(p1);
    free(p2);
    free(p3);

    return 0;
}

