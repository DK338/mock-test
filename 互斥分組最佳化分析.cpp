#include <iostream>
#include <cstdlib>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

const int ALL=200;					// �]�w�����H��
const int N=10; 					// �]�w�����X��
const int K=5; 						// �]�w�C�մX�H
const int Methods=13;				// �]�w�D��覡
const int mGMethods=8;				// �]�w���s�D��覡 			
double alpha=0.5;					// �]�w�v������
double mGAlpha=0.5;					// �]�w�᥸�s�կ�O��  1�����ӤH��O 0�����X�@��O 
double MWEIGHT=80;					// �]�w���s���e�� 20-80
double PWEIGHT=80;					// �]�w�ӤH�D����e�� 20-80
const string selectionName[Methods] ={
"�H��","�ӤH","�X�@",
"�����H��","�����ӤH(�H���u��)","�����X�@(�H���u��)","������X(�H���u��)",
"�����ӤH(��O�u��)","�����X�@(��O�u��)","������X(��O�u��)",
"�����H��(�`���D��)","������O(�`���D��)","����(�X�@�n�ӤH�n)"
};
// �]�w�D��覡���W��  {"�H���D�令��","�ӤH��O�̰�","�X�@��O�[�`�̤j��","","",""};

int personalIndicator[ALL];			// �]�w�ӤH��O�ȡG�ƭȽd�� 10 ~ 90 ���� 
int mutualIndicator[ALL][ALL]; 		// �]�w�X�@��O�ȡG�ƭȽd�� 10 ~ 90 ���� 
int selection[Methods][ALL]; 		// �]�w�Q�襤���H�G0�O���襤�B1�O�w�襤
int mGSelection[mGMethods][ALL];	// �]�w�Q�襤����: 0�O�����աB1�Q�D�� ��mutualGCount
 
int mGroup[ALL];					//mGroup[ALL]�����s��-1�N���Q�D�� 0��ܲĤ@�� 1��ܲĤG��
int mGCount=-1;						//�������s���s�ռ� -1��������s�{��
const int MGDATATYPE=5; 
double mGData[ALL][MGDATATYPE];				//�]�w�s�� �էO0 �ӤH�H��O����1 �X�@��O����2 �[�v����3  �H��4 �򥻤WALL�|�� mGCount
const int PDATATYPE=3; 
int personal[ALL][PDATATYPE];				//�]�w ALL �������s���B�ӤH��O�ȩM�������s���էO

int numberID[N*K];					// numberID[N*K]		�Q�D�諸�H���s�� 
double abilityValue[N*K][3];		// abilityValue[N*K][0] �Q�D�諸�H���ӤH��O�� 
									// abilityValue[N*K][1] �Q�D�諸�H���X�@��O��
									// abilityValue[N*K][2] �Q�D�諸�H�������O�� 

int group[N][K];					// �]�w�N�Q�D�諸�H�����p�աG�s��s���A�s���N��ӤH 

void initialization();				// �H���üƽᤩ��O��

void randomSelection();				// �D��覡 1�G�D���H�� N*K �H 
void personalMaxFirst();			// �D��覡 2�G�D��ӤH��O�ȳ̰��e N*K �H
void mutualMaxFirst();				// �D��覡 3�G�D��X�@��O�[�`�̤j�ȫe N*K �H

void mgRandomSelect();				// �D��覡 4�G�H���D�� N*K�H
void mgMemberMaxPerson(); 			// �D��覡 5: �u���q�H�Ƴ̦h�}�l�D �H�Ƥ@�˫h�u���D�p�դ��ӤH�����̤j
void mgMemberMaxmutual();			// �D��覡 6: �u���q�H�Ƴ̦h�}�l�D �H�Ƥ@�˫h�u���D�p�դ��X�@�����̤j
void mgMemberMaxComplex();			// �D��覡 7: �u���q�H�Ƴ̦h�}�l�D �H�Ƥ@�˫h�u���D�p�դ���X��O�̤j
void mgMaxPerson();					// �D��覡 8: �����D�p�դ��ӤH�����̤j
void mgMaxMutual();					// �D��覡 9: �����D�p�դ��X�@�����̤j
void mgMaxComplex();				// �D��覡 10:�����D�p�դ���X�����̤j
void mgCyclePerCount();				// �D��覡 11:�̸s�դH�Ƥj�ܤp�`���D�� �C�����D�ӤH��O�̦n��
void mgCycleAbility();				// �D��覡 12:�̸s�կ�O�j�ܤp�`���D�� �C�����D�ӤH��O�̦n�� 
void mgPGood(); 					// �D��覡 13:�q�s�̬D�ӤH��O�㦳�@�w��O���H 

double minPerformance(int);			// �p��U�p�ճ̧C��{��������O��

void resetGroup();					// ��l�Ƥp�զ���
void grouping(int);					// �}�l�i����� 
void showGroup(int);				// ��ܤ��յ��G

void showAbilityValue();			// ��ܱƧǵ��G

void smallToLarge(int);				// �ƦC�覡 1�G�p��j�ƦC 
void largeToSmall(int);				// �ƦC�覡 2�G�j��p�ƦC 

