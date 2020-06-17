#include <iostream>
#include <vector>

using namespace std;

// ����������� ������
template<typename T>
class SplayTree {
private:
	int size;
	
	// ��������� �����
	// key - ��������
	// l, r - ��������� �� ��� �� ����� ��������
	// p - �������� �� ������ 
	struct Node {
		Node *l, *r;
		Node *p;
		T key;
		Node(const T& key = T()) : 
			l(NULL), r(NULL), p(NULL), key(key) { }
		~Node() {}
	} *root;
	
	// ���� ������� ������� ����� x
	void leftRotate(Node* x) {
		Node *y = x->r;
		if(y) {
		  x->r = y->l;
		  if( y->l ) y->l->p = x;
		  y->p = x->p;
		}
		if( !x->p ) root = y;
		else if( x == x->p->l ) x->p->l = y;
		else x->p->r = y;
		if(y) y->l = x;
		x->p = y;
	}
	
	// ������ ������� ������� ����� x
	void rightRotate(Node* x) {
		Node *y = x->l;
		if(y) {
		  x->l = y->r;
		  if( y->r ) y->r->p = x;
		  y->p = x->p;
		}
		if( !x->p ) root = y;
		else if( x == x->p->l ) x->p->l = y;
		else x->p->r = y;
		if(y) y->r = x;
		x->p = y;
	}
	
	// ����������
	void splay(Node* x) {
		while( x->p ) {
		  if( !x->p->p ) {
		    if( x->p->l == x ) rightRotate( x->p );
		    else leftRotate( x->p );
		  } else if( x->p->l == x && x->p->p->l == x->p ) {
		    rightRotate( x->p->p );
		    rightRotate( x->p );
		  } else if( x->p->r == x && x->p->p->r == x->p ) {
		    leftRotate( x->p->p );
		    leftRotate( x->p );
		  } else if( x->p->l == x && x->p->p->r == x->p ) {
		    rightRotate( x->p );
		    leftRotate( x->p );
		  } else {
		    leftRotate( x->p );
		    rightRotate( x->p );
		  }
		}
	}
	
	// ����� ����� u �� ����� v
	void replace(Node* u, Node* v) {
		if( !u->p ) root = v;
		else if( u == u->p->l ) u->p->l = v;
		else u->p->r = v;
		if( v ) v->p = u->p;
	}
	
	// ������ ����� � �������, �� ���������� ������ u
	Node* subtreeMinimum(Node* u) {
		while( u->l ) u = u->l;
		return u;
	}
	
	// ������ �������� � �������, �� ���������� ������ u
	Node* subtreeMaximum(Node* u) {
		while( u->r ) u = u->r;
		return u;
	}
	
	// ����� (���������� �������)	
	Node* recFind(const T& key) {
		Node *n = root;
		while( n ) {
		  if(  n->key < key ) n = n->r;
		  else if( key < n->key ) n = n->l;
		  else {
		  	splay(n);
		  	return n;
		  }
		}
		return NULL;
	}
	
	// ��������� ������ (���������� �������)
	void show(Node* node, int depth, char dir) {
		if(!node) return;
		for(int i=0; i < depth; i++) cout << " ";
		cout << dir << "[" << ((string)node->key).c_str() << "]" << endl; 
		show(node->l, depth + 2, '-');
		show(node->r, depth + 2, '+');
	}
	
public:
	// �����������
	SplayTree() : root(NULL), size(0) { }
	
	// ������� ��������
	void insert(const T& key) {
		Node *n = root;
		Node *p = NULL;
		while( n ) {
		  p = n;
		  if( n->key < key ) n = n->r;
		  else n = n->l;
		}
		n = new Node( key );
		n->p = p;
		if( !p ) root = n;
		else if( p->key < n->key ) p->r = n;
		else p->l = n;
		splay( n );
		size++;
	}
	
	// �����	
	bool find(const T& key) {
		return recFind(key) != NULL;
	}
	    
	// ��������� ��������
	void erase(const T& key) {
		Node *n = recFind( key );
		if( !n ) return;
		splay( n );
		if( !n->l ) replace( n, n->r );
		else if( !n->r ) replace( n, n->l );
		else {
		  Node *y = subtreeMinimum( n->r );
		  if( y->p != n ) {
		    replace( y, y->r );
		    y->r = n->r;
		    y->r->p = y;
		  }
		  replace( n, y );
		  y->l = n->l;
		  y->l->p = y;
		}
		delete n;
		size--;
	}
  
  	// ��������� ������
	void show() {
		cout << "[tree]" << endl;
		show(root, 0, '*');
	}
	
};

// ������� ������
class CompanyDivision {
private:
	string name; // �����
	friend void readCompanyDivisions();
	friend bool operator <(const CompanyDivision& a, const CompanyDivision& b);
	
public:
	// ����������� 1
	CompanyDivision() {
	}
	
	// ����������� 2
	CompanyDivision(string name) : name(name) {
	}
	
	// ������� ������������� ��������	
	operator string() {
		return name;	
	}
	
};

// ��������� ���� ��������
bool operator <(const CompanyDivision& a, const CompanyDivision& b) {
	return a.name < b.name;
}

vector <CompanyDivision> compDivs; // ������ �������� ������

// ���������� �������� ������ 
void readCompanyDivisions() {
	int divN;
	cin >> divN;
	for (int i = 0; i < divN; i++) {
		CompanyDivision compDiv;
		cin >> compDiv.name;
		compDivs.push_back(compDiv);
	}
}

// ���������� 
int main()
{
	readCompanyDivisions();
   	SplayTree <CompanyDivision> st;
    for (auto c : compDivs) {
        st.insert(c);
    }
    st.show();
    int op;
    string name;
    while (true) {
        cout << "1 - insert " << endl;
        cout << "2 - find" << endl;
        cout << "3 - erase" << endl;
        cout << "4 - exit" << endl;
        cout << "Enter operation: ";
        cin >> op;
        switch(op) {
	        case 1:
	            cout << "Enter new company division name: ";
	            cin >> name;
	            st.insert(CompanyDivision(name));
	            st.show();
	            break;
	        case 2:
	            cout << "Enter company division name to find: ";
	            cin >> name;
	            cout << (st.find(CompanyDivision(name)) ? "Found" : "Not found") << endl;
	            st.show();
	            break;    
	        case 3:
	            cout << "Enter company division name to erase: ";
	            cin >> name;
	            st.erase(CompanyDivision(name));
	            st.show();
	            break;
	        default:
	            exit(0);
        }
    }
}

