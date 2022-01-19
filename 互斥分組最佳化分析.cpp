#include <iostream>
#include <cstdlib>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

const int ALL=200;					// 設定全部人數
const int N=10; 					// 設定分成幾組
const int K=5; 						// 設定每組幾人
const int Methods=13;				// 設定挑選方式
const int mGMethods=8;				// 設定分群挑選方式 			
double alpha=0.5;					// 設定權重指標
double mGAlpha=0.5;					// 設定戶斥群組能力比重  1完全個人能力 0完全合作能力 
double MWEIGHT=80;					// 設定分群門檻值 20-80
double PWEIGHT=80;					// 設定個人挑選門檻值 20-80
const string selectionName[Methods] ={
"隨機","個人","合作",
"互斥隨機","互斥個人(人數優先)","互斥合作(人數優先)","互斥綜合(人數優先)",
"互斥個人(能力優先)","互斥合作(能力優先)","互斥綜合(能力優先)",
"互斥人數(循環挑選)","互斥能力(循環挑選)","互斥(合作好個人好)"
};
// 設定挑選方式的名稱  {"隨機挑選成員","個人能力最高","合作能力加總最大值","","",""};

int personalIndicator[ALL];			// 設定個人能力值：數值範圍 10 ~ 90 之間 
int mutualIndicator[ALL][ALL]; 		// 設定合作能力值：數值範圍 10 ~ 90 之間 
int selection[Methods][ALL]; 		// 設定被選中的人：0是未選中、1是已選中
int mGSelection[mGMethods][ALL];	// 設定被選中的組: 0是未分組、1被挑選 為mutualGCount
 
int mGroup[ALL];					//mGroup[ALL]互斥群組-1代表未被挑選 0表示第一組 1表示第二組
int mGCount=-1;						//互斥分群的群組數 -1為執行分群程式
const int MGDATATYPE=5; 
double mGData[ALL][MGDATATYPE];				//設定群組 組別0 個人人能力平均1 合作能力平均2 加權平均3  人數4 基本上ALL會放 mGCount
const int PDATATYPE=3; 
int personal[ALL][PDATATYPE];				//設定 ALL 成員的編號、個人能力值和互斥分群的組別

int numberID[N*K];					// numberID[N*K]		被挑選的人的編號 
double abilityValue[N*K][3];		// abilityValue[N*K][0] 被挑選的人的個人能力值 
									// abilityValue[N*K][1] 被挑選的人的合作能力值
									// abilityValue[N*K][2] 被挑選的人的整體能力值 

int group[N][K];					// 設定將被挑選的人分成小組：存放編號，編號代表個人 

void initialization();				// 隨機亂數賦予能力值

void randomSelection();				// 挑選方式 1：挑選隨機 N*K 人 
void personalMaxFirst();			// 挑選方式 2：挑選個人能力值最高前 N*K 人
void mutualMaxFirst();				// 挑選方式 3：挑選合作能力加總最大值前 N*K 人

void mgRandomSelect();				// 挑選方式 4：隨機挑組 N*K人
void mgMemberMaxPerson(); 			// 挑選方式 5: 優先從人數最多開始挑 人數一樣則優先挑小組內個人平均最大
void mgMemberMaxmutual();			// 挑選方式 6: 優先從人數最多開始挑 人數一樣則優先挑小組內合作平均最大
void mgMemberMaxComplex();			// 挑選方式 7: 優先從人數最多開始挑 人數一樣則優先挑小組內綜合能力最大
void mgMaxPerson();					// 挑選方式 8: 直接挑小組內個人平均最大
void mgMaxMutual();					// 挑選方式 9: 直接挑小組內合作平均最大
void mgMaxComplex();				// 挑選方式 10:直接挑小組內綜合平均最大
void mgCyclePerCount();				// 挑選方式 11:依群組人數大至小循環挑選 每次都挑個人能力最好的
void mgCycleAbility();				// 挑選方式 12:依群組能力大至小循環挑選 每次都挑個人能力最好的 
void mgPGood(); 					// 挑選方式 13:從群裡挑個人能力具有一定能力的人 

double minPerformance(int);			// 計算各小組最低表現的平均能力值

void resetGroup();					// 初始化小組成員
void grouping(int);					// 開始進行分組 
void showGroup(int);				// 顯示分組結果

void showAbilityValue();			// 顯示排序結果

void smallToLarge(int);				// 排列方式 1：小到大排列 
void largeToSmall(int);				// 排列方式 2：大到小排列 

void randomNumber(int);				// 分組排序 1：亂數分組
void ballot();						// 分組排序 2：抽籤分組
void SType();						// 分組排序 3：S型排序 
void MGPG();						// 分組排序 4：個人好合作好 
void MGPG_2();						// 分組排序 4：個人好合作好(改版)
void MGPG_3();						// 分組排序 4：個人好合作好(改版)
void MGMG();						// 分組排序 5：合作好合作好

double calculateP(int);				// 計算小組成員的個人平均能力值
double calculateM(int);				// 計算小組成員之間的合作平均能力值 
double newCalculateResult(double);	//計算互斥成員能力加權的方法

void showData();					// 顯示各項資料

void resetMGroup();					//重設互斥群組 		
int getPersonalIndicator(int);		//取得個人的合作能力
int getMutualIndicator(int,int);	//取得兩個人之間的合作能力 
int selectM(int);					//選誰當頭的選擇器
void showMgData();					//顯示互斥群組資料

void initPersonalData();					// 取得 ALL 成員的編號、個人能力值
int getPersonalID(int number);				//取得Personal ID
int getPersonalAbility(int number);			//取得Personal個人能力值 
int getPersonalMGroup(int number);			//取得Personal互斥分群號碼
void personalSmallToLargeSort(int dataN);	// 將成員依dataN 編號:0 個人能力:1 所屬組別:2小到大排列 
void personalLargeToSmallSort(int dataN);	// 將成員依dataN 編號:0 個人能力:1 所屬組別:2大到小排列 
void showDataPersonal();

void mGrouping();					//互斥分群
void mGSelectMaxOne();				//一次挑一人
void mGSIDontKnow();
 
void mGSelectMaxAllIn();			//一次挑整群 
int getLeaderNotInMGroup();			//從未被挑選的人挑選個人能力min的人的編號
int personalMinOne();				//從未被挑選的人挑選個人能力min的人的編號

void initMGroupData();

double getMGNumber(int mgNumber);			//組別號碼	
double getCalculateMGP(int mgNumber);		//計算互斥群組內個人平均 
double getCalculateMGM(int mgNumber);		//計算互斥群組內合作平均 
double getCalculateMGAlpha(int mgNumber);	//每個互斥群組加權能力平均
double getCalculateMGMemCount(int mgNumber);//每個互斥群組所包含的人數

void mutualGLargeToSmall(int dataN);
void showMGData();
void showNKData(int Methods);							//挑出N*K的資料 
void createFileCSV(char* fileName,int maxTime);		// 匯出成csv檔案 
void logI(string a,int b);
void logD(string a,double b);



int main()
{
	alpha = 0.5;
					
	MWEIGHT = 20;	
	createFileCSV("200人-門檻20.csv",100);
	
	MWEIGHT = 30;	
	createFileCSV("200人-門檻30.csv",100);
	
	MWEIGHT = 40;	
	createFileCSV("200人-門檻40.csv",100);
	
	MWEIGHT = 50;	
	createFileCSV("200人-門檻50.csv",100);
	
	MWEIGHT = 60;	
	createFileCSV("200人-門檻60.csv",100);
	
	MWEIGHT = 70;	
	createFileCSV("200人-門檻70.csv",100);
	
	MWEIGHT = 80;	
	createFileCSV("200人-門檻80.csv",100);
	



//	srand(3);
//	initialization();
//	randomSelection();
//	personalMaxFirst();			
//	mutualMaxFirst();			
//	mGrouping();
	

//	cout<<"getPersonalID(1):"<<getPersonalID(2)<<endl;
//	cout<<"getPersonalAbility(1):"<<getPersonalAbility(2)<<endl;
//	cout<<"getPersonalMGroup(1):"<<getPersonalMGroup(2)<<endl;
							
//	mgRandomSelect();
//	mgMemberMaxPerson();
//	mgMemberMaxmutual();
//	mgMemberMaxComplex();
//	mgMaxPerson();
//	mgMaxMutual();
//	mgMaxComplex();
//	mgCyclePerCount();			
//	mgCycleAbility();
//	mgPGood(); 
//	
//	for(int i=0;i<Methods;i++)		// 開始分組 Methods
//	{
//		if(i==4||i==5||i==7||i==8)
//			continue;		
//		//printf("%s：整體平均值 %2.3f　　",selectionName[i].c_str(),minPerformance(i));
//		minPerformance(i);
//		cout<<endl;
//	}

//	minPerformance(12);


	cout<<endl;
	
//	MWEIGHT = 20; 
//
//	for(int i=0;i<7;i++)
//	{
//		cout<<"MWEIGHT:"<<MWEIGHT<<endl;
//		srand(1);
//		initialization();
//		randomSelection();
//		personalMaxFirst();			
//		mutualMaxFirst();			
//		mGrouping();						
//		mgRandomSelect();
//		mgMemberMaxPerson();
//		mgMemberMaxmutual();
//		mgMemberMaxComplex();
//		mgMaxPerson();
//		mgMaxMutual();
//		mgMaxComplex();
//		mgCyclePerCount();			
//		mgCycleAbility();
//		mgPGood(); 
//
//		for(int j=0;j<Methods;j++)		// 開始分組 Methods
//		{
//			printf("%s：整體平均值 %2.3f　　",selectionName[j].c_str(),minPerformance(j));
//			cout<<endl;
//		}
//		cout<<endl;
//		MWEIGHT += 10;
//	}
	
	
//	for(int j=1;j<6;j++)
//	{
//		srand(j);
//		initialization();
//		randomSelection();
//		personalMaxFirst();			
//		mutualMaxFirst();
//		mGrouping();						
//		mgRandomSelect();
//		mgMemberMaxPerson();
//		mgMemberMaxmutual();
//		mgMemberMaxComplex();
//		mgMaxPerson();
//		mgMaxMutual();
//		mgMaxComplex();
//		mgCyclePerCount();			
//		mgCycleAbility();
//		mgPGood(); 				
//		cout<<"第"<<j<<"次"<<endl; 
//		for(int i=0;i<Methods;i++)		// 開始分組 Methods
//		{
//			if(i==1||i==2||i==4||i==5||i==7||i==8)
//				continue;
//			
//			printf("%s：整體平均值 %2.3f　　\n",selectionName[i].c_str(),minPerformance(i));
//			
//			cout<<endl;
//		}
//		cout<<endl<<endl;
//	}

//	showData();						// 顯示各項資料
	
//	showMgData();

	return 0;
}