void randomNumber(int);				// ���ձƧ� 1�G�üƤ���
void ballot();						// ���ձƧ� 2�G���Ҥ���
void SType();						// ���ձƧ� 3�GS���Ƨ� 
void MGPG();						// ���ձƧ� 4�G�ӤH�n�X�@�n 
void MGPG_2();						// ���ձƧ� 4�G�ӤH�n�X�@�n(�睊)
void MGPG_3();						// ���ձƧ� 4�G�ӤH�n�X�@�n(�睊)
void MGMG();						// ���ձƧ� 5�G�X�@�n�X�@�n

double calculateP(int);				// �p��p�զ������ӤH������O��
double calculateM(int);				// �p��p�զ����������X�@������O�� 
double newCalculateResult(double);	//�p�⤬��������O�[�v����k

void showData();					// ��ܦU�����

void resetMGroup();					//���]�����s�� 		
int getPersonalIndicator(int);		//���o�ӤH���X�@��O
int getMutualIndicator(int,int);	//���o��ӤH�������X�@��O 
int selectM(int);					//��ַ��Y����ܾ�
void showMgData();					//��ܤ����s�ո��

void initPersonalData();					// ���o ALL �������s���B�ӤH��O��
int getPersonalID(int number);				//���oPersonal ID
int getPersonalAbility(int number);			//���oPersonal�ӤH��O�� 
int getPersonalMGroup(int number);			//���oPersonal�������s���X
void personalSmallToLargeSort(int dataN);	// �N������dataN �s��:0 �ӤH��O:1 ���ݲէO:2�p��j�ƦC 
void personalLargeToSmallSort(int dataN);	// �N������dataN �s��:0 �ӤH��O:1 ���ݲէO:2�j��p�ƦC 
void showDataPersonal();

void mGrouping();					//�������s
void mGSelectMaxOne();				//�@���D�@�H
void mGSIDontKnow();
 
void mGSelectMaxAllIn();			//�@���D��s 
int getLeaderNotInMGroup();			//�q���Q�D�諸�H�D��ӤH��Omin���H���s��
int personalMinOne();				//�q���Q�D�諸�H�D��ӤH��Omin���H���s��

void initMGroupData();

double getMGNumber(int mgNumber);			//�էO���X	
double getCalculateMGP(int mgNumber);		//�p�⤬���s�դ��ӤH���� 
double getCalculateMGM(int mgNumber);		//�p�⤬���s�դ��X�@���� 
double getCalculateMGAlpha(int mgNumber);	//�C�Ӥ����s�ե[�v��O����
double getCalculateMGMemCount(int mgNumber);//�C�Ӥ����s�թҥ]�t���H��

void mutualGLargeToSmall(int dataN);
void showMGData();
void showNKData(int Methods);							//�D�XN*K����� 
void createFileCSV(char* fileName,int maxTime);		// �ץX��csv�ɮ� 
void logI(string a,int b);
void logD(string a,double b);



int main()
{
	alpha = 0.5;
					
	MWEIGHT = 20;	
	createFileCSV("200�H-���e20.csv",100);
	
	MWEIGHT = 30;	
	createFileCSV("200�H-���e30.csv",100);
	
	MWEIGHT = 40;	
	createFileCSV("200�H-���e40.csv",100);
	
	MWEIGHT = 50;	
	createFileCSV("200�H-���e50.csv",100);
	
	MWEIGHT = 60;	
	createFileCSV("200�H-���e60.csv",100);
	
	MWEIGHT = 70;	
	createFileCSV("200�H-���e70.csv",100);
	
	MWEIGHT = 80;	
	createFileCSV("200�H-���e80.csv",100);
	



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
//	for(int i=0;i<Methods;i++)		// �}�l���� Methods
//	{
//		if(i==4||i==5||i==7||i==8)
//			continue;		
//		//printf("%s�G���饭���� %2.3f�@�@",selectionName[i].c_str(),minPerformance(i));
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
//		for(int j=0;j<Methods;j++)		// �}�l���� Methods
//		{
//			printf("%s�G���饭���� %2.3f�@�@",selectionName[j].c_str(),minPerformance(j));
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
//		cout<<"��"<<j<<"��"<<endl; 
//		for(int i=0;i<Methods;i++)		// �}�l���� Methods
//		{
//			if(i==1||i==2||i==4||i==5||i==7||i==8)
//				continue;
//			
//			printf("%s�G���饭���� %2.3f�@�@\n",selectionName[i].c_str(),minPerformance(i));
//			
//			cout<<endl;
//		}
//		cout<<endl<<endl;
//	}

//	showData();						// ��ܦU�����
	
//	showMgData();

	return 0;
}

