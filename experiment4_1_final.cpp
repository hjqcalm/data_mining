#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#define NUMBER_OF_TRANSACTIONS 15
#define NUMBER_OF_ATTRIBUTES 4
#define MIN_ENTROPY 10000

using namespace std;

typedef struct Node{
	vector<vector<int> > transactions_subset;
	Node* parent;
	Node* lChild;
	Node* mChild;
	Node* rChild;
	int lValue;
	int mValue;
	int rValue;
	int target;
	Node(){
		parent = NULL;
		lChild = NULL;
		mChild = NULL;
		rChild = NULL;
		lValue = -1;
		mValue = -1;
		rValue = -1;
		target = -1;
	}
}Node,*NodePtr;

typedef struct List{
	//vector<NodePtr> buffer;
	vector<Node*> buffer;
	int head;
	int tail;
	List(){
		head = 0;
		tail = 0;

		//if there is not this 'for' statement, the size of 'buffer' is 0!
		//if there is this 'for' statement, the size of 'buffer' is 100,
		//and the value could be 'NULL' or the value that system allocates!
		for(int i = 0;i < 100;i++){
			buffer.push_back(NULL);
		}
	}
}List;

void split_transactions(Node* node);
void copy_vector_except(vector<vector<int> >& first,vector<vector<int> > second,int exception,int value);
void copy_vector(vector<vector<int> >& first,vector<vector<int> > second);
int getTarget(Node* node);
bool all_the_same(Node* node);
void print(Node* node);
void in_list(List& list,Node* node);
Node* out_list(List& list);
bool is_empty(List list);
bool is_leaf(Node* node);
string get_determination(Node* node);

//the testing codes take about 250 lines!
int main(){
	ifstream in_file_stream;
	in_file_stream.open("/home/hjqcalm/Documents/data_mining/transactions_data.txt");
	if(in_file_stream.fail()){
		cout<<"Input file opens unsuccessfully!"<<endl;
		exit(1);
	}

	vector<vector<int> > transactions(NUMBER_OF_TRANSACTIONS + 1,vector<int> (NUMBER_OF_ATTRIBUTES + 1,-1));
	for(int i = 0;i < NUMBER_OF_TRANSACTIONS + 1;i++){
		for(int j = 0;j < NUMBER_OF_ATTRIBUTES + 1;j++){
			in_file_stream>>transactions[i][j];
		}
	}
	
	Node* root = new Node();
	copy_vector(root -> transactions_subset,transactions);

	//create a determination tree!
	split_transactions(root);

	//output the determination tree!
	print(root);

	in_file_stream.close();
	return(0);
} 

void split_transactions(Node* node){
	int temp_target = -1;
	int temp_index = -1;//it represents the index of temp_target which is stable!
	int dimension1_size = 0;
	int dimension2_size = 0;
	vector<int> temp_vector;

	if(node == NULL){
		return;
	}

	temp_target = getTarget(node);

	if(temp_target == -1){
		return;
	}
	
	node -> target = temp_target;
	dimension1_size = node -> transactions_subset.size();
	dimension2_size = node -> transactions_subset[0].size();

	temp_vector.clear();
	for(int j = 0;j < dimension2_size;j++){
		if(node -> transactions_subset[0][j] != temp_target){
			temp_vector.push_back(node -> transactions_subset[0][j]);
		}
		else if(node -> transactions_subset[0][j] == temp_target){
			temp_index = j;
		}
	}

	if(temp_target == 0 || temp_target == 3){
		node -> lChild = new Node();
		node -> mChild = new Node();
		node -> rChild = new Node();
		node -> lValue = 0;
		node -> mValue = 1;
		node -> rValue = 2;
		node -> lChild -> parent = node;
		node -> mChild -> parent = node;
		node -> rChild -> parent = node;


		node -> lChild -> transactions_subset.push_back(temp_vector);
		node -> mChild -> transactions_subset.push_back(temp_vector);
		node -> rChild -> transactions_subset.push_back(temp_vector);

		copy_vector_except(node -> lChild -> transactions_subset,node -> transactions_subset,temp_index,0);
		copy_vector_except(node -> mChild -> transactions_subset,node -> transactions_subset,temp_index,1);
		copy_vector_except(node -> rChild -> transactions_subset,node -> transactions_subset,temp_index,2);
		
	}
	else if(temp_target == 1 || temp_target == 2){
		node -> lChild = new Node();
		node -> rChild = new Node();
		node -> lValue = 0;
		node -> rValue = 1;
		node -> lChild -> parent = node;
		node -> rChild -> parent = node;

		node -> lChild -> transactions_subset.push_back(temp_vector);
		node -> rChild -> transactions_subset.push_back(temp_vector);

		copy_vector_except(node -> lChild -> transactions_subset,node -> transactions_subset,temp_index,0);
		copy_vector_except(node -> rChild -> transactions_subset,node -> transactions_subset,temp_index,1);
	}

	split_transactions(node -> lChild);
	split_transactions(node -> mChild);
	split_transactions(node -> rChild);
}


