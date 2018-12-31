#include <iostream>
#include <vector>
#define NUMBER_OF_TRANSACTIONS 5
#define NUMBER_OF_ATTRIBUTES 1

using namespace std;

int main(){
	vector<vector<bool> > transactions(NUMBER_OF_TRANSACTIONS,vector<bool> (NUMBER_OF_ATTRIBUTES + 1,false));
	int advert_spam = 0;
	int advert_ham = 0;
	int not_advert_spam = 0;
	int not_advert_ham = 0;
	double advert_condition_spam = 0.0;
	double advert_condition_ham = 0.0;
	double not_advert_condition_spam = 0.0;
	double not_advert_condition_ham = 0.0;
	double spam_condition_advert = 0.0;
	double ham_condition_advert = 0.0;
	double spam_condition_not_advert = 0.0;
	double ham_condition_not_advert = 0.0;

	transactions[0][0] = true;
	transactions[0][1] = true;
	transactions[4][1] = true;

	for(int i = 0;i < NUMBER_OF_TRANSACTIONS;i++){
		if(transactions[i][0] && transactions[i][1]){
			advert_spam++;
		}
		if(!transactions[i][0] && transactions[i][1]){
			not_advert_spam++;
		}
		if(transactions[i][0] && !transactions[i][1]){
			advert_ham++;
		}
		if(!transactions[i][0] && !transactions[i][1]){
			not_advert_ham++;
		}
	}
	advert_condition_spam = (double)advert_spam / (advert_spam + not_advert_spam);
	advert_condition_ham = (double)advert_ham / (advert_ham + not_advert_ham);
	not_advert_condition_spam = (double)not_advert_spam / (advert_spam + not_advert_spam);
	not_advert_condition_ham = (double)not_advert_ham / (advert_ham + not_advert_ham);

	spam_condition_advert = (double)advert_spam / (advert_spam + advert_ham); 
	ham_condition_advert = (double)advert_ham / (advert_spam + advert_ham);
	spam_condition_not_advert = (double)not_advert_spam / (not_advert_spam + not_advert_ham);
	ham_condition_not_advert = (double)not_advert_ham / (not_advert_spam + not_advert_ham);

	cout<<"    spam  ham  total"<<endl;
	cout<<"yes   "<<advert_spam<<"    "<<advert_ham<<"     "<<advert_spam + advert_ham<<endl;
	cout<<"no    "<<not_advert_spam<<"    "<<not_advert_ham<<"     "<<not_advert_spam + not_advert_ham<<endl;
	cout<<"total "<<advert_spam + not_advert_spam<<"    "<<advert_ham + not_advert_ham<<"     "<<advert_spam + advert_ham + not_advert_spam + not_advert_ham<<endl<<endl;
	
	cout<<"p(advert|spam):"<<advert_condition_spam<<endl;
	cout<<"p(advert|ham):"<<advert_condition_ham<<endl;
	cout<<"p(not_advert|spam):"<<not_advert_condition_spam<<endl;
	cout<<"p(not_advert|ham):"<<not_advert_condition_ham<<endl<<endl;

	cout<<"p(spam|advert):"<<spam_condition_advert<<endl;
	cout<<"p(ham|advert):"<<ham_condition_advert<<endl;
	cout<<"p(spam|not_advert):"<<spam_condition_not_advert<<endl;
	cout<<"p(ham|not_advert):"<<ham_condition_not_advert<<endl;
	
	return(0);
}
