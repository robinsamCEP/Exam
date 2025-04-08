#include <stdio.h>
#include <stdlib.h>

#define BUCKET_SIZE 10
#define OUTPUT_RATE 3

int leaky_bucket(int packet[],int n){
    int bucket=0;
    int i;
    printf("\nTime  Incoming  Bucket Level  Outgoing\n");
    printf("--------------------------------------------\n");

    for(i=0;i<n;i++){
        printf("%d\t%d\t",i+1,packet[i]);
        if(packet[i]>BUCKET_SIZE){
            printf("Exceeded!\tDropped\n");
            continue;
        }
        if(bucket+packet[i]<=BUCKET_SIZE){
            bucket+=packet[i];
        }else{
            printf("Overflow!\tDropped\n");
            bucket=BUCKET_SIZE;
            continue;
            
        }
        printf("%d\t\t",bucket);
        if(bucket<=OUTPUT_RATE){
            printf("%d\n",bucket);
            bucket=0;
        }else{
            printf("%d\n",OUTPUT_RATE);
            bucket-=OUTPUT_RATE;
           

        }
    }
    while(bucket>0){
        printf("%d\t0\t%d\t\t",++i,bucket);
        if(bucket<=OUTPUT_RATE){
            printf("%d\t\t",bucket);
            bucket=0;
        }else{
            printf("%d\n",OUTPUT_RATE);
            bucket-=OUTPUT_RATE;
           

        }
    }   
    
}
int main(){
    int packet[]={4,8,2,5,3,12};
    int n=sizeof(packet)/sizeof(packet[0]);

    leaky_bucket(packet,n);
    return 0;
}

