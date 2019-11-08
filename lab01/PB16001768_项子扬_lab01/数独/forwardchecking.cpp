#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
int n,k;
int completecheck(int a[][10]){//用于检查是否赋值完毕 
	int flag=1;
	int i,j;
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			if(!a[i][j]) flag=0;
	return flag;
}

int Dcheck(int a[][10],int D[][10][11]){//用于检查其余每个变量值域是否都非空 
	int flag=1;
	int i,j,m;
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			if(!a[i][j]){//变量 
				flag=0;
				for(m=0;m<k;m++)
					if(D[i][j][m]) flag=1;
				if(!flag) return 0;
			}
	return 1;
}

void fowardchecking(int a[][10],int D[][10][11],int change[][10],int p,int q,int r){//前向检验技术，p、q是坐标，r是所赋的值 
	int i,j,m;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(!a[i][j]){//变量 
				if((i==p||j==q)&&D[i][j][r-1]==1){
					D[i][j][r-1]=0;//与刚刚赋值在同一行或同一列，缩小相应值域  
					change[i][j]=r;//记录更改 
				}
			}
		}
	}
}

void deforward(int D[][10][11],int change[][10]){//用于在回溯时将之前前向检验的值域变回 
	int i,j,m;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(change[i][j]) D[i][j][change[i][j]-1]=1;
		}
	}
}

int CSP_BACKTRACKING(int a[][10],int D[][10][11]){//算法核心部分 
	int i,j,m,flag;
	int change[10][10];//用于标识值域是否在fowardchecking中修改过
	memset(change,0,sizeof(change)); 
	if(completecheck(a)) return 1;//若赋值已完全则返回 
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
			a[i][j]=m+1;//赋值
			fowardchecking(a,D,change,i,j,m+1); 
			if(Dcheck(a,D))
				if(CSP_BACKTRACKING(a,D)) return 1;
			a[i][j]=0;
			deforward(D,change);
			memset(change,0,sizeof(change));
		}
	}	
	return 0;//当前赋值无解 
}

int main() {
	double run_time;
	_LARGE_INTEGER time_start;	//开始时间
	_LARGE_INTEGER time_over;	//结束时间
	double dqFreq;		//计时器频率
	LARGE_INTEGER f;	//计时器频率
	QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
	
	FILE *fpr,*fpw;
	int i,j,m,ret,p,q;
	fpr=fopen("input.txt","r");//打开文件指针 
	fscanf(fpr,"%d%d",&n,&k);//读取n和k 
	int a[10][10];
	int D[10][10][11]; //用于存储变量的值域 
	memset(a,0,sizeof(a));
	memset(D,0,sizeof(D));
	for(i=0;i<n;i++)
		for(j=0;j<n;j++) fscanf(fpr,"%d",&a[i][j]);//读取初始矩阵内容 
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(!a[i][j]){//是变量 
				 for(m=0;m<k;m++) D[i][j][m]=1; 
			}
		}
	}
	for(i=0;i<n;i++){//初始值域设定 
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
	QueryPerformanceCounter(&time_start);//计时开始 
	ret=CSP_BACKTRACKING(a,D);
	QueryPerformanceCounter(&time_over);//计时结束 
	run_time=1000000*(time_over.QuadPart-time_start.QuadPart)/dqFreq; 
	fpw=fopen("forward_solution.txt","w");//向文件中写入实验结果 
	fprintf(fpw,"%fus\n",run_time);  
	for(i=0;i<n;i++){
		for(j=0;j<n;j++) fprintf(fpw,"%d ",a[i][j]);
		fprintf(fpw,"\n"); 
	}
	fclose(fpr);
	fclose(fpw);//关闭文件指针 
	return 0;
}
