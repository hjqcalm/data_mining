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
	//vector<Node*> buffer(100);//it is wrong!
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

		/*
		for(int i =0;i < 100;i++){
			buffer.push_back(new Node());
		}
		*/

		/*
		//it is wrong when the beginning!
		for(int i = 0;i < 100;i++){
			buffer[i] = new Node();
		}
		*/
	}
}List;

void split_transactions(Node* node);
void copy_vector_except(vector<vector<int> >& first,vector<vector<int> > second,int exception,int value);
void copy_vector(vector<vector<int> >& first,vector<vector<int> > second);
int getTarget(Node* node);
bool all_the_same(Node* node);
void print(Node* node);
void in_list(List& list,Node* node);
//void copy_node(Node* first,Node* second);
Node* out_list(List& list);
bool is_empty(List list);
bool is_leaf(Node* node);
string get_determination(Node* node);

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

/**/
	for(int i = 0;i < NUMBER_OF_TRANSACTIONS + 1;i++){
		for(int j = 0;j < NUMBER_OF_ATTRIBUTES + 1;j++){
			cout<<root -> transactions_subset[i][j]<<" ";
		}
		cout<<endl;
	}	
	cout<<endl;
/**/

	//create a determination tree!
	split_transactions(root);

	//only test getTarget():
	//cout<<"target1:"<<getTarget(root)<<endl;

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
/**/		cout<<"size:"<<dimension1_size<<" "<<dimension2_size<<"|interept:the pointer is null!"<<endl<<endl;
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
			//break;//it is wrong!
		}
	}
	//temp_vector.push_back(-1);//it is wrong! because the size of transactions_subset[0] should decrease!

	//I should use node -> lChild -> transactions_subset.push_back(temp_vector);and so on
	//node->transactions_subset.push_back(temp_vector);//it is wrong!

	if(temp_target == 0 || temp_target == 3){
		//cout<<"hjq:"<<endl;
		node -> lChild = new Node();
		node -> mChild = new Node();
		node -> rChild = new Node();
		node -> lValue = 0;
		node -> mValue = 1;
		node -> rValue = 2;
		node -> lChild -> parent = node;
		node -> mChild -> parent = node;
		node -> rChild -> parent = node;

//it mainly tests the first layer of the determination tree!
		//cout<<"hjq0:"<<node -> lChild -> transactions_subset.size()<<endl;
		//cout<<"hjq1:"<<node -> mChild -> transactions_subset.size()<<endl;
		//cout<<"hjq2:"<<node -> rChild -> transactions_subset.size()<<endl;
		//cout<<node -> lChild -> transactions_subset[0].size()<<endl;//there will be a fault:segmentation fault!

		node -> lChild -> transactions_subset.push_back(temp_vector);
		node -> mChild -> transactions_subset.push_back(temp_vector);
		node -> rChild -> transactions_subset.push_back(temp_vector);
		//cout<<"hjq0:"<<node -> lChild -> transactions_subset.size()<<" "<<node -> lChild -> transactions_subset[0].size()<<endl;
		//cout<<"hjq1:"<<node -> mChild -> transactions_subset.size()<<" "<<node -> mChild -> transactions_subset[0].size()<<endl;
		//cout<<"hjq2:"<<node -> rChild -> transactions_subset.size()<<" "<<node -> rChild -> transactions_subset[0].size()<<endl;
/*
		for(int p = 0;p < 1;p++){
			for(int q = 0;q < node -> lChild -> transactions_subset[0].size();q++){
				cout<<node -> lChild -> transactions_subset[0][q]<<" ";
				cout<<node -> mChild -> transactions_subset[0][q]<<" ";
				cout<<node -> rChild -> transactions_subset[0][q]<<" ";
			}
			cout<<endl;
		}
*/
/*
//it is wrong!
		for(int i = 1;i < dimension1_size;i++){
			if(node -> transactions_subset[i][temp_index] == 0){
				copy_vector_except(node -> lChild -> transactions_subset,node -> transactions_subset,temp_index,0);
			}
			else if(node -> transactions_subset[i][temp_index] == 1){
				copy_vector_except(node -> mChild -> transactions_subset,node -> transactions_subset,temp_index,1);
			}
			else if(node -> transactions_subset[i][temp_index] == 2){
				copy_vector_except(node -> rChild -> transactions_subset,node -> transactions_subset,temp_index,2);
			}
		}
*/
		copy_vector_except(node -> lChild -> transactions_subset,node -> transactions_subset,temp_index,0);
		copy_vector_except(node -> mChild -> transactions_subset,node -> transactions_subset,temp_index,1);
		copy_vector_except(node -> rChild -> transactions_subset,node -> transactions_subset,temp_index,2);
		
	}
	else if(temp_target == 1 || temp_target == 2){
		//cout<<"hjq:"<<endl;
		node -> lChild = new Node();
		node -> rChild = new Node();
		node -> lValue = 0;
		node -> rValue = 1;
		node -> lChild -> parent = node;
		node -> rChild -> parent = node;

		node -> lChild -> transactions_subset.push_back(temp_vector);
		node -> rChild -> transactions_subset.push_back(temp_vector);
/*
//it is wrong!
		for(int i = 1;i < dimension1_size;i++){
			if(node -> transactions_subset[i][temp_index] == 0){
				copy_vector_except(node -> lChild -> transactions_subset,node -> transactions_subset,temp_index,0);
			}
			else if(node -> transactions_subset[i][temp_index] == 1){
				copy_vector_except(node -> rChild -> transactions_subset,node -> transactions_subset,temp_index,1);
			}
		}
*/
		copy_vector_except(node -> lChild -> transactions_subset,node -> transactions_subset,temp_index,0);
		copy_vector_except(node -> rChild -> transactions_subset,node -> transactions_subset,temp_index,1);
	}

