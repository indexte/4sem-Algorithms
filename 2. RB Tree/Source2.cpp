#include <iostream>
#include <vector>

using namespace std;

// �������-����� ������
template <class T>
class RedBlackTree {
	// ��������� �����
	// value - ��������
	// p1,p2 - ��������� �� ��� �� ����� ��������
	// red   - ���� (true - ���� ��������)
	// childCount - ������� ������� �������
	struct Node {
		Node *p1,*p2;
		T& value;
		bool red;
		int childCount;
		
		Node(T& value) : value(value) {
			this->p1=this->p2=0;
			this->red=true;
			this->childCount = 0;
		}
	};
	
	Node *treeRoot;	// �����
	int size; // �����
	
	// ��������� �������
	void delItem(Node *node)
	{
		delete node;
	}
	
	// ���� ������ (���������� �������)
	void clear(Node *node)
	{
		if(!node) return;
		clear(node->p1);
		clear(node->p2);
		delItem(node);
	}
	
	// ��������� ������, ���������� �������
	// \param node �����
	// \param depth �������
	// \param dir "*" - �����, "-" - ��� ��������, "+" - ����� ��������
	// \code show(root, 0, '*'); \endcode
	void show(Node *node, int depth, char dir)
	{
		int n;
		if(!node) return;
		for(n=0; n<depth; n++) putchar(' ');
		printf("%c[%s] (%s) ch=%d\n", dir,
			((string)node->value).c_str(), node->red?"red":"black", node->childCount);
		show(node->p1,depth+2,'-');
		show(node->p2,depth+2,'+');
	}
	
	// ������� ������� ����� �����
	void updateChildren(Node* node) {
		if (!node) {
			return;
		}
		node->childCount = (node->p1 ? node->p1->childCount + 1 : 0) + 
			               (node->p2 ? node->p2->childCount + 1 : 0);
	}
	
	// ������� ����
	// \param index ������ �������
	// \result ���� ������� ������
	Node *rotate21(Node *node)
	{
		Node *p2 =node->p2;
		Node *p21=p2->p1;
		p2->p1=node;
		node->p2=p21;
		updateChildren(node);
		updateChildren(node->p2);
		updateChildren(p2->p1->p2);
		return p2;
	}
	
	// ������� ������
	// \param index ������ �������
	// \result ���� ������� ������
	Node *rotate12(Node *node)
	{
		Node *p1 =node->p1;
		Node *p12=p1->p2;
		p1->p2=node;
		node->p1=p12;
		updateChildren(node);
		updateChildren(node->p1);
		updateChildren(p1->p2->p1);
		return p1;
	}
	
	
	// ������� ������� � ������������� 
	void balanceInsert(Node **root)
	{
		Node *p1,*p2,*px1,*px2;
		Node *node=*root;
		if(node->red) return;
		p1=node->p1;
		p2=node->p2;
		if(p1 && p1->red) {
			px2=p1->p2;				// ������ �� ����� ��������� ������� �������
			if(px2 && px2->red) p1=node->p1=rotate21(p1);
			px1=p1->p1;
			if(px1 && px1->red) {
				node->red=true;
				p1->red=false;
				if(p2 && p2->red) {	// ���������� ������
					px1->red=true;
					p2->red=false;
					return;
				}
				*root=rotate12(node);
			}
		}
		// ��������� � ���� �������
		if(p2 && p2->red) {
			px1=p2->p1;				// ������ �� ����� ��������� ������� �������
			if(px1 && px1->red) p2=node->p2=rotate12(p2);
			px2=p2->p2;
			if(px2 && px2->red) {
				node->red=true;
				p2->red=false;
				if(p1 && p1->red) {	// ���������� ������
					px2->red=true;
					p1->red=false;
					return;
				}
				*root=rotate21(node);
			}
		}
		updateChildren(node->p1);
		updateChildren(node->p2);
		updateChildren(node);
	}
	
	// ��������� ������� � ������������� 1
	bool balanceRemove1(Node **root)
	{
		Node *node=*root;
		Node *p1=node->p1;
		Node *p2=node->p2;
		if(p1 && p1->red) {
			p1->red=false;return false;
		}
		if(p2 && p2->red) { // ������� 1
			node->red=true;
			p2->red=false;
			node=*root=rotate21(node);
			if(balanceRemove1(&node->p1)) node->p1->red=false;
			return false;
		}
	 	unsigned int mask=0;
		Node *p21=p2->p1;
		Node *p22=p2->p2;
		if(p21 && p21->red) mask|=1;
		if(p22 && p22->red) mask|=2;
		switch(mask)
		{
		case 0:		// ������� 2 - if((!p21 || !p21->red) && (!p22 || !p22->red))
			p2->red=true;
			return true;
		case 1:
		case 3:		// ������� 3 - if(p21 && p21->red)
			p2->red=true;
			p21->red=false;
			p2=node->p2=rotate12(p2);
			p22=p2->p2; 
		case 2:		// ������� 4 - if(p22 && p22->red)
			p2->red=node->red;
			p22->red=node->red=false;
			*root=rotate21(node);
		}
		return false;
	}
	
