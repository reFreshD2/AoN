#include <iostream>
#include <string.h>

using namespace std;

struct Element {
    char *str;
    Element *Prev, *Next;
};

class StringDL {
    Element *H, *T;
    unsigned int LengthElem;

    void Add(const char *elem) {
        Element *temp = new Element;
        temp->Next = nullptr;
        temp->str = new char[LengthElem];
        strncpy(temp->str, elem, LengthElem + 1);
        if (H != nullptr) {
            temp->Prev = T;
            T->Next = temp;
            T = temp;
        } else {
            temp->Prev = nullptr;
            H = T = temp;
        }
    }

    void ToNull(unsigned int pos, unsigned int n) {
        Element *temp = H;
        int num = pos / LengthElem;
        while (num != 0) {
            temp = temp->Next;
            num--;
            pos -= LengthElem;
        }
        int i = pos;
        while (n != 0) {
            if (i < LengthElem) {
                temp->str[i] = '*';
            } else {
                temp = temp->Next;
                i = 0;
                temp->str[i] = '*';
            }
            i++;
            n--;
        }
    }

public:
    StringDL(const char *str = "", unsigned int length = 5) {
        LengthElem = length;
        H = T = nullptr;
        char *temp = new char[LengthElem];
        unsigned int i = 0;
        if ((strlen(str) % LengthElem) == 0) {
            while (i <= strlen(str)) {
                if (((i % LengthElem) == 0) && (i != 0)) {
                    temp[LengthElem] = '\0';
                    Add(temp);
                }
                temp[i % LengthElem] = str[i];
                i++;
            }
        }
        if ((strlen(str) % LengthElem) != 0) {
            unsigned int LengthCopy = (strlen(str) / LengthElem) * LengthElem;
            while (i <= LengthCopy) {
                if (((i % LengthElem) == 0) && (i != 0)) {
                    temp[LengthElem] = '\0';
                    Add(temp);
                }
                temp[i % LengthElem] = str[i];
                i++;
            }
            while (i < strlen(str)) {
                temp[i % LengthElem] = str[i];
                i++;
            }
            while ((i % LengthElem) != 0) {
                temp[i % LengthElem] = '*';
                i++;
            }
            temp[LengthElem] = '\0';
            Add(temp);
        }
    }

    StringDL(const StringDL &copy) {
        H = nullptr;
        T = nullptr;
        LengthElem = copy.LengthElem;
        Element *copyDL = copy.H;
        while (copyDL != nullptr) {
            Element *temp = new Element;
            temp->Next = nullptr;
            temp->str = new char[LengthElem];
            strcpy(temp->str, copyDL->str);
            if (H != nullptr) {
                temp->Prev = T;
                T->Next = temp;
                T = temp;
            } else {
                temp->Prev = nullptr;
                H = T = temp;
            }
            copyDL = copyDL->Next;
        }
    }

    StringDL &operator=(const StringDL &right) {
        if (this == &right) {
            return *this;
        }
        while (H) {
            T = H->Next;
            delete H;
            H = T;
        }
        LengthElem = right.LengthElem;
        Element *rightDL = right.H;
        while (rightDL != nullptr) {
            Element *temp = new Element;
            temp->Next = nullptr;
            temp->str = new char[LengthElem];
            strcpy(temp->str, rightDL->str);
            if (H != nullptr) {
                temp->Prev = T;
                T->Next = temp;
                T = temp;
            } else {
                temp->Prev = nullptr;
                H = T = temp;
            }
            rightDL = rightDL->Next;
        }
        return *this;
    }

    StringDL &operator+(StringDL &right) {
        StringDL str(*this);
        Element *temp = str.T;
        if (temp == nullptr) {
            return right;
        }
        int len = LengthDL();
        str.Paste(len, right);
        while (temp->str[0] == '*') {
            temp = temp->Prev;
            temp->Next = nullptr;
        }
        return str;
    }

    ~StringDL() {
        while (H != nullptr) {
            T = H->Next;
            delete H;
            H = T;
        }
    }

    void Print() {
        Element *temp = H;
        while (temp->Next != nullptr) {
            cout << temp->str;
            temp = temp->Next;
        }
        unsigned int i = 0;
        while (temp->str[i] != '*' && i < LengthElem) {
            cout << temp->str[i];
            i++;
        }
        //delete[] temp;
    }

    unsigned int LengthDL() {
        unsigned int length = 0;
        if (H == nullptr) {
            return length;
        }
        Element *temp = H;
        while (temp->Next != nullptr) {
            length++;
            temp = temp->Next;
        }
        length = length * LengthElem;
        unsigned int i = 0;
        while (temp->str[i] != '*' && i < LengthElem) {
            length++;
            i++;
        }
        //delete[] temp;
        return length;
    }