/*
//it mainly test the second layer of the determination tree(the child layer of the root node)!
	cout<<"hjq0:"<<node -> lChild -> transactions_subset.size()<<" "<<node -> lChild -> transactions_subset[0].size()<<endl;
	for(int p = 0;p < node -> lChild -> transactions_subset.size();p++){
		for(int q = 0;q < node -> lChild -> transactions_subset[p].size();q++){
			cout<<node -> lChild -> transactions_subset[p][q]<<" ";
		}
		cout<<endl;
	}

	cout<<"hjq1:"<<node -> mChild -> transactions_subset.size()<<" "<<node -> mChild -> transactions_subset[0].size()<<endl;
	for(int p = 0;p < node -> mChild -> transactions_subset.size();p++){
		for(int q = 0;q < node -> mChild -> transactions_subset[p].size();q++){
			cout<<node -> mChild -> transactions_subset[p][q]<<" ";
		}
		cout<<endl;
	}

	cout<<"hjq2:"<<node -> rChild -> transactions_subset.size()<<" "<<node -> rChild -> transactions_subset[0].size()<<endl;
	for(int p = 0;p < node -> rChild -> transactions_subset.size();p++){
		for(int q = 0;q < node -> rChild -> transactions_subset[p].size();q++){
			cout<<node -> rChild -> transactions_subset[p][q]<<" ";
		}
		cout<<endl;
	}
*/
	
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
	//cout<<second.size()<<" "<<second[0].size()<<endl;
	
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

	/**/cout<<"every_node|size:"<<dimension1_size<<" "<<dimension2_size<<endl;

	if(dimension2_size == 1 || all_the_same(node)){
		temp_target = -1;
		/**/cout<<"size:"<<dimension1_size<<" "<<dimension2_size<<"|interept:the same labels!"<<endl<<endl;
		return(temp_target);
	}

	for(int j = 0;j < dimension2_size - 1;j++){
		total_entropy = 0.0;
		entropy = 0.0;
		temp_ratio = 1.0;
		//min_entropy = MIN_ENTROPY;//it is wrong when find the minimum entropy!

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
/**/
			cout<<"attritubtes|index = "<<j<<";target = "<<node -> transactions_subset[0][j]<<endl;
			for(int m = 0;m < 3;m++){
				cout<<"when m = "<<m<<":";
				for(int n = 0;n < 2;n++){
					cout<<yes_no[m][n]<<" ";
				}
				cout<<endl;
			}
			cout<<"total number:";
			for(int m = 0;m < 3;m++){
				cout<<number[m]<<" ";
			}
			cout<<endl;
/**/
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
				/**/cout<<"entropy when m = "<<m<<":"<<entropy<<endl;
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
/**/
			cout<<"attributes|index = "<<j<<";target = "<<node -> transactions_subset[0][j]<<endl;
			for(int m = 0;m < 2;m++){
				cout<<"when m = "<<m<<":";
				for(int n = 0;n < 2;n++){
					cout<<yes_no[m][n]<<" ";
				}
				cout<<endl;
			}
			cout<<"total number:";
			for(int m = 0;m < 2;m++){
				cout<<number[m]<<" ";
			}
			cout<<endl;
/**/
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
						//entropy = entropy - temp_ratio * log(temp_ratio) / log(2);//the same reason!
						entropy = entropy - temp_item;
					}
				}
				/**/cout<<"entropy when m = "<<m<<":"<<entropy<<endl;
				total_entropy = total_entropy + (double)number[m] / (dimension1_size - 1) * entropy;
			}
		}

		/**/cout<<"when (index)j = "<<j<<";target = "<<node -> transactions_subset[0][j]<<":"<<endl;
		/**/cout<<"hjq_test:"<<"min_entropy="<<min_entropy<<"|"<<"total_entropy="<<total_entropy<<endl;
		if(total_entropy < min_entropy){
			min_entropy = total_entropy;
			temp_target = node -> transactions_subset[0][j];
		}
		/**/cout<<"min_entropy:"<<min_entropy<<endl;
		/**/cout<<"temp_target:"<<temp_target<<endl<<endl;
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
/*
	//test buffer!
	List list;
	cout<<"size_buffer:"<<list.buffer.size()<<endl;
	//cout<<"buffer[0]:"<<list.buffer[0]<<endl;
*/

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
	//copy_node(list.buffer[list.tail],node);//copy the object value which is pointed by pointer!
	list.buffer[list.tail] = node;//copy the pointer value!
	list.tail++;
}