void createFileCSV(char* fileName,int maxTime)		// 匯出成csv檔案 
{
	ofstream createFile;
	createFile.open(fileName, ios::out | ios::trunc);
	createFile <<"隨機" << "," << "互斥隨機" << "," << "互斥綜合(人數優先)" << ","<< "互斥綜合(能力優先)" << "," << "互斥人數(循環挑選)" << "," << "互斥能力(循環挑選)" << "," << "合作好個人好"<<endl;
	int start=1500;
	int index=1;
	for(int i=start;i<=(maxTime+start-1);i++)
	{
		srand(i);
		initialization();
		
		mGrouping();
		randomSelection(); 
		personalMaxFirst();
		mutualMaxFirst();						
		mgRandomSelect();
		mgMemberMaxPerson();
		mgMemberMaxmutual();
		mgMemberMaxComplex();
		mgMaxPerson();
		mgMaxMutual();
		mgMaxComplex();
		mgCyclePerCount();			
		mgCycleAbility();
		mgPGood(); 
		
		printf("%6d  ",index);
		index++;
		for(int j=0;j<Methods;j++)
		{
			if(j==1||j==2||j==4||j==5||j==7||j==8)
				continue;
			createFile << minPerformance(j) << ",";
			printf("%2.3f  ",minPerformance(j));
		}
	
		createFile << endl;
		cout << endl;
	}
	
	createFile.close();
}

void initialization()				// 隨機亂數賦予能力值
{	
	for(int i=0;i<ALL;i++)			// 賦予個人能力值
	{
		personalIndicator[i]=rand()%81+10;
		for(int k=0;k<Methods;k++)	//重置 selection
			selection[k][i]=0;
	}
	
	for(int x=0;x<ALL;x++)			// 賦予合作能力值
	{
		for(int y=0;y<ALL;y++)
		{
			if(x==y)
				mutualIndicator[x][y]=0;
			else if(x<y)
			{
				mutualIndicator[x][y]=rand()%81+10;
				mutualIndicator[y][x]=mutualIndicator[x][y];
			}
		}
	}
}

void resetMGroup()
{
	mGCount=-1;
	
	for(int i=0;i<ALL;i++)
		mGroup[i]=-1;
		
	for(int i=0;i<mGMethods;i++)
	{
		for(int j=0;j<ALL;j++)
			mGSelection[i][j]=0;
	}
	
	for(int i=0;i<ALL;i++)
	{
		for(int j=0;j<5;j++)
			mGData[i][j]=-1;
		for(int k=0;k<3;k++)
			personal[i][k]=-1;
	}			
	//personal[ALL][3];			
}

void mGrouping()
{		
	resetMGroup();
	//mGSIDontKnow();
	mGSelectMaxOne();
	initPersonalData();
	personalSmallToLargeSort(2);
	initMGroupData();
//	mutualGLargeToSmall(3);
//	showDataPersonal();
//	showMGData(); 
//	showMgData();
} 

void mGSelectMaxOne()
{
	int groupNumber=0;
	bool isFinish=false;	
	int exceedWeight[ALL];
	int insertID=-1;
	int fristID=selectM(0);
	mGroup[fristID]=groupNumber;
	vector<int> exceedWeightGroup;			//exceedWeight[ALL]暫時儲存每次大於門檻值的人
	vector<int> tempMutuallyExclusiveGroup;	//tempmutuallyExclusiveGroup[ALL]暫時儲存目前在分組的人

	do
	{				
		exceedWeightGroup.clear();
		tempMutuallyExclusiveGroup.clear();
		//先假設所有人都設為被挑選將 exceedWeight設為1 
		for(int i=0;i<ALL;i++)
			exceedWeight[i]=1;
		
		for(int i=0;i<ALL;i++)
		{
			if(mGroup[i]==groupNumber) //將目前在分的組員加入 
				tempMutuallyExclusiveGroup.push_back(i);
	
			if(mGroup[i]==groupNumber)
			{				
				for(int j=0;j<ALL;j++)
				{		
					if(mutualIndicator[i][j]<MWEIGHT)//只要小於權重值都設為零 
					{	
						exceedWeight[j]=0;
					}							
				}
			}									
		}
	
		for(int i=0;i<ALL;i++)
		{		
			if(exceedWeight[i]==1&&mGroup[i]==-1)
				exceedWeightGroup.push_back(i);
		}
		
		if(exceedWeightGroup.size()!=0||!exceedWeightGroup.empty())
		{
			int max=0;
			int sum=0;
			
			for(int i=0;i<exceedWeightGroup.size();i++)//符合門檻的人 
			{				
				for(int j=0;j<tempMutuallyExclusiveGroup.size();j++)//目前在群組的人 
				{						
					sum=sum+getMutualIndicator(exceedWeightGroup[i],tempMutuallyExclusiveGroup[j]);						
				}
				if(max<sum)
				{
					max=sum;
					insertID=exceedWeightGroup[i];
				}
				sum=0;
			}
			mGroup[insertID]=groupNumber;

		}
		else
		{		
			groupNumber++;
			fristID=selectM(0);//從未配挑選挑選人的方式 
			mGroup[fristID]=groupNumber;
		}
			
		int s=0;
		for(int i=0;i<ALL;i++)
		{			
			if(mGroup[i]==-1)
			{
				s++;
				break;
			}						
		}
		if(s==0)
		{
			isFinish=true;
		}				
	}while(isFinish==false);
		
	mGCount=groupNumber+1;//設定互斥分群的總群組數 	
}

void mGSIDontKnow()
{
	//mGroup[ALL]
	int mGNumber=0;
	int leaderID;
	bool notFinish=true;

	while(notFinish)
	{		
		leaderID=getLeaderNotInMGroup();
		mGroup[leaderID]=mGNumber;
		//cout<<"組長ID:"<< leaderID;
		int maxMAbility=0;
		int maxID=-1;
		for(int i=0;i<ALL;i++)	//未被挑選的人
		{			
			if(mGroup[i]==-1)
			{
				int totalMAbility=0;
				int find=1;
				for(int m=0;m<ALL;m++)//找目前在 mGNumber裡的人 
				{
					if(mGroup[m]==mGNumber)
					{
						if(getMutualIndicator(i,m)<MWEIGHT)
							find=-1;
						else
						{
							totalMAbility=totalMAbility+getMutualIndicator(i,m);
						}
					}
				}
				if(maxMAbility<totalMAbility&&find==1)
				{
					maxMAbility=totalMAbility;
					maxID=i;
				}
			}			
		}
		if(maxID!=-1)
		{
			mGroup[maxID]=mGNumber;
		}
		else
		{
			mGNumber++;
		}
					
		int s=0;
		for(int i=0;i<ALL;i++)
		{
			if(mGroup[i]==-1)
			{
				s++;
				break;
			}				
		}
		if(s==0)
			notFinish=false;
	}
//	cout<<"mGNumber:"<<mGNumber<<endl;
//	cout<<"mGroup:";for(int i=0;i<ALL;i++)cout<<mGroup[i]<<" ";
	
	mGCount=mGNumber; 
	
}

