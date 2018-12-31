#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#define NUMBER_OF_TRANSACTIONS 7
#define NUMBER_OF_ATTRIBUTES 4

using namespace std;

enum Attributes{
	Advert = 0,Mon = 1,Groc = 2,Unsub = 3
};


void transform_to_binary(int index,vector<int>& instance);
string getNameClass(int parameter);
string getName(int parameter);

int main(){
	vector<vector<bool> > transactions(NUMBER_OF_TRANSACTIONS,vector<bool> (NUMBER_OF_ATTRIBUTES + 1,false));
	vector<vector<int> > attributes_condition_class(4,vector<int> (NUMBER_OF_ATTRIBUTES,0));
	vector<int> classes(2,0);
	vector<int> instance(NUMBER_OF_ATTRIBUTES + 1,0);

	//"true" means "yes" or "spam"
	//"false" means "no" or "ham"
	transactions[0][0] = true;
	transactions[0][1] = true;
	transactions[0][3] = true;
	transactions[0][4] = true;
	transactions[1][0] = true;
	transactions[1][4] = true;
	transactions[2][1] = true;
	transactions[2][2] = true;
	transactions[3][0] = true;
	transactions[4][1] = true;
	transactions[4][2] = true;
	transactions[4][3] = true;
	transactions[5][2] = true;
	transactions[6][3] = true;
	transactions[6][4] = true;

	for(int i = 0;i < NUMBER_OF_TRANSACTIONS;i++){
		for(int j = 0;j < NUMBER_OF_ATTRIBUTES;j++){
			if(transactions[i][j] && transactions[i][NUMBER_OF_ATTRIBUTES]){
				attributes_condition_class[0][j]++;
			}
			if(!transactions[i][j] && transactions[i][NUMBER_OF_ATTRIBUTES]){
				attributes_condition_class[1][j]++;
			}
			if(transactions[i][j] && !transactions[i][NUMBER_OF_ATTRIBUTES]){
				attributes_condition_class[2][j]++;
			}
			if(!transactions[i][j] && !transactions[i][NUMBER_OF_ATTRIBUTES]){
				attributes_condition_class[3][j]++;
			}
		}
		if(transactions[i][NUMBER_OF_ATTRIBUTES]){
			classes[0]++;
		}
		if(!transactions[i][NUMBER_OF_ATTRIBUTES]){
			classes[1]++;
		}
	}

	for(int m = 0;m < pow(2,NUMBER_OF_ATTRIBUTES + 1);m++){
		double temp_probability = 1.0;

		//"1" means "yes" or "spam"
		//"0" means "no" or "ham"
		transform_to_binary(m,instance);
/*
//test whether the binary array is right?
		for(int j = 0;j < NUMBER_OF_ATTRIBUTES + 1;j++){
			cout<<instance[j]<<" ";
		}
		cout<<endl;
*/
		for(int j = 0;j < NUMBER_OF_ATTRIBUTES;j++){
			if(instance[j] == 1 && instance[NUMBER_OF_ATTRIBUTES] == 1){
				temp_probability = temp_probability * attributes_condition_class[0][j] / classes[0];
			}
			else if(instance[j] == 0 && instance[NUMBER_OF_ATTRIBUTES] == 1){
				temp_probability = temp_probability * attributes_condition_class[1][j] / classes[0];	
			}
			else if(instance[j] == 1 && instance[NUMBER_OF_ATTRIBUTES] == 0){
				temp_probability = temp_probability * attributes_condition_class[2][j] / classes[1];
			}
			else if(instance[j] == 0 && instance[NUMBER_OF_ATTRIBUTES] == 0){
				temp_probability = temp_probability * attributes_condition_class[3][j] / classes[1];
			}
		}
		if(instance[NUMBER_OF_ATTRIBUTES] == 1){
			temp_probability = temp_probability * classes[0] / NUMBER_OF_TRANSACTIONS;
		}
		else if(instance[NUMBER_OF_ATTRIBUTES] == 0){
			temp_probability = temp_probability * classes[1] / NUMBER_OF_TRANSACTIONS;
		}

		cout<<"P(class="<<getNameClass(instance[NUMBER_OF_ATTRIBUTES])<<"|Advert="<<getName(instance[0])<<",Mon="<<getName(instance[1])<<",Groc="<<getName(instance[2])<<",Unsub="<<getName(instance[3])<<") = "<<temp_probability<<" * C"<<endl;
	}

/*
//output the internal value!
	for(int i = 0;i < NUMBER_OF_TRANSACTIONS;i++){
		for(int j = 0;j < NUMBER_OF_ATTRIBUTES + 1;j++){
			cout<<transactions[i][j]<<" ";
		}
		cout<<endl;
	}
	
	for(int i = 0;i < 4;i++){
		for(int j = 0;j < NUMBER_OF_ATTRIBUTES;j++){
			cout<<attributes_condition_class[i][j]<<" ";
		}
		cout<<endl;
	}

	for(int i = 0;i < 2;i++){
		cout<<classes[i]<<" ";
	}
	cout<<endl;
*/

	return(0);
}

void transform_to_binary(int index,vector<int>& instance){
	for(int j = 0;j < NUMBER_OF_ATTRIBUTES + 1;j++){
		if(index % 2 == 1){
			instance[NUMBER_OF_ATTRIBUTES - j] = 1;
		}
		else if(index % 2 == 0){
			instance[NUMBER_OF_ATTRIBUTES - j] = 0;
		}
		index = index / 2;
	}
}

string getNameClass(int parameter){
	string temp_name;
	switch(parameter){
		case 0:
			temp_name = "ham";
			break;
		case 1:
			temp_name = "spam";
			break;
		default:
			temp_name = "wrong";
			break;
	}
	return(temp_name);
}

string getName(int parameter){
	string temp_name;
	switch(parameter){
		case 0:
			temp_name = "no";
			break;
		case 1:
			temp_name = "yes";
			break;
		default:
			temp_name = "wrong";
			break;
	}
	return(temp_name);
}
