#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// B+ ������
template <typename T>
class BPlusTree {
	
	// ����������� ������ ������
	static const int B = 31; 
	
	// ����� B+ ������
	struct Node {
	    int keyNum; // ������� ������ �����
	    int key[B]; // ����� �����
	    void* pointers[B + 1]; // ��������� �� ���� ���, 
							   // ���� ����� � ������, �� ��������
	    Node* parent; // �������� �� ������
	    bool isLeaf; // �� � ����� ������
	    
	    // ����������� �����
	    Node() {
	    	isLeaf = false;
	    	keyNum = 0;
	    	pointers[0] = NULL;
	    	parent = NULL;
		}
	};
	
	Node* root; // ����� ������
	int __size; // ������� ����� � �����
	
	// ��������� (���������� �������)
	void __print(Node* node) {
		for (int i = 0; i <= node->keyNum; i++) {
            if (node->isLeaf) {
            	if (i == 0) continue;
            	cout << node->key[i - 1] << ":" << (string)*(T*)(node->pointers[i]) << "  ";
			} else {
                __print((Node*)node->pointers[i]);
            } 
        }
	}
	
public:
	
	// ����������� ������
	BPlusTree() {
	    root = new Node();
	    root->isLeaf = true;
	    __size = 0;
	}
	
	// ������� �������� � ������
	bool insert(int key, void* value) {
	    Node* leaf = findLeaf(key);
	    for (int i = 0; i < leaf->keyNum; i++) {
	    	if (key == leaf->key[i]) {
	    		return false;
			}
		}
	    insertInLeaf(leaf, key, value);
	    __size++;
	    return true;
	}
	
	// ����������� ����� � ����� ����������� ����� ����
	// ���� ������� � ����� ������ ��� � � �����, ����������� false
	Node* findLeaf(int key) {
	    Node* cur = root;
	    while (!cur->isLeaf) {
	        for (int i = 0; i <= cur->keyNum; i++) {
	            if (i == cur->keyNum || key < cur->key[i]) {
	                cur = (Node*)cur->pointers[i];
	                break;
	            }
	        }
	    }
	    return cur;
	}
	
	// ������� �������� � ����
	void insertInLeaf(Node* node, int key, void* value) {
	    int k = 0;
	    while (k < node->keyNum && node->key[k] < key) {
	    	k++;
		}
	    for (int i = node->keyNum; i > k; i--) {
	    	node->key[i] = node->key[i - 1];
		}
	        
	    for (int i = node->keyNum + 1; i > k + 1; i--) {
	    	node->pointers[i] = node->pointers[i - 1];
		}
	    node->key[k] = key;
	    node->pointers[k + 1] = value;
	    node->keyNum++;
		// ���� ���� ������� ���� �������� ����������
		// ��������� ���� �������
	    if (node->keyNum == B) {
	    	split(node);
		}
	}
	
	// �������� ������
	void split(Node* node) {
	    Node* nodeN = new Node();
	    int mid = node->key[B / 2];
		// ���������� ����� ������� �������� node � ����� nodeN
	    nodeN->keyNum = (B - 1) / 2;
	    for (int i = 0; i < nodeN->keyNum; i++) {
	        nodeN->key[i] = node->key[i + (B / 2 + 1)];
	        nodeN->pointers[i] = node->pointers[i + (B / 2 + 1)];
	    }
	    nodeN->pointers[nodeN->keyNum] = node->pointers[B];
	    node->keyNum = B / 2;
		// ���� ����� ����, �� ����, �� ��������� �����
		// �������� � ����� ��������, � ���� ����
		// ������������� � ����������� ����� � ��� ������ ��������
	    if (node->isLeaf) {
	        node->keyNum++;
	        nodeN->pointers[0] = node->pointers[0];
	        node->pointers[0] = nodeN;
	        nodeN->isLeaf = true;
	        mid = node->key[B / 2 + 1];
	    }
		// ���� ����� �����, ����������� ����� �����,
		// �� ������� ����� �� �� ����� �
		// ������ ������ ����������
	    if (!node->parent) {
	        root = new Node();
	        root->key[0] = mid;
	        root->pointers[0] = node;
	        root->pointers[1] = nodeN;
	        root->keyNum = 1;
	        node->parent = nodeN->parent = root;
	    } else {
	        nodeN->parent = node->parent;
	        insertInLeaf((Node*)node->parent, mid, (void*)nodeN);
	    }
	}
	