void showMgData()
{
	int groupN=0;	
	for(int i=0;i<mGCount;i++)
	{
		cout<<"第"<< groupN+1<<"組"; 
		for(int j=0;j<ALL;j++)
		{			
			if(mGroup[j]==groupN)
			{
				cout<<j<<" ";
			}			
		}cout<<endl;
		groupN++; 
	}			
}

int selectM(int Menthod)//選誰當頭的選擇器 
{
	int index;
	switch(Menthod)
	{
		case 0:index=getLeaderNotInMGroup();break; //從未被的人挑選個人能力MAX的人的編號 
		case 1:index=personalMinOne();break; //從未被的人挑選個人能力min的人的編號
		default:cout<<"方法選擇錯誤"<<endl;break; 	
	}	
	return index;
}

int getLeaderNotInMGroup()
{	
	int max=0;
	int index=0;
	for(int i=0;i<ALL;i++)
	{
		if(mGroup[i]==-1)
		{
			if(max<getPersonalIndicator(i))
			{
				max=getPersonalIndicator(i);
				index=i;
			}
		}
	}	
	return index;//回傳未被分組個人能力最大人員編號 
}

int personalMinOne()//從未被的人挑選個人能力min的人的編號
{
	int min=100;
	int index=0;
	for(int i=0;i<ALL;i++)
	{
		if(mGroup[i]==-1)
		{
			if(min>getPersonalIndicator(i))
			{
				min=getPersonalIndicator(i);
				index=i;
			}
		}
	}	
	return index;//回傳未被分組個人能力最小人員編號 
}
			
void initPersonalData()
{
	
	for(int i=0;i<ALL;i++)
	{
		personal[i][0]=i;						// 成員的編號 
		personal[i][1]=personalIndicator[i];	// 成員的個人能力值		 
	}
	int groupN=0;	
	for(int i=0;i<mGCount;i++)
	{ 
		for(int j=0;j<ALL;j++)
		{			
			if(mGroup[j]==groupN)
			{
				personal[j][2]=groupN;
			}			
		}
		groupN++; 
	}
	
}

void initMGroupData()
{
	//------------------------------組別 
	int groupN=0;
	for(int i=0;i<mGCount;i++)
	{
		mGData[i][0]=(double)groupN;
		groupN++;
	}
	//------------------------------組別
	//------------------------------個人能力平均 
	groupN=0;	
	for(int i=0;i<mGCount;i++)
	{
	 	double count=0;
	 	double total=0;
	 	double avg=0; 
		for(int j=0;j<ALL;j++)
		{
			if(personal[j][2]==groupN)
			{
				total=total+getPersonalIndicator(personal[j][0]);
				count++;
			}				
		}
		avg=total/count;
		mGData[groupN][1]=avg;
		groupN++;	
	}
//	for(int i=0;i<mGCount;i++)
//	{
//		cout<<"第"<<mGData[i][0]<<"組個人平均"<<mGData[i][1]<<endl;
//	}
	//------------------------------個人能力平均
	//------------------------------合作能力平均
	groupN=0;	
	for(int i=0;i<mGCount;i++)
	{
		int count=0;
		double total=0;
		double avg=0;
		for(int j=0;j<ALL;j++)
		{
			for(int k=0;k<ALL;k++)
			{
				if(j<k)
				{
					if(personal[j][2]==groupN&&personal[k][2]==groupN)
					{
						total=total+getMutualIndicator(personal[j][0],personal[k][0]);
						count++;
					}
				}
			}
		}
		if(count!=0)
			avg=total/count;
		else
			avg=0;
		
		mGData[groupN][2]=avg;
		groupN++;
	}
//	for(int i=0;i<mGCount;i++)
//	{
//		cout<<"第"<<mGData[i][0]<<"組合作平均"<<mGData[i][2]<<endl;
//	}
	//------------------------------合作能力平均
	//------------------------------加權能力平均
	groupN=0;
	for(int i=0;i<mGCount;i++)
	{		
		mGData[i][3]=mGAlpha*mGData[i][1]+(1-mGAlpha)*mGData[i][2];	// 計算互斥小組加權平均能力值
		groupN++; 
	}
//	for(int i=0;i<mGCount;i++)
//	{
//		cout<<"第"<<mGData[i][0]<<"組加權平均"<<mGData[i][3]<<endl;
//	} 
	//------------------------------加權能力平均
	//------------------------------互斥小組人數
	groupN=0;	
	for(int i=0;i<mGCount;i++)
	{
		double count=0;
		for(int j=0;j<ALL;j++)
		{
			if(personal[j][2]==groupN)
				count++;
		}
		mGData[groupN][4]=count;
		groupN++;
	}
//	for(int i=0;i<mGCount;i++)
//	{
//		cout<<"第"<<mGData[i][0]<<"組人數"<<mGData[i][4]<<endl;
//	}
	//------------------------------互斥小組人數	 
		
}

void personalSmallToLargeSort(int dataN)		// 將成員依dataN 編號:0 個人能力:1 所屬組別:2小到大排列 
{
	for(int i=0;i<ALL;i++)
	{
    	for(int j=1;j<ALL;j++)
		{
       		if(personal[j][dataN]<personal[j-1][dataN])
            {
            	int tmpNum1=personal[j][0];
            	personal[j][0]=personal[j-1][0];
            	personal[j-1][0]=tmpNum1;
            	
            	int tmpNum2=personal[j][1];
            	personal[j][1]=personal[j-1][1];
            	personal[j-1][1]=tmpNum2;
            	
				int tmpNum3=personal[j][2];
            	personal[j][2]=personal[j-1][2];
            	personal[j-1][2]=tmpNum3;
          	}
	   	}
	}
}

void personalLargeToSmallSort(int dataN)		// 將成員依dataN 編號:0 個人能力:1 所屬組別:2大到小排列 
{
	for(int i=0;i<ALL;i++)
	{
    	for(int j=1;j<ALL;j++)
		{
       		if(personal[j][dataN]>personal[j-1][dataN])
            {
            	int tmpNum1=personal[j][0];
            	personal[j][0]=personal[j-1][0];
            	personal[j-1][0]=tmpNum1;
            	
            	int tmpNum2=personal[j][1];
            	personal[j][1]=personal[j-1][1];
            	personal[j-1][1]=tmpNum2;
            	
				int tmpNum3=personal[j][2];
            	personal[j][2]=personal[j-1][2];
            	personal[j-1][2]=tmpNum3;
          	}
	   	}
	}
}		

void showDataPersonal()
{
	cout<<endl <<"結果"<<endl; 
	for(int i=0;i<ALL;i++)
		printf("編號：%3d  個人：%2d 組別：%2d\n",personal[i][0],personal[i][1],personal[i][2]);cout<<endl;
}

void mutualGLargeToSmall(int dataN)//dataN 0第幾組  1 群組個人人能力平均 2 群組合作能力平均 3群組加權平均  4人數 依照某一DATA大到小排序; 
{	
	for(int i=0;i<mGCount;i++)
	{
    	for(int j=1;j<mGCount;j++)
		{
       		if(mGData[j][dataN]>mGData[j-1][dataN])
            {
            	double tmpNum1=mGData[j][0];
            	mGData[j][0]=mGData[j-1][0];
            	mGData[j-1][0]=tmpNum1;
            	
            	double tmpNum2=mGData[j][1];
            	mGData[j][1]=mGData[j-1][1];
            	mGData[j-1][1]=tmpNum2;
            	
				double tmpNum3=mGData[j][2];
            	mGData[j][2]=mGData[j-1][2];
            	mGData[j-1][2]=tmpNum3;
            	
            	double tmpNum4=mGData[j][3];
            	mGData[j][3]=mGData[j-1][3];
            	mGData[j-1][3]=tmpNum4;
            	
            	double tmpNum5=mGData[j][4];
            	mGData[j][4]=mGData[j-1][4];
            	mGData[j-1][4]=tmpNum5;
          	}
	   	}
	}	
}

void showMGData()
{
//for(int i=0;i<mGCount;i++)	cout<<mGData[i][0]<<endl;
	cout<<endl <<"結果"<<endl; 
	for(int i=0;i<mGCount;i++)
		printf("組別：%.0f  組內個人平均：%.2f 組內合作能力平均：%.2f 組內加權平均:%.2f 組內人數:%.0f\n",mGData[i][0],mGData[i][1],mGData[i][2],mGData[i][3],mGData[i][4]);cout<<endl;
}	

void randomSelection()				// 挑選方式 1：挑選隨機 N*K 人 
{
	for(int i=0;i<N*K;i++)
	{
		int select;	
		do{
			select=rand()%ALL;
		}while(selection[0][select]==1);
		selection[0][select]=1;
	}
}

