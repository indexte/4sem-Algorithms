#include <iostream>
#include <vector>

using namespace std;

// ���� Գ�������
template <class T, typename Comp = less<T>> 
class FibonacciHeap {
	
	// ����� ���� Գ�������
	struct Node {
		T key; // ����
		// �������� �� ������ � ���� �������� �����'������ ��������� ������
		Node* right; 
		Node* left;
		Node* child; // �������� �� ���� �� ����� � ������ ����� ������ �����
		Node* parent; // �������� �� ������
		int degree; // ������ �������
		bool mark; // �� ��� ��������� � ������ ���� ����� ��� ���� �������
		
		// ����������� �����
		Node(T key) {
			this->key = key;
			this->right = this->left = this;
			this->child = NULL;
			this->parent = NULL;
			this->degree = 0;
			this->mark = false;
		}
	};
	
	// �������� �� ����� � ���������� ������
	Node* h;
	
	// ������� ����� � ���
	int __size;
	
	// ���������� ��� ��������� ��'����
	Comp comp;
	
	// ������ ���� ���
	// ��������� ������ ���� �����'����� �������� ������
	Node* __merge(Node* a, Node* b) {
		if (a == NULL) {
			return b;
		}
		if (b == NULL) {
			return a;
		}
		if (comp(b->key, a->key)) {
			Node* temp = a;
			a = b;
			b = temp;
		}
		Node* al = a->left;
		Node* br = b->right;
		a->left = b;
		b->right = a;
		al->right = br;
		br->left = al;
		return a;
	}
	
	// ������� ��������� ��'������� ���� 
	// ���������� �����
	void addChild(Node* parent, Node* child) {
		child->right = child->left = child;
		child->parent = parent;
		parent->degree++;
		parent->child = __merge(parent->child, child);
	}

	// ��� ������-������ ������ �����
	// ������ ��������� �� ������ �� NULL,
    // � ������ �� false
	void normalize(Node* node) {
		if (node == NULL) {
			return;
		}
		Node* cur = node;
		do {
			cur->mark = false;
			cur->parent = NULL;
			cur = cur->left;
		} while (cur != node);
	}
	
	// ������������ ����� 
	Node* consolidate(Node* node) {
		Node* ts[1 << 6] = {NULL};
		while (true) {
			if (ts[node->degree] != NULL) {
				// ������� �� �������� ����� �� ��������� ����� � ����,
				// �� ������ � ������ �����
				Node* t = ts[node->degree];
				if (t == node) break; // ������� ������ �� �������
				ts[node->degree] = NULL;
				if (comp(node->key, t->key)) {
					t->right->left = t->left;
					t->left->right = t->right;
					addChild(node, t);
				} else {
					t->right->left = t->left;
					t->left->right = t->right;
					if (node->left == node) {
						t->left = t->right = t;
						addChild(t, node);
						node = t;
					} else {
						node->right->left = t;
						node->left->right = t;
						t->left = node->left;
						t->right = node->right;
						addChild(t, node);
						node = t;
					}
				}
				continue;
			} else { // ���� ������ �����, �� ��������� ���� ����� �����
				ts[node->degree] = node;
			}
			node = node->left;
		}
		// ��������� ������, ���� ���������
		Node* mn = node;
		Node* start = node;
		do {
			if (comp(node->key, mn->key)) {
				mn = node;
			}
			node = node->left;
		} while (node != start);
		return mn;
	}

	// ��������� ����������� �������� � ���� (���������� �������)
	Node* __extractMinimum(Node* node) {
		// ��'������ ������ ���� ����������� ����� � ��������� 
		// �� �������� ����� ����� � ������
		normalize(node->child);
		if (node->left == node) {
			node = node->child;
		} else {
			node->left->right = node->right;
			node->right->left = node->left;
			node = __merge(node->left, node->child);
		}
		if (node == NULL) {
			return node;
		}
		__size--;
		return consolidate(node);
	}

