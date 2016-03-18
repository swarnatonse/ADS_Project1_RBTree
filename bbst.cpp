/*
 * bbst.cpp
 *
 *  Created on: 28-Feb-2016
 *      Author: swarnatonse
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;

enum{RED, BLACK};

struct Event{
	int ID;
	int count;
};

struct node{
	int color;
	Event event;
	//int subtree_sum;
	struct node* left;
	struct node* right;
	struct node* p;
};
typedef node* Node;

class RBTree{
	Node T;
	Node Tnil;

public:
	RBTree(){
		Tnil = new node;
		Tnil->color = BLACK;
		Tnil->event.ID = 0; Tnil->event.count = 0;
		//Tnil->subtree_sum = 0;
		T = Tnil;
	}
	void Initialize(char[]);
	Node sortedArrayToBST(Event*, int, int, Node, int, int);
	void RB_Insert(Node);
	void RB_Insert_Fixup(Node);
	void RB_Delete(Node);
	void RB_Delete_Fixup(Node);
	void Increase(int,int);
	void Reduce(int,int);
	int InRange(int, int);
	void Next(int);
	void Previous(int);
	void Count(int);
	void display();
	void inorder(Node x);
	void RB_Transplant(Node, Node);
	Node TreeSearch(int);
	Node TreeMinimum(Node);
	Node TreeMaximum(Node);
	Node Successor(Node);
	Node Predecessor(Node);
	void sumRange(Node,int,int,int*);
	void LeftRotate(Node);
	void RightRotate(Node);
};

int main(int argc, char* argv[]) {
   string command;
   char ws;
   int ID;
   int count = -1;
   int quit_flag = 0;
   RBTree RB;
   RB.Initialize(argv[1]);
   do{
   cin.sync();
   cin>>command;
   if(command.compare("quit") == 0){
   	   quit_flag = 1;
   	   break;
    }
   else{
	   cin>>ID;
	   cin.get(ws);
	   if(ws != '\n'){ cin>>count; }
   }
   if(command.compare("increase") == 0){
	   RB.Increase(ID, count);
   }
   else if(command.compare("reduce") == 0){
	   RB.Reduce(ID,count);
   }
   else if(command.compare("count") == 0){
   	   RB.Count(ID);
   }
   else if(command.compare("inrange") == 0){
       cout<<"Total count is "<<RB.InRange(ID,count)<<endl;
   }
   else if(command.compare("next") == 0){
       RB.Next(ID);
   }
   else if(command.compare("previous") == 0){
       RB.Previous(ID);
   }
   else{
	   cout<<"Could not understand command!"<<endl;
   }
   }while(!quit_flag);
   return 0;
}

void RBTree::Initialize(char filename[]){
	ifstream read(filename);
	if(read.is_open()){
	int n;
	read >> n;
	Event* arr = new Event[n];
	for(int i = 0; i<n; i++){
		read >> arr[i].ID;
		read >> arr[i].count;
	}
	int h = ceil(log2(n+1));
	T = sortedArrayToBST(arr,0,n-1,Tnil,0,h);
	}
	else{
		cout<<"Error opening file!"<<endl;
	}
	//colorMeRed(T);
}
Node RBTree::sortedArrayToBST(Event* arr, int start, int end, Node parent,int curr_h,int max_h)
{
    /* Base Case */
    if (start > end){
    	if(curr_h == max_h)
    		parent->color = RED;
    	return Tnil;
    }

    int mid = (start + end)/2;
    //cout<<"Mid is "<<arr[mid].ID<<endl;
    Node N = new node;
    N->event = arr[mid];
    N->color = BLACK;
    N->right = Tnil;
    N->left = Tnil;

    N->p = parent;
    N->left =  sortedArrayToBST(arr, start, mid-1,N,curr_h+1,max_h);
    N->right = sortedArrayToBST(arr, mid+1, end,N,curr_h+1,max_h);

    return N;
}

