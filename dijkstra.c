/*
Program digunakan untuk menyimulasikan algoritma dijkstra yang digunakan pada pencarian jarak terdekat antar kota.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define node_max 50
#define max_len 255
#define int_max 2147483647

// Fungsi digunakan untuk menampilkan graph berukuran n x n ke layar
void printgraph(int n, int graph[max_len][max_len]){
    printf("\n-1 menyatakan jarak tak hingga\n");
    printf("0 menyatakan bahwa kedua vertex sama\n\n");
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            if(graph[i][j]==int_max){
                printf("%d ",-1);
            }
            else{
            printf("%d ", graph[i][j]);
            }
        }
        printf("\n");
    }
}

// Fungsi digunakan untuk mencari indeks yang akan diproses pada algoritma dijkstra
int idx_process(int n, int jarak_final[max_len], bool is_final[max_len]){
    bool is_found=false;
    int idx_min, val_minimum;
    int i;
    for(i=0;i<n;i++){
        //Inisiasi nilai minimum awal
        if(!is_final[i]&&!is_found){
            idx_min=i, val_minimum=jarak_final[i];
            is_found=true;
        }
        else if(is_found && !is_final[i] && jarak_final[i]<val_minimum){
            idx_min=i, val_minimum=jarak_final[i];
        }
    }
    if(is_found){
        return idx_min;
    }
    else{
        //Jarak minimum ke seluruh vertex telah ditemukan
        return int_max;
    }
}

int main (){
    char file_namakota[max_len]; 
    // Input file tanaman
    printf("Selamat datang di program pencarian jarak terdekat antar kota.\n");
    printf("Silahkan masukkan nama file database nama kota: ");
    scanf("%s",&file_namakota);
    FILE* fp_kota = fopen(file_namakota,"r");

    // Baca isi file nama kota
    char baris[max_len];
    char* token;
    int n_kota=0;
    char nama_kota[node_max][max_len];
    while (fgets(baris, max_len, fp_kota)){
        token = strtok(baris,"\n");
        strcpy(nama_kota[n_kota],token);
        n_kota += 1;
    }
    printf("\nPembacaan nama-nama kota dalam database berhasil.\n");
    printf("Terdapat %d nama kota di dalam database tersebut\n\n",n_kota);
    // Input graph (berisi jarak masing-masing tanaman). -1 atau 2147483647=INT_MAX menyatakan tak hingga.
    printf("Silahkan masukkan nama file yang berisi jarak antar kota: ");
    char namafile_graph[max_len];
    scanf("%s",&namafile_graph);
    // File eksternal berbentuk matriks segitiga bawah
    int graph_kota[max_len][max_len]; 
    int j=0,k;
    FILE* fp_jarak_kota = fopen(namafile_graph,"r");
    while (fgets(baris, max_len, fp_jarak_kota)){
        k=0;
        token = strtok(baris," ");
        while (token != NULL) {
            graph_kota[j][k] = atoi(token);
            graph_kota[k][j] = atoi(token);
            if(atoi(token)==-1){
                graph_kota[j][k] = int_max;
                graph_kota[k][j] = int_max;
            }
            k+=1;
            token = strtok(NULL," ");
        }
        j+=1;
    }
    printf("\nPembacaan jarak antar kota berhasil dilakukan.\n");
    //printgraph(n_kota,graph_kota);
    char progress='y';
    while(progress=='y'){
        // Pembacaan vertex awal dan akhir
        int idx_awal, idx_tujuan;
        int i;
        printf("\nBerikut adalah daftar nama kota yang terdapat dalam database.\n");
        for(i=1;i<=n_kota;i++){
            printf("%d. %s\n",i,nama_kota[i-1]);
        }
        printf("\nPilih salah satu nama kota sebagai posisi awal (Cukup ketik angka saja): ");
        scanf(" %d",&idx_awal);
        idx_awal-=1;
        printf("Pilih salah satu nama kota sebagai kota tujuan (Cukup ketik angka saja): ");
        scanf(" %d",&idx_tujuan);
        idx_tujuan-=1;
        int jarak_final[max_len]; //Jarak akhir ke masing2 kota, akan di update terus sesuai jarak yang ditemukan
        bool is_final[max_len]; //Memberikan keterangan apakah jarak sudah minimum
        int list_dilalui[max_len][max_len+1]; //Indeks yang dilalui hingga mencapai tujuan
        int idx_dilalui[max_len]; //Mencatat jumlah vertex yang dilalui pada list_dilalui
        //Algoritma Dijsktra  
        for(i=0;i<n_kota;i++){
            //Set jarak menjadi tak hingga pada seluruh vertex selain vertex awal dan
            if(i==idx_awal){
                jarak_final[i]=0;
                is_final[i]=false;
            }
            else{
                jarak_final[i]=int_max;
                is_final[i]=false;
            }
            //Set list dilalui menjadi no vertex (dilambangkan dengan int_max)
            for(j=0;j<=n_kota;j++){
                list_dilalui[i][j]=int_max;
            }
            idx_dilalui[i]=0;
        }
        //Inisiasi jarak awal pada vertex awal
        jarak_final[idx_awal]=0;
        list_dilalui[idx_awal][0]=idx_awal;
        idx_dilalui[idx_awal]+=1;
        int idx_now=idx_awal;
        //Menentukan index yang diproses
        while(idx_now!=int_max){
            is_final[idx_now]=true;
            //printf("idx process = %d\n",idx_now);
            for(i = 0;i<n_kota;i++){
                //Update nilai saat jarak lebih pendek 
                if(!is_final[i] && (graph_kota[idx_now][i]!=int_max) && 
                (jarak_final[idx_now]+graph_kota[idx_now][i]<jarak_final[i])){
                    jarak_final[i] = jarak_final[idx_now]+graph_kota[idx_now][i];
                    idx_dilalui[i]=idx_dilalui[idx_now]+1;
                    for(j=0;j<idx_dilalui[i];j++){
                        if(j==idx_dilalui[i]-1){
                            list_dilalui[i][j]=i;
                        }
                        else{
                            list_dilalui[i][j]=list_dilalui[idx_now][j];
                        }
                    }
                    //printf("Jarak Final[%d] = %d\n",i,jarak_final[i]);
                }
            }
            idx_now = idx_process(n_kota,jarak_final,is_final);
        }
        printf("\nKota yang harus dilalui untuk mencapai Kota %s dengan posisi awal Kota %s adalah: \n",nama_kota[idx_tujuan],nama_kota[idx_awal]);
        for(i=0;i<idx_dilalui[idx_tujuan];i++){
            if(list_dilalui[idx_tujuan][i]!=int_max){
                printf("%d. %s\n",i+1,nama_kota[list_dilalui[idx_tujuan][i]]);
            }
        }
        printf("\nJarak antara kedua kota tersebut adalah %d meter\n\n",jarak_final[idx_tujuan]);
        /* // Digunakan untuk debugging
        printf("Jarak final: %d\n",jarak_final[idx_tujuan]);
        printf("Indeks Tujuan: %d\n",idx_tujuan);
        for(int i=0;i<n_kota;i++){
            printf("Jarak Final[%d] = %d\n",i,jarak_final[i]);
        }
        for(int i=0;i<n_kota;i++){
            printf("List dilalui idx %d: ",i);
            for(int j=0;j<n_kota;j++){
                printf("%d ",list_dilalui[i][j]);
            }
            printf("\n");
        }*/ 
        printf("Apakah anda masih ingin mengetahui jarak antar kota yang terdapat dalam database? [y/n]: ");
        scanf(" %c",&progress);
    }
}