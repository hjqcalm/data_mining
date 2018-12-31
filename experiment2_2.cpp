#include <iostream> 
#include <vector>
#include <string>
#define NUMBER_OF_TRANSACTIONS 5
#define NUMBER_OF_ITEMS 4
#define MIN_SUPPORT 0.4
#define MIN_CONFIDENCE 0.6

using namespace std;

string getName(int index);

enum Item{
	A = 0,B,C,E
};

/*
Actually,my method is brute mothod instead of Apriori;
and the front contains one item,so do the back!
*/
int main(){
	vector<vector<bool> > transactions(NUMBER_OF_TRANSACTIONS,vector<bool> (NUMBER_OF_ITEMS,false));
	vector<int> support_amount_of_item(NUMBER_OF_ITEMS,0);
	vector<bool> frequent_item(NUMBER_OF_ITEMS,false);
	int support_front_back = 0;

	transactions[0][0] = true;
	transactions[1][1] = true;
	transactions[2][0] = true;
	transactions[2][1] = true;
	transactions[2][2] = true;
	transactions[3][0] = true;
	transactions[3][1] = true;
	transactions[4][1] = true;
	transactions[4][2] = true;

	for(int i = 0;i < NUMBER_OF_TRANSACTIONS;i++){
		for(int j = 0;j < NUMBER_OF_ITEMS;j++){
			if(transactions[i][j]){
				support_amount_of_item[j]++;
			}
		}
	}

	for(int j = 0;j < NUMBER_OF_ITEMS;j++){
		if((double)support_amount_of_item[j] / (double)NUMBER_OF_TRANSACTIONS >= MIN_SUPPORT){
			frequent_item[j] = true;
		}
	}

	for(int m = 0;m < NUMBER_OF_ITEMS;m++){
		for(int n = 0;n < NUMBER_OF_ITEMS;n++){
			if(m != n && frequent_item[m] && frequent_item[n]){
				support_front_back = 0;
				for(int i = 0;i < NUMBER_OF_TRANSACTIONS;i++){
					if(transactions[i][m] && transactions[i][n]){
						support_front_back++;
					}
				}
				if((double)support_front_back / support_amount_of_item[m] >= MIN_CONFIDENCE){
					cout<<"rule:"<<getName(m)<<"->"<<getName(n)<<":yes"<<endl;
				}
				else if((double)support_front_back / support_amount_of_item[m] < MIN_CONFIDENCE){
					cout<<"rule:"<<getName(m)<<"->"<<getName(n)<<":no"<<endl;
				}
			}
		}
	}

	return(0);
}

string getName(int index){
	string name;
	switch(index){
		case 0:
			name = "A";
			break;
		case 1:
			name = "B";
			break;
		case 2:
			name = "C";
			break;
		case 3:
			name = "E";
			break;
		default:
			name = "wrong";
			break;		
	}

	return(name);
}
