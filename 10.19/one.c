//
// Created by 何明阳 on 2025/10/19.
//
#include "stdio.h"
#define N 10001
int main()
{
    int n=0;
    int a[N];
    scanf("%d",&n);
    int p=1;
    for (p = 1; p <=n; ++p) {
        scanf("%d",&a[p]);
    }
    int i=1;
    while (i<n){
        if(a[i]!=a[i+1]){
            ++i;
        } else{
            int j=i+1;
            for (j=i+1;j<=n;j++)
            {
                a[j-1]=a[j];
            }
            n--;
        }
    }
    int m;
    for (m = 1; m <= n; ++m) {
        printf("%d ",a[m]);
    }
}