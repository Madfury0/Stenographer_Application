#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

string get_password() {
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    string password;
    getline(cin, password);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return password;
}

bool copy_file(const string &src, const string &dest) {
    ifstream source(src, ios::binary);
    if (!source) {
        cerr << "Error opening source file." << endl;
        return false;
    }
    ofstream destination(dest, ios::binary);
    if (!destination) {
        cerr << "Error creating destination file." << endl;
        return false;
    }
    destination << source.rdbuf();
    return source && destination;
}

int execute_steghide_embed(const string &output, const string &secret, const string &password) {
    pid_t pid = fork();
    if (pid == 0) {
        vector<const char*> args = {"steghide", "embed", "-cf", output.c_str(), "-ef", secret.c_str(), "-p", password.c_str(), "-f", nullptr};
        execvp("steghide", const_cast<char* const*>(args.data()));
        cerr << "execvp failed" << endl;
        exit(1);
    } else if (pid < 0) {
        cerr << "fork failed" << endl;
        return -1;
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
}

void embed_file() {
    string cover, secret, output, password;
    
    cout << "Enter cover file: ";
    getline(cin, cover);
    cout << "Enter secret file: ";
    getline(cin, secret);
    cout << "Enter output file: ";
    getline(cin, output);
    cout << "Enter password: ";
    password = get_password();
    
    if (!copy_file(cover, output)) {
        cerr << "Failed to copy cover file." << endl;
        return;
    }
    
    int result = execute_steghide_embed(output, secret, password);
    cout << (result == 0 ? "Successfully embedded." : "Embedding failed.") << endl;
}

int execute_steghide_extract(const string &embedded, const string &password, const string &output) {
    pid_t pid = fork();
    if (pid == 0) {
        vector<const char*> args = {"steghide", "extract", "-sf", embedded.c_str(), "-xf", output.c_str(), "-p", password.c_str(), "-f", nullptr};
        execvp("steghide", const_cast<char* const*>(args.data()));
        cerr << "execvp failed" << endl;
        exit(1);
    } else if (pid < 0) {
        cerr << "fork failed" << endl;
        return -1;
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
}

void extract_file() {
    string embedded, output, password;
    
    cout << "Enter embedded file: ";
    getline(cin, embedded);
    cout << "Enter output file: ";
    getline(cin, output);
    cout << "Enter password: ";
    password = get_password();
    
    int result = execute_steghide_extract(embedded, password, output);
    cout << (result == 0 ? "Successfully extracted." : "Extraction failed.") << endl;
}

int main() {
    int choice;
    do {
        cout << "\n1. Embed\n2. Extract\n3. Exit\nChoice: ";
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: embed_file(); break;
            case 2: extract_file(); break;
            case 3: cout << "Exiting." << endl; break;
            default: cout << "Invalid choice." << endl;
        }
    } while (choice != 3);
    
    return 0;
}