    int PosSub(StringDL &sub) {
        Element *temp = H;
        Element *temp1 = sub.H;
        int count = 0, j = 0, countOfEq = 0;
        int pos;
        while (temp != nullptr) {
            int i = 0;
            for (i; i < LengthElem; i++) {
                if (temp->str[i] == temp1->str[j]) {
                    countOfEq++;
                    pos = i;
                    j++;
                    if (countOfEq == sub.LengthDL()) {
                        return (count * LengthElem) + pos - sub.LengthDL() + 1;
                    }
                    if (j == sub.LengthElem) {
                        temp1 = temp1->Next;
                        j = 0;
                    }
                } else {
                    temp1 = sub.H;
                    j = 0;
                    countOfEq = 0;
                }
            }
            temp = temp->Next;
            count++;
        }
        return -1;
    }

    StringDL SubStr(unsigned int k, unsigned int n) {
        if (k + n > this->LengthDL()) {
            char *res = "";
            return res;
        }
        char *res = new char[n + 1];
        Element *temp = H;
        int num = k / LengthElem;
        while (num != 0) {
            temp = temp->Next;
            num--;
            k -= LengthElem;
        }
        int i = k, j = 0;
        while (n != 0) {
            if (i < LengthElem) {
                res[j] = temp->str[i];
            } else {
                temp = temp->Next;
                i = 0;
                res[j] = temp->str[i];
            }
            j++;
            i++;
            n--;
        }
        res[j] = '\0';
        StringDL str(res);
        return str;
    }

    int DelSub(StringDL &sub) {
        int pos = this->PosSub(sub);
        if (pos == -1) {
            return -1;
        }
        int len = sub.LengthDL(), len1 = LengthDL();
        StringDL paste;
        paste = this->SubStr(pos + len, LengthDL() - (pos + len));
        Paste(pos, paste);
        ToNull(len1 - len, len);
        Element *temp = H;
        if (temp->str[0] == '*') {
            temp = temp->Next;
            while (temp != nullptr) {
                T = temp->Next;
                delete temp;
                temp = T;
            }
            temp = H;
            temp->Next = nullptr;
        } else {
            temp = T;
            while (temp->str[0] == '*' && temp->Prev != nullptr) {
                temp = temp->Prev;
                temp->Next = nullptr;
            }
        }
        return 0;
    }

    void Paste(unsigned int pos, StringDL &paste) {
        Element *temp = H;
        Element *temp1 = paste.H;
        int num = pos / LengthElem;
        while (num != 0) {
            temp = temp->Next;
            num--;
            pos -= LengthElem;
        }
        int i = pos, j = 0, len = paste.LengthDL();
        while (len != 0) {
            if (i < LengthElem) {
                temp->str[i] = temp1->str[j];
            } else {
                if (temp->Next != nullptr) {
                    temp = temp->Next;
                } else {
                    char *str = new char[LengthElem];
                    for (int k = 0; k < LengthElem; k++) {
                        str[k] = '*';
                    }
                    str[LengthElem] = '\0';
                    Add(str);
                    temp = temp->Next;
                }
                i = 0;
                temp->str[i] = temp1->str[j];
            }
            j++;
            if (j == paste.LengthElem) {
                temp1 = temp1->Next;
                j = 0;
            }
            i++;
            len--;
        }
    }
};

void Replace(StringDL &str, StringDL &sub, StringDL &paste) {
    if (str.PosSub(sub) != -1) {
        int pos = str.PosSub(sub);
        StringDL temp;
        temp = str.SubStr(pos, str.LengthDL() - pos);
        str.DelSub(temp);
        str = str + paste;
        temp.DelSub(sub);
        while (temp.PosSub(sub) != -1) {
            pos = temp.PosSub(sub);
            StringDL str1;
            str1 = temp.SubStr(0, pos);
            temp.DelSub(str1);
            temp.DelSub(sub);
            str = str + str1 + paste;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    StringDL str1("The Forest Raised a Christmas Tree.", 10);
    StringDL str2("The Forest Raised a Christmas Tree.", 3);
    Replace(str1, str2, str1 + str2);
    cout << "String 1 after replacing: ";
    str1.Print();
    cout << endl;
    /*cout << "String 1: ";
    str1.Print();
    cout << endl;
    cout << "Dlina stroki: " << str1.LengthDL() << endl;
    StringDL sub("Tree", 3);
    if (str1.PosSub(sub) != -1) {
        cout << "Poziciya podstroki \'";
        sub.Print();
        cout << "\' " << str1.PosSub(sub) << endl;
    } else {
        cout << "Stroka - ";
        sub.Print();
        cout << " ne naydena" << endl;
    }
    sub = str1.SubStr(4, 6);
    cout << "Videlena podstroka: ";
    sub.Print();
    cout << endl;
    StringDL str2(str1);
    str1.DelSub(sub);
    cout << "String 1: ";
    str1.Print();
    cout << endl;
    cout << "String 2: ";
    str2.Print();
    cout << endl;
    str1 = str2;
    cout << "str1 = str2: ";
    str1.Print();
    cout << endl;
    StringDL str3(" End.");
    str1 + str3;
    cout << "str1 + str3: ";
    str1.Print();
    cout << endl;
    StringDL s1("."), s2("...");
    Replace(str1, s1, s2);
    cout << "String 1 after replacing: ";
    str1.Print();
    cout << endl;*/
    int z;
    cin >> z;
    return 0;
}