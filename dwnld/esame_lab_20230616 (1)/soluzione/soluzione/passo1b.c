// open
#include <fcntl.h>
// dup2
#include <unistd.h>
// giocatore
#include "giocatore.h"

// Prima di eseguire la funzione, ridirige lo standard error verso /dev/null
int main() {
    int fd = open("/dev/null", O_WRONLY);
    dup2(fd, 2);
    giocatore();
}