void personalMaxFirst()				// 挑選方式 2：挑選個人能力值最高前 N*K 人
{
	int tempPersonal[ALL];
	int tempIndex[ALL];
	
	for(int i=0;i<ALL;i++)
	{
		selection[1][i]=0;
		tempPersonal[i]=personalIndicator[i];
		tempIndex[i]=i;	
	}
	
	for(int x=0;x<ALL;x++)
	{
		for(int y=0;y<ALL-1;y++)	
		{
			if(tempPersonal[y]<tempPersonal[y+1])
			{
				int temp=tempPersonal[y];
				tempPersonal[y]=tempPersonal[y+1];
				tempPersonal[y+1]=temp;
				
				int tIndex=tempIndex[y];
				tempIndex[y]=tempIndex[y+1];
				tempIndex[y+1]=tIndex; 
			}
		}
	}	
	
	for(int i=0;i<N*K;i++)
		selection[1][tempIndex[i]]=1;
}

void mutualMaxFirst()				// 挑選方式 3：挑選合作能力加總最大值前 N*K 人 
{
	int tempMutual[ALL];
	int tempIndex[ALL];
	
	for(int i=0;i<ALL;i++)
	{
		selection[2][i]=0;
		tempMutual[i]=0;
		for(int j=0;j<ALL;j++)
			tempMutual[i]=tempMutual[i]+mutualIndicator[i][j];
		tempIndex[i]=i;	
	}
	
	for(int x=0;x<ALL;x++)
	{
		for(int y=0;y<ALL-1;y++)	
		{
			if(tempMutual[y]<tempMutual[y+1])
			{
				int temp=tempMutual[y];
				tempMutual[y]=tempMutual[y+1];
				tempMutual[y+1]=temp;
				
				int tIndex=tempIndex[y];
				tempIndex[y]=tempIndex[y+1];
				tempIndex[y+1]=tIndex; 
			}
		}
	}	
	
	for(int i=0;i<N*K;i++)
		selection[2][tempIndex[i]]=1;
}

void mgRandomSelect()				//挑選方式 4 : 隨機挑組 組內人員 N*K 人
{
	int maxPoint=N*K;
	int lessPoint=N*K;				//空位 
	int addPoint=0;					//每次加入的人數 
	
	while(lessPoint>0)		//剩下的位置
	{
		int mgSelect;
		int mgNumber;
		do
		{
			mgSelect=rand()%mGCount;
			mgNumber=getMGNumber(mgSelect);
		}while(mGSelection[0][mgNumber]==1);
		mGSelection[0][mgNumber]=1;
		
		addPoint=getCalculateMGMemCount(mgNumber);
		
		if(addPoint<=lessPoint)
		{				
			for(int i=0;i<ALL;i++)	//找屬於 mgSelect組的人加入 
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					selection[3][getPersonalID(i)]=1;
				}
			}
			lessPoint=lessPoint-addPoint; 
		}
		else//lessPoint<addPoint 從被挑到的組挑 個人能力最好的 lessPoint個人 
		{
			int tempPersonal[addPoint][3];
			
			int addP=0;
			for(int i=0;i<ALL;i++)
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					tempPersonal[addP][0]=getPersonalID(i);
					tempPersonal[addP][1]=getPersonalAbility(i);
					tempPersonal[addP][2]=getPersonalMGroup(i);
					addP++;
				}
				
			}
			for(int i=0;i<addPoint;i++)
			{
				for(int j=1;j<addPoint;j++)
				{
					if(tempPersonal[j-1][1]<tempPersonal[j][1])
					{
						int temp1=tempPersonal[j-1][0];
						tempPersonal[j-1][0]=tempPersonal[j][0];
						tempPersonal[j][0]=temp1;
						
						int temp2=tempPersonal[j-1][1];
						tempPersonal[j-1][1]=tempPersonal[j][1];
						tempPersonal[j][1]=temp2;
						
						int temp3=tempPersonal[j-1][2];
						tempPersonal[j-1][2]=tempPersonal[j][2];
						tempPersonal[j][2]=temp3;
						
					}
				}
			}
			
//			for(int i=0;i<addPoint;i++)
//			{
//				cout<<"PID:"<<tempPersonal[i][0]<<", PAbility:"<<tempPersonal[i][1]<<", PGroupNum:"<<tempPersonal[i][2]<<endl;
//			}
			for(int i=0;i<lessPoint;i++)
			{
				selection[3][tempPersonal[i][0]]=1;
			}
			lessPoint=0;			
		}		
	} 
} 

void mgMemberMaxPerson()			//挑選方式 5: 優先從人數最多開始挑 人數一樣則優先挑小組內個人平均最大 
{									//mgData[mGCount][]設定群組 組別0 個人人能力平均1 合作能力平均2 加權平均3  人數4 基本上ALL會放 mGCount
	int maxPoint=N*K;
	int lessPoint=N*K;				//空位 
	int addPoint=0;					//每次加入的人數 

	while(lessPoint>0)
	{
		int maxCount=-1;			//暫存最大人數
		for(int i=0;i<mGCount;i++)
		{
			if(getCalculateMGMemCount(i)>maxCount&&mGSelection[1][i]==0)
			{
				maxCount=getCalculateMGMemCount(i);
			}			
		}
		int maxPersonalAbility=0;	//組內個人平均 
		int mgNumber=-1;			//小組編號 
		for(int i=0;i<mGCount;i++)
		{
			if(getCalculateMGMemCount(i)==maxCount)
			{
				if(getCalculateMGP(i)>maxPersonalAbility&&mGSelection[1][i]==0)
				{
					maxPersonalAbility=getCalculateMGP(i);
					mgNumber=getMGNumber(i);					
				}
			}
		}
		mGSelection[1][mgNumber]=1;	//將被挑到的組設為1;
		addPoint=getCalculateMGMemCount(mgNumber);
		if(addPoint<=lessPoint)
		{				
			for(int i=0;i<ALL;i++)	//找屬於 mgSelect組的人加入 
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					selection[4][getPersonalID(i)]=1;
				}
			}
			lessPoint=lessPoint-addPoint; 
		}
		else//lessPoint<addPoint 從被挑到的組挑 個人能力最好的 lessPoint個人 
		{
			int tempPersonal[addPoint][3];
			
			int addP=0;
			for(int i=0;i<ALL;i++)
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					tempPersonal[addP][0]=getPersonalID(i);
					tempPersonal[addP][1]=getPersonalAbility(i);
					tempPersonal[addP][2]=getPersonalMGroup(i);
					addP++;
				}
				
			}
			for(int i=0;i<addPoint;i++)
			{
				for(int j=1;j<addPoint;j++)
				{
					if(tempPersonal[j-1][1]<tempPersonal[j][1])
					{
						int temp1=tempPersonal[j-1][0];
						tempPersonal[j-1][0]=tempPersonal[j][0];
						tempPersonal[j][0]=temp1;
						
						int temp2=tempPersonal[j-1][1];
						tempPersonal[j-1][1]=tempPersonal[j][1];
						tempPersonal[j][1]=temp2;
						
						int temp3=tempPersonal[j-1][2];
						tempPersonal[j-1][2]=tempPersonal[j][2];
						tempPersonal[j][2]=temp3;
						
					}
				}
			}
			
//			for(int i=0;i<addPoint;i++)
//			{
//				cout<<"PID:"<<tempPersonal[i][0]<<", PAbility:"<<tempPersonal[i][1]<<", PGroupNum:"<<tempPersonal[i][2]<<endl;
//			}
			for(int i=0;i<lessPoint;i++)
			{
				selection[4][tempPersonal[i][0]]=1;
			}
			lessPoint=0;			
		}
	}								
}

void mgMemberMaxmutual()			// 挑選方式 6: 優先從人數最多開始挑N*K 人數一樣則優先挑小組內合作平均最大
{
	int maxPoint=N*K;
	int lessPoint=N*K;				//空位 
	int addPoint=0;					//每次加入的人數 

	while(lessPoint>0)
	{
		int maxCount=-1;			//暫存最大人數
		for(int i=0;i<mGCount;i++)
		{
			//cout<<"mGSelection[2][i]"<<mGSelection[2][i]<<endl;
			if(getCalculateMGMemCount(i)>maxCount&&mGSelection[2][i]==0)
			{
				maxCount=getCalculateMGMemCount(i);
			}			
		}
		int maxMutualAbility=0;	//組內合作平均 
		int mgNumber=-1;			//小組編號 
		for(int i=0;i<mGCount;i++)
		{
			if(getCalculateMGMemCount(i)==maxCount)
			{
				if(getCalculateMGM(i)>maxMutualAbility&&mGSelection[2][i]==0)
				{
					maxMutualAbility=getCalculateMGM(i);
					mgNumber=getMGNumber(i);					
				}
			}
		}
		mGSelection[2][mgNumber]=1;	//將被挑到的組設為1;
		addPoint=getCalculateMGMemCount(mgNumber);
		if(addPoint<=lessPoint)
		{				
			for(int i=0;i<ALL;i++)	//找屬於 mgSelect組的人加入 
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					selection[5][getPersonalID(i)]=1;
				}
			}
			lessPoint=lessPoint-addPoint; 
		}
		else//lessPoint<addPoint 從被挑到的組挑 個人能力最好的 lessPoint個人 
		{
			int tempPersonal[addPoint][3];
			
			int addP=0;
			for(int i=0;i<ALL;i++)
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					tempPersonal[addP][0]=getPersonalID(i);
					tempPersonal[addP][1]=getPersonalAbility(i);
					tempPersonal[addP][2]=getPersonalMGroup(i);
					addP++;
				}
				
			}
			for(int i=0;i<addPoint;i++)
			{
				for(int j=1;j<addPoint;j++)
				{
					if(tempPersonal[j-1][1]<tempPersonal[j][1])
					{
						int temp1=tempPersonal[j-1][0];
						tempPersonal[j-1][0]=tempPersonal[j][0];
						tempPersonal[j][0]=temp1;
						
						int temp2=tempPersonal[j-1][1];
						tempPersonal[j-1][1]=tempPersonal[j][1];
						tempPersonal[j][1]=temp2;
						
						int temp3=tempPersonal[j-1][2];
						tempPersonal[j-1][2]=tempPersonal[j][2];
						tempPersonal[j][2]=temp3;
						
					}
				}
			}
			
