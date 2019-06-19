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

    void Paste(unsigned int pos, const char *paste) {
        Element *temp = H;
        int num = pos / LengthElem;
        while (num != 0) {
            temp = temp->Next;
            num--;
            pos -= LengthElem;
        }
        int i = pos, j = 0, len = strlen(paste);
        while (len != 0) {
            if (i < LengthElem) {
                temp->str[i] = paste[j];
            } else {
                temp = temp->Next;
                i = 0;
                temp->str[i] = paste[j];
            }
            j++;
            i++;
            len--;
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
        Element *temp = T;
        unsigned int i = 0;
        while (temp->str[i] != '*') {
            i++;
        }
        unsigned int len = LengthElem - i - 1, pos = 0;
        char * paste = new char[len];
        strcpy(paste,right.SubStr(pos,len));
        Paste(LengthDL() - i + 1, paste);
        delete[] paste;
        paste = new char[LengthElem];
        pos+=len;
        while (pos + LengthElem < this->LengthDL() - 1) {
            strcpy(paste, this->SubStr(pos, LengthElem));
            Add(paste);
            pos += LengthElem;
        }

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
        while (temp->str[i] != '*') {
            cout << temp->str[i];
            i++;
        }
        cout << endl;
        //delete[] temp;
    }

    int LengthDL() {
        unsigned int length = 0;
        Element *temp = H;
        while (temp->Next != nullptr) {
            length++;
            temp = temp->Next;
        }
        length = length * LengthElem;
        unsigned int i = 0;
        while (temp->str[i] != '*') {
            length++;
            i++;
        }
        //delete[] temp;
        return length;
    }

    int PosSub(const char *sub) {
        Element *temp = H;
        int count = 0, j = 0;
        int pos;
        while (temp->Next != nullptr) {
            int i = 0;
            for (i; i < LengthElem; i++) {
                if (temp->str[i] == sub[j]) {
                    pos = i;
                    j++;
                    if (j == strlen(sub)) {
                        return (count * LengthElem) + pos - strlen(sub) + 1;
                    }
                } else {
                    j = 0;
                }
            }
            temp = temp->Next;
            count++;
        }
        int i = 0;
        for (i; i < LengthElem; i++) {
            if (temp->str[i] == sub[j]) {
                pos = i;
                j++;
                if (j == strlen(sub)) {
                    return (count * LengthElem) + pos - strlen(sub) + 1;
                }
            } else {
                j = 0;
            }
        }
        return -1;
    }

    char *SubStr(unsigned int k, unsigned int n) {
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
        return res;
    }

    int DelSub(const char *sub) {
        int pos = this->PosSub(sub);
        if (pos == -1) {
            return -1;
        }
        int len = strlen(sub);
        char *paste = new char[len];
        while (pos + len < this->LengthDL() - 1) {
            strcpy(paste, this->SubStr(pos + len, len));
            Paste(pos, paste);
            pos += len;
        }
        if (this->LengthDL() - (pos + len) != 0) {
            strcpy(paste, this->SubStr(pos + len, LengthDL() - (pos + len)));
            for (int i = LengthDL() - (pos + len); i < len; i++) {
                paste[i] = '*';
            }
        } else {
            for (int i = 0; i < len; i++) {
                paste[i] = '*';
            }
        }
        Paste(pos, paste);
        Element *temp = H;
        while (temp->Next != nullptr) {
            temp = temp->Next;
        }
        while (temp->str[0] == '*') {
            temp = temp->Prev;
            temp->Next = nullptr;
        }
        return 0;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    StringDL str1("The Forest Raised a Christmas Tree.", 10);
    cout << "String 1: ";
    str1.Print();
    cout << "Dlina stroki: " << str1.LengthDL() << endl;
    char *sub = "ee";
    if (str1.PosSub(sub) != -1) {
        cout << "Poziciya podstroki \'" << sub << "\' " << str1.PosSub(sub) << endl;
    } else {
        cout << "Stroka - " << sub << " ne naydena" << endl;
    }
    sub = str1.SubStr(4, 6);
    cout << "Videlena podstroka: " << sub << endl;
    StringDL str2(str1);
    str1.DelSub(sub);
    cout << "String 1: ";
    str1.Print();
    cout << "String 2: ";
    str2.Print();
    str1 = str2;
    cout << "str1 = str2: ";
    str1.Print();
    int z;
    cin >> z;
    return 0;
}