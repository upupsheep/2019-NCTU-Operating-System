#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;
int main() {
    string s = "Scissor";
    int key;
    cin >> key;
    string temp;

    for (int i = 0; i < 100; i++) {
        pid_t cpid;
        if (fork() == 0) {
            int usr;
            cin >> usr;
            cout << usr << endl;
            exit(0); /* terminate child */
        } else {
            cpid = wait(NULL); /* reaping parent */
            cin >> temp;
        }
        // cin >> temp;
        cout.flush();
    }
}