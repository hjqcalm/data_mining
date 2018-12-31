#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#define NUMBER_OF_TRANSACTIONS 15
#define NUMBER_OF_ATTRIBUTES 4
#define MIN_DISTANCE 10000
#define K 3

using namespace std;

enum Attributes{
	sepal_length = 0,sepal_width = 1,petal_length = 2,petal_width = 3
};

bool not_changed(vector<int> first,vector<int> second);
void getClusterE(vector<vector<double> > transactions);
double getEuclidDistance(vector<double> first,vector<double> second);

int main(){
	ifstream in_file_stream;
	in_file_stream.open("/home/hjqcalm/Documents/data_mining/flower_data.txt");
	if(in_file_stream.fail()){
		cout<<"the input file opended unsuccessfully!"<<endl;
		exit(1);
	}
	
	vector<vector<double> > transactions(NUMBER_OF_TRANSACTIONS,vector<double> (NUMBER_OF_ATTRIBUTES + 1,0.0));
	for(int i = 0;i < NUMBER_OF_TRANSACTIONS;i++){
		for(int j = 0;j < NUMBER_OF_ATTRIBUTES + 1;j++){
			in_file_stream>>transactions[i][j];
		}
	}

	//getClusterE(transactions);
	getClusterM(transactions);

	in_file_stream.close();
	return(0);
}


double getEuclidDistance(vector<double> first,vector<double> second){
	double temp_distance = 0.0;
	double temp_pow_sum = 0.0;

	for(int j = 0;j < NUMBER_OF_ATTRIBUTES;j++){
		temp_pow_sum += pow(first[j] - second[j],2);
	}
	//temp_distance = sqrt(temp_pow_sum / NUMBER_OF_ATTRIBUTES);
	temp_distance = pow(temp_pow_sum / NUMBER_OF_ATTRIBUTES,0.5);

	return(temp_distance);
}


double getMahalanobisDistance(){
	double temp_distance = 0.0;
	
	return(temp_distance);
}

void getClusterM(vector<vector<double> > transactions){
	
}


void getClusterE(vector<vector<double> > transactions){
	double temp_distance = 0.0;
	double min_distance = MIN_DISTANCE;
	int which_class = 0;
	vector<int> classes(NUMBER_OF_TRANSACTIONS,0);
	vector<int> old_classes(NUMBER_OF_TRANSACTIONS,0);
	vector<vector<double> > center_points(K,vector<double> (NUMBER_OF_ATTRIBUTES,0.0));
	vector<int> number(K,0);

	for(int m = 0;m < K;m++){
		for(int j = 0;j < NUMBER_OF_ATTRIBUTES;j++){
			center_points[m][j] = transactions[m][j];
		}
	}

	do{
		for(int i = 0;i < NUMBER_OF_TRANSACTIONS;i++){
			old_classes[i] = classes[i];
		}

		for(int i = 0;i < NUMBER_OF_TRANSACTIONS;i++){
			min_distance = MIN_DISTANCE;

			for(int m = 0;m < K;m++){
				temp_distance = getEuclidDistance(transactions[i],center_points[m]);
				if(temp_distance < min_distance){
					min_distance = temp_distance;
					which_class = m;
				}
			}
			classes[i] = which_class;
		}

		for(int m = 0;m < K;m++){
			for(int j = 0;j < NUMBER_OF_ATTRIBUTES;j++){
				center_points[m][j] = 0;
			}
			number[m] = 0;
		}

		for(int i = 0;i < NUMBER_OF_TRANSACTIONS;i++){
			if(classes[i] == 0){
				for(int j = 0;j < NUMBER_OF_ATTRIBUTES;j++){
					center_points[0][j] += transactions[i][j];
				}
				number[0]++;
			}
			else if(classes[i] == 1){
				for(int j = 0;j < NUMBER_OF_ATTRIBUTES;j++){
					center_points[1][j] += transactions[i][j];
				}
				number[1]++;	
			}
			else if(classes[i] == 2){
				for(int j = 0;j < NUMBER_OF_ATTRIBUTES;j++){
					center_points[2][j] += transactions[i][j];
				}
				number[2]++;	
			}
		}

		for(int m = 0;m < K;m++){
			for(int j = 0;j < NUMBER_OF_ATTRIBUTES;j++){
				center_points[m][j] = center_points[m][j] / number[m];
			}
		}
	}while(!not_changed(classes,old_classes));

//print:
	cout<<"center points:"<<endl;
	for(int m = 0;m < K;m++){
		for(int j = 0;j < NUMBER_OF_ATTRIBUTES;j++){
			cout<<center_points[m][j]<<" ";
		}
		cout<<endl;
	}

	cout<<endl<<"classes:";
	for(int i = 0;i < NUMBER_OF_TRANSACTIONS;i++){
		cout<<classes[i]<<" ";
	}
	cout<<endl;
}

bool not_changed(vector<int> first,vector<int> second){
	bool judge = true;

	for(int i = 0;i < NUMBER_OF_TRANSACTIONS;i++){
		if(first[i] == second[i]){
			judge = judge && true;
		}
		else if(first[i] != second[i]){
			judge = false;
			break;
		}
	}

	return(judge);
}
