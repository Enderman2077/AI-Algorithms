#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
typedef struct node{
	int data[5][5];//����״̬ 
	int zerox;//0���ڵ�����
	int zeroy;
	int dir;//�ƶ����� 1��2��3��4�� 
	int g;//·����ɢ 
	int h;//����ʽ���� 
	struct node *parent;//���ڵ� 
	struct node *next;//��һ���ڵ� 
}node,*pnode;

int min(int a,int b,int c,int d,int e){
	int m;
	m=100000;
	if(a<m) m=a;
	if(b<m) m=b;
	if(c<m) m=c;
	if(d<m) m=d;
	if(e<m) m=e;
	return m;
}

int geth(pnode p){//���������� 
	int ret,i,j,a,b;
	ret=0;
	for(i=0;i<5;i++){
		for(j=0;j<5;j++){
			if(p->data[i][j]!=0){
				a=(p->data[i][j]-1)/5;
				b=(p->data[i][j]-1)%5;	
			}
			else{
				a=4;
				b=4;
			}
			ret+=min(abs(a-i)+abs(b-j),abs(i-2)+abs(a-2)+abs(j-0)+abs(b-4),abs(i-2)+abs(a-2)+abs(j-4)+abs(b-0),abs(i-0)+abs(a-4)+abs(j-2)+abs(b-2),abs(i-4)+abs(a-0)+abs(j-2)+abs(b-2));
		}
	}
	return ret;
}

int Isgoal(pnode p){//�����жϽڵ��Ƿ�ΪĿ��״̬ 1��0�� 
	int i,j;
	for(i=0;i<5;i++){
		for(j=0;j<5;j++){
			if(p->data[i][j]!=(i*5+j+1)%25) return 0;
		}
	}
	return 1;
}

int Isequal(pnode p,pnode q){//�����ж������ڵ������ֲ��Ƿ���ͬ 
	int i,j;
	for(i=0;i<5;i++){
		for(j=0;j<5;j++){
			if(p->data[i][j]!=q->data[i][j]) return 0;
		}
	}
	return 1;
}

int Isclose(pnode close,pnode p){//�����жϽڵ��Ƿ���close���� 
	pnode q;
	for(q=close->next;q!=NULL;q=q->next){
		if(Isequal(p,q)) return 1;
	} 
	return 0;
}

pnode Isopen(pnode open,pnode p){//�����жϽڵ��Ƿ���open���� 
	pnode q;
	for(q=open->next;q!=NULL;q=q->next){
		if(Isequal(p,q)) return q;
	}
	return NULL;
}

int move(pnode p,int dir){//�ж��ƶ������Ƿ���� 
	if(p->dir==0){
		switch(dir){
			case 1:{//�����ƶ� 
				if(p->zerox==0&&p->zeroy!=2) return 0;
				break;
			}
			case 2:{
				if(p->zerox==4&&p->zeroy!=2) return 0;
				break;
			}
			case 3:{
				if(p->zeroy==0&&p->zerox!=2) return 0;
				break;
			}
			case 4:{
				if(p->zeroy==4&&p->zerox!=2) return 0;
				break;
			}
		}	
	}
	else{
		switch(dir){
			case 1:{//�����ƶ� 
				if((p->dir==2)||(p->zerox==0&&p->zeroy!=2)) return 0;
				break;
			}
			case 2:{
				if((p->dir==1)||(p->zerox==4&&p->zeroy!=2)) return 0;
				break;
			}
			case 3:{
				if((p->dir==4)||(p->zeroy==0&&p->zerox!=2)) return 0;
				break;
			}
			case 4:{
				if((p->dir==3)||(p->zeroy==4&&p->zerox!=2)) return 0;
				break;
			}
		}
	}
	return 1;
}

void copy(pnode to,pnode from){
	int i,j;
	for(i=0;i<5;i++){
		for(j=0;j<5;j++) to->data[i][j]=from->data[i][j];
	}
}

void print(pnode p){
	int i,j;
	for(i=0;i<5;i++){
		for(j=0;j<5;j++){
			if(p->data[i][j]<10) printf("%d  ",p->data[i][j]);
			else printf("%d ",p->data[i][j]);
		}
		printf("\n");
	}
	printf("g=%d,h=%d,f=%d\n",p->g,p->h,p->g+p->h);
	printf("\n");
}

void RearInsert(pnode open,pnode p){
	pnode q;
	for(q=open;q->next!=NULL;q=q->next);
	q->next=p;
	p->next=NULL;
}

char tran(int i){
	if(i==1) return 'U';
	if(i==2) return 'D';
	if(i==3) return 'L';
	if(i==4) return 'R'; 
}

