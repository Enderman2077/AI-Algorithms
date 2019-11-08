#include<stdio.h>
#include<time.h>
#include<stdlib.h>
struct SAM{
	double feature[160];
	int id;
	int video;
	int label[2];
	int emo;
};

int main(){
	FILE *fpr,*fpw[5];
	int i,j,k;
	int temp[533];
	struct SAM sample[533];
	fpr=fopen("data/MAHNOB-HCI/EEG_feature.txt","r");
	for(i=0;i<533;i++){
		for(j=0;j<160;j++){
			fscanf(fpr,"%lf",&sample[i].feature[j]);
		}
	}
	fpr=fopen("data/MAHNOB-HCI/subject_video.txt","r");
	for(i=0;i<533;i++){
		fscanf(fpr,"%d",&sample[i].id);
		fscanf(fpr,"%d",&sample[i].video);
	}
	fpr=fopen("data/MAHNOB-HCI/valence_arousal_label.txt","r");
	for(i=0;i<533;i++){
		fscanf(fpr,"%d",&sample[i].label[0]);
		fscanf(fpr,"%d",&sample[i].label[1]);
	}
	fpr=fopen("data/MAHNOB-HCI/EEG_emotion_category.txt","r");
	for(i=0;i<533;i++){
		fscanf(fpr,"%d",&sample[i].emo);
	}
	fpw[0]=fopen("MAHNOB-HCI_data1.txt","w");
	fpw[1]=fopen("MAHNOB-HCI_data2.txt","w");
	fpw[2]=fopen("MAHNOB-HCI_data3.txt","w");
	fpw[3]=fopen("MAHNOB-HCI_data4.txt","w");
	fpw[4]=fopen("MAHNOB-HCI_data5.txt","w");
	k=0;
	for(i=0;i<530;i++){
		for(j=0;j<160;j++) fprintf(fpw[k],"%lf ",sample[i].feature[j]);
		fprintf(fpw[k],"%d ",sample[i].id);
		if(sample[i].label[0]==1&&sample[i].label[1]==1) fprintf(fpw[k],"11 ");
		else if(sample[i].label[0]==1&&sample[i].label[1]==2) fprintf(fpw[k],"12 ");
		else if(sample[i].label[0]==2&&sample[i].label[1]==1) fprintf(fpw[k],"21 ");
		else fprintf(fpw[k],"22 ");
		fprintf(fpw[k],"%d\n",sample[i].emo);
		k=(k+1)%5;
	}
}
