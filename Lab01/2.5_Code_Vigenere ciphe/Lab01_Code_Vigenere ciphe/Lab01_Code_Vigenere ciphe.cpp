#include <iostream>
#include <string>
#include <cctype>
#include <limits>

using namespace std;

// Lấy khóa chỉ gồm chữ cái, chuyển về in hoa
string normalizeKey(const string& key) {
    string k;
    for (char ch : key) {
        if (isalpha(static_cast<unsigned char>(ch))) {
            k.push_back(static_cast<char>(toupper(static_cast<unsigned char>(ch))));
        }
    }
    return k;
}

// Mã hóa Vigenere
string vigenereEncrypt(const string& plain, const string& keyInput) {
    string key = normalizeKey(keyInput);
    if (key.empty()) return "";

    string cipher = plain;
    int keyIndex = 0;

    for (size_t i = 0; i < plain.size(); ++i) {
        char ch = plain[i];

        if (isalpha(static_cast<unsigned char>(ch))) {
            char base = isupper(static_cast<unsigned char>(ch)) ? 'A' : 'a';
            int p = ch - base;
            int k = key[keyIndex % key.size()] - 'A';

            cipher[i] = static_cast<char>((p + k) % 26 + base);
            keyIndex++;
        }
        else {
            cipher[i] = ch; 
        }
    }

    return cipher;
}

// Giải mã Vigenere
string vigenereDecrypt(const string& cipher, const string& keyInput) {
    string key = normalizeKey(keyInput);
    if (key.empty()) return "";

    string plain = cipher;
    int keyIndex = 0;

    for (size_t i = 0; i < cipher.size(); ++i) {
        char ch = cipher[i];

        if (isalpha(static_cast<unsigned char>(ch))) {
            char base = isupper(static_cast<unsigned char>(ch)) ? 'A' : 'a';
            int c = ch - base;
            int k = key[keyIndex % key.size()] - 'A';

            plain[i] = static_cast<char>((c - k + 26) % 26 + base);
            keyIndex++;
        }
        else {
            plain[i] = ch;
        }
    }

    return plain;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string key;
    cout << "Nhap khoa: ";
    getline(cin, key);

    if (normalizeKey(key).empty()) {
        cout << "Khoa khong hop le. Khoa phai co it nhat 1 chu cai.\n";
        return 0;
    }

    cout << "\nChon che do:\n";
    cout << "1) Ma hoa\n";
    cout << "2) Giai ma\n";
    cout << "Lua chon: ";

    int mode;
    cin >> mode;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (mode == 1) {
        string plain;
        cout << "\nNhap ban ro: ";
        getline(cin, plain);

        string cipher = vigenereEncrypt(plain, key);
        cout << "\nCIPHERTEXT:\n" << cipher << "\n";
    }
    else if (mode == 2) {
        string cipher;
        cout << "\nNhap ciphertext: ";
        getline(cin, cipher);

        string plain = vigenereDecrypt(cipher, key);
        cout << "\nPLAINTEXT: " << plain << "\n";
    }
    else {
        cout << "Lua chon khong hop le.\n";
    }

    return 0;
}