void RBTree::Increase(int ID, int count){
	Node new_node = new node;
	new_node->event.ID = ID;
	new_node->event.count = count;
	RB_Insert(new_node);
}
void RBTree::Reduce(int ID, int count){
	Node x = TreeSearch(ID);
	if(x != Tnil){
		x->event.count -= count;
		cout<<"ID: "<<x->event.ID<<" count: "<<x->event.count<<endl;
		if(x->event.count <= 0){
			cout<<"Deleting Node!"<<endl;
			RB_Delete(x);
		}
	}
	else{
		cout<<"Node not found!"<<endl;
	}
}
void RBTree::Count(int ID){
	Node x = TreeSearch(ID);
	cout<<"Count of ID "<<ID<<"is ";
	cout<<x->event.count<<endl;
}
void RBTree::Next(int ID){
	Node x = T;
	Node y = 0;
	cout<<"NEXT ID"<<endl;
	while( x != Tnil ){
		y = x;
		if(x->event.ID == ID){
			cout<<Successor(x)->event.ID<<" "<<Successor(x)->event.count<<endl;
			return;
		}
		else{
			if(ID < x->event.ID){
				x = x->left;
			}
			else{
				x = x->right;
			}
		}
	}
	if(y->event.ID > ID){
		cout<<y->event.ID<<" "<<y->event.count<<endl;
		return;
	}
	cout<<Successor(y)->event.ID<<" "<<Successor(y)->event.count<<endl;
}
void RBTree::Previous(int ID){
	Node x = T;
	Node y = 0;
	cout<<"PREVIOUS ID"<<endl;
	while( x != Tnil ){
		y = x;
		if(x->event.ID == ID){
			cout<<Predecessor(x)->event.ID<<" "<<Predecessor(x)->event.count<<endl;
			return;
		}
		else{
			if(ID < x->event.ID){
				x = x->left;
			}
			else{
				x = x->right;
			}
		}
	}
	if(y->event.ID < ID){
		cout<<y->event.ID<<" "<<y->event.count<<endl;
		return;
	}
	cout<<Predecessor(y)->event.ID<<" "<<Predecessor(y)->event.count<<endl;
}
int RBTree::InRange(int ID1, int ID2){
	int sum = 0;
	sumRange(T, ID1, ID2, &sum);
	return sum;
}
Node RBTree::Successor(Node x){
	Node y = 0;
	if(x->right != Tnil){
		return TreeMinimum(x->right);
	}
	y = x->p;
	while(y != Tnil && x == y->right){
		//cout<<"This is happening"<<endl;
		x = y;
		y = y->p;
	}
	return y;
}
Node RBTree::Predecessor(Node x){
	Node y = 0;
	if(x->left != Tnil){
		return TreeMaximum(x->left);
	}
	y = x->p;
	while(y != Tnil && x == y->left){
		x = y;
		y = y->p;
	}
	return y;
}
void RBTree::sumRange(Node x, int ID1, int ID2, int* sum)
{
   if(x == Tnil) return;
   if ( ID1 < x->event.ID ){
     sumRange(x->left, ID1, ID2, sum);
   }
   if ( ID2 > x->event.ID ){
     sumRange(x->right, ID1, ID2, sum);
   }
   if ( ID1 <= x->event.ID && ID2 >= x->event.ID ){
   	   //cout<<x->event.count<<" ";
   	  *sum = *sum+x->event.count;
   }
}

void RBTree::LeftRotate(Node x){
	//cout<<"Entered Left Rotate!"<<endl;
	Node y = x->right;
	x->right = y->left;
	if(y->left != Tnil){
		y->left->p = x;
	}
	y->p = x->p;
	if(x->p == Tnil){
		T = y;
	}
	else if(x == x->p->left){
		x->p->left = y;
	}
	else{
		x->p->right = y;
	}
	y->left = x;
	x->p = y;
}

void RBTree::RightRotate(Node x){
	//cout<<"Entered Right Rotate!"<<endl;
	Node y = x->left;
	x->left = y->right;
	if(y->right != Tnil){
		y->right->p = x;
	}
	y->p = x->p;
	if(x->p == Tnil){
		T = y;
	}
	else if(x == x->p->right){
		x->p->right = y;
	}
	else{
		x->p->left = y;
	}
	y->right = x;
	x->p = y;
}

void RBTree::RB_Insert(Node new_node){
	Node y = Tnil;
	Node x = T;
	while(x != Tnil){
		y = x;
		//x->subtree_sum += new_node->event.count;
		if(new_node->event.ID < x->event.ID){
			x = x->left;
		}
		else if(new_node->event.ID > x->event.ID){
			x = x->right;
		}
		else{
			x->event.count+=new_node->event.count;
			cout<<"ID: "<<x->event.ID<<" count: "<<x->event.count<<endl;
			delete new_node;
			return;
		}
	}
	new_node->p = y;
	if( y == Tnil ){
		T = new_node;
	}
	else if(new_node->event.ID < y->event.ID){
		y->left = new_node;
	}
	else{
		y->right = new_node;
	}
	new_node->left = Tnil;
	new_node->right = Tnil;
	new_node->color = RED;
	cout<<"ID: "<<new_node->event.ID<<" count: "<<new_node->event.count<<endl;
	RB_Insert_Fixup(new_node);
}

