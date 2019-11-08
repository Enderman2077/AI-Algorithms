#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include<windows.h>
int n,k;
int value(int a[][10]){//����������ǰ������� 
	int v=0;
	int i,j,p,q;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			for(p=0;p<n;p++){
				if(p!=i&&(a[p][j]!=a[i][j])) v++;
			}
			for(q=0;q<n;q++){
				if(q!=j&&(a[i][q]!=a[i][j])) v++;
			}
		}
	}
	return v;
}

void Simulated_Annealing(int a[][10],int fixed[][10]){//�㷨���Ĳ��� 
	double T=5.0;//��ʼ��TΪ5
	int i,j,p,q,r,delta,temp;
	int oldv,newv;
	for(i=0;i<n;i++){//����ֵ���˴�Ϊ������0����1 
		for(j=0;j<n;j++)
		if(!a[i][j]) a[i][j]=1;
	}
	while(T>0){//ѭ��ֱ���¶��㹻�� 
		oldv=value(a);
		if(oldv==2*n*n*n-2*n*n) return;//�Ѿ�������ͬ������ 
		p=rand()%n;
		q=rand()%n;
		r=1+rand()%k;//�Ȳ���һ��������� 
		while(fixed[p][q]==1||a[p][q]==r){//�÷������Ϸ� 
			p=rand()%n;
			q=rand()%n;
			r=1+rand()%k;//�����Ϸ������������������ 
		}
		temp=a[p][q];
		a[p][q]=r;
		newv=value(a);
		delta=newv-oldv;
		if(delta<=0&&((float)(rand()%1000)/1000)>=exp(delta/T)){
			a[p][q]=temp;
		}
		T-=0.000005;//�¶��½� 
	}
	printf("timeout��\n");
	return;
}
int main(){
	srand((unsigned)time(NULL));
	double run_time;
	_LARGE_INTEGER time_start;	//��ʼʱ�� 
	_LARGE_INTEGER time_over;	//����ʱ��
	double dqFreq;		//��ʱ��Ƶ��
	LARGE_INTEGER f;	//��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
	
	FILE *fpr,*fpw;
	int i,j,m,ret,p,q;
	fpr=fopen("input.txt","r");//���ļ�ָ�� 
	fscanf(fpr,"%d%d",&n,&k);//��ȡn��k 
	int a[10][10];
	int fixed[10][10];//���ڱ�ʶ�Ƿ�Ϊ��ʼ���� 
	memset(a,0,sizeof(a));
	memset(fixed,0,sizeof(fixed));
	for(i=0;i<n;i++)
		for(j=0;j<n;j++) fscanf(fpr,"%d",&a[i][j]);//��ȡ��ʼ��������
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			if(a[i][j]) fixed[i][j]=1;//�ǹ̶�ֵ 
	QueryPerformanceCounter(&time_start);//��ʱ��ʼ
	Simulated_Annealing(a,fixed);
	QueryPerformanceCounter(&time_over);//��ʱ���� 
	run_time=1000000*(time_over.QuadPart-time_start.QuadPart)/dqFreq; 
	fpw=fopen("annealing_solution.txt","w");//���ļ���д��ʵ���� 
	fprintf(fpw,"%fus\n",run_time);  
	for(i=0;i<n;i++){
		for(j=0;j<n;j++) fprintf(fpw,"%d ",a[i][j]);
		fprintf(fpw,"\n"); 
	}
	fclose(fpr);
	fclose(fpw);//�ر��ļ�ָ�� 
	return 0;
} 
