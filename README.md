# OperatingSystemProject
Multithreaded banking application

4. Consider a multithreaded banking application. The main process receives requests to tranfer
money from one account to the other, and each request is handled by a separate worker thread
in the application. All threads access shared data of all user bank accounts. Bank accounts are
represented by a unique integer account number, a balance, and a lock of type mylock (much
like a pthreads mutex) as shown below.
struct account {
int accountnum;
int balance;
mylock lock;
};
Each thread that receives a transfer request must implement the transfer function shown below,
which transfers money from one account to the other. Add correct locking (by calling the
dolock(&lock) and unlock(&lock) functions on a mylock variable) to the tranfer
function below, so that no race conditions occur when several worker threads concurrently
perform transfers. Note that you must use the fine-grained per account lock provided as part of
the account object itself, and not a global lock of your own. Also make sure your solution is
deadlock free, when multiple threads access the same pair of accounts concurrently.
void transfer(struct account *from, struct account *to, int amount) {
from->balance -= amount; // dont write anything...
to->balance += amount; // ...between these two lines
}