void RBTree::RB_Insert_Fixup(Node z){
	Node y;
	while((z->p->color == RED)){
		if(z->p == z->p->p->left){
			y = z->p->p->right;
			if(y->color == RED){
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}
			else{
				if(z == z->p->right){
					z = z->p;
					LeftRotate(z);
				}
			z->p->color = BLACK;
			z->p->p->color = RED;
			RightRotate(z->p->p);
			}
		}
		else{
				y = z->p->p->left;
				if(y->color == RED){
					z->p->color = BLACK;
					y->color = BLACK;
					z->p->p->color = RED;
					z = z->p->p;
				}
				else{
					if(z == z->p->left){
					z = z->p;
					RightRotate(z);
					}
				z->p->color = BLACK;
				z->p->p->color = RED;
				LeftRotate(z->p->p);
				}
		}
	}
	T->color = BLACK;
}

void RBTree::RB_Transplant(Node u, Node v){
	if(u->p == Tnil){
		T = v;
	}
	else if(u == u->p->left){
		u->p->left = v;
	}
	else{
		u->p->right = v;
	}
	v->p = u->p;
}

Node RBTree::TreeMinimum(Node x){
	while(x->left != Tnil){
		x = x->left;
	}
	//cout<<"Found minimum"<<endl;
	return x;
}
Node RBTree::TreeMaximum(Node x){
	while(x->right != Tnil){
		x = x->right;
	}
	return x;
}

Node RBTree::TreeSearch(int ID){
	Node x = T;
	while(x != Tnil && x->event.ID != ID){
		if(ID < x->event.ID){
			x = x->left;
		}
		else{
			x = x->right;
		}
	}
	return x;
}

void RBTree::RB_Delete(Node z){
	Node y = z;
	Node x = 0;
	int y_org = y->color;
	if(z->left == Tnil){
		x = z->right;
		RB_Transplant(z, z->right);
	}
	else if(z->right == Tnil){
		x = z->left;
		RB_Transplant(z, z->left);
	}
	else{
		y = TreeMinimum(z->right);
		y_org = y->color;
		x = y->right;
		if(y->p == z){
			x->p = y;
		}
		else{
			RB_Transplant(y, y->right);
			y->right = z->right;
			y->right->p = y;
		}
		RB_Transplant(z,y);
		y->left = z->left;
		y->left->p = y;
		y->color = z->color;
	}
	if(y_org == BLACK){
		RB_Delete_Fixup(x);
	}
}

void RBTree::RB_Delete_Fixup(Node x){
	Node w = 0;
	while( x != T && x->color == BLACK){
		if(x == x->p->left){
			w = x->p->right;
			if(w->color == RED){
				w->color = BLACK;
				x->p->color = RED;
				LeftRotate(x->p);
				w = x->p->right;
			}
			if(w->left->color == BLACK && w->right->color == BLACK){
				w->color = RED;
				x = x->p;
			}
			else{
				if(w->right->color == BLACK){
					w->left->color = BLACK;
					w->color = RED;
					RightRotate(w);
					w = x->p->right;
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->right->color = BLACK;
				LeftRotate(x->p);
				x=T;
			}
		}
		else{
			w = x->p->left;
			if(w->color == RED){
				w->color = BLACK;
				x->p->color = RED;
				RightRotate(x->p);
				w = x->p->left;
			}
			if(w->right->color == BLACK && w->left->color == BLACK){
				w->color = RED;
				x = x->p;
			}
			else{
				if(w->left->color == BLACK){
					w->right->color = BLACK;
					w->color = RED;
					LeftRotate(w);
					w = x->p->left;
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->left->color = BLACK;
				RightRotate(x->p);
				x=T;
			}
		}
	}
	x->color = BLACK;
}

void RBTree::display(){
	inorder(T);
}

void RBTree::inorder(Node x){
	if(x == Tnil){ cout<<"Tnil "; return; }
	cout<<"["<<x->event.ID<<" "<<x->event.count<<" "<<x->color<<"] ";
	inorder(x->left);
	inorder(x->right);
}