//			for(int i=0;i<addPoint;i++)
//			{
//				cout<<"PID:"<<tempPersonal[i][0]<<", PAbility:"<<tempPersonal[i][1]<<", PGroupNum:"<<tempPersonal[i][2]<<endl;
//			}
			for(int i=0;i<lessPoint;i++)
			{
				selection[5][tempPersonal[i][0]]=1;
			}
			lessPoint=0;			
		}
	}
}	
		
void mgMemberMaxComplex()			// 挑選方式 7: 優先從人數最多開始挑N*K 人數一樣則優先挑小組內綜合能力最大
{
	int maxPoint=N*K;
	int lessPoint=N*K;				//空位 
	int addPoint=0;					//每次加入的人數 

	while(lessPoint>0)
	{
		int maxCount=-1;			//暫存最大人數
		for(int i=0;i<mGCount;i++)
		{
			if(getCalculateMGMemCount(i)>maxCount&&mGSelection[3][i]==0)
			{
				maxCount=getCalculateMGMemCount(i);
			}			
		}
		int maxComplexAbility=0;	//組內個人平均 
		int mgNumber=-1;			//小組編號 
		for(int i=0;i<mGCount;i++)
		{
			if(getCalculateMGMemCount(i)==maxCount)
			{
				if(getCalculateMGAlpha(i)>maxComplexAbility&&mGSelection[3][i]==0)
				{
					maxComplexAbility=getCalculateMGAlpha(i);
					mgNumber=getMGNumber(i);					
				}
			}
		}
		mGSelection[3][mgNumber]=1;	//將被挑到的組設為1;
		addPoint=getCalculateMGMemCount(mgNumber);
		if(addPoint<=lessPoint)
		{				
			for(int i=0;i<ALL;i++)	//找屬於 mgSelect組的人加入 
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					selection[6][getPersonalID(i)]=1;
				}
			}
			lessPoint=lessPoint-addPoint; 
		}
		else//lessPoint<addPoint 從被挑到的組挑 個人能力最好的 lessPoint個人 
		{
			int tempPersonal[addPoint][3];
			
			int addP=0;
			for(int i=0;i<ALL;i++)
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					tempPersonal[addP][0]=getPersonalID(i);
					tempPersonal[addP][1]=getPersonalAbility(i);
					tempPersonal[addP][2]=getPersonalMGroup(i);
					addP++;
				}
				
			}
			for(int i=0;i<addPoint;i++)
			{
				for(int j=1;j<addPoint;j++)
				{
					if(tempPersonal[j-1][1]<tempPersonal[j][1])
					{
						int temp1=tempPersonal[j-1][0];
						tempPersonal[j-1][0]=tempPersonal[j][0];
						tempPersonal[j][0]=temp1;
						
						int temp2=tempPersonal[j-1][1];
						tempPersonal[j-1][1]=tempPersonal[j][1];
						tempPersonal[j][1]=temp2;
						
						int temp3=tempPersonal[j-1][2];
						tempPersonal[j-1][2]=tempPersonal[j][2];
						tempPersonal[j][2]=temp3;
						
					}
				}
			}
			
//			for(int i=0;i<addPoint;i++)
//			{
//				cout<<"PID:"<<tempPersonal[i][0]<<", PAbility:"<<tempPersonal[i][1]<<", PGroupNum:"<<tempPersonal[i][2]<<endl;
//			}
			for(int i=0;i<lessPoint;i++)
			{
				selection[6][tempPersonal[i][0]]=1;
			}
			lessPoint=0;			
		}
	}
}

void mgMaxPerson()					// 挑選方式 8: 直接挑小組內個人平均最大N*K
{
	int maxPoint=N*K;
	int lessPoint=N*K;				//空位 
	int addPoint=0;					//每次加入的人數
	
	while(lessPoint>0)				//剩下的位置
	{
		int maxAbility=0;			//組內平均 
		int mgNumber=-1;			//小組編號 
		for(int i=0;i<mGCount;i++)
		{
			if(getCalculateMGP(i)>maxAbility&&mGSelection[4][i]==0)
			{
				maxAbility=getCalculateMGP(i);
				mgNumber=getMGNumber(i);					
			}
			
		}
		mGSelection[4][mgNumber]=1;	//將被挑到的組設為1;
		addPoint=getCalculateMGMemCount(mgNumber);		
		if(addPoint<=lessPoint)
		{				
			for(int i=0;i<ALL;i++)	//找屬於 mgSelect組的人加入 
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					selection[7][getPersonalID(i)]=1;
				}
			}
			lessPoint=lessPoint-addPoint; 
		}
		else//lessPoint<addPoint 從被挑到的組挑 個人能力最好的 lessPoint個人N*K 
		{
			int tempPersonal[addPoint][3];
			
			int addP=0;
			for(int i=0;i<ALL;i++)
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					tempPersonal[addP][0]=getPersonalID(i);
					tempPersonal[addP][1]=getPersonalAbility(i);
					tempPersonal[addP][2]=getPersonalMGroup(i);
					addP++;
				}
				
			}
			for(int i=0;i<addPoint;i++)
			{
				for(int j=1;j<addPoint;j++)
				{
					if(tempPersonal[j-1][1]<tempPersonal[j][1])
					{
						int temp1=tempPersonal[j-1][0];
						tempPersonal[j-1][0]=tempPersonal[j][0];
						tempPersonal[j][0]=temp1;
						
						int temp2=tempPersonal[j-1][1];
						tempPersonal[j-1][1]=tempPersonal[j][1];
						tempPersonal[j][1]=temp2;
						
						int temp3=tempPersonal[j-1][2];
						tempPersonal[j-1][2]=tempPersonal[j][2];
						tempPersonal[j][2]=temp3;
						
					}
				}
			}
			
//			for(int i=0;i<addPoint;i++)
//			{
//				cout<<"PID:"<<tempPersonal[i][0]<<", PAbility:"<<tempPersonal[i][1]<<", PGroupNum:"<<tempPersonal[i][2]<<endl;
//			}
			for(int i=0;i<lessPoint;i++)
			{
				selection[7][tempPersonal[i][0]]=1;
			}
			lessPoint=0;			
		}		
	}	 
}

void mgMaxMutual()					// 挑選方式 9: 直接挑小組內合作平均最大N*K
{
	int maxPoint=N*K;
	int lessPoint=N*K;				//空位 
	int addPoint=0;					//每次加入的人數
	
	while(lessPoint>0)				//剩下的位置
	{
		int maxAbility=0;			//組內平均 
		int mgNumber=-1;			//小組編號 
		for(int i=0;i<mGCount;i++)
		{
			if(getCalculateMGM(i)>maxAbility&&mGSelection[5][i]==0)
			{
				maxAbility=getCalculateMGM(i);
				mgNumber=getMGNumber(i);					
			}
			
		}
		mGSelection[5][mgNumber]=1;	//將被挑到的組設為1;
		addPoint=getCalculateMGMemCount(mgNumber);		
		if(addPoint<=lessPoint)
		{				
			for(int i=0;i<ALL;i++)	//找屬於 mgSelect組的人加入 
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					selection[8][getPersonalID(i)]=1;
				}
			}
			lessPoint=lessPoint-addPoint; 
		}
		else//lessPoint<addPoint 從被挑到的組挑 個人能力最好的 lessPoint個人N*K 
		{
			int tempPersonal[addPoint][3];
			
			int addP=0;
			for(int i=0;i<ALL;i++)
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					tempPersonal[addP][0]=getPersonalID(i);
					tempPersonal[addP][1]=getPersonalAbility(i);
					tempPersonal[addP][2]=getPersonalMGroup(i);
					addP++;
				}
				
			}
			for(int i=0;i<addPoint;i++)
			{
				for(int j=1;j<addPoint;j++)
				{
					if(tempPersonal[j-1][1]<tempPersonal[j][1])
					{
						int temp1=tempPersonal[j-1][0];
						tempPersonal[j-1][0]=tempPersonal[j][0];
						tempPersonal[j][0]=temp1;
						
						int temp2=tempPersonal[j-1][1];
						tempPersonal[j-1][1]=tempPersonal[j][1];
						tempPersonal[j][1]=temp2;
						
						int temp3=tempPersonal[j-1][2];
						tempPersonal[j-1][2]=tempPersonal[j][2];
						tempPersonal[j][2]=temp3;
						
					}
				}
			}
			