	// ��������� �������� �� ������
	// ���� ������ �������� �� ��, ����������� false
	bool erase(int key) {
	    Node* leaf = findLeaf(key);
	    for (int i = 0; i < leaf->keyNum; i++) {
	    	if (key == leaf->key[i]) {
	            eraseFromLeaf(leaf, key);
	            __size--;
	            return true;
	        }
		}
	    return false;
	}
	
	// ��������� �������� � ����� �� ������
	void eraseFromLeaf(Node* node, int key) {
	    int k = 0;
	    while (key != node->key[k]) {
	    	k++;
		}
	    for (int i = k; i < node->keyNum - 1; i++) {
	    	node->key[i] = node->key[i + 1];
		}
	    for (int i = k + 1; i < node->keyNum; i++) {
	    	node->pointers[i] = node->pointers[i + 1];
		}
	    node->keyNum--;
	}
	
	// ����� �������� �� ������
	// ���� �� ��������, �� ����������� NULL
	T* search(int key) {
	    Node* leaf = findLeaf(key);
	    for (int i = 0; i < leaf->keyNum; i++) {
	    	if (key == leaf->key[i]) {
	    		return (T*)leaf->pointers[i + 1];
			}
		}
	    return NULL;
	}
	
	// ������� ����� ������
	int size() {
		return __size;
	}
	
	// ��������� ������
	void print() {
		cout << "B+-tree (size = " << __size << "):" << endl;
		__print(root);
		cout << endl;
	}
};

// ������� ������
class CompanyDivision {
	// �����
	string name;
	friend void read();
	
public:
	
	// ����������� 1
	CompanyDivision() : name("") {
	}
	
	// ����������� 2
	CompanyDivision(const string name) : name(name) {
	}
	
	// ������� ������������� ������
	operator string() const {
		return name;
	}
};

// ����� ��������
vector <CompanyDivision> compDivs;
vector <int> keys;

// ���������� �������� ������ �� ��������� ������
// � ����� test.txt
void read() {
	ifstream fin("test.txt");
	int divN;
	fin >> divN;
	for (int i = 0; i < divN; i++) {
		int key;
		CompanyDivision compDiv;
		fin >> key >> compDiv.name;  
		compDivs.push_back(compDiv);
		keys.push_back(key);
	}
}

// ����������
int main() {
	read();
	BPlusTree <CompanyDivision> bpt;
	for (int i = 0; i < compDivs.size(); i++) {
		bpt.insert(keys[i], &compDivs[i]);
	}
	cout << "Operations: insert - 1, delete - 2, search - 3, exit - 4" << endl << endl;
	bpt.print();
    while (true) {
    	int op;
    	cout << "operation: ";
    	cin >> op;
    	switch (op) {
    		case 1: {
    			cout << "--insertion--" << endl;
    			int key;
    			string name;
    			cout << "key: ";
				cin >> key;
				cout << "company division name: ";
				cin >>  name;
				if (!bpt.insert(key, new CompanyDivision(name))) {
					cout << "The key can not repeat!" << endl;
				} else {
					bpt.print();
				}
    			break;
    		}
    		case 2: {
    			cout << "--deleting--" << endl;
    			int key;
    			cout << "key: ";
				cin >> key;
    			if (!bpt.erase(key)) {
					cout << "There isn't company division with such key!" << endl;
				} else {
					bpt.print();
				}
    			break;
    		}
    		case 3: {
    			cout << "--searching--" << endl;
    			int key;
    			cout << "key: ";
				cin >> key;
				CompanyDivision* compDiv = bpt.search(key);
				if (compDiv == NULL) {
					cout << "Not found" << endl;
				} else {
					cout << "Company division with key = " << key << " is " << (string)*compDiv << endl;
				}
    			break;
    		}
    		case 4: {
    			exit(0);
    		}
    		default: {
				break;
			}
		}
	}
}
