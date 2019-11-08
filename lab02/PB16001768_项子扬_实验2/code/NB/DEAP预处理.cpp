#include<stdio.h>
#include<time.h>
#include<stdlib.h>
struct SAM{
	double feature[160];
	int id;
	int label[2];
};

int main(){
	FILE *fpr,*fpw[5];
	int i,j,k;
	int temp[1216];
	struct SAM sample[1216];
	fpr=fopen("data/DEAP/EEG_feature.txt","r");
	for(i=0;i<1216;i++){
		for(j=0;j<160;j++){
			fscanf(fpr,"%lf",&sample[i].feature[j]);
		}
	}
	fpr=fopen("data/DEAP/subject_video.txt","r");
	for(i=0;i<1216;i++){
		fscanf(fpr,"%d",&sample[i].id);
		fscanf(fpr,"%d",&temp[i]);
	}
	fpr=fopen("data/DEAP/valence_arousal_label.txt","r");
	for(i=0;i<1216;i++){
		fscanf(fpr,"%d",&sample[i].label[0]);
		fscanf(fpr,"%d",&sample[i].label[1]);
	}
	fpw[0]=fopen("DEAP_data1.txt","w");
	fpw[1]=fopen("DEAP_data2.txt","w");
	fpw[2]=fopen("DEAP_data3.txt","w");
	fpw[3]=fopen("DEAP_data4.txt","w");
	fpw[4]=fopen("DEAP_data5.txt","w");
	k=0;
	for(i=0;i<1215;i++){
		for(j=0;j<160;j++) fprintf(fpw[k],"%lf ",sample[i].feature[j]);
		fprintf(fpw[k],"%d ",sample[i].id);
		if(sample[i].label[0]==1&&sample[i].label[1]==1) fprintf(fpw[k],"11\n");
		else if(sample[i].label[0]==1&&sample[i].label[1]==2) fprintf(fpw[k],"12\n");
		else if(sample[i].label[0]==2&&sample[i].label[1]==1) fprintf(fpw[k],"21\n");
		else fprintf(fpw[k],"22\n");
		k=(k+1)%5;
	}
}
