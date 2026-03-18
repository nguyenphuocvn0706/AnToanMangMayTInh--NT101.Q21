#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <unordered_set>

using namespace std;

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
mt19937 gen(random_device{}());

const double bgScoreMatrix[26][26] = 
{
    {-3.56,-3.14,-2.45,-2.44,-3.69,-3.39,-3.12,-4.00,-2.68,-4.67,-3.02,-2.14,-2.36,-1.82,-3.96,-2.87,-4.81,-2.05,-2.06,-2.00,-3.21,-2.85,-3.03,-4.29,-2.48,-4.76},
    {-2.82,-3.57,-4.59,-4.81,-2.34,-4.66,-4.68,-4.67,-3.01,-4.93,-4.93,-3.00,-4.58,-4.69,-2.61,-4.73,-4.93,-2.99,-3.31,-4.06,-2.78,-4.93,-4.81,-4.93,-2.61,-4.93},
    {-2.24,-4.62,-3.13,-4.68,-2.19,-4.68,-4.66,-1.80,-2.87,-4.93,-2.90,-2.90,-4.56,-4.66,-2.17,-4.65,-4.42,-3.11,-4.68,-2.68,-2.80,-4.93,-4.68,-4.93,-3.87,-4.93},
    {-2.99,-4.44,-4.53,-3.45,-2.23,-4.15,-3.95,-4.57,-2.34,-4.63,-4.67,-4.56,-4.34,-4.45,-2.98,-4.60,-4.93,-3.90,-3.08,-4.57,-3.38,-4.46,-4.18,-4.93,-3.69,-4.93},
    {-2.60,-3.40,-2.53,-2.23,-2.72,-3.05,-3.48,-4.05,-2.93,-4.45,-4.11,-2.40,-2.58,-2.13,-3.91,-3.19,-4.09,-1.85,-2.04,-2.53,-3.83,-2.98,-2.96,-3.65,-2.83,-4.84},
    {-2.91,-4.34,-4.31,-4.55,-2.71,-2.87,-4.54,-4.64,-2.53,-4.93,-4.93,-3.54,-4.51,-4.59,-2.37,-4.63,-4.93,-3.04,-4.50,-2.99,-3.09,-4.93,-4.66,-4.93,-4.48,-4.93},
    {-2.74,-4.46,-4.59,-4.57,-2.42,-4.49,-4.44,-2.39,-2.76,-4.93,-4.93,-3.55,-4.53,-4.55,-2.81,-4.62,-4.93,-2.84,-3.32,-3.53,-3.21,-4.93,-4.21,-4.93,-4.44,-4.93},
    {-2.12,-4.47,-4.54,-4.60,-1.51,-4.50,-4.60,-4.57,-2.04,-4.93,-4.93,-4.43,-4.28,-4.52,-2.44,-4.62,-4.93,-4.42,-4.60,-3.40,-3.69,-4.93,-4.50,-4.93,-3.97,-4.93},
    {-3.03,-3.58,-2.37,-2.75,-2.92,-3.07,-2.68,-4.10,-4.00,-4.65,-3.47,-2.54,-2.52,-1.67,-2.67,-3.25,-4.34,-2.89,-2.06,-2.09,-3.89,-2.92,-4.47,-4.19,-4.63,-4.12},
    {-2.82,-4.93,-4.93,-4.93,-2.65,-4.93,-4.93,-4.93,-4.04,-4.93,-4.93,-4.93,-4.93,-4.93,-2.67,-4.93,-4.93,-4.93,-4.93,-4.93,-2.53,-4.93,-4.93,-4.93,-4.93,-4.93},
    {-3.68,-4.77,-4.93,-4.93,-2.31,-4.93,-4.93,-4.68,-3.06,-4.93,-4.93,-4.93,-4.93,-4.81,-3.89,-4.93,-4.93,-4.93,-3.39,-4.79,-4.93,-4.93,-4.93,-4.93,-3.69,-4.93},
    {-2.65,-4.48,-4.31,-3.23,-2.11,-3.59,-4.47,-4.51,-2.33,-4.93,-4.31,-2.28,-4.40,-4.51,-2.80,-4.36,-4.93,-4.47,-3.59,-3.35,-3.35,-3.91,-4.32,-4.93,-2.57,-4.93},
    {-2.17,-3.21,-4.63,-4.68,-2.00,-4.61,-4.65,-4.65,-2.48,-4.93,-4.93,-4.60,-3.57,-4.68,-2.53,-2.95,-4.93,-4.64,-3.33,-4.66,-3.26,-4.93,-4.63,-4.93,-3.61,-4.93},
    {-2.78,-4.25,-2.89,-2.09,-2.23,-3.86,-2.31,-4.43,-2.83,-4.80,-3.72,-4.17,-4.30,-4.38,-2.61,-4.32,-4.77,-4.53,-2.72,-2.16,-3.57,-3.77,-4.21,-4.93,-3.45,-4.93},
    {-3.41,-3.30,-3.19,-3.01,-3.75,-2.84,-3.59,-4.21,-3.52,-4.78,-3.19,-2.65,-2.31,-2.08,-2.67,-2.87,-4.66,-2.03,-2.38,-2.51,-2.30,-2.85,-2.98,-4.23,-3.94,-4.87},
    {-2.34,-4.66,-4.64,-4.66,-2.07,-4.63,-4.66,-3.48,-2.99,-4.93,-4.93,-2.63,-4.57,-4.64,-2.45,-3.25,-4.93,-2.43,-3.63,-3.24,-3.14,-4.93,-4.63,-4.93,-4.63,-4.93},
    {-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-1.82,-4.93,-4.93,-4.93,-4.93,-4.93},
    {-2.34,-4.28,-3.26,-2.96,-1.79,-3.90,-3.52,-4.41,-2.29,-4.93,-3.32,-4.12,-3.21,-3.33,-2.48,-3.93,-4.93,-4.44,-2.64,-3.05,-3.55,-3.42,-4.47,-4.93,-2.80,-4.93},
    {-2.65,-3.70,-2.99,-4.15,-2.21,-3.84,-4.37,-2.19,-2.60,-4.79,-3.63,-3.67,-3.33,-3.97,-2.69,-3.01,-4.40,-4.28,-2.57,-1.89,-2.80,-4.46,-3.43,-4.93,-3.59,-4.93},
    {-2.52,-4.40,-4.13,-4.51,-1.98,-4.34,-4.48,-1.42,-2.17,-4.93,-4.78,-3.87,-4.16,-4.43,-2.17,-4.35,-4.88,-3.33,-2.84,-3.02,-3.05,-4.53,-3.54,-4.93,-3.04,-4.93},
    {-3.56,-3.36,-2.88,-3.53,-3.41,-3.95,-3.15,-4.68,-3.57,-4.93,-4.66,-2.54,-3.25,-2.49,-4.02,-2.94,-4.93,-2.23,-2.32,-2.31,-4.63,-4.61,-4.60,-4.44,-4.64,-4.93},
    {-2.67,-4.93,-4.93,-4.93,-1.72,-4.93,-4.93,-4.93,-2.27,-4.93,-4.93,-4.93,-4.93,-4.93,-3.40,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.80,-4.93},
    {-2.14,-4.69,-4.66,-4.67,-2.27,-4.66,-4.67,-2.72,-2.43,-4.93,-4.93,-4.64,-4.63,-3.58,-2.32,-4.66,-4.93,-4.63,-4.04,-4.64,-4.63,-4.93,-4.66,-4.93,-4.63,-4.93},
    {-3.29,-4.93,-3.23,-4.93,-3.57,-4.93,-4.93,-4.93,-3.26,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-3.14,-4.93,-4.93,-4.93,-3.14,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93},
    {-3.82,-3.63,-3.44,-3.97,-3.22,-3.72,-4.44,-4.50,-4.24,-4.93,-4.60,-4.47,-3.30,-4.48,-3.15,-3.56,-4.93,-4.47,-2.85,-3.65,-4.64,-4.93,-4.03,-4.93,-4.93,-4.93},
    {-3.29,-4.93,-4.93,-4.93,-2.68,-4.93,-4.93,-4.93,-3.82,-4.93,-4.93,-4.93,-4.93,-4.93,-3.58,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-4.93,-3.82}
};