	// ��������� ������� � ������������� 1
	bool balanceRemove2(Node **root)
	{
		Node *node=*root;
		Node *p1=node->p1;
		Node *p2=node->p2;
		if(p2 && p2->red) {p2->red=false;return false;}
		if(p1 && p1->red) { // ������� 1
			node->red=true;	
			p1->red=false;
			node=*root=rotate12(node);
			if(balanceRemove2(&node->p2)) node->p2->red=false;
			return false;
		}
	 	unsigned int mask=0;
		Node *p11=p1->p1;
		Node *p12=p1->p2;
		if(p11 && p11->red) mask|=1;
		if(p12 && p12->red) mask|=2;
		switch(mask) {
		case 0:		// ������� 2 - if((!p12 || !p12->red) && (!p11 || !p11->red))
			p1->red=true;
			return true;
		case 2:
		case 3:		// ������� 3 - if(p12 && p12->red)
			p1->red=true;
			p12->red=false;
			p1=node->p1=rotate21(p1);
			p11=p1->p1;
		case 1:		// ������� 4 - if(p11 && p11->red)
			p1->red=node->red;
			p11->red=node->red=false;
			*root=rotate12(node);
		}
		return false;
	}
	
	
	// ���������� ������� �������
	// \result true ���� ��� �� �������� ��� ������������ � ����� ������� �� �������
	bool insert(T& value,Node **root)
	{
		Node *node=*root;
		if(!node) *root=new Node(value);
		else {
			(*root)->childCount++;
			if(value==node->value) return true; 
			if(insert(value,value<node->value?&node->p1:&node->p2)) return true;
			size++;
			balanceInsert(root);
		}
		return false;
	}
	
	
	// ����� �� �������� ������������ ����� ��������
	// \param root ����� ������
	// \param res �������, �� ��� ���������
	// \result true ���� ������� ������������
	bool getMin(Node **root,Node **res)
	{
		Node *node=*root;
		if(node->p1) {
			if(getMin(&node->p1,res)) return balanceRemove1(root);
		} else {
			*root=node->p2;
			*res=node;
			return !node->red;
		}
		return false;
	}
	
	
	// ���������� ������� ���������
	// \result true ���� ������� ������
	bool remove(Node **root,int value)
	{
		Node *t,*node=*root;
		if(!node) return false;
		if(node->value<value) {
			if(remove(&node->p2,value))	return balanceRemove2(root);
		} else if(node->value>value) {
			if(remove(&node->p1,value))	return balanceRemove1(root);
		} else {
			bool res;
			if(!node->p2) {
				*root=node->p1;
				res=!node->red;
				delItem(node);
				return res;
			}
			res=getMin(&node->p2,root);
			t=*root;
			t->red=node->red;
			t->p1=node->p1;
			t->p2=node->p2;
			delItem(node);
			if(res) res=balanceRemove2(root);
			return res;
		}
		return 0;
	}
	
	// ������ �-�� ������ (���������� �������)
	// \param root ����� ������
	// \param k ������� ����������
	T& statistic(Node* root, int k) {
		int lc = root->p1 ? root->p1->childCount + 1 : 0; 
		if (k == lc + 1) {
			return root->value;
		}
		if (k <= lc) {
			return statistic(root->p1, k);
		} else {
			return statistic(root->p2, k - lc - 1);
		}
	}
	
	public:
	
	RedBlackTree() {
		size = 0;
		treeRoot = 0;
	}
	
	// ��������� ������
	void show()
	{
		printf("[tree]\n");
		show(treeRoot,0,'*');
	}
	
	// ������� �����
	void insert(T& value)
	{
		insert(value,&treeRoot);
		if(treeRoot) treeRoot->red=false;
	}
	
	// ��������� �����
	void remove(int value)
	{
		remove(&treeRoot,value); 
	}
	
	// ���� ������
	void clear()
	{
		clear(treeRoot);
		treeRoot=0;
	}
	
	// ������ �-�� ������
	T& statistic(int k) {
		if (k > size) {
			throw "Statistic out of range!";
		}
		statistic(treeRoot, k);
	}
};

// ������� ��������
class CompanyDivision {
	string name; // �����
	vector <string> employees; // ����� ����������
	friend void readCompanyDivisions();
	
	public:
	
	// ������� ������������� ��������	
	operator string() {
		return name;	
	}
	
	// ��������� ���� ��������
	auto operator <(CompanyDivision& a) {
		return this->name < a.name;
	}
	
	// �������� �� ������� ���� ��������
	auto operator ==(CompanyDivision& a) {
		return this->name == a.name;
	}
};

// ��������� �� ������� ���������, �� �����������
const int MAXK = 100000;

vector <CompanyDivision> compDivs; // ������ �������� ��������
int k, s[MAXK]; // ������� ��������� �� ��� ����������

// ���������� �������� �������� �� �� ����������
void readCompanyDivisions() {
	int divN;
	cin >> divN;
	for (int i = 0; i < divN; i++) {
		CompanyDivision compDiv;
		cin >> compDiv.name;
		int empNum;
		cin >> empNum;
		for (int i = 0; i < empNum; i++) {
			string empName;
			cin >> empName;
			compDiv.employees.push_back(empName);
		}
		compDivs.push_back(compDiv);
	}
}

// ���������� ���������
void readStatistics() {
	cin >> k;
	for (int i = 0; i < k; i++) {
		cin >> s[i];
	}
}

// ���������� 
int main() {
	readCompanyDivisions();
	readStatistics();
	RedBlackTree <CompanyDivision> rbt;
	for (auto& c : compDivs) {
		rbt.insert(c);
	}
	rbt.show();
	for (int i = 0; i < k; i++) {
		cout << "Statistic N" << s[i] << " = " << (string)rbt.statistic(s[i]) << endl;
	}
}

