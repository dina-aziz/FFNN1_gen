#include<iostream>
#include<cmath>
#include<iomanip>
#include<ctime>
#include<conio.h>
#include<fstream>
#include<string>

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))    // gets the length of an array
using namespace std;
float randomizeAround(float n)  // returns a random number around the argument n (the number may be multiplied by -1)
{
	int sign=rand()%2;
	int addSub=rand()%2;
	int r=rand()%30;
	float range=float(r);
	range/=100;
	if(addSub==0) n+=range;
	else n-=range;
	if(sign==0) return n;
	else return -1*n;
}
class logisticNeuron    // this class represents a neuron
{
private:
	float * weights; 
	float output;
	int numberOfLinks;
    int neuronNumber;
	static int number;
public:
	logisticNeuron()
	{
		neuronNumber= ++number;
		//cout<<"\n creating neuron..";
	}
	logisticNeuron(int numOfLinks,float initialWeight)
	{
		neuronNumber= ++number;
		numberOfLinks=numOfLinks;
		weights=new float[numOfLinks];
		for(int j=0;j<numOfLinks;j++) weights[j]=randomizeAround(initialWeight); // initialize every weight with a number around: initialWeight
	}
	void displayWeights()
	{
		cout<<"\n Weights of neuron"<<neuronNumber<<" :     ";
		for(int j=0;j<numberOfLinks;j++)
			cout<<"    "<<weights[j];
		cout<<endl;

	}
	void activate(float Input[],int size) // takes an array of Inputs given to a neuron and returns the result of the activation function
	{
		//float net=weights[size];
		float net=0;
		for(int j=0;j<size;j++) net+=Input[j]*weights[j];
        output=(2/(1+pow(2.718f,-1*net)))-1;                 // bipolar
		//output=1/(1+pow(2.718f,-1*net));                //unipolar
	}
	float getOutput(){return output;}
	void setWeight(int n,float w){weights[n]=w;}
	float getWeight(int n){return weights[n];}
	void setNeuronData(int numOfLinks,float initialWeight)   // used to specify the number of links to a neuron, and set the weights of these links
	{
		numberOfLinks=numOfLinks;
		weights=new float[numOfLinks];
		for(int j=0;j<numOfLinks;j++) weights[j]=randomizeAround(initialWeight);
	}
	void displayOuput()
	{
		cout<<"\n Output of neuron"<<neuronNumber<<" :     "<<output<<endl;
	}
	void displayneuronNumber()
	{
		cout<<"\n Neuron # "<<neuronNumber<<endl;
	}
};
int logisticNeuron::number=0;
class Layer
{
private:
	logisticNeuron* layer;
	int numberOfNeurons;
	int numberOfInputs;
	int layerNumber;
	static int Number;
public:
	float* layerOutput;
	float* layerInput;
	Layer()
	{layerNumber= ++Number;};
	Layer(int nn, int ni, float w)
	{
		layerNumber= ++Number;
		numberOfNeurons=nn;
		numberOfInputs=ni;           // bias is included
		layer=new logisticNeuron[nn];
		layerInput=new float[numberOfInputs];
		layerOutput=new float[nn+1];   
		for(int j=0;j<nn;j++) layer[j].setNeuronData(numberOfInputs,w);
	}
	void layerPropagate(float Input[])
	{
		for(int j=0;j<numberOfInputs;j++)
		{
			layerInput[j]=Input[j];
			//cout<<layerInput[j]<<endl;
		}
		for(int j=0;j<numberOfNeurons;j++){layer[j].activate(layerInput,numberOfInputs);layerOutput[j]=layer[j].getOutput();}
		layerOutput[numberOfNeurons]=-1.0;
	}
	void displayLayerWeights()
	{
		cout<<"\n Weights of Layer "<<layerNumber<<" : "<<endl;
		for(int j=0;j<numberOfNeurons;j++)
			layer[j].displayWeights();
	}
	int getNumberOfLayerInputs(){return numberOfInputs;}
	float getNeuronOutput(int nn)
	{
		return layer[nn].getOutput();
	}
	void displayLayerInput()
	{
		cout<<"\n Input of Layer "<<layerNumber<<" : ";
		for(int j=0;j<numberOfInputs;j++)
		{
			cout<<"    "<<layerInput[j];
		}
		cout<<endl;
	}
	void displayLayerOutput()
	{
		cout<<"\n Output of Layer "<<layerNumber<<" : ";
		for(int j=0;j<=numberOfNeurons;j++)
		{
			cout<<"    "<<layerOutput[j];
		}
		cout<<endl;
	}
	void setLayerData(int nn,int ni,float w)
	{
		numberOfNeurons=nn;
		numberOfInputs=ni;
		layer=new logisticNeuron[nn];
		layerInput=new float[numberOfInputs];
		layerOutput=new float[nn];   
		for(int j=0;j<nn;j++) layer[j].setNeuronData(numberOfInputs,w);
	}
	void setLayerWeight(int nn, int wn,float val)
	{
		layer[nn].setWeight(wn,val);
	}
	int getNumberOfneurons() {return numberOfNeurons;}
	float getNeuronWeight(int nn,int wn){return layer[nn].getWeight(wn);}
	void setNeuronWeight(int nn,int wn,float val){layer[nn].setWeight(wn,val);}
};
int Layer::Number=0;