string normalizeText(const string& text) 
{
    string res;
    for (char c : text) 
    {
        if (isalpha(c)) res += toupper(c);
    }
    return res;
}

string restoreFormatting(const string& originalText, const string& decryptedNoSpaces) 
{
    string result = "";
    int idex = 0;
    for (char c : originalText) 
    {
        if (isalpha(c)) 
        {
            result += islower(c) ? tolower(decryptedNoSpaces[idex]) : decryptedNoSpaces[idex];
            idex++;
        }
        else 
        {
            result += c;
        }
    }
    return result;
}

string decrypt(const string& cipher, const string& key) 
{
    char decryptMap[256];
    for (int i = 0; i < 26; i++) 
    {
        decryptMap[key[i]] = ALPHABET[i];
    }
    string result = cipher;
    for (int i = 0; i < cipher.size(); i++) 
    {
        result[i] = decryptMap[cipher[i]];
    }
    return result;
}

string generateRandomKey()
{
    string key = ALPHABET;
    shuffle(key.begin(), key.end(), gen);
    return key;
}

double calculateFitness(const string& text)
{
    double score = 0;
    int len = text.size();
    for (int i = 0; i < len - 1; i++)
    {
        int r = text[i] - 'A';
        int c = text[i + 1] - 'A';
        score += bgScoreMatrix[r][c];
    }
    static const unordered_set<string> genericQuadgrams =
    {
        "THAT", "THER", "WITH", "TION", "HERE", "OULD", "MENT", "THEI", "FROM",
        "THIS", "THIN", "TING", "WHIC", "EVER", "OUGH", "SOME", "TIME", "HAVE",
        "THEM", "LIKE", "WHEN", "WHAT", "OVER", "THEY", "WILL", "MORE", "ABOUT",
        "YOUR", "BEEN", "WERE", "MUCH", "UPON", "OFTH", "FTHE", "ANDT", "NDTH"
    };
    static const unordered_set<string> genericTrigrams =
    {
        "THE", "AND", "ING", "ENT", "ION", "HER", "FOR", "THA", "NTH", "INT",
        "TIO", "TER", "EST", "ERS", "ATI", "HAT", "ATE", "ALL", "ETH", "HES",
        "VER", "OFT", "ITH", "FRO", "THI", "WIT", "ARE", "ESS", "NOT", "IVE"
    };
    for (int i = 0; i < len - 2; i++)
    {
        if (genericTrigrams.count(text.substr(i, 3))) score += 2.0;
    }
    for (int i = 0; i < len - 3; i++)
    {
        if (genericQuadgrams.count(text.substr(i, 4))) score += 4.0;
    }
    return score;
}

