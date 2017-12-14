#include <map>
#include <ncurses.h>
#include <signal.h>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

constexpr int ESC_KEY = 'e';
constexpr int TEXT_KEY = 't';

static int send_command(vector<char const*> const parts){
    stringstream sst;
    for (auto item : parts){
        if (not sst.str().empty()){
            sst<<" ";
        }
        sst<<item;
    }
    string s=sst.str();
    printw("%s\n", s.c_str());
    return system((s+=" &").c_str());
}

// Returns exit code from system()
static int send_keys(string const text){
    return send_command({"adb","shell","input","text",text.c_str()});
}

// Returns exit code from system()
static int send_keys(int keycode){
    static map<int,string> const key_mappings={
        {KEY_UP, "DPAD_UP"},
        {KEY_DOWN, "DPAD_DOWN"},
        {KEY_LEFT, "DPAD_LEFT"},
        {KEY_RIGHT, "DPAD_RIGHT"},
        {KEY_ENTER, "ENTER"},
        {KEY_HOME, "HOME"},
        {KEY_BACKSPACE, "DEL"},
        {0x0A, "ENTER"},
        {0x1B, "BACK"}
    };

    if (!key_mappings.count(keycode)){
        printw("Keycode 0x%04x ignored\n", keycode);
        // No known keycode.
        return -1;
    }
    string text=string("KEYCODE_") + key_mappings.at(keycode);
    return send_command({"adb","shell","input","keyevent",text.c_str()});
}

void usage(){
    printw("Press %c to exit, %c to send text\n", ESC_KEY, TEXT_KEY);
}

int main(){
    initscr();
    raw();
    keypad(stdscr, true);
    noecho();

    halfdelay(2 /*=200ms*/);

    static volatile bool exit_handler_called=false;
    signal(SIGINT, [](int unused){exit_handler_called=true;});

    usage();
    for (int ch; not exit_handler_called and (ch=getch())!=ESC_KEY;) if (ch!=ERR){
        clear();
        usage();

        if (ch==TEXT_KEY){
            printw("Text: ");
            stringstream text;
            while (not exit_handler_called and (ch=getch())!=0x0A) if (ch!=ERR){
                if (isprint(ch)){
                    printw("%c", (char) ch);
                    if (ch=='"' or ch=='\\' or ch=='\''){
                        text<<"\\";
                    }
                    text<<(char)ch;
                }else{
                    break;
                }
            }
            printw("\n");
            if (ch==0x0A){
                send_keys("\\\"" + text.str() + "\\\"");
            }
        }else{
            send_keys(ch);
        }
    }

    endwin();
}
