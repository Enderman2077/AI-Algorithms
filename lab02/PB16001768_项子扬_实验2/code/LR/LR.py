import warnings
from sklearn.linear_model import LogisticRegression
from sklearn import preprocessing
from sklearn.model_selection import cross_val_score
warnings.filterwarnings('ignore')
#读取数据
#存储一个文件中的数据
S = []
#DEAP数据集的字典
D ={}
#MAHNOB-HCI数据集的字典
M ={}
#读取DEAP
S = []
with open('data/DEAP/EEG_feature.txt', 'r') as fo:
	for line in fo:
		line = line.strip('\n')#除去行尾的换行符
		line = line.split('\t')#以制表符分割数据
		S.append([float(i) for i in line])
D['EEG_feature'] = S
S = []
with open('data/DEAP/subject_video.txt', 'r') as fo:
	for line in fo:
		line = line.strip('\n')#除去行尾的换行符
		line = line.split('\t')#以制表符分割数据
		S.append([int(i) for i in line])
D['subject_video'] = S
S = []
with open('data/DEAP/valence_arousal_label.txt', 'r') as fo:
	for line in fo:
		line = line.strip('\n')#除去行尾的换行符
		line = line.split('\t')#以制表符分割数据
		S.append([int(i) for i in line])
D['valence_arousal_label'] = S
#读取MAHNOB-HCI
S = []
with open('data/MAHNOB-HCI/EEG_emotion_category.txt', 'r') as fo:
	for line in fo:
		line = line.strip('\n')#除去行尾的换行符
		line = line.split('\t')#以制表符分割数据
		S.append([int(i) for i in line])
M['EEG_emotion_category'] = S
S = []
with open('data/MAHNOB-HCI/EEG_feature.txt', 'r') as fo:
	for line in fo:
		line = line.strip('\n')#除去行尾的换行符
		line = line.split('\t')#以制表符分割数据
		S.append([float(i) for i in line])
M['EEG_feature'] = S
S = []
with open('data/MAHNOB-HCI/subject_video.txt', 'r') as fo:
	for line in fo:
		line = line.strip('\n')#除去行尾的换行符
		line = line.split('\t')#以制表符分割数据
		S.append([int(i) for i in line])
M['subject_video'] = S
S = []
with open('data/MAHNOB-HCI/valence_arousal_label.txt', 'r') as fo:
	for line in fo:
		line = line.strip('\n')#除去行尾的换行符
		line = line.split('\t')#以制表符分割数据
		S.append([int(i) for i in line])
M['valence_arousal_label'] = S

#对DEAP的计算
print('DEAP数据集：')
length = len(D['EEG_feature'])
X = []
for i in range(0, length):
	X.append(D['EEG_feature'][i] + D['subject_video'][i])#将两个文件中的属性合并作为输入存入X表中
scaler = preprocessing.MinMaxScaler()
X = scaler.fit_transform(X)#将X表中的属性归一化
Y = [10 * x[0] + x[1] for x in D['valence_arousal_label']]#将待分类标签存入Y表中
model = LogisticRegression()#调用库函数进行分类
res = cross_val_score(model, X, Y, cv = 5)#调用交叉验证得到正确率
print('交叉验证正确率:{}'.format(res))#输出结果
print('平均正确率:{}'.format(res.mean()))

#对MAHNOB-HCI的计算
print('MAHNOB-HCI数据集：')
length = len(M['EEG_feature'])
X = []
for i in range(0, length):
	X.append(M['EEG_emotion_category'][i] + M['EEG_feature'][i] + M['subject_video'][i])
scaler = preprocessing.MinMaxScaler()
X = scaler.fit_transform(X)#将X表中的属性归一化
Y = [10 * x[0] + x[1] for x in M['valence_arousal_label']]#将待分类标签存入Y表中
model = LogisticRegression()#调用库函数进行分类
res = cross_val_score(model, X, Y, cv = 5)#调用交叉验证得到正确率
print('交叉验证正确率:{}'.format(res))#输出结果
print('平均正确率:{}'.format(res.mean()))