	// ��� �������� ������� ��
	// ��������� �� � ������ ���� ����� ������,
	// �������� ������ �� ������ � ������� ���� � ���� �������
	Node* cut(Node* h, Node* node) {
		if (node->left == node) {
			node->parent->child = NULL;
		} else {
			node->left->right = node->right;
			node->right->left = node->left;
			node->parent->child = node->left;
		}
		node->left = node->right = node;
		node->mark = false;
		return __merge(h, node);
	}

	// ��������� �������� �������� (���������� �������)
	Node* __decreaseKey(Node* h, Node* node, T key) {
		if (comp(node->key, key)) {
			return h;
			// ���� �������� ������
		}
		node->key = key;
		if (node->parent) {
			if (comp(node->key, node->parent->key)) {
				// ��������
				h = cut(h, node);
				Node* parent = node->parent;
				node->parent = NULL;
				// �������� ��������
				while (parent != NULL && parent->mark) {
					h = cut(h, parent);
					node = parent;
					parent = node->parent;
					node->parent = NULL;
				}
				if (parent != NULL && parent->parent != NULL) {
					parent->mark = true;
				}
			}
		} else {
			if (comp(node->key, h->key)) {
				h = node;
			}
		}
		return h;
	}
	
	// ��������� ���� Գ������� (���������� �������)
	void __print(Node* node, int depth) {
		if (node == NULL) {
			return;
		}
		Node* cur = node;
		do {
			auto makeSpaces = [&]() {
				for (int i = 0; i < depth; i++) {
					cout << " ";
				}	
			};
			makeSpaces();
			cout << "(" << (string)cur->key << ", " << cur->degree << ") ->" << endl;
			makeSpaces();
			cout << "|" << endl;
			makeSpaces();
			cout << "V" << endl;
			__print(cur->child, depth + 4);
			cur = cur->right;
		} while (cur != node);
	}

public:

	// ����������� ����
	FibonacciHeap(Comp comp = Comp()) : h(NULL), __size(0), comp(comp) {
	}

	// ������� ��������
	void insert(T key) {
		Node* nh = new Node(key);
		h = __merge(h, nh);
		__size++;
	}
	
	// ������ ������
	T getMinimum() {
		return h->key;
	}
	
	// ������ ���� ��� Գ�������
	void merge(FibonacciHeap other) {
		this->h = __merge(this->h, other.h);
	}

	// �������� ������
	void extractMinimum() {
		Node* oh = h;
		h = __extractMinimum(h);
		T nh = oh->key;
	}
	
	// �������� ���� �� ����������
	bool isEmpty() {
		return h == NULL;
	}
	
	// ���������� ������ ����
	int size() {
		return __size;
	}
	
	// ��������� ����
	void print() {
		if (isEmpty()) {
			cout << "Heap is empty" << endl;
			return;
		}
		cout << "Size of heap = " << __size << endl;
		__print(h, 0);
		cout << endl;
	}

};

// ������� ��������
class CompanyDivision {
	// �����
	string name;
	
public:
	
	// ����������� 1
	CompanyDivision() : name("") {
	}
	
	// ����������� 2
	CompanyDivision(string name) : name(name) {
	}
	
	// ������� ������������� ��������
	operator string() const {
		return name;
	}
};

// �������� ��������� ���� ��������
bool operator <(const CompanyDivision& a, const CompanyDivision& b) {
	return (string)a < (string)b;
}

// ����������
int main() {
	vector <string> names = {"D4", "D5", "D2", "D7", "D6", "D3"};
	FibonacciHeap <CompanyDivision> fh;
	for (auto name : names) {
		fh.insert(CompanyDivision(name));
	}
	cout << "Heap after inserting ";
	cout << names[0];
	for (int i = 1; i < names.size(); i++) {
		cout << ", " << names[i]; 
	}
	cout << endl;
	fh.print();
	cout << "Minimum is " << (string)fh.getMinimum() << endl;
	fh.extractMinimum();
	string minName = "D1";
	fh.insert(CompanyDivision(minName));
	cout << "Heap after extracting minumum and inserting " << minName << ": " << endl;
	fh.print();
	cout << "Minimum is " << (string)fh.getMinimum() << endl;
}
