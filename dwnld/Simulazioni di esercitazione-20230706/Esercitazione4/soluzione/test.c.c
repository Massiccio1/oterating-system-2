#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
int contatore = 0;
int ritardo = 1;
void scrivi_pipe(int n) {
    char buffer[100];
    sprintf(buffer, "%d\n", n);
    int fd = open("pipe", O_WRONLY);
    write(fd, buffer, strlen(buffer));
    close(fd);
}
int leggi_pipe() {
    char buffer[100];
    int fd = open("pipe", O_RDONLY);
    int l = read(fd, buffer, 100);
    close(fd);
    buffer[l-1] = '\0';
    return atoi(buffer);
}
void invia_contatore() {
    scrivi_pipe(contatore);
}
void ricevi_ritardo() {
    ritardo = leggi_pipe();
}
void sdoppia() {
    if (!fork()) {
        scrivi_pipe(getpid());
    }
}
int main() {
    scrivi_pipe(getpid());
    signal(SIGUSR1, invia_contatore);
    signal(SIGUSR2, ricevi_ritardo);
    signal(SIGALRM, sdoppia);
    for (;;) {
        fprintf(stderr, "%d %d\n", getpid(), ++contatore);
        sleep(ritardo);
    }
    return 0;
}