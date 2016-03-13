/*
 * Hello.cpp
 *
 *  Created on: 28-Feb-2016
 *      Author: swarnatonse
 */

#include <iostream>
using namespace std;

enum{RED, BLACK};

struct node{
	int color;
	struct {int ID, count;}event;
	struct node* left;
	struct node* right;
	struct node* p;
};
typedef node* Node;

void RB_Insert(Node);
void RB_Insert_Fixup(Node);
void RB_Delete(Node);
void RB_Delete_Fixup(Node);
void display(Node);
void Increase(int);
void Reduce(int);
int InRange(int, int);
void Next(int);
void Previous(int);
void Count(int);
void RB_Transplant(Node, Node);
Node TreeSearch(int);
Node TreeMinimum(Node);
Node TreeMaximum(Node);
Node Successor(Node);
Node Predecessor(Node);
void sumRange(Node,int,int,int&);

Node T;
Node Tnil;

int main() {
   Tnil = new node;
   Tnil->color = BLACK;
   Tnil->event.ID = 0; Tnil->event.count = 0;
   T = Tnil;
   int sum = 0;
   int ID = 1;
   do{
	   cout<<"Enter ID or 0: ";
	   cin>>ID;
	   if(ID != 0){
	   Increase(ID);}
	   display(T);
   }while(ID != 0);
   do{
	   	   cout<<"Enter ID or 0: ";
      	   cin>>ID;
      	   if(ID != 0){
      		   Count(ID);
      		   Previous(ID);
      		   Next(ID);
      	   }
         }while(ID != 0);
   return 0;
}
void Increase(int ID){
	Node new_node = new node;
	new_node->event.ID = ID;
	RB_Insert(new_node);
}
void Reduce(int ID){
	Node x = TreeSearch(ID);
	if(x != Tnil){
		if(x->event.count > 1){
			x->event.count--;
		}
		else{
			RB_Delete(x);
		}
	}
}
void Count(int ID){
	Node x = TreeSearch(ID);
	cout<<"Count of ID "<<ID<<"is ";
	cout<<x->event.count<<endl;
}
void Next(int ID){
	Node x = T;
	Node y = 0;
	cout<<"NEXT ID"<<endl;
	while( x != Tnil ){
		y = x;
		if(x->event.ID == ID){
			cout<<Successor(x)->event.ID<<" "<<Successor(x)->event.count<<endl;
			return;
		}
		else if((x->event.ID < ID && x->p->event.ID > ID)){
			cout<<Successor(x)->event.ID<<" "<<Successor(x)->event.count<<endl;
			return;
		}
		else if((x->event.ID > ID && x->p->event.ID < ID) && x->p != Tnil){
			cout<<Successor(x->p)->event.ID<<" "<<Successor(x->p)->event.count<<endl;
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
	cout<<x->event.ID<<" "<<x->event.count<<endl;
}
void Previous(int ID){
	Node x = T;
	Node y = 0;
	cout<<"PREVIOUS ID"<<endl;
	while( x != Tnil ){
		y = x;
		if(x->event.ID == ID){
			cout<<Predecessor(x)->event.ID<<" "<<Predecessor(x)->event.count<<endl;
			return;
		}
		else if((x->event.ID < ID && x->p->event.ID > ID)){
			cout<<Predecessor(x->p)->event.ID<<" "<<Predecessor(x->p)->event.count<<endl;
			return;
		}
		else if((x->event.ID > ID && x->p->event.ID < ID) && x->p != Tnil){
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
	cout<<x->event.ID<<" "<<x->event.count<<endl;
}
int InRange(int ID1, int ID2){
	int sum = 0;
	sumRange(T, ID1, ID2, sum);
	return sum;
}
Node Successor(Node x){
	Node y = 0;
	if(x->right != Tnil){
		return TreeMinimum(x->right);
	}
	y = x->p;
	while(y != Tnil && x == y->right){
		cout<<"This is happening"<<endl;
		x = y;
		y = y->p;
	}
	return y;
}
Node Predecessor(Node x){
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
void sumRange(Node x, int ID1, int ID2, int& sum)
{
   if(x == Tnil) return;
   if ( ID1 < x->event.ID ){
     sumRange(x->left, ID1, ID2, sum);
   }
   if ( ID1 <= x->event.ID && ID2 >= x->event.ID ){
	  sum = sum+x->event.count;
   }
   if ( ID2 > x->event.ID )
     sumRange(x->right, ID1, ID2, sum);
}

void display(Node x){
	if(x == Tnil) return;
	cout<<x->event.ID<<x->event.count<<x->color<<" ";
	display(x->left);
	display(x->right);
}
void LeftRotate(Node x){
	cout<<"Entered Left Rotate!"<<endl;
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

void RightRotate(Node x){
	cout<<"Entered Right Rotate!"<<endl;
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

void RB_Insert(Node new_node){
	Node y = Tnil;
	Node x = T;
	while(x != Tnil){
		y = x;
		if(new_node->event.ID < x->event.ID){
			x = x->left;
		}
		else if(new_node->event.ID > x->event.ID){
			x = x->right;
		}
		else{
			x->event.count++;
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
	new_node->event.count = 1;
	RB_Insert_Fixup(new_node);
}

void RB_Insert_Fixup(Node z){
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

void RB_Transplant(Node u, Node v){
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

Node TreeMinimum(Node x){
	while(x->left != Tnil){
		x = x->left;
	}
	cout<<"Found minimum"<<endl;
	return x;
}
Node TreeMaximum(Node x){
	while(x->right != Tnil){
		x = x->right;
	}
	return x;
}

Node TreeSearch(int ID){
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

void RB_Delete(Node z){
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

void RB_Delete_Fixup(Node x){
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









