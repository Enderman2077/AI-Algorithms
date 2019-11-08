#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
int n,k;
int completecheck(int a[][10]){//���ڼ���Ƿ�ֵ��� 
	int flag=1;
	int i,j;
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			if(!a[i][j]) flag=0;
	return flag;
}

int Dcheck(int a[][10],int D[][10][11]){//���ڼ������ÿ������ֵ���Ƿ񶼷ǿ� 
	int flag=1;
	int i,j,m;
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			if(!a[i][j]){//���� 
				flag=0;
				for(m=0;m<k;m++)
					if(D[i][j][m]) flag=1;
				if(!flag) return 0;
			}
	return 1;
}

void fowardchecking(int a[][10],int D[][10][11],int change[][10],int p,int q,int r){//ǰ����鼼����p��q�����꣬r��������ֵ 
	int i,j,m;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(!a[i][j]){//���� 
				if((i==p||j==q)&&D[i][j][r-1]==1){
					D[i][j][r-1]=0;//��ոո�ֵ��ͬһ�л�ͬһ�У���С��Ӧֵ��  
					change[i][j]=r;//��¼���� 
				}
			}
		}
	}
}

void deforward(int D[][10][11],int change[][10]){//�����ڻ���ʱ��֮ǰǰ������ֵ���� 
	int i,j,m;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(change[i][j]) D[i][j][change[i][j]-1]=1;
		}
	}
}

int CSP_BACKTRACKING(int a[][10],int D[][10][11]){//�㷨���Ĳ��� 
	int i,j,m,flag;
	int change[10][10];//���ڱ�ʶֵ���Ƿ���fowardchecking���޸Ĺ�
	memset(change,0,sizeof(change)); 
	if(completecheck(a)) return 1;//����ֵ����ȫ�򷵻� 
	flag=0;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(!a[i][j]){
				flag=1;
				break;
			}
		}
		if(flag) break;
	}
	for(m=0;m<k;m++){
		if(D[i][j][m]){
			a[i][j]=m+1;//��ֵ
			fowardchecking(a,D,change,i,j,m+1); 
			if(Dcheck(a,D))
				if(CSP_BACKTRACKING(a,D)) return 1;
			a[i][j]=0;
			deforward(D,change);
			memset(change,0,sizeof(change));
		}
	}	
	return 0;//��ǰ��ֵ�޽� 
}

int main() {
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
	int D[10][10][11]; //���ڴ洢������ֵ�� 
	memset(a,0,sizeof(a));
	memset(D,0,sizeof(D));
	for(i=0;i<n;i++)
		for(j=0;j<n;j++) fscanf(fpr,"%d",&a[i][j]);//��ȡ��ʼ�������� 
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(!a[i][j]){//�Ǳ��� 
				 for(m=0;m<k;m++) D[i][j][m]=1; 
			}
		}
	}
	for(i=0;i<n;i++){//��ʼֵ���趨 
		for(j=0;j<n;j++){
			if(a[i][j]){
				for(p=0;p<n;p++){
					for(q=0;q<n;q++){
						if(!a[p][q]){
							if(p==i||q==j) D[p][q][a[i][j]-1]=0;
						}
					}
				}
			}
		}
	}
	QueryPerformanceCounter(&time_start);//��ʱ��ʼ 
	ret=CSP_BACKTRACKING(a,D);
	QueryPerformanceCounter(&time_over);//��ʱ���� 
	run_time=1000000*(time_over.QuadPart-time_start.QuadPart)/dqFreq; 
	fpw=fopen("forward_solution.txt","w");//���ļ���д��ʵ���� 
	fprintf(fpw,"%fus\n",run_time);  
	for(i=0;i<n;i++){
		for(j=0;j<n;j++) fprintf(fpw,"%d ",a[i][j]);
		fprintf(fpw,"\n"); 
	}
	fclose(fpr);
	fclose(fpw);//�ر��ļ�ָ�� 
	return 0;
}