int main()
{
	int numOfL1Neurons;
	int numOfInputsWithBias;
	cout<<"\nNumber of Layer 1 neurons: ";cin>>numOfL1Neurons;
	cout<<"\nNumber of inputs (including bias input): ";cin>>numOfInputsWithBias;
	cout<<endl<<endl;
	int numOfL2InputsWithBias=numOfL1Neurons+1;
	srand(time(NULL));
	Layer l1(numOfL1Neurons,numOfInputsWithBias,0.5);  
	Layer l2(1,numOfL2InputsWithBias,0.5);
	float *Output1=new float[numOfL2InputsWithBias];
	float Output;
	float *Input=new float[numOfInputsWithBias];   
	float *delta1=new float[numOfL1Neurons];
	float delta2;
	float **deltaWL1=new float*[numOfL1Neurons];  
	for(int g=0;g<numOfL1Neurons;g++) deltaWL1[g]=new float[numOfInputsWithBias];
	for(int j=0;j<numOfL1Neurons;j++)
		for(int i=0;i<numOfInputsWithBias;i++)deltaWL1[j][i]=0.0; 
	float *deltaWL2=new float[numOfL2InputsWithBias];
	for(int j=0;j<numOfL2InputsWithBias;j++) deltaWL2[j]=0.0;
	float lr=0.001;
	float target;
	for(int j=0;j<100000;j++)
	{
		Input[numOfInputsWithBias-1]=-1.0; 
		int a=(int)(pow(2.0,numOfInputsWithBias-1));  
		for(int i=0;i<a;i++)   
		{
			for(int h=0;h<numOfInputsWithBias-1;h++) 
			{
				int b= (int)(pow(2.0,h));
				Input[h]=1.0;
				if((i/b)%2 ==0)Input[h]*=-1;
			}
			//Input[numOfInputsWithBias-2]=-1.0;   // remove this
			int count=0;
			for(int h=0;h<numOfInputsWithBias-1;h++) if(Input[h]==1.0) count++;
			if(count==1) target=1.0; 
			else target=-1.0;
			l1.layerPropagate(Input);
			for(int m=0;m<numOfL2InputsWithBias;m++)   
			Output1[m]=l1.layerOutput[m];
			l2.layerPropagate(l1.layerOutput);
			Output=*l2.layerOutput;
			delta2=0.5*(target-Output)*(1-Output*Output);
			for(int k=0;k<numOfL1Neurons;k++ )delta1[k]=delta2*l2.getNeuronWeight(0,k)*0.5*(1-Output1[k]*Output1[k]);
			for(int k=0;k<numOfL2InputsWithBias-1;k++)deltaWL2[k]+=lr*delta2*Output1[k];
			deltaWL2[numOfL2InputsWithBias-1]-=lr*delta2;
			for(int m=0;m<numOfL1Neurons;m++)
			{
				for(int n=0;n<numOfInputsWithBias-1;n++)deltaWL1[m][n]+=lr*delta1[m]*Input[n];  //change
					deltaWL1[m][numOfInputsWithBias-1]-=lr*delta1[m];  // we may double lr
			}
		}
		///////////////////////////////////////////////////////////////// make the network focus on learning the wrong pattern
		for(int g=0;g<5;g++)
		{
			for(int h=0;h<numOfInputsWithBias;h++)Input[h]=-1.0;
			int count=0;
			for(int h=0;h<numOfInputsWithBias-1;h++) if(Input[h]==1.0) count++;
			if(count==1) target=1.0; 
			else target=-1.0;
			l1.layerPropagate(Input);
			for(int m=0;m<numOfL2InputsWithBias;m++)   
			Output1[m]=l1.layerOutput[m];
			l2.layerPropagate(l1.layerOutput);
			Output=*l2.layerOutput;
			delta2=0.5*(target-Output)*(1-Output*Output);
			for(int k=0;k<numOfL1Neurons;k++ )delta1[k]=delta2*l2.getNeuronWeight(0,k)*0.5*(1-Output1[k]*Output1[k]);
			for(int k=0;k<numOfL2InputsWithBias-1;k++)deltaWL2[k]+=lr*delta2*Output1[k];
			deltaWL2[numOfL2InputsWithBias-1]-=lr*delta2;
			for(int m=0;m<numOfL1Neurons;m++)
			{
				for(int n=0;n<numOfInputsWithBias-1;n++)deltaWL1[m][n]+=lr*delta1[m]*Input[n];  //change
					deltaWL1[m][numOfInputsWithBias-1]-=lr*delta1[m];  // we may double lr
			}
		}
		/////////////////////////////////////////////////////////////////
		for(int m=0;m<numOfL2InputsWithBias;m++) 
		{
			l2.setNeuronWeight(0,m,l2.getNeuronWeight(0,m)+deltaWL2[m]);
			deltaWL2[m]=0;
		}
		for(int m=0;m<numOfL1Neurons;m++)
			for(int n=0;n<numOfL2InputsWithBias;n++)  
			{
				l1.setNeuronWeight(m,n,l1.getNeuronWeight(m,n)+deltaWL1[m][n]);
				deltaWL1[m][n]=0;
			}
	}
	ofstream outfile("exp.txt");
	string s;
	char c='\n';
	s="\n- Number of layer 1 neurons:  ";
	outfile<<s;
	outfile<<numOfL1Neurons;
	s="\n- Number of inputs (including bias input):  ";
	outfile<<s<<numOfInputsWithBias;
	outfile<<c<<c;
	int a=(int)(pow(2.0,numOfInputsWithBias-1));   
	for(int i=0;i<a;i++)
	{
		Input[numOfInputsWithBias-1]=-1.0; 
		for(int h=0;h<numOfInputsWithBias-1;h++) 
		{
			int b= (int)(pow(2.0,h));
			Input[h]=1.0;
			if((i/b)%2 ==0)Input[h]*=-1;
		}
	//	Input[numOfInputsWithBias-2]=-1.0;   // remove this
		l1.layerPropagate(Input);
		l2.layerPropagate(l1.layerOutput);
		
			for(int q=0;q<numOfInputsWithBias-1;q++)
			{
				cout<<"\nx"<<q<<"= "<<Input[q]<<endl;
				string x="\nx";
				string eq="= ";
				outfile<<x<<q<<eq<<Input[q]<<c;
			}
			
		cout<<"\n\noutput: "<<*l2.layerOutput;
		string o="\n\noutput: ";
		outfile<<o<<*l2.layerOutput;
		string ln="\n----------------------------------------------------------\n";
		outfile<<ln;
		cout<<"\n----------------------------------------------------------"<<endl;
		
	}
	s="\n Layer 1 outputs : ";
	outfile<<s;
	float pattern[8];
	float net=0;
	for(int j=0;j<8;j++)pattern[j]=-1.0;
		for(int j=0;j<numOfL1Neurons;j++)
		{
			l1.layerPropagate(pattern);
			net=l1.getNeuronOutput(j)*l2.getNeuronWeight(0,j);
		}
		net-=l2.getNeuronWeight(0,numOfL1Neurons);
		s="\n\nL2 net : ";
		outfile<<s<<net;
		l2.layerPropagate(l1.layerOutput);
		outfile<<endl<<l2.getNeuronOutput(0);
	cout<<endl;
	getch();
	return 0;
}