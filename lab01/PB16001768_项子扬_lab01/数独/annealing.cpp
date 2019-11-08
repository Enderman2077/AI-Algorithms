#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include<windows.h>
int n,k;
int value(int a[][10]){//用于评估当前填法的优劣 
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

void Simulated_Annealing(int a[][10],int fixed[][10]){//算法核心部分 
	double T=5.0;//初始化T为5
	int i,j,p,q,r,delta,temp;
	int oldv,newv;
	for(i=0;i<n;i++){//赋初值，此处为对所有0填入1 
		for(j=0;j<n;j++)
		if(!a[i][j]) a[i][j]=1;
	}
	while(T>0){//循环直到温度足够低 
		oldv=value(a);
		if(oldv==2*n*n*n-2*n*n) return;//已经互不相同，返回 
		p=rand()%n;
		q=rand()%n;
		r=1+rand()%k;//先产生一个随机方向 
		while(fixed[p][q]==1||a[p][q]==r){//该方向必须合法 
			p=rand()%n;
			q=rand()%n;
			r=1+rand()%k;//若不合法则重新生成随机方向 
		}
		temp=a[p][q];
		a[p][q]=r;
		newv=value(a);
		delta=newv-oldv;
		if(delta<=0&&((float)(rand()%1000)/1000)>=exp(delta/T)){
			a[p][q]=temp;
		}
		T-=0.000005;//温度下降 
	}
	printf("timeout！\n");
	return;
}
int main(){
	srand((unsigned)time(NULL));
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
	int fixed[10][10];//用于标识是否为初始格子 
	memset(a,0,sizeof(a));
	memset(fixed,0,sizeof(fixed));
	for(i=0;i<n;i++)
		for(j=0;j<n;j++) fscanf(fpr,"%d",&a[i][j]);//读取初始矩阵内容
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			if(a[i][j]) fixed[i][j]=1;//是固定值 
	QueryPerformanceCounter(&time_start);//计时开始
	Simulated_Annealing(a,fixed);
	QueryPerformanceCounter(&time_over);//计时结束 
	run_time=1000000*(time_over.QuadPart-time_start.QuadPart)/dqFreq; 
	fpw=fopen("annealing_solution.txt","w");//向文件中写入实验结果 
	fprintf(fpw,"%fus\n",run_time);  
	for(i=0;i<n;i++){
		for(j=0;j<n;j++) fprintf(fpw,"%d ",a[i][j]);
		fprintf(fpw,"\n"); 
	}
	fclose(fpr);
	fclose(fpw);//关闭文件指针 
	return 0;
} 
