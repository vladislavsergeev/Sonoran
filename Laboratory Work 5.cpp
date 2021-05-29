#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>

using namespace std;

struct PhoneInfo {
    string os;
    float diagonal, ram, storage;
    void Out();
};

struct DLList {
    struct Node {
        PhoneInfo data;     // Информация
        Node* next;   // Позиция следующего указателя
        Node* prev;   // Позиция предыдущего указателя
    };

    Node* F = NULL; // Указатель на первый элемент
    Node* L = NULL;     // Указатель на последний элемент
    Node* C = NULL;     // Указатель на текущий элемент
    int Count;        // Счётчик указателя

    void Out();
    void Info();
    void Clear();

    bool MoveNext();
    bool MovePrev();
    bool MoveFirst();
    bool MoveLast();

    bool Init(PhoneInfo);
    bool AddNext(PhoneInfo);
    bool AddPrev(PhoneInfo);
    bool AddFirst(PhoneInfo);
    bool AddLast(PhoneInfo);

    bool Del(PhoneInfo&);
    bool DelNext(PhoneInfo&);
    bool DelPrev(PhoneInfo&);
    bool DelFirst(PhoneInfo&);
    bool DelLast(PhoneInfo&);
};

bool DLList::MoveNext() {
    if (!F) {
        return false;
    }
    if (!C) {
        C = F; return true;
    }
    if (!C->next) {
        return false;
    }
    C = C->next;
    return true;
}

bool DLList::MovePrev() {
    if (!F)       return false;
    if (!C) {
        C = F; return true;
    }
    if (!C->prev) {
        return false;
    }
    C = C->prev;
    return true;
}

bool DLList::MoveFirst() {
    if (!F) {
        return false;
    }
    C = F;
    return true;
}

bool DLList::MoveLast() {
    if (!L) {
        return false;
    }
    C = L;
    return true;
}

bool DLList::Init(PhoneInfo data) {
    if (!F) {
        F = new Node;
        L = F;
        C = F;
        F->prev = NULL;
        F->next = NULL;
        F->data = data;
        Count = 1;
        return true;
    }
    else
        return false;
}

bool DLList::AddNext(PhoneInfo data) {
    if (!F) {
        return Init(data);
    }

    Node* temp = C->next;
    C->next = new Node;
    C->next->next = temp;
    C->next->prev = C;

    if (!temp)
        L = C->next;
    else
        temp->prev = C->next;

    C = C->next;
    C->data = data;
    Count++;
    return true;
}

bool DLList::AddPrev(PhoneInfo data) {
    if (!F) {
        return Init(data);
    }

    Node* temp = C->prev;
    C->prev = new Node;
    C->prev->next = C;
    C->prev->prev = temp;

    if (!temp) {
        F = C->prev;
    }
    else {
        temp->next = C->prev;
    }
    C = C->prev;
    C->data = data;
    Count++;
    return true;
}

bool DLList::AddFirst(PhoneInfo data) {
    if (MoveFirst()) {
        return AddPrev(data);
    }
    else return false;
}

bool DLList::AddLast(PhoneInfo data) {
    if (MoveLast()) {
        return AddNext(data);
    }
    else {
        return false;
    }
}

void DLList::Out() {
    if (!F) {
        cout << "List is empty" << endl;
        return;
    }

    Node* temp = F;
    cout << "List: " << endl << endl;
    do {
        temp->data.Out();
        temp = temp->next;
    } while (temp);
    cout << endl;
}

void DLList::Info() {

    if (Count)
        cout << "List node count: "
        << Count << endl << endl;
    else
        cout << "List is empty" << endl;
    if (C) {
        if (MoveFirst()) {
            cout << "Current node data: ";
            C->data.Out();
        }
    }
}

bool DLList::DelFirst(PhoneInfo& data) {
    if (!F)      return false;
    if (C != F) {
        MoveFirst();
    }
    Node* temp = C->next;
    data = C->data;

    if (temp) temp->prev = NULL;
    delete C;

    C = temp;
    F = temp;
    Count--;
    if (!temp) {
        L = NULL;
        return false;
    }
    return true;
}

bool DLList::DelLast(PhoneInfo& data) {
    if (!F) {
        return false;
    }
    if (C != L) {
        MoveLast();
    }
    Node* temp = C->prev;
    data = C->data;

    if (temp) temp->next = NULL;
    delete C;

    C = temp;
    L = temp;
    Count--;

    if (!temp) {
        F = NULL;
        return false;
    }
    return true;
}

bool DLList::Del(PhoneInfo& data) {
    if (!F) {
        return false;
    }
    if (!C) {
        return false;
    }
    if (C == F) {
        return DelFirst(data);
    }
    if (C == L) {
        return DelLast(data);
    }
    Node* temp = C->next;
    data = C->data;

    C->prev->next = C->next;
    C->next->prev = C->prev;

    delete C;
    C = temp;
    Count--;
    return true;
}

bool DLList::DelNext(PhoneInfo& data) {
    if (!F) {
        return false;
    }
    if (!C) {
        return false;
    }
    if (MoveNext()) {
        return Del(data);
    }
    return false;
}
bool DLList::DelPrev(PhoneInfo& data) {
    if (!F) {
        return false;
    }
    if (!C) {
        return false;
    }
    if (MovePrev()) {
        return Del(data);
    }
    return false;
}

void DLList::Clear() {
    if (!MoveFirst()) {
        cout << "List is empty" << endl;
        return;
    }
    PhoneInfo k;
    while (Del(k));
}

void PhoneInfo::Out() {
    cout << "OS: " << os << "\t\t" << "Diagonal: " << diagonal << "\t" << "RAM Amount: " << ram << "\t" << "Storage Amount: " << storage << endl << endl;
}

