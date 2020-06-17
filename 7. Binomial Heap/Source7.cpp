#include <iostream>
#include <list>
#include <vector>

using namespace std; 
  
// ��������� ����
template <typename T, typename Comp = less<T>>
class BinomialHeap {
	
	// ����� ����������� ������
	struct Node { 
	    T key;
		int degree; 
	    Node *child, *sibling, *parent; 
	    
	    // ����������� �����
	    Node(T key)  {  
		    this->key = key; 
		    this->degree = 0; 
		    this->child = this->parent = this->sibling = NULL; 
		} 
	};
	
	// ������ ������
	list <Node*> roots; 
	
	// ���������� ��� ��������� ��'����
	Comp comp;
	
	// ������ ���� ���������� �����
	Node* mergeBinomialTrees(Node *t1, Node *t2) { 
	    if (comp(t2->key, t1->key)) {
	    	swap(t1, t2);
		} 
	  	// ������ ������ � ������ ���������� ����� ������ � ������
	    t2->parent = t1; 
	    t2->sibling = t1->child; 
	    t1->child = t2; 
	    t1->degree++; 
	    return t1; 
	} 
	  
	// ��'������� ���� ���
	list <Node*> unionBinomialHeaps(list <Node*> h1, list <Node*> h2) { 
	    list <Node*> h; 
	    auto it1 = h1.begin(); 
	    auto it2 = h2.begin(); 
	    while (it1!=h1.end() && it2!=h2.end()) { 
	        if ((*it1)->degree <= (*it2)->degree) { 
	            h.push_back(*it1); 
	            it1++; 
	        } else { 
	            h.push_back(*it2); 
	            it2++; 
	        } 
	    } 
	    while (it1 != h1.end()) { 
	        h.push_back(*it1); 
	        it1++; 
	    } 
	    while (it2 != h2.end()) { 
	        h.push_back(*it2); 
	        it2++; 
	    } 
	    return h; 
	} 
	  
	// ������� ������ ����� ��������� �� ������� ������� �����
	list <Node*> normalize(list <Node*> h) { 
	    if (h.size() <= 1) {
	    	return h;
		} 
	    list <Node*> nh; 
	    auto it1 = h.begin(), it2 = h.begin(), it3 = h.begin(); 
	    if (h.size() == 2) { 
	        it2 = it1; 
	        it2++; 
	        it3 = h.end(); 
	    } else { 
	        it2++; 
	        it3 = it2; 
	        it3++; 
	    } 
	    while (it1 != h.end()) { 
	        if (it2 == h.end()) {
	            it1++; 
	        } else if ((*it1)->degree < (*it2)->degree) { 
	            it1++; 
	            it2++; 
	            if (it3 != h.end()) {
	            	it3++;
				} 
	        } else if (it3 != h.end() && 
	                  (*it1)->degree == (*it2)->degree && 
	                  (*it1)->degree == (*it3)->degree) { 
	            it1++; 
	            it2++; 
	            it3++; 
	        } else if ((*it1)->degree == (*it2)->degree) { 
	            *it1 = mergeBinomialTrees(*it1, *it2); 
	            it2 = h.erase(it2); 
	            if(it3 != h.end()) {
	            	it3++;
				}
	        } 
	    } 
	    return h; 
	} 
	  
	// ������� ����������� ������ � ��������� ����
	list <Node*> insertTree(list <Node*> h, Node *t) { 
	    list <Node*> nh; 
	    nh.push_back(t); 
	    nh = unionBinomialHeaps(h, nh); 
	    return normalize(nh); 
	} 
	  
	// ��������� ���������� ����� � ����
	// � ��'������� ���� ������ ����� � ���� ����
	list <Node*> cutMinimalTree(Node *t) { 
	    list <Node*> h; 
	    Node *p1 = t->child; 
	    Node *p2; 
	    while (p1) { 
	        p2 = p1; 
	        p1 = p1->sibling; 
	        p2->sibling = NULL; 
	        h.push_front(p2); 
	    } 
	    return h; 
	}
	
	// ����������� ����� � ��������� ������
	Node* __getMin() { 
	    auto it = this->roots.begin(); 
	    Node *p = *it; 
	    while (it != this->roots.end()) { 
	        if ((*it)->key < p->key) {
	            p = *it; 
	        }
	        it++; 
	    } 
	    return p; 
	}   
	  
	// ��������� ����������� ������
	void printTree(Node *h) { 
	    while (h) { 
	        cout << (string)h->key << " "; 
	        printTree(h->child); 
	        h = h->sibling; 
	    } 
	} 
	
public:
	
	// ����������� �������� ����
	BinomialHeap(Comp comp = Comp()) : comp(comp) {
	}
	  
	// �������
	void insert(T key) { 
	    Node *temp = new Node(key); 
	    this->roots = insertTree(this->roots, temp); 
	}
	
	// ����������� ���������� ��������
	T getMin() {
	    return __getMin()->key; 
	}  
	
	// ��������� ���������� ��������
	void extractMin() { 
	    list <Node*> h1, h2; 
	    Node *p;
	    p = __getMin(); 
	    auto it = this->roots.begin(); 
	    while (it != this->roots.end()) { 
	        if (*it != p) {
	            h1.push_back(*it); 
	        } 
	        it++; 
	    } 
	    h2 = cutMinimalTree(p); 
	    h1 = unionBinomialHeaps(h1, h2); 
	    h1 = normalize(h1); 
	    this->roots = h1; 
	} 
	
	// ��������� ��������� ����
	void print() { 
	    auto it = this->roots.begin(); 
	    while (it != this->roots.end()) { 
	        printTree(*it); 
	        it++; 
	    } 
	    cout << endl;
	} 
};  

// ������� ������
class CompanyDivision {
	// �����
	string name;
	friend void read(); 
	
public:
	// ������� ������������� ������
	operator string() const {
		return name;
	}
};

// �������� ��������� ���� ��������
bool operator <(const CompanyDivision& a, const CompanyDivision& b) {
	return (string)a < (string)b;
}

// ����� ��������
vector <CompanyDivision> compDivs;

// ���������� �������� ������
void read() {
	int divN;
	cin >> divN;
	for (int i = 0; i < divN; i++) {
		CompanyDivision compDiv;
		cin >> compDiv.name; 
		compDivs.push_back(compDiv);
	}
}
  
// ����������
int main() {
	read();
	// ������� �������� ��������
	BinomialHeap <CompanyDivision> bh;
	for (auto compDiv : compDivs) {
		bh.insert(compDiv);
	}
	cout << "Binomial heap:" << endl;
	bh.print();
	cout << "Minimal element is " << (string)bh.getMin() << endl;
	bh.extractMin();
	cout << "Heap after extracting minimal element:" << endl;
	bh.print(); 
	cout << "Now minimal element is " << (string)bh.getMin() << endl;
} 
