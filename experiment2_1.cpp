#include <iostream>
#include <vector>
#define NUMBER_OF_TRANSACTIONS 5
#define NUMBER_OF_ITEMS 6

using namespace std;

enum Item{
	flower = 0,card = 1,water = 2,bear = 3,balloon = 4,candy = 5
};

int main(){
	vector<vector<bool> > transactions(NUMBER_OF_TRANSACTIONS,vector<bool> (NUMBER_OF_ITEMS,false));
	vector<int> support_amount_of_item (NUMBER_OF_ITEMS,0);
	int support_card_flower = 0;

	transactions[0][0] = true;
	transactions[0][1] = true;
	transactions[0][2] = true;
	transactions[1][3] = true;
	transactions[1][0] = true;
	transactions[1][4] = true;
	transactions[1][5] = true;
	transactions[2][1] = true;
	transactions[2][5] = true;
	transactions[2][0] = true;
	transactions[3][3] = true;
	transactions[3][4] = true;
	transactions[3][2] = true;
	transactions[4][0] = true;
	transactions[4][1] = true;
	transactions[4][2] = true;

	for(int i = 0;i < NUMBER_OF_TRANSACTIONS;i++){
		for(int j = 0;j < NUMBER_OF_ITEMS;j++){
			if(transactions[i][j]){
				support_amount_of_item[j]++;
			}
		}
		if(transactions[i][0] && transactions[i][1]){
			support_card_flower++;
		}
	}

	cout<<"support amount of all items:";
	for(int j = 0;j < NUMBER_OF_ITEMS;j++){
		cout<<support_amount_of_item[j]<<" ";
	}
	cout<<endl;
	cout<<"support amount of card && flower:"<<support_card_flower<<endl<<endl;
	cout<<"support:"<<(double)support_card_flower / (double)NUMBER_OF_TRANSACTIONS<<endl;
	cout<<"confidence:"<<(double)support_card_flower / (double)support_amount_of_item[1]<<endl;
	cout<<"lift:"<<(double)support_card_flower / (double)support_amount_of_item[0] / (double)support_amount_of_item[1]<<endl;

	return(0);
}
