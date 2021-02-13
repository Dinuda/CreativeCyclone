// Imports
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

// Enable raw mode and turn off echo
void enableRawMode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    // Turn off terminal emulators
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_cflag |= (CS8);
    // Fic CTRL+M
    raw.c_iflag &= ~(ICRNL | IXON);
    // Disable CTRL+S and CTRL+Q
    raw.c_iflag &= ~(IXON);
    // Turn off all processing
    raw.c_oflag &= ~(OPOST);
    // local flag and disable CTRL-V
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    // A timeout for read
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Disable raw mode and turn off echo
void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

// Error Handling
void die(const char *s)
{
    perror(s);
    exit(1);
}

int main()
{
    enableRawMode();
    while (1)
    {
        char c = '\0';
        read(STDIN_FILENO, &c, 1);
        if (iscntrl(c))
        {
            printf("%d\r\n", c);
        }
        else
        {
            printf("%d ('%c')\r\n", c, c);
        }
        if (c == 'q')
            break;
    }
    return 0;
}