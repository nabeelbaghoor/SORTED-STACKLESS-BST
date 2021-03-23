#include<iostream>
#include<vector>
using namespace std;
template<class T>
class SortedBST;

template<class T>
class Node {
	T data;
	Node<T>* left;
	Node<T>* right;
	Node<T>* nextInOrder;
public:
	Node(T d)
	{
		data = d;
		left = right = nullptr;
		nextInOrder = nullptr;
	}
	Node(T d,Node<T>*next)
		:nextInOrder(next)
	{
		data = d;
		left = right = nullptr;
	}
	bool isLeafNode() const
	{
		return !(left || right);
	}
	int geth(Node<T>* p)
	{
		if (p != nullptr)
			return p->height;
		else
			return -1;
	}
	friend class SortedBST<T>;
};
template<class T>
class SortedBST {
	Node<T>* root;
	bool IsSuccessor;
public:
	SortedBST(bool is_successor = true)
		:IsSuccessor(is_successor)
	{
		root = nullptr;
	}
	void deepCopy(Node<T>*& r)
	{
		if (r)
		{
			Insert(r->data);
			deepCopy(r->left);
			deepCopy(r->right);
		}
	}
	SortedBST(SortedBST& obj)
	{
		root = nullptr;
		IsSuccessor = obj.IsSuccessor;
		deepCopy(obj.root);
	}
	SortedBST& operator=(SortedBST& obj)
	{
		Node<T>* head;
		Node<T>* curr;
		if (IsSuccessor)
			head = GetMin();
		else
			head = GetMax();
		while (head)
		{
			curr = head->nextInOrder;
			delete  head;
			head = curr;
		}
		root = nullptr;
		IsSuccessor = obj.IsSuccessor;
		deepCopy(obj.root);
		return *this;
	}
	bool Insert(T d)
	{
		if (!root)
			root = new Node<T>(d);
		else
		{
			bool flag;//0 for left,1 for right
			Node<T>* r = root;
			Node<T>* rightPtr = nullptr;
			Node<T>* leftPtr = nullptr;
			while (r)
			{
				if (d == r->data)
					return false;
				if (d < r->data)
				{
					leftPtr = r;//it Updates When We MoveLeft from RootPtr
					if (r->left == nullptr)
					{
						flag = 0;
						break;
					}
					r = r->left;
				}
				else
				{
					rightPtr = r;//it Updates When We MoveRight from RootPtr
					if (r->right == nullptr)
					{
						flag = 1;
						break;
					}
					r = r->right;
				}

			}
			if (IsSuccessor)
			{
				if (!flag)//0 for left case
				{
					r->left = new Node<T>(d, r);//mb ok
					if (rightPtr)
						rightPtr->nextInOrder = r->left;
				}
				else//1 for right case
				{
					r->right = new Node<T>(d, r->nextInOrder);
					r->nextInOrder = r->right;
					//other way is to just implement the same logic as in left case
				}
			}
			else
			{
				if (!flag)//0 for left case
				{
					r->left = new Node<T>(d, r->nextInOrder);
					r->nextInOrder = r->left;
					//other way is to just implement the same logic as in right case
				}
				else//1 for right case
				{
					r->right = new Node<T>(d,r);//mb ok
					if (leftPtr)
						leftPtr->nextInOrder = r->right;
				}
			}
		}
		return true;
	}
	int max(int a, int b)
	{
		if (a > b)
			return a;
		return b;
	}
	int getHeight(Node<T>* r)
	{
		if (r == nullptr)
			return -1;
		return 1 + max(getHeight(r->left), getHeight(r->right));
	}
	T findMax(Node<T>*& t)//& is used to make more efficient
	{
		while (t->right)
			t = t->right;
		return t->data;
	}
	void RecursiveRemove(T d, Node<T>*& r)
	{
		if (r == nullptr)
			return;
		if (d < r->data)
			RecursiveRemove(d, r->left);
		else if (d > r->data)
			RecursiveRemove(d, r->right);
		else
		{
			if (r->left == nullptr && r->right == nullptr)
			{
				delete r;
				r = nullptr;
			}
			else if (r->left && r->right)
			{
				r->data = findMax(r->left);
				RecursiveRemove(r->data, r->left);
			}
			else
			{
				Node<T>* t;
				if (r->left)
					t = r->left;
				else
					t=r->right;
				delete r;
				r = t;
			}

		}

	}
	void RecursiveSetOrder(T d, Node<T>*curr)
	{
		if (curr && curr->nextInOrder)
		{
			if (curr->nextInOrder->data == d)
			{
				curr->nextInOrder = curr->nextInOrder->nextInOrder;
				return;
			}
			RecursiveSetOrder(d,curr->nextInOrder);
		}
	}
	bool Remove(T d)	//not yet bool/////////////////////////////////////////////////////////
	{
		if (Search(d))
		{
			Node<T>* head;
			if (IsSuccessor)
				head = GetMin();
			else
				head = GetMax();
			//to handle nextInorder
			RecursiveSetOrder(d, head);
			//to Remove
			RecursiveRemove(d, root);
			return true;
		}
		return false;
	}
	bool Search(T d)
	{
		Node<T>* curr;
		if (IsSuccessor)
			curr = GetMin();
		else
			curr = GetMax();
		while (curr)
		{
			if (curr->data == d)
				return true;
			curr = curr->nextInOrder;
		}
		return false;
	}
	Node<T>* GetMin()
	{
		Node<T>* r = root;
		while (r && r->left)
			r = r->left;
		return r;
	}
	Node<T>* GetMax()
	{
		Node<T>* r = root;
		while (r && r->right)
			r = r->right;
		return r;
	}
	void Print()
	{
		Node<T>* curr;
		if (IsSuccessor)
			curr = GetMin();
		else
			curr = GetMax();
		while (curr)
		{
			cout << curr->data << endl;
			curr = curr->nextInOrder;
		}	
	}
	void Print(T low,T high)
	{
		Node<T>* curr;
		if (IsSuccessor)
			curr = GetMin();
		else
			curr = GetMax();
		while(curr && curr->data!=low && curr->data != high)
			curr = curr->nextInOrder;
		
		if (curr && Search(low) && Search(high))
		{
			if (high == low)
				cout << curr->data << endl;
			else
			{
				while (curr)
				{
					cout << curr->data << endl;
					curr = curr->nextInOrder;
					if (curr && (curr->data == high || curr->data == low))
					{
						cout << curr->data << endl;
						break;
					}
				}
			}
		}
		else
			cout << "Not in Range!!!\n";
	}
	void RecursiveGetLevel(Node<T>*r,T d,int&level)
	{
		if (r == nullptr)
			return;
		if (r->data == d)
			return;
		else if (d < r->data)
			return RecursiveGetLevel(r->left, d,++level);
		else
			return RecursiveGetLevel(r->right, d,++level);
	}
	int getLevel(T d)
	{
		int level=0;
		RecursiveGetLevel(root, d,level);
		return level;
	}
	void RecursiveIsLeafBalanced(int&Level,Node<T>* r,bool&flag)
	{
		if (flag && r)
		{
			RecursiveIsLeafBalanced(Level,r->left,flag);
			RecursiveIsLeafBalanced(Level,r->right,flag);
			if (r->isLeafNode())
			{
				flag = (Level == getLevel(r->data));
				//cout << "\nFirst Leaf:" << getLevel(r->data) << endl<<"flag::\t"<<flag<<endl;
			}
		}
	}
	bool IsLeafBalanced()
	{
		if (root)
		{
			int Level = getLevel(GetMin()->data);
			//cout << "Level\t:" << Level << endl;
			bool flag = 1;
			RecursiveIsLeafBalanced(Level, root, flag);
			return flag;
		}
		return true;
	}
	void RecursiveReverse(Node<T>*curr)
	{
		if (curr->nextInOrder)
		{
			RecursiveReverse(curr->nextInOrder);
			curr->nextInOrder->nextInOrder = curr;
		}
	}
	void ReveseOrder()
	{
		Node<T>* head;
		if(IsSuccessor)
			head = GetMin();
		else
			head = GetMax();
		RecursiveReverse(head);
		head->nextInOrder = nullptr;
		IsSuccessor = !IsSuccessor;
	}
	~SortedBST()
	{
		Node<T>* head;
		Node<T>* curr;
		if (IsSuccessor)
			head = GetMin();
		else
			head = GetMax();
		while (head)
		{
			curr = head->nextInOrder;
			delete  head;
			head = curr;
		}
		root = nullptr;
	}
};

int main()
{
	SortedBST<float>s(false);
	s.Insert(6);
	s.Insert(2);
	s.Insert(20);
	s.Insert(1);
	s.Insert(3);
	s.Insert(19);
	s.Insert(21);
	s.Insert(22);
	s.Insert(23);
	s.Insert(22.5);
	s.Insert(18);
	s.Insert(17);
	s.Insert(16);
	s.Insert(0.9);
	s.Insert(0.8);
	s.Insert(0.7);
	s.Insert(2.5);
	s.Insert(2.6);
	s.Insert(2.8);
	s.Insert(200.8);
	s.Insert(17.8);
	s.Insert(1.1);
	s.Insert(1.2);
	s.Insert(1.3);
	s.ReveseOrder();
	s.Print();
	SortedBST<float>p=s;
	p.Print();
	cout << "\nbool:\t" << s.IsLeafBalanced() << endl;
	cout << "\nbool:\t" << p.IsLeafBalanced() << endl;
	system("pause");
}
//done