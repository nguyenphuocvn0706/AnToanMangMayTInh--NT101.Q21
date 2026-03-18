#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <cmath>

using namespace std;

struct KeyLengthScore {
    int keyLen;
    double avgIC;
};

struct ShiftScore {
    int shift;
    double score;
};

struct Result {
    int keyLen;
    string key;
    string plaintext;
    double score;
};

const vector<double> EN_FREQ = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015,
    6.094, 6.966, 0.153, 0.772, 4.025, 2.406, 6.749,
    7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758,
    0.978, 2.360, 0.150, 1.974, 0.074
};

string normalizeText(const string& text) {
    string res;
    for (char ch : text) {
        if (isalpha(static_cast<unsigned char>(ch))) {
            res.push_back(static_cast<char>(toupper(ch)));
        }
    }
    return res;
}

vector<int> countLetters(const string& s) {
    vector<int> cnt(26, 0);
    for (char ch : s) cnt[ch - 'A']++;
    return cnt;
}

double indexOfCoincidence(const string& s) {
    int n = (int)s.size();
    if (n <= 1) return 0.0;

    vector<int> freq = countLetters(s);
    long long numerator = 0;
    for (int f : freq) numerator += 1LL * f * (f - 1);

    return (double)numerator / (1LL * n * (n - 1));
}

vector<string> splitIntoGroups(const string& text, int keyLen) {
    vector<string> groups(keyLen);
    for (int i = 0; i < (int)text.size(); i++) {
        groups[i % keyLen].push_back(text[i]);
    }
    return groups;
}

vector<KeyLengthScore> estimateKeyLengths(const string& cipher, int maxKeyLen) {
    vector<KeyLengthScore> scores;

    for (int k = 1; k <= maxKeyLen; k++) {
        vector<string> groups = splitIntoGroups(cipher, k);
        double sumIC = 0.0;
        int validGroups = 0;

        for (const string& g : groups) {
            if (g.size() >= 2) {
                sumIC += indexOfCoincidence(g);
                validGroups++;
            }
        }

        double avgIC = (validGroups > 0 ? sumIC / validGroups : 0.0);
        scores.push_back({ k, avgIC });
    }

    sort(scores.begin(), scores.end(), [](const KeyLengthScore& a, const KeyLengthScore& b) {
        if (fabs(a.avgIC - b.avgIC) > 1e-12) return a.avgIC > b.avgIC;
        return a.keyLen < b.keyLen;
        });

    return scores;
}

string caesarDecrypt(const string& s, int shift) {
    string res = s;
    for (char& ch : res) {
        int x = ch - 'A';
        x = (x - shift + 26) % 26;
        ch = char('A' + x);
    }
    return res;
}

double chiSquaredScore(const string& s) {
    int n = (int)s.size();
    if (n == 0) return 1e18;

    vector<int> observed = countLetters(s);
    double score = 0.0;

    for (int i = 0; i < 26; i++) {
        double expected = EN_FREQ[i] * n / 100.0;
        if (expected > 0) {
            double diff = observed[i] - expected;
            score += diff * diff / expected;
        }
    }

    return score;
}

double englishLikeScore(const string& text) {
    double score = chiSquaredScore(text);

    vector<string> commonWords = {
        "THE", "AND", "ING", "ION", "ENT", "HER", "FOR", "THA", "NTH",
        "INT", "ERE", "TIO", "TER", "EST", "ERS", "ATI", "HAT", "ATE"
    };

    int bonus = 0;
    for (const string& w : commonWords) {
        size_t pos = text.find(w);
        while (pos != string::npos) {
            bonus++;
            pos = text.find(w, pos + 1);
        }
    }

    return score - bonus * 2.5;
}

vector<ShiftScore> topShiftsForGroup(const string& group, int topN) {
    vector<ShiftScore> all;

    for (int shift = 0; shift < 26; shift++) {
        string dec = caesarDecrypt(group, shift);
        double score = chiSquaredScore(dec);
        all.push_back({ shift, score });
    }

    sort(all.begin(), all.end(), [](const ShiftScore& a, const ShiftScore& b) {
        return a.score < b.score;
        });

    if ((int)all.size() > topN) all.resize(topN);
    return all;
}

string vigenereDecrypt(const string& cipher, const string& key) {
    string plain = cipher;
    int m = (int)key.size();

    for (int i = 0; i < (int)cipher.size(); i++) {
        int c = cipher[i] - 'A';
        int k = key[i % m] - 'A';
        plain[i] = char('A' + (c - k + 26) % 26);
    }

    return plain;
}