void createFileCSV(char* fileName,int maxTime)		// �ץX��csv�ɮ� 
{
	ofstream createFile;
	createFile.open(fileName, ios::out | ios::trunc);
	createFile <<"�H��" << "," << "�����H��" << "," << "������X(�H���u��)" << ","<< "������X(��O�u��)" << "," << "�����H��(�`���D��)" << "," << "������O(�`���D��)" << "," << "�X�@�n�ӤH�n"<<endl;
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

void initialization()				// �H���üƽᤩ��O��
{	
	for(int i=0;i<ALL;i++)			// �ᤩ�ӤH��O��
	{
		personalIndicator[i]=rand()%81+10;
		for(int k=0;k<Methods;k++)	//���m selection
			selection[k][i]=0;
	}
	
	for(int x=0;x<ALL;x++)			// �ᤩ�X�@��O��
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
	vector<int> exceedWeightGroup;			//exceedWeight[ALL]�Ȯ��x�s�C���j����e�Ȫ��H
	vector<int> tempMutuallyExclusiveGroup;	//tempmutuallyExclusiveGroup[ALL]�Ȯ��x�s�ثe�b���ժ��H

	do
	{				
		exceedWeightGroup.clear();
		tempMutuallyExclusiveGroup.clear();
		//�����]�Ҧ��H���]���Q�D��N exceedWeight�]��1 
		for(int i=0;i<ALL;i++)
			exceedWeight[i]=1;
		
		for(int i=0;i<ALL;i++)
		{
			if(mGroup[i]==groupNumber) //�N�ثe�b�����խ��[�J 
				tempMutuallyExclusiveGroup.push_back(i);
	
			if(mGroup[i]==groupNumber)
			{				
				for(int j=0;j<ALL;j++)
				{		
					if(mutualIndicator[i][j]<MWEIGHT)//�u�n�p���v���ȳ��]���s 
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
			
			for(int i=0;i<exceedWeightGroup.size();i++)//�ŦX���e���H 
			{				
				for(int j=0;j<tempMutuallyExclusiveGroup.size();j++)//�ثe�b�s�ժ��H 
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
			fristID=selectM(0);//�q���t�D��D��H���覡 
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
		
	mGCount=groupNumber+1;//�]�w�������s���`�s�ռ� 	
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
		//cout<<"�ժ�ID:"<< leaderID;
		int maxMAbility=0;
		int maxID=-1;
		for(int i=0;i<ALL;i++)	//���Q�D�諸�H
		{			
			if(mGroup[i]==-1)
			{
				int totalMAbility=0;
				int find=1;
				for(int m=0;m<ALL;m++)//��ثe�b mGNumber�̪��H 
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
		cout<<"��"<< groupN+1<<"��"; 
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

int selectM(int Menthod)//��ַ��Y����ܾ� 
{
	int index;
	switch(Menthod)
	{
		case 0:index=getLeaderNotInMGroup();break; //�q���Q���H�D��ӤH��OMAX���H���s�� 
		case 1:index=personalMinOne();break; //�q���Q���H�D��ӤH��Omin���H���s��
		default:cout<<"��k��ܿ��~"<<endl;break; 	
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
	return index;//�^�ǥ��Q���խӤH��O�̤j�H���s�� 
}

int personalMinOne()//�q���Q���H�D��ӤH��Omin���H���s��
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
	return index;//�^�ǥ��Q���խӤH��O�̤p�H���s�� 
}
			
void initPersonalData()
{
	
	for(int i=0;i<ALL;i++)
	{
		personal[i][0]=i;						// �������s�� 
		personal[i][1]=personalIndicator[i];	// �������ӤH��O��		 
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
	//------------------------------�էO 
	int groupN=0;
	for(int i=0;i<mGCount;i++)
	{
		mGData[i][0]=(double)groupN;
		groupN++;
	}
	//------------------------------�էO
	//------------------------------�ӤH��O���� 
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
//		cout<<"��"<<mGData[i][0]<<"�խӤH����"<<mGData[i][1]<<endl;
//	}
	//------------------------------�ӤH��O����
	//------------------------------�X�@��O����
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
//		cout<<"��"<<mGData[i][0]<<"�զX�@����"<<mGData[i][2]<<endl;
//	}
	//------------------------------�X�@��O����
	//------------------------------�[�v��O����
	groupN=0;
	for(int i=0;i<mGCount;i++)
	{		
		mGData[i][3]=mGAlpha*mGData[i][1]+(1-mGAlpha)*mGData[i][2];	// �p�⤬���p�ե[�v������O��
		groupN++; 
	}
//	for(int i=0;i<mGCount;i++)
//	{
//		cout<<"��"<<mGData[i][0]<<"�ե[�v����"<<mGData[i][3]<<endl;
//	} 
	//------------------------------�[�v��O����
	//------------------------------�����p�դH��
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
//		cout<<"��"<<mGData[i][0]<<"�դH��"<<mGData[i][4]<<endl;
//	}
	//------------------------------�����p�դH��	 
		
}

void personalSmallToLargeSort(int dataN)		// �N������dataN �s��:0 �ӤH��O:1 ���ݲէO:2�p��j�ƦC 
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

void personalLargeToSmallSort(int dataN)		// �N������dataN �s��:0 �ӤH��O:1 ���ݲէO:2�j��p�ƦC 
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
	cout<<endl <<"���G"<<endl; 
	for(int i=0;i<ALL;i++)
		printf("�s���G%3d  �ӤH�G%2d �էO�G%2d\n",personal[i][0],personal[i][1],personal[i][2]);cout<<endl;
}

void mutualGLargeToSmall(int dataN)//dataN 0�ĴX��  1 �s�խӤH�H��O���� 2 �s�զX�@��O���� 3�s�ե[�v����  4�H�� �̷ӬY�@DATA�j��p�Ƨ�; 
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
	cout<<endl <<"���G"<<endl; 
	for(int i=0;i<mGCount;i++)
		printf("�էO�G%.0f  �դ��ӤH�����G%.2f �դ��X�@��O�����G%.2f �դ��[�v����:%.2f �դ��H��:%.0f\n",mGData[i][0],mGData[i][1],mGData[i][2],mGData[i][3],mGData[i][4]);cout<<endl;
}	

void randomSelection()				// �D��覡 1�G�D���H�� N*K �H 
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

void personalMaxFirst()				// �D��覡 2�G�D��ӤH��O�ȳ̰��e N*K �H
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

void mutualMaxFirst()				// �D��覡 3�G�D��X�@��O�[�`�̤j�ȫe N*K �H 
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

void mgRandomSelect()				//�D��覡 4 : �H���D�� �դ��H�� N*K �H
{
	int maxPoint=N*K;
	int lessPoint=N*K;				//�Ŧ� 
	int addPoint=0;					//�C���[�J���H�� 
	
	while(lessPoint>0)		//�ѤU����m
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
			for(int i=0;i<ALL;i++)	//���ݩ� mgSelect�ժ��H�[�J 
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					selection[3][getPersonalID(i)]=1;
				}
			}
			lessPoint=lessPoint-addPoint; 
		}
		else//lessPoint<addPoint �q�Q�D�쪺�լD �ӤH��O�̦n�� lessPoint�ӤH 
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

void mgMemberMaxPerson()			//�D��覡 5: �u���q�H�Ƴ̦h�}�l�D �H�Ƥ@�˫h�u���D�p�դ��ӤH�����̤j 
{									//mgData[mGCount][]�]�w�s�� �էO0 �ӤH�H��O����1 �X�@��O����2 �[�v����3  �H��4 �򥻤WALL�|�� mGCount
	int maxPoint=N*K;
	int lessPoint=N*K;				//�Ŧ� 
	int addPoint=0;					//�C���[�J���H�� 

	while(lessPoint>0)
	{
		int maxCount=-1;			//�Ȧs�̤j�H��
		for(int i=0;i<mGCount;i++)
		{
			if(getCalculateMGMemCount(i)>maxCount&&mGSelection[1][i]==0)
			{
				maxCount=getCalculateMGMemCount(i);
			}			
		}
		int maxPersonalAbility=0;	//�դ��ӤH���� 
		int mgNumber=-1;			//�p�սs�� 
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
		mGSelection[1][mgNumber]=1;	//�N�Q�D�쪺�ճ]��1;
		addPoint=getCalculateMGMemCount(mgNumber);
		if(addPoint<=lessPoint)
		{				
			for(int i=0;i<ALL;i++)	//���ݩ� mgSelect�ժ��H�[�J 
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					selection[4][getPersonalID(i)]=1;
				}
			}
			lessPoint=lessPoint-addPoint; 
		}
		else//lessPoint<addPoint �q�Q�D�쪺�լD �ӤH��O�̦n�� lessPoint�ӤH 
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

void mgMemberMaxmutual()			// �D��覡 6: �u���q�H�Ƴ̦h�}�l�DN*K �H�Ƥ@�˫h�u���D�p�դ��X�@�����̤j
{
	int maxPoint=N*K;
	int lessPoint=N*K;				//�Ŧ� 
	int addPoint=0;					//�C���[�J���H�� 

	while(lessPoint>0)
	{
		int maxCount=-1;			//�Ȧs�̤j�H��
		for(int i=0;i<mGCount;i++)
		{
			//cout<<"mGSelection[2][i]"<<mGSelection[2][i]<<endl;
			if(getCalculateMGMemCount(i)>maxCount&&mGSelection[2][i]==0)
			{
				maxCount=getCalculateMGMemCount(i);
			}			
		}
		int maxMutualAbility=0;	//�դ��X�@���� 
		int mgNumber=-1;			//�p�սs�� 
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
		mGSelection[2][mgNumber]=1;	//�N�Q�D�쪺�ճ]��1;
		addPoint=getCalculateMGMemCount(mgNumber);
		if(addPoint<=lessPoint)
		{				
			for(int i=0;i<ALL;i++)	//���ݩ� mgSelect�ժ��H�[�J 
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					selection[5][getPersonalID(i)]=1;
				}
			}
			lessPoint=lessPoint-addPoint; 
		}
		else//lessPoint<addPoint �q�Q�D�쪺�լD �ӤH��O�̦n�� lessPoint�ӤH 
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
		
void mgMemberMaxComplex()			// �D��覡 7: �u���q�H�Ƴ̦h�}�l�DN*K �H�Ƥ@�˫h�u���D�p�դ���X��O�̤j
{
	int maxPoint=N*K;
	int lessPoint=N*K;				//�Ŧ� 
	int addPoint=0;					//�C���[�J���H�� 

	while(lessPoint>0)
	{
		int maxCount=-1;			//�Ȧs�̤j�H��
		for(int i=0;i<mGCount;i++)
		{
			if(getCalculateMGMemCount(i)>maxCount&&mGSelection[3][i]==0)
			{
				maxCount=getCalculateMGMemCount(i);
			}			
		}
		int maxComplexAbility=0;	//�դ��ӤH���� 
		int mgNumber=-1;			//�p�սs�� 
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
		mGSelection[3][mgNumber]=1;	//�N�Q�D�쪺�ճ]��1;
		addPoint=getCalculateMGMemCount(mgNumber);
		if(addPoint<=lessPoint)
		{				
			for(int i=0;i<ALL;i++)	//���ݩ� mgSelect�ժ��H�[�J 
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					selection[6][getPersonalID(i)]=1;
				}
			}
			lessPoint=lessPoint-addPoint; 
		}
		else//lessPoint<addPoint �q�Q�D�쪺�լD �ӤH��O�̦n�� lessPoint�ӤH 
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

void mgMaxPerson()					// �D��覡 8: �����D�p�դ��ӤH�����̤jN*K
{
	int maxPoint=N*K;
	int lessPoint=N*K;				//�Ŧ� 
	int addPoint=0;					//�C���[�J���H��
	
	while(lessPoint>0)				//�ѤU����m
	{
		int maxAbility=0;			//�դ����� 
		int mgNumber=-1;			//�p�սs�� 
		for(int i=0;i<mGCount;i++)
		{
			if(getCalculateMGP(i)>maxAbility&&mGSelection[4][i]==0)
			{
				maxAbility=getCalculateMGP(i);
				mgNumber=getMGNumber(i);					
			}
			
		}
		mGSelection[4][mgNumber]=1;	//�N�Q�D�쪺�ճ]��1;
		addPoint=getCalculateMGMemCount(mgNumber);		
		if(addPoint<=lessPoint)
		{				
			for(int i=0;i<ALL;i++)	//���ݩ� mgSelect�ժ��H�[�J 
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					selection[7][getPersonalID(i)]=1;
				}
			}
			lessPoint=lessPoint-addPoint; 
		}
		else//lessPoint<addPoint �q�Q�D�쪺�լD �ӤH��O�̦n�� lessPoint�ӤHN*K 
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

void mgMaxMutual()					// �D��覡 9: �����D�p�դ��X�@�����̤jN*K
{
	int maxPoint=N*K;
	int lessPoint=N*K;				//�Ŧ� 
	int addPoint=0;					//�C���[�J���H��
	
	while(lessPoint>0)				//�ѤU����m
	{
		int maxAbility=0;			//�դ����� 
		int mgNumber=-1;			//�p�սs�� 
		for(int i=0;i<mGCount;i++)
		{
			if(getCalculateMGM(i)>maxAbility&&mGSelection[5][i]==0)
			{
				maxAbility=getCalculateMGM(i);
				mgNumber=getMGNumber(i);					
			}
			
		}
		mGSelection[5][mgNumber]=1;	//�N�Q�D�쪺�ճ]��1;
		addPoint=getCalculateMGMemCount(mgNumber);		
		if(addPoint<=lessPoint)
		{				
			for(int i=0;i<ALL;i++)	//���ݩ� mgSelect�ժ��H�[�J 
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					selection[8][getPersonalID(i)]=1;
				}
			}
			lessPoint=lessPoint-addPoint; 
		}
		else//lessPoint<addPoint �q�Q�D�쪺�լD �ӤH��O�̦n�� lessPoint�ӤHN*K 
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

void mgMaxComplex()					// �D��覡 10:�����D�p�դ���X�����̤j
{
	int maxPoint=N*K;
	int lessPoint=N*K;				//�Ŧ� 
	int addPoint=0;					//�C���[�J���H��
	
	while(lessPoint>0)				//�ѤU����m
	{
		int maxAbility=0;			//�դ����� 
		int mgNumber=-1;			//�p�սs�� 
		for(int i=0;i<mGCount;i++)
		{
			if(getCalculateMGAlpha(i)>maxAbility&&mGSelection[6][i]==0)
			{
				maxAbility=getCalculateMGAlpha(i);
				mgNumber=getMGNumber(i);					
			}
			
		}
		mGSelection[6][mgNumber]=1;	//�N�Q�D�쪺�ճ]��1;
		addPoint=getCalculateMGMemCount(mgNumber);		
		if(addPoint<=lessPoint)
		{				
			for(int i=0;i<ALL;i++)	//���ݩ� mgSelect�ժ��H�[�J 
			{
				if(getPersonalMGroup(i)==mgNumber)
				{
					selection[9][getPersonalID(i)]=1;
				}
			}
			lessPoint=lessPoint-addPoint; 
		}
		else//lessPoint<addPoint �q�Q�D�쪺�լD �ӤH��O�̦n�� lessPoint�ӤHN*K 
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

void mgCyclePerCount()				// �D��覡 11:�̸s�դH�Ƥj�ܤp�`���D�� �C�����D�ӤH��O�̦n��
{
	int maxPoint=N*K;
	int lessPoint=N*K;				//�Ŧ� 
	int gNum=0;						//�аO�ثe�b�D���@�� 
	int maxLocal=mGCount;			//�i���u�n����o�ӼƴN�n���s�`��
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

void mgCycleAbility()				// �D��覡 12:�̸s�կ�O�j�ܤp�`���D�� �C�����D�ӤH��O�̦n�� 
{
	int maxPoint=N*K;
	int lessPoint=N*K;				//�Ŧ� 
	int gNum=0;						//�аO�ثe�b�D���@�� 
	int maxLocal=mGCount;			//�i���u�n����o�ӼƴN�n���s�`��
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

void mgPGood()					// �D��覡 13:�q�s�̬D�ӤH��O�㦳�@�w��O���H
{
	//cout<<"mgPGood"<<endl;
	int maxPoint=N*K;
	int lessPoint=N*K;				//�Ŧ� 
	int addPoint=0;					//�C���[�J���H�� 
	int pWeight=PWEIGHT;
	int resetMark=false;			//�p�G�Ĥ@���D���٨S�D��N*K�H  pWeight-10 �A���s�D�@�� 
	int reCount=0;
	int gNum=0;						//�аO�ثe�b�D���@�� 
	int maxLocal=mGCount;			//�i���u�n����o�ӼƴN�n���s�`��
	mutualGLargeToSmall(4);
	
	
	while(lessPoint>0)		//�ѤU����m
	{
		//cout<<"mgPGood1"<<endl;
		int mgSelect;
		int mgNumber;
		int local=getMGNumber(gNum);
		int addmemberID=-1;		
		//addPoint=getCalculateMGMemCount(mgNumber);
		//addPoint=0;
			
		for(int i=0;i<ALL;i++)	//���ݩ� mgSelect�ժ��H�[�J 
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



double minPerformance(int method)	// �p��p�ճ̧C��{��������O��
{
	resetGroup();			// ��l�Ƥp�զ���
	grouping(method);		// �}�l�i�����
//	showGroup(method);		// ��ܤ��յ��G
	
	double avg = 10000; 		// �w�]�̧C�����O�Ȭ� 1000
	
	int c = (K*(K+1))/2;

	for(int i=0;i<N;i++)
	{
		double personal=calculateP(i);					// �p��p�խӤH������O��
		double mutual=calculateM(i);					// �p��p�զX�@������O��
		double total=(personal+mutual)/c;					// �p��p�վ��饭����O��
		
		if( total < avg)
			avg = total;
		
//		printf("�� %d �ա@�ӤH�����ȡG%2.3f�@�X�@�����ȡG%2.3f�@���饭���ȡG%2.3f \n\n",i,personal,mutual,total);
//		printf("�� %d �ա@�ӤH��O�[�`�G%8f�@�X�@��O�[�`�G%8f�@�`�M�G%8f  �`�M�����G%8f \n\n",i,personal,mutual,personal+mutual,total);
		
	}

	return avg;
};
double calculateP(int i)			// �p��p�զ������ӤH������O��
{
	double score=0;
	
 	for(int j=0;j<K;j++)
 		score += personalIndicator[group[i][j]];
 		
 	return score;  
}

double calculateM(int i)			// �p��p�զ����������X�@������O��
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

void resetGroup()					// ��l�Ƥp�զ���
{
	for(int i=0;i<N;i++)
		for(int j=0;j<K;j++)group[i][j]=-1;
}

void grouping(int m)				// �}�l�i�����
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
		// �Q�D�諸�H���ӤH��O  1st 
		
		double tmpSum=0;
		for(int j=0;j<N*K;j++)
			tmpSum += mutualIndicator[numberID[i]][numberID[j]];
			
		abilityValue[i][1]=tmpSum/(N*K-1);
		// �Q�D�諸�H�P�H�������X�@��O  2st 
		
		abilityValue[i][2]=alpha*abilityValue[i][0]+(1-alpha)*(abilityValue[i][1]);	
		// �Q�D�諸�H�������O  3st 
	}
	//showNKData(m);
	smallToLarge(1);
	// �ƦC�覡 1�G�p��j�ƦC
	// �Ѽƽվ�G 0 �O�ӤH�ƧǡB 1 �O�X�@�ƧǡB 2 �O����Ƨ� 

//	showAbilityValue();
	// ��ܱƧǵ��G
	
//	randomNumber(m);	// ���ձƧ� 1�G�H������
//	ballot();			// ���ձƧ� 2�G���Ҥ��� ���g
//	SType(); 			// ���ձƧ� 3�GS���Ƨ�
//	MGPG();				// ���ձƧ� 4 
//	MGPG_2();
	MGPG_3();		
//	MGMG();				// ���ձƧ� 5 
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
	cout<<selectionName[Methods]<<"��k:"<<"�ӤH��O�[�`:"<< pTotal<<"\t�X�@��O�[�`:"<< mTotal<<endl; 
}

void randomNumber(int n)			// ���ձƧ� 1�G�H������
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

void ballot()						// ���ձƧ� 2�G���Ҥ���
{
	for(int i=0;i<N;i++)
	{	
		for(int j=0;j<K;j++)
		{
			group[i][j]=numberID[N*i+j];
		}
	}
}

void SType()						// ���ձƧ� 3�GS���Ƨ�  
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
	
	int select[N*K];		// �����O�_�D�� 
	
	for(int i=0;i<N*K;i++)	// ���m 
		select[i] = 0;
	
	for(int i=0;i<N;i++)		// �C�դ��O�D��ժ��M�խ� 
	{
		int leaderID = -1;		// �ӤH��O�̤j���ժ�
		int pMax = 0;			// �ӤH��O�̤j�� 
		int mark = -1;			// �аO��m
		
		for(int x=0;x<N*K;x++)	// �ӤH��O�ȬD��ժ� 
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
		
		for(int x=1;x<K;x++)		// �X�@��O�ȬD��խ� 
		{
			int memberID = -1;		// �X�@��O�̤j������ 
			pMax = 0;				// �X�@��O�̤j������
			
			for(int y=0;y<N*K;y++)		//��n�[�J���H 
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
		
	for(int x=0;x<N;x++)	// �ˬd�C�@�լO�_���ѤU������
	{
		for(int y=0;y<K;y++)
		{
			if(group[x][y] == -1)
			{
				int maxSum = 0;
				int maxUser = -1;
				int teamNumber[K];
				
				for(int i=0;i<K;i++)	// ���o�Ӥp�ղ{�b������ 
					teamNumber[i] = group[x][i];
				
				for(int i=0;i<N*K;i++)	// �Ӥp�ժ������P���Q�D�諸�����i��X�@��O��� 
				{
					if(select[i] == 0)
					{
						int sum=0;
						
//						for(int j=0;j<K;j++)
//						{
//							if(teamNumber[j]!=-1)	// ���Q�D�諸�H�i����
//								sum += getMutualIndicator(numberID[i],teamNumber[j]);
//						}
						sum=getPersonalIndicator(numberID[i]);
						if(sum > maxSum) 	// �P�_�����ƭȽְ� 
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
	
	int select[N*K];		// �����O�_�D�� 
	
	for(int i=0;i<N*K;i++)	// ���m 
		select[i] = 0;
	
	for(int i=0;i<N;i++)		// �C�դ��O�D��ժ��M�խ� 
	{
		int leaderID = -1;		// �ӤH��O�̤j���ժ�
		int pMax = 0;			// �ӤH��O�̤j�� 
		int mark = -1;			// �аO��m
		
		for(int x=0;x<N*K;x++)	// �ӤH��O�ȬD��ժ� 
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
		
		for(int x=1;x<K;x++)		// �X�@��O�ȬD��խ� 
		{
			int memberID = -1;		// �X�@��O�̤j������ 
			int mMax = 0;			// �X�@��O�̤j������
			
			for(int y=0;y<N*K;y++)		//��n�[�J���H 
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
		
	for(int x=0;x<N;x++)	// �ˬd�C�@�լO�_���ѤU������
	{
		for(int y=0;y<K;y++)
		{
			if(group[x][y] == -1)
			{
				int maxSum = 0;
				int maxUser = -1;
				int teamNumber[K];
				
				for(int i=0;i<K;i++)	// ���o�Ӥp�ղ{�b������ 
					teamNumber[i] = group[x][i];
				
				for(int i=0;i<N*K;i++)	// �Ӥp�ժ������P���Q�D�諸�����i��X�@��O��� 
				{
					if(select[i] == 0)
					{
						int sum=0;
						
//						for(int j=0;j<K;j++)
//						{
//							if(teamNumber[j]!=-1)	// ���Q�D�諸�H�i����
//								sum += getMutualIndicator(numberID[i],teamNumber[j]);
//						}
						sum=getPersonalIndicator(numberID[i]);
						if(sum > maxSum) 	// �P�_�����ƭȽְ� 
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
	int select[N*K];		// �����O�_�D�� 
	
	for(int i=0;i<N*K;i++)	// ���m 
		select[i] = 0;
		
	for(int i=0;i<N;i++)
	{
		int leaderID = -1;		// �ӤH��O�̤j���ժ�
		int pMax = 0;			// �ӤH��O�̤j�� 
		int mark = -1;			// �аO��m
		
		for(int x=0;x<N*K;x++)	// �ӤH��O�ȬD��ժ� 
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
	
	for(int i=0;i<N;i++)		// �C�դ��O�D��ժ��M�խ� 
	{
		int maxleader = -1;		// �ӤH��O�̤j���ժ�
		int mLMax = 0;			// �ӤH��O�̤j�� 
		int mark = -1;			// �аO��m

		
		for(int x=1;x<K;x++)		// �X�@��O�ȬD��խ� 
		{
			int memberID = -1;		// �X�@��O�̤j������ 
			int mMax = 0;				// �X�@��O�̤j������
			
			for(int y=0;y<N*K;y++)		//��n�[�J���H 
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
void MGMG()		// �q�Ĥ@�ըϥΦX�@��O�D��ժ��A�X�@��O�D��խ��A�C�@�դ��жi�� 
{
//	cout << "�X�@�X�@�D�� MGMG" << endl;
	
	int select[N*K];		// �����O�_�D�� 
	
	for(int i=0;i<N*K;i++)	// ���m 
		select[i] = 0;
	
	for(int i=0;i<N;i++)		// �C�դ��O�D��ժ��M�խ� 
	{
		int maxleader = -1;		// �ӤH��O�̤j���ժ�
		int mLMax = 0;			// �ӤH��O�̤j�� 
		int mark = -1;			// �аO��m
		
		for(int x=0;x<N*K;x++)	// �ӤH��O�ȬD��ժ� 
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
		
		for(int x=1;x<K;x++)		// �X�@��O�ȬD��խ� 
		{
			int memberID = -1;		// �X�@��O�̤j������ 
			int mMax = 0;				// �X�@��O�̤j������
			
			for(int y=0;y<N*K;y++)		//��n�[�J���H 
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

void showGroup(int m)				// ��ܤ��յ��G
{
	printf("�� %d �جD��覡�G%s�����յ��G \n\n",m+1,selectionName[m].c_str());
	
	for(int i=0;i<N;i++)
	{
		printf("�� %d �սs��",i);
		for(int j=0;j<K;j++)
			printf("%5d",group[i][j]);
		cout<<endl;
	}
	cout<<endl;
}

void showAbilityValue()				// ��ܱƧǵ��G
{
	cout << "�Q�D�諸�����G"<< endl;
	cout << "�s���G"; for(int i=0;i<N*K;i++)printf("%-6d ",numberID[i]); cout << endl ;
	cout << "�ӤH�G"; for(int i=0;i<N*K;i++)printf("%2.3f ",abilityValue[i][0]); cout << endl ;
	cout << "�X�@�G"; for(int i=0;i<N*K;i++)printf("%2.3f ",abilityValue[i][1]); cout << endl ;
	cout << "����G"; for(int i=0;i<N*K;i++)printf("%2.3f ",abilityValue[i][2]); cout << endl ;
	cout << endl;
}

void smallToLarge(int n)			// �ƦC�覡 1�G�p��j�ƦC 
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

void largeToSmall(int n)			// �ƦC�覡 2�G�j��p�ƦC
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


void showData()						// ��ܦU�����
{
	cout<<endl;
	
	printf("�����H�� ALL�G%4d�H \n",ALL);
	printf("�����X�� N  �G%4d�H \n",N);
	printf("�C�մX�H K  �G%4d�H \n",K);
	
	cout<<endl;
	
	printf("�ӤH��O�ȡG \n");
	for(int i=0;i<ALL;i++)printf("%4d",i);
	cout<<endl;
	for(int i=0;i<ALL;i++)printf("%4d",personalIndicator[i]);
	cout<<endl<<endl;

	printf("�O�_�Q�D��G \n");
	for(int i=0;i<Methods;i++)
	{
		cout<<selectionName[i]<<endl;
		for(int j=0;j<ALL;j++)printf("%4d",selection[i][j]);
		cout<<endl;
	}
	
	cout<<endl;
	
//	printf("�X�@��O�ȡG \n�s��");
//	for(int i=0;i<ALL;i++)printf("%4d",i);cout<<endl;
//	for(int i=0;i<ALL;i++)
//	{
//		printf("%4d",i);
//		for(int j=0;j<ALL;j++)printf("%4d",mutualIndicator[i][j]);
//		cout<<endl;
//	}
//	
//	cout<<endl;
//	printf("�`�s�ռ�:%d��\n",mGCount);
//	printf("�����s�աG\n");
//	printf("�էO %6s %20s","�s��","�ӤH����");
//	cout<<endl;
//	int groupN=0;
//	int a=1;	
//	for(int i=0;i<mGCount;i++)
//	{
//		printf("��%2d��:",groupN);
//		for(int j=0;j<ALL;j++)
//		{			
//			if(personal[j][2]==groupN)
//			{
//				printf("%3d ",personal[j][0]);
//			}			
//		}
//		//printf("��%2d��:",groupN);
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

int getPersonalIndicator(int number)//���o�ӤH���X�@��O
{
	return personalIndicator[number];
}
		
int getMutualIndicator(int number,int number2)//���o��ӤH�������X�@��O
{
	return mutualIndicator[number][number2];
} 

double getMGNumber(int mgNumber)//�C�Ӥ����p�ո��X 
{
	return mGData[mgNumber][0];
}

double getCalculateMGP(int mgNumber)//�C�Ӥ����p�խӤH��O���� 
{	
	return mGData[mgNumber][1];
}

double getCalculateMGM(int mgNumber)//�C�Ӥ����p�զX�@��O���� 
{	
	return mGData[mgNumber][2];
}  

double getCalculateMGAlpha(int mgNumber)//�C�Ӥ����p�ղե[�v��O����
{
	return mGData[mgNumber][3];
}

double getCalculateMGMemCount(int mgNumber)//�C�Ӥ����p�դH�� 
{	
	return 	mGData[mgNumber][4];
}

int getPersonalID(int number)//���oPersonalID
{
	return personal[number][0];
}

int getPersonalAbility(int number)//���oPersonal�ӤH��O�� 
{
	return personal[number][1];
}
			 
int getPersonalMGroup(int number)//���o�H�����ݸ��X�������s���X
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


		 
