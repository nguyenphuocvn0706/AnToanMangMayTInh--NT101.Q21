#include <iostream>
#include <vector>
#include <string>

using namespace std;

string encrypt(string text, vector<int> perm) {

    int d = perm.size();
    string result = text;

    while (result.size() % d != 0)
        result += 'X';

    string cipher = "";

    for (int i = 0; i < result.size(); i += d) {
        for (int j = 0; j < d; j++) {
            cipher += result[i + perm[j] - 1];
        }
    }

    return cipher;
}

vector<int> inversePermutation(vector<int> perm) {

    int d = perm.size();
    vector<int> inv(d);

    for (int i = 0; i < d; i++) {
        inv[perm[i] - 1] = i + 1;
    }

    return inv;
}

string decrypt(string cipher, vector<int> perm) {

    int d = perm.size();
    vector<int> inv = inversePermutation(perm);

    string plaintext = "";

    for (int i = 0; i < cipher.size(); i += d) {
        for (int j = 0; j < d; j++) {
            plaintext += cipher[i + inv[j] - 1];
        }
    }

    return plaintext;
}

int main() {

    string plaintext;
    int d;

    cout << "Nhap plaintext: ";
    getline(cin, plaintext);

    cout << "Nhap bac hoan vi d: ";
    cin >> d;

    vector<int> perm(d);

    cout << "Nhap hoan vi h: ";
    for (int i = 0; i < d; i++)
        cin >> perm[i];

    string cipher = encrypt(plaintext, perm);

    cout << "\nCiphertext: " << cipher << endl;

    string plain = decrypt(cipher, perm);

    cout << "Pha ma: " << plain << endl;

}