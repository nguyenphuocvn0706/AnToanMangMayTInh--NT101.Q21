#include <iostream>
#include <string>

using namespace std;

string encrypt(string plaintext, int k)
{
    string ciphertext = "";
    for (int i = 0; i < plaintext.length(); i++)
    {
        char C = plaintext[i];
        if (isupper(C))
        {
            ciphertext.push_back((C + k - 65) % 26 + 65); // Vì A = 65 nên thay thẳng vào luôn
        }
        else if (islower(C))
        {
            ciphertext.push_back((C + k - 97) % 26 + 97); // Vì a = 97 nên thay thẳng vào luôn
        }
        else
        {
            ciphertext.push_back(C);
        }
    }
    return ciphertext;
}

string decrypt(string ciphertext, int k)
{
    string plaintext = "";
    for (int i = 0; i < ciphertext.length(); i++)
    {
        char C = ciphertext[i];
        if (isupper(C))
        {
            plaintext.push_back((C - k - 65 + 26) % 26 + 65); // Vì A = 65 nên thay thẳng vào luôn
        }
        else if (islower(C))
        {
            plaintext.push_back((C - k - 97 + 26) % 26 + 97); // Vì a = 97 nên thay thẳng vào luôn
        }
        else
        {
            plaintext.push_back(C);
        }
    }
    return plaintext;
}

void bruteforce(string ciphertext)
{
    cout << "Result: \n";
    for (int k = 1; k < 26; k++)
    {
        string plaintext = decrypt(ciphertext, k);
        cout << "K = " << k << ": " << plaintext << endl;
    }
}

int main()
{
    string plaintext;
    string ciphertext;
    string choice;
    int key;
    cout << "What do you want?\n";
    cout << "encrypt " << "decrypt " << "Brute-force\n";
    cin >> choice;
    cin.ignore();
    if (choice == "encrypt")
    {
        cout << "Enter plaintext: ";
        getline(cin, plaintext);
        cout << "Enter key:";
        cin >> key;
        cout << encrypt(plaintext, key);
    }
    else if (choice == "decrypt")
    {
        cout << "Enter ciphertext: ";
        getline(cin, ciphertext);
        cout << "Enter key:";
        cin >> key;
        cout << decrypt(ciphertext, key);
    }
    else if (choice == "Brute-force")
    {
        cout << "Enter ciphertext: ";
        getline(cin, ciphertext);
        bruteforce(ciphertext);
    }
    return 0;
}