//			for(int i=0;i<addPoint;i++)
//			{
//				cout<<"PID:"<<tempPersonal[i][0]<<", PAbility:"<<tempPersonal[i][1]<<", PGroupNum:"<<tempPersonal[i][2]<<endl;
//			}
			for(int i=0;i<lessPoint;i++)
			{
				selection[8][tempPersonal[i][0]]=1;
			}
			lessPoint=0;			
		}		
	}
}

void mgMaxComplex()					// 挑選方式 10:直接挑小組內綜合平均最大
{
	int maxPoint=N*K;
	int lessPoint=N*K;				//空位 
	int addPoint=0;					//每次加入的人數
	
	while(lessPoint>0)				//剩下的位置
	{
		int maxAbility=0;			//組內平均 
		int mgNumber=-1;			//小組編號 
		for(int i=0;i<mGCount;i++)
		{
			if(getCalculateMGAlpha(i)>maxAbility&&mGSelection[6][i]==0)
			{
				maxAbility=getCalculateMGAlpha(i);
				mgNumber=getMGNumber(i);					
			}
			
		}
		mGSelection[6][mgNumber]=1;	//將被挑到的組設為1;
		addPoint=getCalculateMGMemCount(mgNumber);		
		if(addPoint<=lessPoint)
		{				
			for(int i=0;i<ALL;i++)	//找屬於 mgSelect組的人加入 
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					selection[9][getPersonalID(i)]=1;
				}
			}
			lessPoint=lessPoint-addPoint; 
		}
		else//lessPoint<addPoint 從被挑到的組挑 個人能力最好的 lessPoint個人N*K 
		{
			int tempPersonal[addPoint][3];
			
			int addP=0;
			for(int i=0;i<ALL;i++)
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					tempPersonal[addP][0]=getPersonalID(i);
					tempPersonal[addP][1]=getPersonalAbility(i);
					tempPersonal[addP][2]=getPersonalMGroup(i);
					addP++;
				}
				
			}
			for(int i=0;i<addPoint;i++)
			{
				for(int j=1;j<addPoint;j++)
				{
					if(tempPersonal[j-1][1]<tempPersonal[j][1])
					{
						int temp1=tempPersonal[j-1][0];
						tempPersonal[j-1][0]=tempPersonal[j][0];
						tempPersonal[j][0]=temp1;
						
						int temp2=tempPersonal[j-1][1];
						tempPersonal[j-1][1]=tempPersonal[j][1];
						tempPersonal[j][1]=temp2;
						
						int temp3=tempPersonal[j-1][2];
						tempPersonal[j-1][2]=tempPersonal[j][2];
						tempPersonal[j][2]=temp3;
						
					}
				}
			}
			
//			for(int i=0;i<addPoint;i++)
//			{
//				cout<<"PID:"<<tempPersonal[i][0]<<", PAbility:"<<tempPersonal[i][1]<<", PGroupNum:"<<tempPersonal[i][2]<<endl;
//			}
			for(int i=0;i<lessPoint;i++)
			{
				selection[9][tempPersonal[i][0]]=1;
			}
			lessPoint=0;			
		}		
	}
}

void mgCyclePerCount()				// 挑選方式 11:依群組人數大至小循環挑選 每次都挑個人能力最好的
{
	int maxPoint=N*K;
	int lessPoint=N*K;				//空位 
	int gNum=0;						//標記目前在挑哪一組 
	int maxLocal=mGCount;			//告知只要等於這個數就要重新循環
	mutualGLargeToSmall(4);
	//showMGData();	
	while(lessPoint>0)
	{
		int local=getMGNumber(gNum);
		int addmemberID=-1;
		int max=0;
		for(int j=0;j<ALL;j++)
		{			
			if(getPersonalMGroup(j)==local&&selection[10][getPersonalID(j)]==0)
			{
				if(getPersonalAbility(j)>max)
				{
					max=getPersonalAbility(j);
					addmemberID=getPersonalID(j);
				}
			}
		}
		if(addmemberID!=-1)
		{
			selection[10][addmemberID]=1;
			lessPoint--;
		}
		gNum++;		
		if(gNum==maxLocal)
			gNum=0;
	}	
}				

void mgCycleAbility()				// 挑選方式 12:依群組能力大至小循環挑選 每次都挑個人能力最好的 
{
	int maxPoint=N*K;
	int lessPoint=N*K;				//空位 
	int gNum=0;						//標記目前在挑哪一組 
	int maxLocal=mGCount;			//告知只要等於這個數就要重新循環
	mutualGLargeToSmall(3);
	//showMGData();	
	while(lessPoint>0)
	{
		int local=getMGNumber(gNum);
		int addmemberID=-1;
		int max=0;
		for(int j=0;j<ALL;j++)
		{			
			if(getPersonalMGroup(j)==local&&selection[11][getPersonalID(j)]==0)
			{
				if(getPersonalAbility(j)>max)
				{
					max=getPersonalAbility(j);
					addmemberID=getPersonalID(j);
				}
			}
		}
		if(addmemberID!=-1)
		{
			selection[11][addmemberID]=1;
			lessPoint--;
		}
		gNum++;		
		if(gNum==maxLocal)
			gNum=0;
	}
} 

void mgPGood()					// 挑選方式 13:從群裡挑個人能力具有一定能力的人
{
	//cout<<"mgPGood"<<endl;
	int maxPoint=N*K;
	int lessPoint=N*K;				//空位 
	int addPoint=0;					//每次加入的人數 
	int pWeight=PWEIGHT;
	int resetMark=false;			//如果第一輪挑完還沒挑至N*K人  pWeight-10 再重新挑一輪 
	int reCount=0;
	int gNum=0;						//標記目前在挑哪一組 
	int maxLocal=mGCount;			//告知只要等於這個數就要重新循環
	mutualGLargeToSmall(4);
	
	
	while(lessPoint>0)		//剩下的位置
	{
		//cout<<"mgPGood1"<<endl;
		int mgSelect;
		int mgNumber;
		int local=getMGNumber(gNum);
		int addmemberID=-1;		
		//addPoint=getCalculateMGMemCount(mgNumber);
		//addPoint=0;
			
		for(int i=0;i<ALL;i++)	//找屬於 mgSelect組的人加入 
		{
			if(getPersonalMGroup(getPersonalID(i))==local)
			{
				if(selection[12][getPersonalID(i)]==0&&getPersonalIndicator(getPersonalID(i))>pWeight)
				{
					selection[12][getPersonalID(i)]=1;
					lessPoint--;
					//cout<<"mgPGood:"<<lessPoint<<endl;
				}						
			}
			if(lessPoint==0)
				break;
		}
		gNum++;
		reCount++;
		if(reCount==mGCount)
		{
			gNum=0;
			pWeight=pWeight-10;
			reCount=0;
		}
		
//			for(int i=0;i<addPoint;i++)
//			{
//				cout<<"PID:"<<tempPersonal[i][0]<<", PAbility:"<<tempPersonal[i][1]<<", PGroupNum:"<<tempPersonal[i][2]<<endl;
//			}
	
	} 
	
}



double minPerformance(int method)	// 計算小組最低表現的平均能力值
{
	resetGroup();			// 初始化小組成員
	grouping(method);		// 開始進行分組
//	showGroup(method);		// 顯示分組結果
	
	double avg = 10000; 		// 預設最低整體能力值為 1000
	
	int c = (K*(K+1))/2;

	for(int i=0;i<N;i++)
	{
		double personal=calculateP(i);					// 計算小組個人平均能力值
		double mutual=calculateM(i);					// 計算小組合作平均能力值
		double total=(personal+mutual)/c;					// 計算小組整體平均能力值
		
		if( total < avg)
			avg = total;
		
//		printf("第 %d 組　個人平均值：%2.3f　合作平均值：%2.3f　整體平均值：%2.3f \n\n",i,personal,mutual,total);
//		printf("第 %d 組　個人能力加總：%8f　合作能力加總：%8f　總和：%8f  總和平均：%8f \n\n",i,personal,mutual,personal+mutual,total);
		
	}

	return avg;
};
double calculateP(int i)			// 計算小組成員的個人平均能力值
{
	double score=0;
	
 	for(int j=0;j<K;j++)
 		score += personalIndicator[group[i][j]];
 		
 	return score;  
}

double calculateM(int i)			// 計算小組成員之間的合作平均能力值
{	
	int count=0;
	double score=0;
 	
 	for(int j=0;j<K;j++)
 	{
  		for(int k=0;k<K;k++)
  		{
   			if(j<k)
   			{
    			score+=mutualIndicator[group[i][j]][group[i][k]];
    			count++;
   			}
  		}
 	}

 	return score;
}