int main(){
	double run_time;
	_LARGE_INTEGER time_start;	//��ʼʱ�� 
	_LARGE_INTEGER time_over;	//����ʱ�� 
	double dqFreq;				//��ʱ��Ƶ�� 
	LARGE_INTEGER f;			//��ʱ��Ƶ�� 
	QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
	
	FILE *fpr,*fpw;
	int a[5][5]; //�洢���뵱ǰ״̬ 
	int i,j,m,zerox,zeroy,flag,step;
	int temp[200];
	fpr=fopen("input.txt","r");
	for(i=0;i<5;i++){
		for(j=0;j<5;j++){
			fscanf(fpr,"%d",&a[i][j]);
			if(!a[i][j]){
				zerox=i;
				zeroy=j;
			}
		}
	}
	pnode open,close,p,q,min,minp;
	open=(pnode)malloc(sizeof(node));//ͷ���
	open->next=NULL;
	close=(pnode)malloc(sizeof(node));//ͷ���
	close->next=NULL;
	p=(pnode)malloc(sizeof(node));
	for(i=0;i<5;i++){//��ʼ���ڵ� 
		for(j=0;j<5;j++){
			p->data[i][j]=a[i][j];
		}
	}
	p->zerox=zerox;
	p->zeroy=zeroy;
	p->dir=0;
	p->g=0;
	p->h=geth(p);
	p->parent=NULL;
	p->next=NULL;
	open->next=p;
	QueryPerformanceCounter(&time_start);//��ʱ��ʼ 
	while(open->next!=NULL){
		flag=0;
		m=1000000;
		min=open->next;
		for(q=open->next;q!=NULL;q=q->next){
			if(q->g+q->h<m){
				m=q->g+q->h;
			}
		}
		for(min=open->next,minp=open;min!=NULL;min=min->next,minp=minp->next){
			if(min->g+min->h==m) break;
		}
		minp->next=min->next;//��open����ɾ����С�ڵ� 
		min->next=close->next;		//����close��ͷ			
		close->next=min;
		if(Isgoal(min)) break;
		for(i=1;i<=4;i++){//�����п��ܵ��½ڵ����open�� 
			if(move(min,i)){//�����ַ����ж� 
				p=(pnode)malloc(sizeof(node));
				copy(p,min);
				if(i==1){//�����ƶ�����Ϊ�½ڵ�����ݸ�ֵ 
					p->zerox=(min->zerox+4)%5;
					p->zeroy=min->zeroy;
					p->data[p->zerox][p->zeroy]=0;
					p->data[(p->zerox+1)%5][p->zeroy]=min->data[(min->zerox+4)%5][min->zeroy];
				} 
				else if(i==2){
					p->zerox=(min->zerox+1)%5;
					p->zeroy=min->zeroy;
					p->data[p->zerox][p->zeroy]=0;
					p->data[(p->zerox+4)%5][p->zeroy]=min->data[(min->zerox+1)%5][min->zeroy];
				} 
				else if(i==3){
					p->zerox=min->zerox;
					p->zeroy=(min->zeroy+4)%5;
					p->data[p->zerox][p->zeroy]=0;
					p->data[p->zerox][(p->zeroy+1)%5]=min->data[min->zerox][(min->zeroy+4)%5];
				}
				else{
					p->zerox=min->zerox;
					p->zeroy=(min->zeroy+1)%5;
					p->data[p->zerox][p->zeroy]=0;
					p->data[p->zerox][(p->zeroy+4)%5]=min->data[min->zerox][(min->zeroy+1)%5];
				}
				p->dir=i;
				p->g=min->g+1;
				p->h=geth(p);
				p->parent=min;
				if(Isgoal(p)){
					min=p;
					flag=1;
					break;
				}
				if(Isclose(close,p)){//���½ڵ���close���У����� 
					free(p);
					continue;
				}
				if(q=Isopen(open,p)){//��open���� 
					if(p->g+p->h<q->g+q->h){//���½ڵ�Ĵ��۱�open�е�С�����滻���� 
						q->g=p->g;
						q->h=p->h;
						q->parent=p->parent; 
					}
					free(p);
				}
				else{//����open���У����½ڵ����openβ�� 
					RearInsert(open,p); 
				}
			}
		}
		if(flag) break;
	}
	QueryPerformanceCounter(&time_over);//��ʱ���� 
	memset(temp,0,sizeof(temp));
	i=0;
	for(p=min;p!=NULL;p=p->parent){
		temp[i++]=p->dir;
	}
	step=i-1;
	run_time=1000000*(time_over.QuadPart-time_start.QuadPart)/dqFreq; 
	fpw=fopen("Ah2_solution.txt","w");//���ļ���д��ʵ���� 
	fprintf(fpw,"%fus\n",run_time);  
	for(i=0;temp[i]!=0;i++);
	for(j=i-1;j>=0;j--){
		fprintf(fpw,"%c",tran(temp[j]));
	}
	fprintf(fpw,"\n%d",step);
	fclose(fpr);
	fclose(fpw);//�ر��ļ�ָ�� 
	return 0;
} 