void generateKeysDFS(
    int pos,
    const vector<vector<ShiftScore>>& bestShifts,
    string& currentKey,
    vector<string>& keys,
    int limitKeys
) {
    if ((int)keys.size() >= limitKeys) return;

    if (pos == (int)bestShifts.size()) {
        keys.push_back(currentKey);
        return;
    }

    for (const auto& cand : bestShifts[pos]) {
        currentKey.push_back(char('A' + cand.shift));
        generateKeysDFS(pos + 1, bestShifts, currentKey, keys, limitKeys);
        currentKey.pop_back();

        if ((int)keys.size() >= limitKeys) return;
    }
}

vector<string> generateCandidateKeys(const string& cipher, int keyLen, int topShiftPerGroup, int limitKeys) {
    vector<string> groups = splitIntoGroups(cipher, keyLen);
    vector<vector<ShiftScore>> bestShifts;

    for (const string& g : groups) {
        bestShifts.push_back(topShiftsForGroup(g, topShiftPerGroup));
    }

    vector<string> keys;
    string currentKey;
    generateKeysDFS(0, bestShifts, currentKey, keys, limitKeys);
    return keys;
}

vector<Result> crackVigenereUnknownKeyLength(
    const string& rawCipher,
    int maxKeyLen,
    int topKeyLenToTry,
    int topShiftPerGroup,
    int maxKeysPerLength,
    int topResultsToShow
) {
    string cipher = normalizeText(rawCipher);
    vector<Result> results;

    if (cipher.empty()) return results;

    vector<KeyLengthScore> keyLenScores = estimateKeyLengths(cipher, maxKeyLen);

    int lengthsToTry = min(topKeyLenToTry, (int)keyLenScores.size());

    for (int i = 0; i < lengthsToTry; i++) {
        int keyLen = keyLenScores[i].keyLen;

        vector<string> candidateKeys = generateCandidateKeys(
            cipher, keyLen, topShiftPerGroup, maxKeysPerLength
        );

        for (const string& key : candidateKeys) {
            string plain = vigenereDecrypt(cipher, key);
            double score = englishLikeScore(plain);
            results.push_back({ keyLen, key, plain, score });
        }
    }

    sort(results.begin(), results.end(), [](const Result& a, const Result& b) {
        return a.score < b.score;
        });

    vector<Result> uniqueResults;
    for (const auto& r : results) {
        bool duplicated = false;
        for (const auto& u : uniqueResults) {
            if (u.plaintext == r.plaintext) {
                duplicated = true;
                break;
            }
        }
        if (!duplicated) uniqueResults.push_back(r);
        if ((int)uniqueResults.size() >= topResultsToShow) break;
    }

    return uniqueResults;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string rawCipher;
    cout << "Nhap ciphertext: ";
    getline(cin, rawCipher);

    int maxKeyLen;
    cout << "\nNhap do dai khoa toi da muon thu: ";
    cin >> maxKeyLen;

    string cipher = normalizeText(rawCipher);
    if (cipher.empty()) {
        cout << "Ciphertext khong hop le sau khi chuan hoa.\n";
        return 0;
    }

    vector<KeyLengthScore> keyLenScores = estimateKeyLengths(cipher, maxKeyLen);

    cout << "\nUoc luong do dai khoa bang Index of Coincidence:\n";
    cout << left << setw(12) << "KeyLen" << "Avg IC\n";
    cout << "-------------------------\n";
    for (int i = 0; i < min(10, (int)keyLenScores.size()); i++) {
        cout << left << setw(12) << keyLenScores[i].keyLen
            << fixed << setprecision(6) << keyLenScores[i].avgIC << "\n";
    }

    int topKeyLenToTry = min(5, maxKeyLen);
    int topShiftPerGroup = 3;
    int maxKeysPerLength = 300;
    int topResultsToShow = 10;

    vector<Result> results = crackVigenereUnknownKeyLength(
        rawCipher,
        maxKeyLen,
        topKeyLenToTry,
        topShiftPerGroup,
        maxKeysPerLength,
        topResultsToShow
    );

    if (results.empty()) {
        cout << "\nKhong tim duoc ket qua.\n";
        return 0;
    }

    cout << "\nPhuong an giai ma tot nhat:\n \n";
    cout << "Do dai khoa: " << results[0].keyLen << "\n";
    cout << "Khoa du doan: " << results[0].key << "\n";
    cout << "Diem danh gia: " << fixed << setprecision(4) << results[0].score << "\n";
    cout << "Plaintext:\n" << results[0].plaintext << "\n";

    return 0;
}
