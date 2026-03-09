#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cctype>
#include <limits>

using namespace std;

static const char FILL_DUP = 'X'; 
static const char PAD_LAST = 'Y'; 

static inline bool isLetter(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) != 0;
}

string normalizeLettersOnlyUpperIJ(const string& s) {
    string out;
    out.reserve(s.size());
    for (char ch : s) {
        if (!isLetter(ch)) continue;
        char c = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
        if (c == 'J') c = 'I'; 
        out.push_back(c);
    }
    return out;
}

void buildPlayfairMatrix(const string& keyInput,
    char mat[5][5],
    pair<int, int> pos[26]) {
    string key = normalizeLettersOnlyUpperIJ(keyInput);

    vector<bool> used(26, false);
    used['J' - 'A'] = true; 
    string seq;
    seq.reserve(25);

    // thêm key 
    for (char c : key) {
        int idx = c - 'A';
        if (!used[idx]) {
            used[idx] = true;
            seq.push_back(c);
        }
    }

    // thêm phần còn lại
    for (char c = 'A'; c <= 'Z'; ++c) {
        if (c == 'J') continue;
        int idx = c - 'A';
        if (!used[idx]) {
            used[idx] = true;
            seq.push_back(c);
        }
    }

    int k = 0;
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            mat[r][c] = seq[k++];
        }
    }

    for (int i = 0; i < 26; ++i) pos[i] = make_pair(-1, -1);

    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            char ch = mat[r][c];
            pos[ch - 'A'] = make_pair(r, c);
        }
    }
    pos['J' - 'A'] = pos['I' - 'A'];
}

void printMatrix(const char mat[5][5]) {
    cout << "\nMa tran Playfair 5x5:\n";
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            cout << mat[r][c] << (c == 4 ? '\n' : ' ');
        }
    }
}

// Chuẩn bị plaintext để mã hoá
string preparePlaintextForEncrypt(const string& plainInput) {
    string p = normalizeLettersOnlyUpperIJ(plainInput);

    string out;
    out.reserve(p.size() + 10);

    for (size_t i = 0; i < p.size(); ) {
        char a = p[i];
        char b = 0;

        if (i + 1 < p.size()) b = p[i + 1];

        if (i + 1 >= p.size()) {
            // còn 1 ký tự uối 
            out.push_back(a);
            out.push_back(PAD_LAST);
            break;
        }
        if (a == b) {
            // trùng chữ trong 1 cặp 
            out.push_back(a);
            out.push_back(FILL_DUP);
            i += 1;
        }
        else {
            out.push_back(a);
            out.push_back(b);
            i += 2;
        }
    }

    if (out.size() % 2 == 1) out.push_back(PAD_LAST);
    return out;
}

string encryptPlayfair(const string& plainInput,
    const char mat[5][5],
    const pair<int, int> pos[26]) {
    string p = preparePlaintextForEncrypt(plainInput);

    string c;
    c.reserve(p.size());

    for (size_t i = 0; i < p.size(); i += 2) {
        char a = p[i];
        char b = p[i + 1];

        auto p1 = pos[a - 'A'];
        auto p2 = pos[b - 'A'];
        int r1 = p1.first, c1 = p1.second;
        int r2 = p2.first, c2 = p2.second;

        if (r1 == r2) {
            // cùng hàng
            c.push_back(mat[r1][(c1 + 1) % 5]);
            c.push_back(mat[r2][(c2 + 1) % 5]);
        }
        else if (c1 == c2) {
            // cùng cột
            c.push_back(mat[(r1 + 1) % 5][c1]);
            c.push_back(mat[(r2 + 1) % 5][c2]);
        }
        else {
            // hình chữ nhật
            c.push_back(mat[r1][c2]);
            c.push_back(mat[r2][c1]);
        }
    }

    return c;
}

string decryptPlayfairRaw(const string& cipherInput,
    const char mat[5][5],
    const pair<int, int> pos[26]) {
    string c = normalizeLettersOnlyUpperIJ(cipherInput);
    if (c.size() % 2 == 1) c.push_back(FILL_DUP); // trường hợp lẻ

    string p;
    p.reserve(c.size());

    for (size_t i = 0; i < c.size(); i += 2) {
        char a = c[i];
        char b = c[i + 1];

        auto p1 = pos[a - 'A'];
        auto p2 = pos[b - 'A'];
        int r1 = p1.first, c1 = p1.second;
        int r2 = p2.first, c2 = p2.second;

        if (r1 == r2) {
            // cùng hàng
            p.push_back(mat[r1][(c1 + 4) % 5]);
            p.push_back(mat[r2][(c2 + 4) % 5]);
        }
        else if (c1 == c2) {
            // cùng cột
            p.push_back(mat[(r1 + 4) % 5][c1]);
            p.push_back(mat[(r2 + 4) % 5][c2]);
        }
        else {
            // hình chữ nhật
            p.push_back(mat[r1][c2]);
            p.push_back(mat[r2][c1]);
        }
    }
    return p;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string key;
    cout << "Nhap Key: ";
    getline(cin, key);

    char mat[5][5];
    pair<int, int> pos[26];
    buildPlayfairMatrix(key, mat, pos);
    printMatrix(mat);

    cout << "\nChon che do:\n";
    cout << "1) Ma hoa (Encrypt)\n";
    cout << "2) Giai ma (Decrypt)\n";
    cout << "Lua chon: ";

    int mode = 0;
    cin >> mode;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    if (mode == 1) {
        string plain;
        cout << "\nNhap Plaintext: ";
        getline(cin, plain);

        string cipher = encryptPlayfair(plain, mat, pos);

        cout << "\nCIPHERTEXT:\n" << cipher << endl;
    }
    else if (mode == 2) {
        string cipher;
        cout << "\nNhap CIPHERTEXT: ";
        getline(cin, cipher);

        string raw = decryptPlayfairRaw(cipher, mat, pos);

        cout << "\nPlaintext: " << raw << endl;

    }
    else {
        cout << "Lua chon khong hop le.\n";
    }

    return 0;
}