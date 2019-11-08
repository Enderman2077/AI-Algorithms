#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define pi 3.14159265
#define M 424//训练样本数
#define N 106//测试样本数
#define C 13

struct SAM {
	double feature[160];//160维脑电波特征数据
	int id;//被试者编号
	int label;//愉悦度标签，只能取11、12、21、22 
	int emo;//情感标签
};

int main() {
	FILE *fpr,*fpw;//读写文件指针
	int i,j,k,cnt[C],right,res;
	int count1[C][4],count2[C][13];
	double pc[C],mean[C][160],var[C][160],sum[C][160],pt[C],max;
	double pxc1[C][4];//愉悦唤醒度标签的条件密度 
	double pxc2[C][13];//情感标签的条件密度 
	double correct;
	int h[N];//测试结果
	struct SAM sample[M];
	struct SAM test[N];
	fpr=fopen("MAHNOB-HCI_data1.txt","r");//读取数据1
	for(i=0;i<N;i++){
		for(j=0;j<160;j++){
			fscanf(fpr,"%lf",&sample[i].feature[j]);
		}
		fscanf(fpr,"%d%d%d", &sample[i].id, &sample[i].label,&sample[i].emo);
	}
	fpr=fopen("MAHNOB-HCI_data2.txt","r");//读取数据2
	for(i=N;i<2*N;i++){
		for(j=0;j<160;j++){
			fscanf(fpr,"%lf",&sample[i].feature[j]);
		}
		fscanf(fpr,"%d%d%d", &sample[i].id, &sample[i].label,&sample[i].emo);
	}
	fpr=fopen("MAHNOB-HCI_data3.txt","r");//读取数据3
	for(i=2*N;i<3*N;i++){
		for(j=0;j<160;j++){
			fscanf(fpr,"%lf",&sample[i].feature[j]);
		}
		fscanf(fpr,"%d%d%d", &sample[i].id, &sample[i].label,&sample[i].emo);
	}
	fpr=fopen("MAHNOB-HCI_data4.txt","r");//读取数据4
	for(i=3*N;i<M;i++){
		for(j=0;j<160;j++){
			fscanf(fpr,"%lf",&sample[i].feature[j]);
		}
		fscanf(fpr,"%d%d%d", &sample[i].id, &sample[i].label,&sample[i].emo);
	}
	//计算先验概率 
	for(i=0;i<C;i++) cnt[i]=0;
	for(i=0;i<M;i++){
		cnt[sample[i].emo]++;
	}
	for(i=0;i<C;i++){//拉普拉斯修正 
		pc[i]=(double)(cnt[i]+1)/(M+C);
	}
	//计算条件概率 
	for(i=0;i<C;i++){//初始化 
		for(j=0;j<160;j++) sum[i][j]=0.0;
	}
	for(i=0;i<M;i++){//遍历训练集 
		for(j=0;j<160;j++){
			sum[sample[i].emo][j]+=sample[i].feature[j];
		}
	}
	for(i=0;i<C;i++){//平均值 
		for(j=0;j<160;j++){
			mean[i][j]=(double)sum[i][j]/cnt[i];
		}
	}
	for(i=0;i<C;i++){//初始化 
		for(j=0;j<160;j++) sum[i][j]=0.0;
	} 
	for(i=0;i<M;i++){//遍历训练集 
		for(j=0;j<160;j++){
			sum[sample[i].emo][j]+=pow(sample[i].feature[j]-mean[sample[i].emo][j],2);
		}
	}
	for(i=0;i<C;i++){//方差
		for(j=0;j<160;j++){
			var[i][j]=(double)sum[i][j]/cnt[i];
		}
	}
	//计算标签的条件概率
	for(i=0;i<C;i++){//初始化 
		for(j=0;j<4;j++) count1[i][j]=0;
	}
	for(i=0;i<M;i++){
		switch(sample[i].label){//统计label标签 
			case 11:{
				count1[sample[i].emo][0]++;
				break;
			}
			case 12:{
				count1[sample[i].emo][1]++;
				break;
			}
			case 21:{
				count1[sample[i].emo][2]++;
				break;
			}
			case 22:{
				count1[sample[i].emo][3]++;
				break;
			}
			default:printf("error!\n");
		}
	}
	for(i=0;i<C;i++){
		for(j=0;j<4;j++){//拉普拉斯修正
			pxc1[i][j]=(double)(count1[i][j]+1)/(M+4);
		} 
	}
	/*for(i=0;i<C;i++){//计算情感标签的条件概率 
		for(j=0;j<13;j++) count2[i][j]=0;
	}
	for(i=0;i<M;i++){
		count2[sample[i].id-1][sample[i].emo]++; 
	}
	for(i=0;i<C;i++){
		for(j=0;j<13;j++){//拉普拉斯修正
			pxc2[i][j]=(double)(count2[i][j]+1)/(M+13);
		}
	}
	*/
	//验证阶段 
	fpr=fopen("MAHNOB-HCI_data5.txt","r");//读取数据5
	for(i=0;i<N;i++){
		for(j=0;j<160;j++){
			fscanf(fpr,"%lf",&test[i].feature[j]);
		}
		fscanf(fpr,"%d%d%d", &test[i].id, &test[i].label,&test[i].emo);
	}
	for(i=0;i<N;i++){//计算概率估计
		for(j=0;j<C;j++){
			pt[j]=pc[j];//乘上离散条件概率 
			for(k=0;k<160;k++){//乘上连续条件概率 
				pt[j]*=exp(-pow(test[i].feature[k]-mean[j][k],2)/(2*var[j][k]))/sqrt(2*pi*var[j][k]);
			}
			switch(test[i].label){//根据测试样例的label选择相应的条件概率 
				case 11:{
					pt[j]*=pxc1[j][0];
					break;
				}
				case 12:{
					pt[j]*=pxc1[j][1];
					break;
				}
				case 21:{
					pt[j]*=pxc1[j][2];
					break;
				}
				case 22:{
					pt[j]*=pxc1[j][3];
					break;
				}
			}
			//pt[j]*=pxc2[j][test[i].emo];//乘上情感标签条件概率 
		}
		max=-1;
		for(j=0;j<C;j++){
			if(pt[j]>max){
				max=pt[j];
				res=j;
			}
		}
		h[i]=res;//得出分类结果 
	}
	printf("分类器运算结果为：\n");//输出分类结果 
	for(i=0;i<N;i++) printf("%d ",h[i]);
	printf("\n");
	
	//计算正确率 
	right=0;
	for(i=0;i<N;i++){
		if(h[i]==test[i].emo) right++;//统计正确分类数 
	}
	correct=(double)right/N;
	printf("分类正确数：%d\n分类错误数：%d\n正确率为：%lf",right,N-right,correct); 
	fclose(fpr);//关闭文件指针
	return 0;
}