void copy_vector_except(vector<vector<int> >& first,vector<vector<int> > second,int exception,int value){
	vector<int> temp_vector;

	for(int m = 1;m < second.size();m++){
		temp_vector.clear();
		for(int n = 0;n < second[0].size();n++){
			if(n != exception && second[m][exception] == value){
				temp_vector.push_back(second[m][n]);
			}
		}
		if(second[m][exception] == value){//if there is not this sentence,the null vector will be push_back into "first"!
			first.push_back(temp_vector);
		}
	}
}


//I should pass reference by using "&" instead of passing value!
void copy_vector(vector<vector<int> >& first,vector<vector<int> > second){
	//I should use push_back instead of index
	vector<int> temp_vector;

	for(int m = 0;m < second.size();m++){
		temp_vector.clear();
		for(int n = 0;n < second[0].size();n++){
			temp_vector.push_back(second[m][n]);
		}
		first.push_back(temp_vector);
	}
}

int getTarget(Node* node){
	int temp_target = -1;
	vector<int> number(3,0);
	vector<vector<int> > yes_no(3,vector<int> (2,0));
	double entropy = 0.0;
	double total_entropy = 0.0;
	double temp_ratio = 1.0;
	double min_entropy = MIN_ENTROPY;
	double temp_item = 0.0;
	int dimension1_size = node -> transactions_subset.size();
	int dimension2_size = node -> transactions_subset[0].size();

	if(dimension2_size == 1 || all_the_same(node)){
		temp_target = -1;
		return(temp_target);
	}

	for(int j = 0;j < dimension2_size - 1;j++){
		total_entropy = 0.0;
		entropy = 0.0;
		temp_ratio = 1.0;

		for(int m = 0;m < 3;m++){
			for(int n = 0;n < 2;n++){
				yes_no[m][n] = 0;
			}
			number[m] = 0;
		}
		
		//when I using yes_no[][] instead of yes_no.push_back(), it is unnecessary to use yes_no.clear()!
		if(node -> transactions_subset[0][j] == 0 || node -> transactions_subset[0][j] == 3){
			for(int i = 1;i < dimension1_size;i++){
				if(node -> transactions_subset[i][j] == 0){
					number[0]++;
					if(node -> transactions_subset[i][dimension2_size - 1] == 0){
						yes_no[0][0]++;
					}
					else if(node -> transactions_subset[i][dimension2_size - 1] == 1){
						yes_no[0][1]++;
					}
				}
				else if(node -> transactions_subset[i][j] == 1){
					number[1]++;
					if(node -> transactions_subset[i][dimension2_size - 1] == 0){
						yes_no[1][0]++;
					}
					else if(node -> transactions_subset[i][dimension2_size - 1] == 1){
						yes_no[1][1]++;
					}
				}
				else if(node -> transactions_subset[i][j] == 2){
					number[2]++;
					if(node -> transactions_subset[i][dimension2_size - 1] == 0){
						yes_no[2][0]++;
					}
					else if(node -> transactions_subset[i][dimension2_size - 1] == 1){
						yes_no[2][1]++;
					}
				}
			}

			for(int m = 0;m < 3;m++){
				entropy = 0.0;

				if(number[m] == 0){
					entropy = 0.0;
				}
				else if(number[m] != 0){
					for(int n = 0;n < 2;n++){
						temp_ratio = (double)yes_no[m][n] / (double)number[m];
						if(temp_ratio == 0){
							temp_item = 0;
						}
						else if(temp_ratio != 0){
							temp_item = temp_ratio * log(temp_ratio) / log(2);
						}
						//entropy = entropy - temp_ratio * log(temp_ratio) / log(2);//it is wrong when temp_ratio == 0!
						entropy = entropy - temp_item;
					}
					
				}
				total_entropy = total_entropy + (double)number[m] / (dimension1_size - 1) * entropy;
			}
		}
		else if(node -> transactions_subset[0][j] == 1 || node -> transactions_subset[0][j] == 2){
			for(int i = 1;i < dimension1_size;i++){
				if(node -> transactions_subset[i][j] == 0){
					number[0]++;
					if(node -> transactions_subset[i][dimension2_size - 1] == 0){
						yes_no[0][0]++;
					}
					else if(node -> transactions_subset[i][dimension2_size - 1] == 1){
						yes_no[0][1]++;
					}
				}
				else if(node -> transactions_subset[i][j] == 1){
					number[1]++;
					if(node -> transactions_subset[i][dimension2_size - 1] == 0){
						yes_no[1][0]++;
					}
					else if(node -> transactions_subset[i][dimension2_size - 1] == 1){
						yes_no[1][1]++;
					}
				}
			}

			for(int m = 0;m < 2;m++){
				entropy = 0.0;

				if(number[m] == 0){
					entropy = 0.0;
				}
				else if(number[m] != 0){
					for(int n = 0;n < 2;n++){
						temp_ratio = (double)yes_no[m][n] / (double)number[m];
						if(temp_ratio == 0){
							temp_item = 0;
						}
						else if(temp_ratio != 0){
							temp_item = temp_ratio * log(temp_ratio) / log(2);
						}
						entropy = entropy - temp_item;
					}
				}
				total_entropy = total_entropy + (double)number[m] / (dimension1_size - 1) * entropy;
			}
		}

		if(total_entropy < min_entropy){
			min_entropy = total_entropy;
			temp_target = node -> transactions_subset[0][j];
		}
	}
	return(temp_target);
}