pair<string, string> hillClimb(const string& cipher)
{
    string currentKey = generateRandomKey();
    string currentPlain = decrypt(cipher, currentKey);
    double currentScore = calculateFitness(currentPlain);
    bool improved = true;
    while (improved)
    {
        improved = false;
        for (int i = 0; i < 25; i++)
        {
            for (int j = i + 1; j < 26; j++)
            {
                string testKey = currentKey;
                swap(testKey[i], testKey[j]);
                string testPlain = decrypt(cipher, testKey);
                double testScore = calculateFitness(testPlain);
                if (testScore > currentScore)
                {
                    currentScore = testScore;
                    currentKey = testKey;
                    currentPlain = testPlain;
                    improved = true;
                }
            }
        }
    }
    return { currentKey, currentPlain };
}

void solveCipher(const string& cipher, const string& originalCipher)
{
    int restarts = 10;
    string bestGlobalKey;
    string bestGlobalPlain;
    double bestGlobalScore = -1e9;
    cout << "Wait a minute...\n";
    for (int i = 0; i < restarts; i++)
    {
        auto result = hillClimb(cipher);
        double sc = calculateFitness(result.second);
        if (sc > bestGlobalScore)
        {
            bestGlobalScore = sc;
            bestGlobalKey = result.first;
            bestGlobalPlain = result.second;
        }
    }
    string finalResult = restoreFormatting(originalCipher, bestGlobalPlain);
    cout << "Plaintext:\n" << finalResult << "\n";
}

int main()
{
    cout << "Enter ciphertext:\n";
    string originalCipher;
    getline(cin, originalCipher);
    string cipher = normalizeText(originalCipher);
    if (cipher.empty())
    {
        cout << "Loi: Ciphertext khong chua chu cai hop le.\n";
        return 0;
    }
    solveCipher(cipher, originalCipher);
    return 0;
}