bool GetFile(DLList&, PhoneInfo&);
bool WriteFile(DLList&, PhoneInfo&);
void GetPhone(PhoneInfo&);

int main() {
    PhoneInfo Info;
    DLList DLList;
    GetFile(DLList, Info);
    DLList.Out();

    PhoneInfo r;
    PhoneInfo o;


    int num, key = 0, key1 = 0, key2 = 0;
    do {
        cout << "1) Add element" << endl
            << "2) Delete element" << endl
            << "3) Sort by amount of RAM" << endl
            << "4) Sort by name of OS" << endl
            << "5) Clear list" << endl
            << "6) Upload data" << endl
            << "0) Exit" << endl;
        cout << endl << "Select an action: ";
        cin >> key;
        cout << endl;
        switch (key) {
        case 1:
            cout << "1) Add to start" << endl
                << "2) Add to end" << endl
                << "3) Add to k position" << endl
                << "0) Back" << endl;
            cout << endl << "Select an action: ";
            cin >> key1;
            cout << endl;
            switch (key1) {
            case 1:
                GetPhone(Info);
                DLList.AddFirst(Info);
                DLList.Out();
                break;
            case 2:
                GetPhone(Info);
                DLList.AddLast(Info);
                DLList.Out();
                break;
            case 3:
                cout << "Enter position: ";
                cin >> num;
                GetPhone(Info);
                if (num == 1) {
                    DLList.AddFirst(Info);
                    DLList.Out();
                    break;
                }
                else if (num == DLList.Count + 1) {
                    DLList.AddLast(Info);
                    DLList.Out();
                    break;
                }
                DLList.MoveFirst();
                for (int i = 1; i < num - 1; i++) {
                    DLList.MoveNext();
                }
                DLList.AddNext(Info);
                DLList.Out();
                break;
            default:
                if (key1 != 0) {
                    cout << "There is no such action!" << endl << endl;
                }
                break;
            }
            break;
        case 2:
            cout << "1) Delete first" << endl
                << "2) Delete last" << endl
                << "3) Delete k position" << endl
                << "0) Back" << endl;
            cout << endl << "Select an action: ";
            cin >> key2;
            cout << endl;
            switch (key2) {
            case 1:
                DLList.DelFirst(Info);
                DLList.Out();
                break;
            case 2:
                DLList.DelLast(Info);
                DLList.Out();
                break;
            case 3:
                cout << "Enter position: ";
                cin >> num;
                if (num == 1) {
                    DLList.DelFirst(Info);
                    DLList.Out();
                    break;
                }
                else if (num == DLList.Count) {
                    DLList.DelLast(Info);
                    DLList.Out();
                    break;
                }
                DLList.MoveFirst();
                for (int i = 1; i < num; i++) {
                    DLList.MoveNext();
                }
                DLList.Del(Info);
                DLList.Out();
                break;
            default:
                if (key2 != 0) {
                    cout << "There is no such action!" << endl << endl;
                }
                break;
            }
            break;
        case 3:
            for (int i = 0; i < DLList.Count - 1; i++)
            {
                DLList.MoveLast();
                for (int j = (DLList.Count - 1); j > 0; j--)
                {
                    if ((DLList.C->prev->data.ram) > (DLList.C->data.ram))
                    {
                        r = DLList.C->data;
                        DLList.C->data = DLList.C->prev->data;
                        DLList.C->prev->data = r;
                    }
                    DLList.MovePrev();
                }


            }
            DLList.Info();
            DLList.Out();
            break;




        case 4:
            for (int i = 0; i < DLList.Count - 1; i++)
            {
                DLList.MoveLast();
                for (int j = (DLList.Count - 1); j > 0; j--)
                {
                    if (strcmp(DLList.C->prev->data.os.c_str(), DLList.C->data.os.c_str())>0)
                    {
                        o = DLList.C->data;
                        DLList.C->data = DLList.C->prev->data;
                        DLList.C->prev->data = o;
                    }
                    DLList.MovePrev();
                }


            }
            DLList.Info();
            DLList.Out();
            break;




        case 5:
            DLList.Clear();
            DLList.Out();
            break;
        case 6:
            WriteFile(DLList, Info);
            break;
        default:
            if (key != 0) {
                cout << "There is no such action!" << endl << endl;
            }
            DLList.Clear();
            DLList.Out();
            break;
        }
    } while (key != 0);
}

bool GetFile(DLList& DLList, PhoneInfo& Info) {
    ifstream F("Phones.txt");
    if (!F) {
        cout << "File not found!" << endl;
        return false;
    }
    while (F >> Info.os >> Info.diagonal >> Info.ram >> Info.storage) {
        DLList.AddNext(Info);
    }
    F.close();
    return true;
}

bool WriteFile(DLList& DLList, PhoneInfo& Info) {
    ofstream F("Phones.txt");
    if (!F) {
        cout << "File not found!" << endl;
        return false;
    }
    DLList.MoveFirst();
    do {
        F << DLList.C->data.os << " " << DLList.C->data.ram << " " << DLList.C->data.diagonal << " " << DLList.C->data.storage << endl;
    } while (DLList.MoveNext());
    DLList.MoveFirst();
    F.close();
    return true;
}

void GetPhone(PhoneInfo& Info) {
    cout << "Enter the title of operating system: ";
    cin >> Info.os;
    cout << endl;
    cout << "Enter the diagonal: ";
    cin >> Info.diagonal;
    cout << endl;
    cout << "Enter the amount of ram: ";
    cin >> Info.ram;
    cout << endl;
    cout << "Enter the amount of storage: ";
    cin >> Info.storage;
    cout << endl;
}