bool all_the_same(Node* node){
	bool same = true;
	int dimension1_size = node -> transactions_subset.size();
	int dimension2_size = node -> transactions_subset[0].size();
	int first_value = node -> transactions_subset[1][dimension2_size - 1];

	for(int i = 2;i < dimension1_size;i++){
		if(node -> transactions_subset[i][dimension2_size - 1] == first_value){
			same = same && true;
		}
		else if(node -> transactions_subset[i][dimension2_size - 1] != first_value){
			same = false;
			break;
		}
	}
	
	return(same);
}

void print(Node *node){
	int dimension1_size;
	int dimension2_size;
	string determination;
	List list;
	Node* current_node;

	in_list(list,node);

	while(!is_empty(list)){
		current_node = out_list(list);
		dimension1_size = current_node -> transactions_subset.size();
		dimension2_size = current_node -> transactions_subset[0].size();

		if(is_leaf(current_node)){
			determination = get_determination(current_node);
			cout<<"leaf:"<<current_node<<endl;
			cout<<"    parent:"<<current_node -> parent<<endl;
			cout<<"    determination:"<<determination<<endl;
		}
		else if(!is_leaf(current_node)){
			cout<<"intermediate node:"<<current_node<<endl;
			cout<<"    parent:"<<current_node -> parent<<endl;
			cout<<"    lChild:"<<current_node -> lChild<<endl;
			cout<<"    mChild:"<<current_node -> mChild<<endl;
			cout<<"    rChild:"<<current_node -> rChild<<endl;
			cout<<"    target:"<<current_node -> target<<endl;
		}

		for(int i = 0;i < dimension1_size;i++){
			cout<<"    ";
			for(int j = 0;j < dimension2_size;j++){
				cout<<current_node -> transactions_subset[i][j]<<" ";
			}
			cout<<endl;
		}
		cout<<endl;

		if(current_node -> lChild != NULL){
			in_list(list,current_node -> lChild);
		}

		if(current_node -> mChild != NULL){
			in_list(list,current_node -> mChild);
		}

		if(current_node -> rChild != NULL){
			in_list(list,current_node -> rChild);
		}
	}
}

void in_list(List& list,Node* node){
	list.buffer[list.tail] = node;//copy the pointer value!
	list.tail++;
}

Node* out_list(List& list){
	Node* temp_node = new Node();
	
	temp_node = list.buffer[list.head];
	list.head++;

	return(temp_node);
}

bool is_empty(List list){
	bool judge = false;

	if(list.head >= list.tail){
		judge = true;
	}
	else if(list.head < list.tail){
		judge = false;
	}

	return(judge);
}

bool is_leaf(Node* node){
	bool judge = false;

	if((node -> lChild != NULL) || (node -> mChild != NULL) || (node -> rChild != NULL)){
		judge = false;
	}
	else if((node -> lChild == NULL) && (node -> mChild == NULL) && (node -> rChild == NULL)){
		judge = true;
	}

	return(judge);
}

string get_determination(Node* node){
	string determination;
	int temp_class = 0;
	int dimension2_size = node -> transactions_subset[0].size();

	temp_class = node -> transactions_subset[1][dimension2_size - 1];
	if(temp_class == 0){
		determination = "no";
	}
	else if(temp_class == 1){
		determination = "yes";
	}
	else{
		determination = "wrong!";
	}

	return(determination);
}
