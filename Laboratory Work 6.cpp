#include <iostream>
#include <fstream>

using namespace std;

struct PhoneInfo {
    string name;
    int price;
    string os;
    double storage;
    void Out();
};

struct Tree {
    PhoneInfo data;
    Tree* left;
    Tree* right;
};

int GetTreeSize(Tree* T) {
    
    if (!T) {
        return 0;
    }
    
    if (!T->left && !T->right) {
        return 1;
    }
    
    int left = 0, right = 0;
    
    if (T->left) {
        left = GetTreeSize(T->left);
    }
    
    if (T->right) {
        right = GetTreeSize(T->right);
    }
    
    return left + right + 1;
}

void Add(Tree*& T, PhoneInfo data, int type) {
    
    if (!T) {
        
        T = new Tree;
        T->data = data;
        T->left = NULL;
        T->right = NULL;
        return;
        
    }
    
    if (type == 0) {
        
        if (data.name < T->data.name) {
            Add(T->left, data, 0);
        }
        
        else {
            Add(T->right, data, 0);
        }
    }
    
    else if (type == 1) {
        
        if (data.price < T->data.price) {
            Add(T->left, data, 1);
        }
        
        else {
            Add(T->right, data, 1);
        }
    }
}

void PrintLRR(Tree* T, ofstream& SF) {
    
    if (!T) {
        return;
    }
    
    PrintLRR(T->left, SF);
    T->data.Out();
    SF << T->data.name << " " << T->data.price << " " << T->data.os << " " << T->data.storage << endl;
    PrintLRR(T->right, SF);
    
}

void SelectionPrintLRR(Tree* T, PhoneInfo Info, int type) {
    
    if (!T) {
        return;
    }
    
    SelectionPrintLRR(T->left, Info, type);
    
    if (type == 0) {
        if (T->data.name[0] == Info.name[0]) {
            T->data.Out();
        }
    }
    
    else if (type == 1) {
        if (T->data.price < Info.price) {
            T->data.Out();
        }
    }
    
    SelectionPrintLRR(T->right, Info, type);
    
}

Tree* Search(Tree*& T, PhoneInfo data) {
    
    if (!T) {
        return 0;
    }
    
    while (T->data.price != data.price) {
        if (data.price < T->data.price) {
            T = T->left;
        }
        
        else {
            T = T->right;
            if (!T) {
                break;
            }
        }
    }
    
    return T;
}

void Delete(Tree*& T) {
    
    if (T) {
        if (T->left) {
            Delete(T->left);
            if (T->right) {
                Delete(T->right);
                delete T;
                T = NULL;
            }
        }
    }
    
    else {
        cout << "Tree is empty!" << endl << endl;
    }
    
}

void PhoneInfo::Out() {
    cout << "Phone: " << name << "\t" << "Price: " << price << "$\t" << "OS: " << os << "\t" << "Storage: " << storage << endl << endl;
}

bool FileAdd(Tree*&, int);

int main() {
    
    ofstream SF;
    Tree* T = NULL;
    PhoneInfo Info;
    int key = 0;
    do {
        cout << "1) Sort by string field" << endl
            << "2) Sort by numeric field" << endl
            << "3) Information up to a given numerical value" << endl
            << "4) Information up to a given symbol" << endl
            << "0) Exit" << endl;
        cout << endl << "Select an action: ";
        cin >> key;
        cout << endl;
        switch (key) {
            case 1: {
                SF.open("SortPhone.txt");
                FileAdd(T, 0);
                PrintLRR(T, SF);
                cout << "Tree size = " << GetTreeSize(T) << endl << endl;
                Delete(T);
                SF.close();
                break;
            }
                    
            case 2: {
                
                SF.open("SortPhone.txt");
                FileAdd(T, 1);
                PrintLRR(T, SF);
                cout << "Tree size = " << GetTreeSize(T) << endl << endl;
                Delete(T);
                SF.close();
                break;
            }

            case 3: {
                
                cout << "Enter the phone price: ";
                cin >> Info.price;
                cout << endl;
                FileAdd(T, 1);
                SelectionPrintLRR(T, Info, 1);
                Delete(T);
                break;
            }
                
            case 4: {
                
                cout << "Enter the first symbol: ";
                cin >> Info.name;
                cout << endl;
                FileAdd(T, 0);
                SelectionPrintLRR(T, Info, 0);
                Delete(T);
                break;
            }
                
            default:
                
                if (key != 0) {
                    cout << "There is no such action!" << endl << endl;
                }
                break;
        }
        
    } while (key != 0);
    
    Delete(T);
}

bool FileAdd(Tree*& T, int type) {
    
    PhoneInfo Info;
    ifstream F("Phone.txt");
    
    if (!F) {
        cout << "File not found!" << endl;
        return false;
    }
    
    while (F >> Info.name >> Info.price >> Info.os >> Info.storage) {
        Add(T, Info, type);
    }
    
    F.close();
    return true;
    
}