//we must not copy object value instead of pointer!
//because when we copy object value, the object that is pointed by pointer has been changed twice,
//and when we copy pointer,the object that is pointed by pointer has not been changed.
//it means that when taking operations 'in_list()' and 'out_list()', all the pointers point to the same object!
/*
void copy_node(Node* first,Node* second){
	vector<int> temp_vector;
	int dimension1_size = second -> transactions_subset.size();
	int dimension2_size = second -> transactions_subset[0].size();

	for(int i = 0;i < dimension1_size;i++){
		temp_vector.clear();
		for(int j = 0;j < dimension2_size;j++){
			temp_vector.push_back(second -> transactions_subset[i][j]);
		}
		first -> transactions_subset.push_back(temp_vector);
	}

	first -> parent = second -> parent;
	first -> lChild = second -> lChild;
	first -> mChild = second -> mChild;
	first -> rChild = second -> rChild;
	first -> lValue = second -> lValue;
	first -> mValue = second -> mValue;
	first -> rValue = second -> rValue;
	first -> target = second -> target;
}
*/

Node* out_list(List& list){
	Node* temp_node = new Node();
	
	//copy_node(temp_node,list.buffer[list.head]);
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

/*
void print(Node* node){
	int dimension1_size = -1;
	int dimension2_size = -1;
	
	dimension1_size = node -> transactions_subset.size();
	dimension2_size = node -> transactions_subset[0].size();
	cout<<"node -> target:"<<node -> target<<endl;
	cout<<dimension1_size<<" "<<dimension2_size<<endl;
	for(int i = 0;i < dimension1_size;i++){
		for(int j = 0;j < dimension2_size;j++){
			cout<<node -> transactions_subset[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"over1"<<endl<<endl;
	
	dimension1_size = node -> lChild -> transactions_subset.size();
	dimension2_size = node -> lChild -> transactions_subset[0].size();
	cout<<"node -> lChild -> target:"<<node -> lChild -> target<<endl;
	cout<<dimension1_size<<" "<<dimension2_size<<endl;
	for(int i = 0;i < dimension1_size;i++){
		for(int j = 0;j < dimension2_size;j++){
			cout<<node -> lChild -> transactions_subset[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"over2"<<endl<<endl;

	dimension1_size = node -> mChild -> transactions_subset.size();
	dimension2_size = node -> mChild -> transactions_subset[0].size();
	cout<<"node -> mChild -> target:"<<node -> mChild -> target<<endl;
	cout<<dimension1_size<<" "<<dimension2_size<<endl;
	for(int i = 0;i < dimension1_size;i++){
		for(int j = 0;j < dimension2_size;j++){
			cout<<node -> mChild -> transactions_subset[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"over3"<<endl<<endl;

	dimension1_size = node -> rChild -> transactions_subset.size();
	dimension2_size = node -> rChild -> transactions_subset[0].size();
	cout<<"node -> rChild -> target:"<<node -> rChild ->target<<endl;
	cout<<dimension1_size<<" "<<dimension2_size<<endl;
	for(int i = 0;i < dimension1_size;i++){
		for(int j = 0;j < dimension2_size;j++){
			cout<<node -> rChild -> transactions_subset[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"over4"<<endl<<endl;
}
*/