void resetGroup()					// 初始化小組成員
{
	for(int i=0;i<N;i++)
		for(int j=0;j<K;j++)group[i][j]=-1;
}

void grouping(int m)				// 開始進行分組
{
	int s=0;
	for(int i=0;i<N*K;i++)
	{
		while(selection[m][s]==0)
			s++; 	
		numberID[i]=s;
		s++;
	}
	
	for(int i=0;i<N*K;i++) 
	{
		abilityValue[i][0]=personalIndicator[numberID[i]];
		// 被挑選的人的個人能力  1st 
		
		double tmpSum=0;
		for(int j=0;j<N*K;j++)
			tmpSum += mutualIndicator[numberID[i]][numberID[j]];
			
		abilityValue[i][1]=tmpSum/(N*K-1);
		// 被挑選的人與人之間的合作能力  2st 
		
		abilityValue[i][2]=alpha*abilityValue[i][0]+(1-alpha)*(abilityValue[i][1]);	
		// 被挑選的人的整體能力  3st 
	}
	//showNKData(m);
	smallToLarge(1);
	// 排列方式 1：小到大排列
	// 參數調整： 0 是個人排序、 1 是合作排序、 2 是整體排序 

//	showAbilityValue();
	// 顯示排序結果
	
//	randomNumber(m);	// 分組排序 1：隨機分組
//	ballot();			// 分組排序 2：抽籤分組 重寫
//	SType(); 			// 分組排序 3：S型排序
//	MGPG();				// 分組排序 4 
//	MGPG_2();
	MGPG_3();		
//	MGMG();				// 分組排序 5 
}

void showNKData(int Methods)
{
	int pTotal=0;
	int mTotal=0;
	for(int i=0;i<N*K;i++)
	{
		pTotal=pTotal+getPersonalIndicator(numberID[i]);
		
		for(int j=0;j<N*K;j++)
		{
			if(i<j)
				mTotal = mTotal+getMutualIndicator(numberID[i],numberID[j]);
		}
	}
	cout<<selectionName[Methods]<<"方法:"<<"個人能力加總:"<< pTotal<<"\t合作能力加總:"<< mTotal<<endl; 
}

void randomNumber(int n)			// 分組排序 1：隨機分組
{
	int s=0;
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<K;j++)
		{	
			while(selection[n][s]==0)
				s++; 
			group[i][j]=s;
			s=s+1;
		}
	}
}

void ballot()						// 分組排序 2：抽籤分組
{
	for(int i=0;i<N;i++)
	{	
		for(int j=0;j<K;j++)
		{
			group[i][j]=numberID[N*i+j];
		}
	}
}

void SType()						// 分組排序 3：S型排序  
{
	for(int i=0;i<N;i++)
	{	
		for(int j=0;j<K;j++)
		{
			if(j%2==0)
				group[i][j]=numberID[j*N+i];
			else
				group[N-1-i][j]=numberID[j*N+i];
		}
	}
}

void MGPG()
{
//	cout << "MGPG" << endl;
	
	int select[N*K];		// 成員是否挑選 
	
	for(int i=0;i<N*K;i++)	// 重置 
		select[i] = 0;
	
	for(int i=0;i<N;i++)		// 每組分別挑選組長和組員 
	{
		int leaderID = -1;		// 個人能力最大的組長
		int pMax = 0;			// 個人能力最大值 
		int mark = -1;			// 標記位置
		
		for(int x=0;x<N*K;x++)	// 個人能力值挑選組長 
		{
			if(select[x] == 0 && getPersonalIndicator(numberID[x]) > pMax)
			{
				pMax = getPersonalIndicator(numberID[x]) ;
				leaderID = numberID[x];
				mark = x;
			}
		}
		
		group[i][0] = leaderID;
		select[mark] = 1;
		
		for(int x=1;x<K;x++)		// 合作能力值挑選組員 
		{
			int memberID = -1;		// 合作能力最大的成員 
			pMax = 0;				// 合作能力最大的成員
			
			for(int y=0;y<N*K;y++)		//找要加入的人 
			{
				if(select[y] == 0 && getMutualIndicator(leaderID,numberID[y]) > 70)
				{
					if(getPersonalIndicator(numberID[y]) > pMax)
					{
						pMax = getPersonalIndicator(numberID[y]) ;
						memberID = numberID[y];
						mark = y;
					}
				} 
			}
			
			if(memberID != -1)
			{
				group[i][x] = memberID;
				select[mark] = 1;
			}			
		}
	}
		
	for(int x=0;x<N;x++)	// 檢查每一組是否有剩下的成員
	{
		for(int y=0;y<K;y++)
		{
			if(group[x][y] == -1)
			{
				int maxSum = 0;
				int maxUser = -1;
				int teamNumber[K];
				
				for(int i=0;i<K;i++)	// 取得該小組現在的成員 
					teamNumber[i] = group[x][i];
				
				for(int i=0;i<N*K;i++)	// 該小組的成員與未被挑選的成員進行合作能力比較 
				{
					if(select[i] == 0)
					{
						int sum=0;
						
//						for(int j=0;j<K;j++)
//						{
//							if(teamNumber[j]!=-1)	// 有被挑選的人進行比對
//								sum += getMutualIndicator(numberID[i],teamNumber[j]);
//						}
						sum=getPersonalIndicator(numberID[i]);
						if(sum > maxSum) 	// 判斷成員數值誰高 
						{
							maxUser=i;
							maxSum=sum;
						}
					}
				}
				select[maxUser]=1;
				group[x][y]=numberID[maxUser];
			}
		}
	}
}

void MGPG_2()
{
//	cout << "MGPG" << endl;
	
	int select[N*K];		// 成員是否挑選 
	
	for(int i=0;i<N*K;i++)	// 重置 
		select[i] = 0;
	
	for(int i=0;i<N;i++)		// 每組分別挑選組長和組員 
	{
		int leaderID = -1;		// 個人能力最大的組長
		int pMax = 0;			// 個人能力最大值 
		int mark = -1;			// 標記位置
		
		for(int x=0;x<N*K;x++)	// 個人能力值挑選組長 
		{
			if(select[x] == 0 && getPersonalIndicator(numberID[x]) > pMax)
			{
				pMax = getPersonalIndicator(numberID[x]) ;
				leaderID = numberID[x];
				mark = x;
			}
		}
		
		group[i][0] = leaderID;
		select[mark] = 1;
		
		for(int x=1;x<K;x++)		// 合作能力值挑選組員 
		{
			int memberID = -1;		// 合作能力最大的成員 
			int mMax = 0;			// 合作能力最大的成員
			
			for(int y=0;y<N*K;y++)		//找要加入的人 
			{
				int totalMAblty=0;
				if(select[y]==0)
				{
					bool isSelect=true;
					for(int p=0;p<K;p++)
					{
						//logI("p",p);
						if(group[i][p]==-1)
							break;
						if(getMutualIndicator(numberID[y],group[i][p])<70)
						{
							isSelect=false;
						}
						else
						{
							isSelect=true;
						}				
					}
					if(isSelect==true)
					{
						memberID=numberID[y];
						mark=y;
					}	
				}								
			
			}
			
			if(memberID != -1)
			{
				group[i][x] = memberID;
				select[mark] = 1;
			}			
		}
	}
		
	for(int x=0;x<N;x++)	// 檢查每一組是否有剩下的成員
	{
		for(int y=0;y<K;y++)
		{
			if(group[x][y] == -1)
			{
				int maxSum = 0;
				int maxUser = -1;
				int teamNumber[K];
				
				for(int i=0;i<K;i++)	// 取得該小組現在的成員 
					teamNumber[i] = group[x][i];
				
				for(int i=0;i<N*K;i++)	// 該小組的成員與未被挑選的成員進行合作能力比較 
				{
					if(select[i] == 0)
					{
						int sum=0;
						
//						for(int j=0;j<K;j++)
//						{
//							if(teamNumber[j]!=-1)	// 有被挑選的人進行比對
//								sum += getMutualIndicator(numberID[i],teamNumber[j]);
//						}
						sum=getPersonalIndicator(numberID[i]);
						if(sum > maxSum) 	// 判斷成員數值誰高 
						{
							maxUser=i;
							maxSum=sum;
						}
					}
				}
				select[maxUser]=1;
				group[x][y]=numberID[maxUser];
			}
		}
	}
}

