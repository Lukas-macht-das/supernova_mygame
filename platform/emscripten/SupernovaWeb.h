#ifndef SupernovaWeb_h
#define SupernovaWeb_h

#include <emscripten/html5.h>

#include "System.h"

class SupernovaWeb: public Supernova::System{

private:

    static std::string canvas;

    static int syncWaitTime;
    static bool enabledIDB;

    static int screenWidth;
    static int screenHeight;

    static int sampleCount;

    static EM_BOOL key_callback(int eventType, const EmscriptenKeyboardEvent *e, void *userData);
    static EM_BOOL mouse_callback(int eventType, const EmscriptenMouseEvent *e, void *userData);
    static EM_BOOL wheel_callback(int eventType, const EmscriptenWheelEvent *e, void *userData);
    static EM_BOOL touch_callback(int emsc_type, const EmscriptenTouchEvent* emsc_event, void* user_data);
    static EM_BOOL resize_callback(int event_type, const EmscriptenUiEvent* ui_event, void* user_data);

    static EM_BOOL canvas_resize(int eventType, const void *reserved, void *userData);
    static EM_BOOL webgl_context_callback(int emsc_type, const void* reserved, void* user_data);

    static EM_BOOL renderLoop(double time, void* userdata);

    static wchar_t toCodepoint(const std::string &u);
    static std::string toUTF8(wchar_t cp);
    static int supernova_mouse_button(int button);
    static int supernova_legacy_input(int code);
    static int supernova_input(const char code[32]);

public:

    SupernovaWeb();

    static void setEnabledIDB(bool enabledIDB);

    static int init(int argc, char **argv);
    static void changeCanvasSize(int width, int height);

    virtual int getScreenWidth();
    virtual int getScreenHeight();

    virtual int getSampleCount();

    virtual bool isFullscreen();
    virtual void requestFullscreen();
    virtual void exitFullscreen();

    virtual void setMouseCursor(Supernova::CursorType type);
    virtual void setShowCursor(bool showCursor);

    virtual std::string getUserDataPath();

    virtual bool syncFileSystem();

    virtual std::string getStringForKey(const char *key, const std::string& defaultValue);
    virtual void setStringForKey(const char* key, const std::string& value);
    virtual void removeKey(const char *key);

    virtual void initializeCrazyGamesSDK();
    virtual void showCrazyGamesAd(const std::string& type);
    virtual void happytimeCrazyGames();
    virtual void gameplayStartCrazyGames();
    virtual void gameplayStopCrazyGames();
    virtual void loadingStartCrazyGames();
    virtual void loadingStopCrazyGames();
    
};


#endif /* SupernovaWeb_h */