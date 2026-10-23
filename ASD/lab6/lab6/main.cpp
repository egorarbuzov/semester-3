#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;

struct Node {
    char symbol;
    int  freq;
    Node *left;
    Node *right;
};

Node* makeNode(char sym, int freq) {
    Node* n = new Node;
    n->symbol = sym;
    n->freq   = freq;
    n->left   = 0;
    n->right  = 0;
    return n;
}

void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

void buildCodes(Node* root, char* code, int len,
                char codes[256][256], int lengths[256]) {
    if (!root) return;

    if (!root->left && !root->right) {
        code[len] = '\0';
        strcpy(codes[(unsigned char)root->symbol], code);
        lengths[(unsigned char)root->symbol] = len;
        return;
    }

    code[len] = '0';
    buildCodes(root->left, code, len + 1, codes, lengths);

    code[len] = '1';
    buildCodes(root->right, code, len + 1, codes, lengths);
}

void findTwoMin(Node** nodes, int count, int& i1, int& i2) {
    i1 = -1; i2 = -1;
    for (int i = 0; i < count; i++) {
        if (i1 == -1 || nodes[i]->freq < nodes[i1]->freq) {
            i2 = i1;
            i1 = i;
        } else if (i2 == -1 || nodes[i]->freq < nodes[i2]->freq) {
            i2 = i;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    char text[4096];
    cout << "Введите текст: ";
    cin.getline(text, sizeof(text));

    int textLen = strlen(text);
    if (textLen == 0) {
        cout << "Текст пуст!" << endl;
        return 0;
    }

    int freq[256];
    for (int i = 0; i < 256; i++) freq[i] = 0;
    for (int i = 0; i < textLen; i++) {
        freq[(unsigned char)text[i]]++;
    }

    Node* nodes[256];
    int nodeCount = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            nodes[nodeCount++] = makeNode((char)i, freq[i]);
        }
    }

    if (nodeCount == 1) {
        cout << "\nВ тексте только один уникальный символ." << endl;
        cout << "Его код: 0" << endl;
        cout << "\nВыходная последовательность:" << endl;
        for (int i = 0; i < textLen; i++) cout << '0';
        cout << endl;
        deleteTree(nodes[0]);
        return 0;
    }

    while (nodeCount > 1) {
        int i1, i2;
        findTwoMin(nodes, nodeCount, i1, i2);

        Node* left  = nodes[i1];
        Node* right = nodes[i2];
        Node* parent = makeNode('\0', left->freq + right->freq);
        parent->left  = left;
        parent->right = right;

        if (i1 > i2) { int t = i1; i1 = i2; i2 = t; }

        nodes[i1] = nodes[nodeCount - 1];
        nodes[i2] = nodes[nodeCount - 2];
        nodeCount -= 2;

        nodes[nodeCount++] = parent;
    }

    Node* root = nodes[0];

    char codes[256][256];
    int  lengths[256];
    for (int i = 0; i < 256; i++) {
        codes[i][0] = '\0';
        lengths[i]  = 0;
    }
    char buffer[256];
    buildCodes(root, buffer, 0, codes, lengths);

    cout << "\n--- Таблица встречаемости символов ---\n";
    cout << "Символ\tЧастота\tПроцент\n";
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            char sym = (char)i;
            double percent = 100.0 * freq[i] / textLen;
            if (sym == ' ')       cout << "' '"    << "\t";
            else if (sym == '\t') cout << "'\\t'"  << "\t";
            else                  cout << sym      << "\t";
            cout << freq[i] << "\t" << percent << "%\n";
        }
    }

    cout << "\n--- Таблица кодов ---\n";
    cout << "Символ\tКод\n";
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            char sym = (char)i;
            if (sym == ' ')       cout << "' '"    << "\t";
            else if (sym == '\t') cout << "'\\t'"  << "\t";
            else                  cout << sym      << "\t";
            cout << codes[i] << "\n";
        }
    }

    cout << "\n--- Выходная последовательность ---\n";
    for (int i = 0; i < textLen; i++) {
        cout << codes[(unsigned char)text[i]];
    }
    cout << endl;

    int totalBits = 0;
    for (int i = 0; i < textLen; i++) {
        totalBits += lengths[(unsigned char)text[i]];
    }
    cout << "\nИсходный размер (ASCII): " << textLen * 8 << " бит\n";
    cout << "Сжатый размер:           " << totalBits << " бит\n";
    cout << "Коэффициент сжатия:      "
         << (double)(textLen * 8) / totalBits << endl;

    deleteTree(root);
    return 0;
}