void MGPG_3()
{
	int select[N*K];		// 成員是否挑選 
	
	for(int i=0;i<N*K;i++)	// 重置 
		select[i] = 0;
		
	for(int i=0;i<N;i++)
	{
		int leaderID = -1;		// 個人能力最大的組長
		int pMax = 0;			// 個人能力最大值 
		int mark = -1;			// 標記位置
		
		for(int x=0;x<N*K;x++)	// 個人能力值挑選組長 
		{
			if(select[x] == 0 && getPersonalIndicator(numberID[x]) > pMax)
			{
				pMax = getPersonalIndicator(numberID[x]) ;
				leaderID = numberID[x];
				mark = x;
			}
		}		
		group[i][0] = leaderID;
		select[mark] = 1;
	}
	
	for(int i=0;i<N;i++)		// 每組分別挑選組長和組員 
	{
		int maxleader = -1;		// 個人能力最大的組長
		int mLMax = 0;			// 個人能力最大值 
		int mark = -1;			// 標記位置

		
		for(int x=1;x<K;x++)		// 合作能力值挑選組員 
		{
			int memberID = -1;		// 合作能力最大的成員 
			int mMax = 0;				// 合作能力最大的成員
			
			for(int y=0;y<N*K;y++)		//找要加入的人 
			{
				if(select[y] == 0)
				{
					int mSum = 0;
					
					for(int z=0;z<K;z++)
					{
						if(group[i][z] != -1)
							mSum += getMutualIndicator(numberID[y],group[i][z]);
					}
					if(mSum > mMax)
					{
						mMax = mSum;
						memberID = y;
					}
				}
			}
			if(memberID != -1)
			{
				group[i][x] = numberID[memberID];
				select[memberID] = 1;
			}			
		}
	}	
}
void MGMG()		// 從第一組使用合作能力挑選組長再合作能力挑選組員，每一組反覆進行 
{
//	cout << "合作合作挑選 MGMG" << endl;
	
	int select[N*K];		// 成員是否挑選 
	
	for(int i=0;i<N*K;i++)	// 重置 
		select[i] = 0;
	
	for(int i=0;i<N;i++)		// 每組分別挑選組長和組員 
	{
		int maxleader = -1;		// 個人能力最大的組長
		int mLMax = 0;			// 個人能力最大值 
		int mark = -1;			// 標記位置
		
		for(int x=0;x<N*K;x++)	// 個人能力值挑選組長 
		{
			int mSum = 0;
			
			for(int y=0;y<N*K;y++)
			{
				if(select[x] == 0)
					mSum += getMutualIndicator(numberID[x],numberID[y]);
			}
			if(mSum > mLMax)
			{
				mLMax = mSum;
				maxleader = x;
			}
		}
		
		group[i][0] = numberID[maxleader];
		select[maxleader] = 1;
		
		for(int x=1;x<K;x++)		// 合作能力值挑選組員 
		{
			int memberID = -1;		// 合作能力最大的成員 
			int mMax = 0;				// 合作能力最大的成員
			
			for(int y=0;y<N*K;y++)		//找要加入的人 
			{
				if(select[y] == 0)
				{
					int mSum = 0;
					
					for(int z=0;z<K;z++)
					{
						if(group[i][z] != -1)
							mSum += getMutualIndicator(numberID[y],group[i][z]);
					}
					if(mSum > mMax)
					{
						mMax = mSum;
						memberID = y;
					}
				}
			}
			if(memberID != -1)
			{
				group[i][x] = numberID[memberID];
				select[memberID] = 1;
			}			
		}
	}	
}

void showGroup(int m)				// 顯示分組結果
{
	printf("第 %d 種挑選方式：%s的分組結果 \n\n",m+1,selectionName[m].c_str());
	
	for(int i=0;i<N;i++)
	{
		printf("第 %d 組編號",i);
		for(int j=0;j<K;j++)
			printf("%5d",group[i][j]);
		cout<<endl;
	}
	cout<<endl;
}

void showAbilityValue()				// 顯示排序結果
{
	cout << "被挑選的成員："<< endl;
	cout << "編號："; for(int i=0;i<N*K;i++)printf("%-6d ",numberID[i]); cout << endl ;
	cout << "個人："; for(int i=0;i<N*K;i++)printf("%2.3f ",abilityValue[i][0]); cout << endl ;
	cout << "合作："; for(int i=0;i<N*K;i++)printf("%2.3f ",abilityValue[i][1]); cout << endl ;
	cout << "整體："; for(int i=0;i<N*K;i++)printf("%2.3f ",abilityValue[i][2]); cout << endl ;
	cout << endl;
}

void smallToLarge(int n)			// 排列方式 1：小到大排列 
{
    for(int i=0;i<N*K;i++)
	{
    	for(int j=1;j<N*K;j++)
		{
       		if(abilityValue[j][n]<abilityValue[j-1][n])
            {
            	int tmpNum=numberID[j];
            	numberID[j]=numberID[j-1];
            	numberID[j-1]=tmpNum;
            	
             	double tmpN1=abilityValue[j][0];
                abilityValue[j][0]=abilityValue[j-1][0];
                abilityValue[j-1][0]=tmpN1;
                
                double tmpN2=abilityValue[j][1];
                abilityValue[j][1]=abilityValue[j-1][1];
                abilityValue[j-1][1]=tmpN2;
                
                double tmpN3=abilityValue[j][2];
                abilityValue[j][2]=abilityValue[j-1][2];
                abilityValue[j-1][2]=tmpN3;
          	}
	   	}
	}
}

void largeToSmall(int n)			// 排列方式 2：大到小排列
{
	for(int i=0;i<N*K;i++)
	{
    	for(int j=1;j<N*K;j++)
		{
       		if(abilityValue[j][n]>abilityValue[j-1][n])
            {
            	int tmpNum=numberID[j];
            	numberID[j]=numberID[j-1];
            	numberID[j-1]=tmpNum;
            	
             	double tmpN1=abilityValue[j][0];
                abilityValue[j][0]=abilityValue[j-1][0];
                abilityValue[j-1][0]=tmpN1;
                
                double tmpN2=abilityValue[j][1];
                abilityValue[j][1]=abilityValue[j-1][1];
                abilityValue[j-1][1]=tmpN2;
                
                double tmpN3=abilityValue[j][2];
                abilityValue[j][2]=abilityValue[j-1][2];
                abilityValue[j-1][2]=tmpN3;
          	}
	   	}
	}
}


void showData()						// 顯示各項資料
{
	cout<<endl;
	
	printf("全部人數 ALL：%4d人 \n",ALL);
	printf("分成幾組 N  ：%4d人 \n",N);
	printf("每組幾人 K  ：%4d人 \n",K);
	
	cout<<endl;
	
	printf("個人能力值： \n");
	for(int i=0;i<ALL;i++)printf("%4d",i);
	cout<<endl;
	for(int i=0;i<ALL;i++)printf("%4d",personalIndicator[i]);
	cout<<endl<<endl;

	printf("是否被挑選： \n");
	for(int i=0;i<Methods;i++)
	{
		cout<<selectionName[i]<<endl;
		for(int j=0;j<ALL;j++)printf("%4d",selection[i][j]);
		cout<<endl;
	}
	
	cout<<endl;
	
//	printf("合作能力值： \n編號");
//	for(int i=0;i<ALL;i++)printf("%4d",i);cout<<endl;
//	for(int i=0;i<ALL;i++)
//	{
//		printf("%4d",i);
//		for(int j=0;j<ALL;j++)printf("%4d",mutualIndicator[i][j]);
//		cout<<endl;
//	}
//	
//	cout<<endl;
//	printf("總群組數:%d組\n",mGCount);
//	printf("互斥群組：\n");
//	printf("組別 %6s %20s","編號","個人平均");
//	cout<<endl;
//	int groupN=0;
//	int a=1;	
//	for(int i=0;i<mGCount;i++)
//	{
//		printf("第%2d組:",groupN);
//		for(int j=0;j<ALL;j++)
//		{			
//			if(personal[j][2]==groupN)
//			{
//				printf("%3d ",personal[j][0]);
//			}			
//		}
//		//printf("第%2d組:",groupN);
////		mGData[ALL][5];
////		
////
////		printf("%.2f ",mGData[a][1]);
//		a++;
//
//		
//		cout<<endl;
//		groupN++; 
//	}
	
}

int getPersonalIndicator(int number)//取得個人的合作能力
{
	return personalIndicator[number];
}
		
int getMutualIndicator(int number,int number2)//取得兩個人之間的合作能力
{
	return mutualIndicator[number][number2];
} 

double getMGNumber(int mgNumber)//每個互斥小組號碼 
{
	return mGData[mgNumber][0];
}

double getCalculateMGP(int mgNumber)//每個互斥小組個人能力平均 
{	
	return mGData[mgNumber][1];
}

double getCalculateMGM(int mgNumber)//每個互斥小組合作能力平均 
{	
	return mGData[mgNumber][2];
}  

double getCalculateMGAlpha(int mgNumber)//每個互斥小組組加權能力平均
{
	return mGData[mgNumber][3];
}

double getCalculateMGMemCount(int mgNumber)//每個互斥小組人數 
{	
	return 	mGData[mgNumber][4];
}

int getPersonalID(int number)//取得PersonalID
{
	return personal[number][0];
}

int getPersonalAbility(int number)//取得Personal個人能力值 
{
	return personal[number][1];
}
			 
int getPersonalMGroup(int number)//取得人的所屬號碼互斥分群號碼
{
	return personal[number][2];
}

void logI(string a,int b)
{
	cout<<a<<":"<<b<<endl;
}
void logD(string a,double b)
{
	cout<<a<<":"<<b<<endl;
}


		 
